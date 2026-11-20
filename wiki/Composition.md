# Composition

Each facility is represented as a Platform-domain capability with explicit properties where provider qualification matters. Concrete providers live in EDP-Platform-* repositories. Higher-level domains consume these capabilities through Requirements rather than depending on provider SDKs.

The abstraction layer must never depend upward on higher EDP domains.

## Optional current-context binding

CurrentContextBinding providers must advertise true ConstantTimeLookup, AllocationFreeLookup, CurrentContextOnly and RejectsInterruptContexts properties. The optional trait checks record Type, exact bind/read/retire outcomes, default construction, noncopyable/nonmovable lifetime and noexcept methods. Existing execution provider/default contracts are unchanged.

See [exact reference](Reference-execution-CurrentContextBinding).
