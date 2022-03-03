# Genebits TODO

Completed tasks are marked with and 'x', they will be removed at the next release.

If there is nothing todo the list will be: [x] None

## Patch

High priority!

These tasks may be minor bug fixes or optimizations. They do not change the interface.

- [x] None

## Minor

These tasks are small or large updates that are non-breaking for non deprecated functionalities.

Debug

- [ ] Implement stack tracing for GCC
- [ ] Implement stack tracing for Clang
- [ ] Verify logging functionality for linux (gcc and clang)

OS

- [ ] Implement CPUInfo for linux
- [ ] Very thread utilities for linux

ECS

- [ ] Archetype swapping
- [ ] Phase parallelization statistics (for debug/profiling)
- [ ] Phase dynamic reordering optimization?

Async

- [ ] WhenAll/WhenAllReady optimizations and benchmarks

Containers

- [ ] Map
- [ ] Set
- [ ] Vector range operations
- [ ] SmallVector
- [ ] StaticVector

Utilities

- [ ] TypeInfo PrettyName (Same for every compiler)
- [ ] TypeInfo UniqueId Investigation & Cross-Boundaries tests
- [ ] Smart pointer casting
- [ ] AllocateRef, AllocateRefWithDeleter and MakeRefWithDeleter

Other

- [ ] Build sandboxes/examples cmake option

## Major

These tasks are large updates with potentially breaking changes.

- [ ] Make engine more generic than "genebits". (Use different name for the engine)
- [x] None

## Experimental

These tasks are experiments to be done in the experimental branch.

- [ ] Cmake precompiled headers
