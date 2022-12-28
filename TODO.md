# TODO Checklist

Completed tasks are marked with and 'x', they will be removed at the next release.

If there is nothing todo the list will be: [x] None

## Patch

High priority!

These tasks may be minor bug fixes or optimizations. They do not change the interface.

- [x] None

## Minor

These tasks are small or large updates that are non-breaking for non deprecated functionalities.

### Core

Async

- [ ] Work stealing thread pool

Config

- [ ] Split compiler.h
- [ ] ASSUME(condition) and UNREACHABLE hints
- [ ] RESTRICT hint
- [ ] PURE hint

Containers

- [ ] Bitset (SIMD optimized)
- [x] Deque
- [ ] Map
- [ ] Set

Debug

- [ ] Remove stacktrace (standard stacktrace in c++23)

System

- [ ] Support member function systems

Scheduler

- [ ] Optimize scheduler graph computations
- [ ] Scheduler debug info & statistics
- [ ] Runtime optimizations

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

### ECS

- [x] Entities query
- [ ] Lazy create & destroy queries
- [ ] Archetype swapping
- [ ] Component adding & removing
- [ ] Empty type optimizations
- [ ] Investigate Groups
- [ ] Investigate Hierarchies
- [ ] Investigate Scripting
- [ ] ArchetypeStorage extra indirection for very large components. (Speeds up insert/destroy/swapping)

### Events

- [ ] Make events work with graphic window input
- [ ] Remove old events

### IO

- [ ] IO Service
- [ ] File IO
- [ ] Serialization/Deserialization
- [ ] YAML Parser
- [ ] JSON Parser
- [ ] XML Parser
- [ ] Networking abstraction

### Graphics

- [ ] Make Vulkan loader work for linux
- [ ] Make the Vulkan api wrapper generate extension functions
- [ ] Implement shader check script for Vulkan

### Other

- [ ] Static analysis
- [ ] Sanitizers
- [ ] Code coverage
- [ ] Architecture optimization
- [ ] Link time optimizations (LTO)
- [ ] Linux GCC build
- [ ] Github Actions
- [ ] Precompiled headers
- [ ] Update style guide
- [ ] Measure and reduce compile time
- [ ] Add CONTRIBUTING.md

Official C++ 20 Support

- [ ] Remove coroutine CLion feature test patch (awaitable.h)
- [ ] Change cmake c++ standard from 23 (latest) to 20

## Major

These tasks are large updates with potentially breaking changes.

- [x] Rename framework
- [x] Split libraries (Core, ...)
- [x] Restructure test folders to include mock, unit and system
- [x] Restructure bench folder

## Experimental

These tasks are experiments to be done in the experimental branch.

- [x] None
