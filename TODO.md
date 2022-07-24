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

- [ ] Work stealing thread pool

Config

- [ ] Split compiler.h

Containers

- [ ] Bitset (SIMD optimized)
- [ ] Deque
- [ ] Map
- [ ] Set

ECS

- [ ] Archetype swapping
- [ ] Empty type optimizations
- [ ] Investigate Hierarchies
- [ ] Investigate Scripting
- [ ] Storage extra indirection for very large components. (Speeds up insert/destroy/swapping)
- [ ] Optimize scheduler graph computations
- [ ] Optimize scheduler execution
- [ ] Scheduler profile guided optimization for dynamic reordering of systems
- [ ] Scheduler debug info & statistics

Events

- [ ] Rewrite event system to be integrated with ECS
- [ ] Move to ECS and erase

IO

- [ ] IO Service
- [ ] File IO
- [ ] Networking abstraction
- [ ] Serialization/Deserialization
- [ ] YAML Parser
- [ ] JSON Parser
- [ ] XML Parser

Math

- [ ] Non-square matrices
- [ ] Make exponential functions constexpr
- [ ] Make power functions constexpr
- [ ] Make trigonometric functions constexpr
- [ ] Add documentation

OS

- [ ] CPUInfo for linux
- [ ] Thread utilities for linux

Graph

- [ ] Dense adjacency matrix
- [ ] Optimized BFS based topological sort
- [ ] Optimized Transitive reduction algorithm

Utilities

- [ ] UUID

Other

- [ ] Static analysis
- [ ] Sanitizers
- [ ] Code coverage
- [ ] Architecture optimization
- [ ] Link time optimizations (LTO)
- [ ] Linux GCC build
- [ ] Github Actions
- [ ] Precompiled headers
- [ ] Update style guide
- [ ] Add CONTRIBUTING.md

Official C++ 20 Support

- [ ] Remove coroutine CLion feature test patch (awaitable.h)
- [ ] Change cmake c++ standard from 23 (latest) to 20

## Major

These tasks are large updates with potentially breaking changes.

- [ ] Rename framework
- [ ] Split libraries (Core, ...)
- [ ] Restructure test folders to include mock, unit and system
- [ ] Restructure bench folder

## Experimental

These tasks are experiments to be done in the experimental branch.

- [x] None
