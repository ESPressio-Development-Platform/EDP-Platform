#!/usr/bin/env python3
"""Frozen source/model/native/actual-host regression evidence; never device access."""
from pathlib import Path
import json,hashlib
root=Path(__file__).resolve().parents[2];e=root/'docs/evidence/H471';load=lambda n:json.loads((e/n).read_text())
for rel,sha in load('FINAL-SOURCE-SHA256.json').items():assert hashlib.sha256((e/('final-'+rel.replace('/','-')+'.txt')).read_bytes()).hexdigest()==sha
p=load('VALIDATION-PROVENANCE.json');assert not p['hardware'] and not p['root'] and p['logical_generation_bits']==63 and p['new_storage_members']==0
for rid in p['runs']:assert load(rid+'-state.json')['state']=='PASS' and load(rid+'-state.json')['exit_code']=='0'
assert load(p['original_clock_release_failure']+'-original-state.json')['state']=='FAIL'
old=load('H470-SNAPSHOT-ISOLATION.json');assert [(x['label'],x['failures']) for x in old]==[('before',3),('current',1)]
after=load('H471-FIXED-PLATFORM-READER-REPEATS.json');assert len(after)==2 and all(len(x['outcomes'])==100 and x['failures']==0 for x in after)
model=load('H471-BEFORE-AFTER-VISIBILITY-AND-ROLLOVER.json');assert len(model)==4 and all(x['compiler_include_trace_selected_before_header'] if 'compiler_include_trace_selected_before_header' in x else x['include_trace_selected_header'] for x in model)
assert all(x['exit']!=0 if x['source']=='before' else x['exit']==0 for x in model)
cmake=load('H471-PLATFORM-CMAKE-RESULTS.json');assert len(cmake)==2 and all(x['targets']==5 and x['assertions_enabled_all_commands'] for x in cmake)
for name,count in [('H471-PLATFORM-GCC_SANITIZE-RESULTS.json',5),('H471-PLATFORM-CLANG_SANITIZE-RESULTS.json',5),('H471-PLATFORM-ANALYZE-RESULTS.json',5),('H471-CLOCK-RELEASE-RESULTS.json',18),('H471-CLOCK-GCC_SANITIZE-RESULTS.json',9),('H471-CLOCK-CLANG_SANITIZE-RESULTS.json',9),('H471-CLOCK-ANALYZE-RESULTS.json',9)]:assert len(load(name))==count
native=load('H471-NATIVE-COMPOSITION-SOURCE-BASIS.json');assert native['equal'] and native['before']==native['after'] and native['native_targets']==6
for row in load('H471-NATIVE-BEFORE-AFTER-LAYOUT.json'):assert row['compiler_include_trace_selected_before_header'] and row['current']['facts']==row['before']['facts']==[32,72,72,4,4] and not row['current']['atomic_64_helpers'] and not row['before']['atomic_64_helpers']
assert len(load('H471-NATIVE-BEFORE-AFTER-LAYOUT.json'))==4 and len(load('H471-CANONICAL-PLACEMENT.json'))==2
g=load('DEPENDENCY-REVIEW.json');assert len(g['owning_graph'])==2 and len(g['complete_native_application_graph'])==23 and g['acyclic'] and g['new_edges']==0
print('H471_FROZEN_COHERENT_REUSE_NONBLOCKING_ROLLOVER_FIVE_PLATFORM_NINE_CLOCK_200_REPEATS_NATIVE_LAYOUT_PASS|hardware=0|root=0')
