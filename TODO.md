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
- [ ] Hierarchies
- [ ] Scripts
- [ ] Resources
- [ ] Pipelined parallel systems
- [ ] Phase parallelization statistics (for debug/profiling)
- [ ] Phase dynamic reordering optimization?
- [ ] For large components (or medium non-relocatable) use pointer-based storage. (Maybe)

Events

- [ ] Rewrite event system to be integrated with ECS

Async

- [ ] WhenAll/WhenAllReady optimizations and benchmarks
- [ ] Thread local storage

IO

- [ ] Serialization/Deserialization
- [ ] YAML Parser
- [ ] JSON Parser
- [ ] XML Parser

Audio

- [ ] OpenAL Integration

Physics

- [ ] Custom Top-Down 2D Physics
- [ ] Box2D Integration (2D)
- [ ] Nvidia PhysX Integration (3D)

Editor

- [ ] IMGUI Integration

Math

- [ ] Non-square matrices
- [ ] Optimize matrix computations
- [ ] Improve matrix and vector hashing
- [ ] Make exponential functions constexpr
- [ ] Make power functions constexpr
- [ ] Make trigonometric functions constexpr
- [ ] Add documentation

Containers

- [ ] Bitset (SIMD optimized)
- [ ] Map
- [ ] Set
- [ ] Vector range operations
- [ ] SmallVector
- [ ] StaticVector

Utilities

- [ ] TypeInfo PrettyName (Same for every compiler)
- [ ] TypeInfo UniqueId Investigation & Cross-Boundaries tests
- [ ] TypeInfo use class static vs function static (2x performance on prototype)
- [ ] Smart pointer casting
- [ ] Intrusive to non-intrusive ref conversion. (Would allow better type erasure)
- [ ] AllocateRef, AllocateRefWithDeleter and MakeRefWithDeleter
- [ ] UUID

Other

- [ ] Build sandboxes/examples cmake option
- [ ] Precompiled headers
- [ ] Added info about tests and benchmark to style guide
- [ ] Add CONTRIBUTING.md
- [ ] Sanitizers
- [ ] cppcheck

Official C++ 20 Support

- [ ] Remove coroutine CLion feature test patch (awaitable.h)
- [ ] Change cmake c++ standard from 23 (latest) to 20

## Major

These tasks are large updates with potentially breaking changes.

- [ ] Make engine more generic than "genebits". (Use different name for the engine)

## Experimental

These tasks are experiments to be done in the experimental branch.

- [x] None
