# TODO Checklist

Completed tasks are marked with and 'x', they will be removed at the next release.

If there is nothing todo the list will be: [x] None

## Patch

High priority!

These tasks may be minor bug fixes or optimizations. They do not change the interface.

- [x] None

## Minor

These tasks are small or large updates that are non-breaking for non deprecated functionalities.

Async

- [x] None

Config

- [ ] Split compiler.h

Containers

- [ ] Bitset (SIMD optimized)
- [ ] Dequeue
- [ ] Map
- [ ] Set

Debug

- [ ] Implement stack tracing for linux
- [ ] Verify logging functionality for linux

OS

- [ ] CPUInfo for linux
- [ ] Thread utilities for linux

ECS

- [ ] Archetype swapping
- [ ] Empty type optimizations
- [ ] Hierarchies
- [ ] Scripts
- [ ] Const views
- [ ] Storage extra indirection for very large components. (Speeds up insert/destroy/swapping)
- [ ] Optimize scheduler graph computations
- [ ] Optimize scheduler execution
- [ ] Scheduler dynamic reordering
- [ ] Scheduler debug info & statistics

Events

- [ ] Rewrite event system to be integrated with ECS
- [ ] Move to ECS and erase

IO

- [ ] Serialization/Deserialization
- [ ] YAML Parser
- [ ] JSON Parser
- [ ] XML Parser

Audio

- [ ] OpenAL Integration

Physics

- [ ] 2D Physics
- [ ] 3D Physics

Math

- [ ] Non-square matrices
- [ ] Make exponential functions constexpr
- [ ] Make power functions constexpr
- [ ] Make trigonometric functions constexpr
- [ ] Add documentation

Graph

- [ ] Dense adjacency matrix
- [ ] Optimized BFS based topological sort
- [ ] Optimized Transitive reduction algorithm

Utilities

- [ ] TypeInfo PrettyName (Same for every compiler)
- [ ] UUID

Other

- [ ] Static analysis
- [ ] Sanitizers
- [ ] Architecture optimization
- [ ] Link time optimizations (LTO)
- [ ] Linux build
- [ ] Precompiled headers
- [ ] Update style guide
- [ ] Add CONTRIBUTING.md

Official C++ 20 Support

- [ ] Remove coroutine CLion feature test patch (awaitable.h)
- [ ] Change cmake c++ standard from 23 (latest) to 20

## Major

These tasks are large updates with potentially breaking changes.

- [ ] Make engine more generic than "genebits". (Use different name for the engine)
- [ ] Move graphics to separate lib
- [ ] Rename 'Containers' to 'adt' or 'container'
- [ ] Restructure test folders to include mock, unit and system
- [ ] Restructure benchmark folders to include helper, micro and macro (or similar structure)
- [ ] Update version.h
- [ ] Update cmake structure

## Experimental

These tasks are experiments to be done in the experimental branch.

- [x] None
