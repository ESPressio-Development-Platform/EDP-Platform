# Coherent snapshot reuse and rollover ordering

H471 / Platform#6. The unchanged optimized Clock reader test reproduced backward
readings with the old implementation. Include-traced unmodified tests failed3/30
against old Clock and1/30 against the new projection source. An instrumented copy
classified the failures as timestamp regressions, not writer rejection or missing
synchronization. These are separate original receipts. The projection is not
called by that old test.

## Buffer reuse

The single writer fills the inactive buffer before publishing. Another reader may
still hold the older identity for that buffer when it is reused two publications
later. The old relaxed data accesses did not create synchronization from such a
newer word to the reader's final generation loads. Matching stale generation
values could therefore accept a mixture.

Every data word is now stored with release and read with acquire using the
EXISTING AtomicWord32 contract. If any copied word comes from a later reuse,
its store follows the intervening publication in the sole writer's sequence.
The acquire imports those preceding effects. Write/read coherence then prevents
the reader's subsequent generation loads from returning an identity older than
that intervening publication. The comparison differs and the reader retries.
If no newer reused word is observed, the copied words remain from the selected
complete publication. No new storage or synchronization primitive is required.

This is a source-level happens-before argument. The primary C++ rules are
[release/acquire synchronization and atomic coherence](https://eel.is/c++draft/intro.races)
and [atomic ordering](https://eel.is/c++draft/atomics.order). The explicit host
visibility MODEL explores a permitted stale-generation/new-buffer read using
per-word modification order and imported release frontiers; it is not presented
as a concrete native execution. Both old selected-header overlays return an
incoherent signature in that model; both repaired GCC/Clang cases reject the stale
generation and return a coherent value. Independent concrete threaded tests and
the original Clock regression supplement the model.

## Rollover without waiting for a writer

The high word uses31epochbits and one marker bit. The low word uses all32bits,
including its active-buffer selector. At low rollover the sole writer:

1. Finishes the inactive buffer0.
2. Releases marker(oldHigh).
3. Releases low0.
4. Releases nextHigh with the marker cleared.

A stable marker normalizes to the old published identity `(oldHigh,UINT32_MAX)`
and buffer1. That old buffer is not changed until the transition is complete.
A reader paused with the writer can return the old coherent value at either
marker point; it does not spin waiting for the writer to finish.

A generation read acquires high, low and high again. A changed high retries.
If low zero is acquired from the transition, its preceding marker becomes visible
to the later high load, preventing an old ordinary high from being paired with
that wrapped low. If a reader observes the final new high, that release imports
the completed low/buffer writes. A stable marker remains the old identity.
Any subsequent reuse of old buffer1 follows the final high release; acquiring
one of those newer data words forces the final generation read beyond the marker
identity. Thus an incomplete rollover cannot accept a mixed state.

There are2^63 logical publication identities. A complete identity cycle during a
single read is outside the finite contract, and is explicitly documented. No
atomic64, hidden lock, writer-held reader gate or infinite ABA guarantee is claimed.
The internal marker consumes an existing bit; all state buffers and the two
metadata words retain their previous byte geometry. Extra acquire/release
operations have a code cost; no unmeasured ISR or latency guarantee is supplied.

## Validation and boundaries

The owning five CTest suites now include the previously unregistered concurrency
suite and the new reuse/rollover suite. Release retains assertions in every target.
The visibility model, two paused-writer points, epoch wrap and100,000 actual host
publications with eight readers pass GCC/Clang. The original unchanged optimized
Clock reader test passes100 repetitions with old Clock source and100 with new
projection source over the repaired Platform. All existing Clock nine-unit normal,
assertion-enabled Release, both ASan/UBSan and Clang analyzer/oracle suites pass.
Original failures remain preserved; repeated success is evidence, not a proof
against every future fault.

Native compilation forces actual read/publish bodies and records before/after
layout and word-operation helpers on ESP32/ESP32-S3 across ESP-IDF/Arduino. The
32-byte test state remains a72-byte snapshot. Code/source/SDK/callback evidence is
reported separately. Native execution, full ISR, physical time, lease or Root
qualification is not inferred from a host or compile-only result.

No H471 board, flash, serial, key, profile, NVS, host-time or RPI400 action.
H469 final C53/D70 snapshots and both ROM halts remain current. H468/H469 failures
and conditional timing inputs are unchanged. Root stays disabled. Receive
admission, actual same-candidate/receiver/lease/holdover/fleet/fullV2 work remains.
