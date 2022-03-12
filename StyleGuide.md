# Genebits Style Guide

Welcome to the genebits style guide. This style guide is heavily influences by the **Google C++ Style Guide** and can be
found [here](https://google.github.io/styleguide/cppguide.html).

# Table of Contents

* [Project Structure](#project-structure)
    * [Dependencies](#dependencies)
        * [Guidelines](#guidelines)
        * [Package Manager](#package-manager)
    * [Private Implementation](#private-implementation)
    * [Define Guard](#define-guard)
    * [Include Order](#includes-order)
* [Naming](#naming)
    * [File Names](#file-names)
    * [Macro Names](#macro-names)
    * [Namespace Names](#namespace-names)
    * [Type Names](#type-names)
    * [Variable Names](#variable-names)
        * [Struct Data Members](#struct-data-members)
        * [Class Data Members](#class-data-members)
    * [Constant Names](#constant-names)
    * [Function Names](#function-names)
    * [Enumerator Names](#enumerator-names)

# Project Structure

## Dependencies

### Guidelines

Dependencies should be avoided as much as possible. Whenever a dependency is added to the project it must be discussed
rigorously with the rest of the group.

If you need a library to handle something specific and simple, it is almost always better to simply implement a
minimalist version in the engine.

Dependencies that are only for development tools such as the editor, or debugging tools are much more likely to be
accepted.

Avoid needing to modify an external library. We don't want to be maintaining other libraries. Make sure the library is
well-supported long term, and doesn't suck us into its ecosystem.

### Package Manager

We use the **Conan Package Manager** for most of our dependencies.

Always prefer using this package manager over other alternatives.

## Private Implementation

In order to expose a bare minimum, it is encouraged to place headers inside sources as much as possible.

For public headers with many or large dependencies it is encouraged to break the dependencies by putting as much code as
possible in the sources. This can sometimes be achieved using a `Private Implementation Pattern (Pimpl)` and is
regularly used in the engine.

Normally, dependencies that are used for release builds should never be exposed in public headers.

## Define Guard

All header files should have `#define` guards. Do not use `#pragma once`. The name should
be `<PROJECT>_<PATH/SCOPE>_<FILE>_H_`.

The path/scope should be somewhat based on the projects source tree.

```
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_

...

#endif // FOO_BAR_BAZ_H_
```

## Includes Order

Includes should be in the following order:

* Related Header
* Standard Library Headers
* Other Libraries Headers
* Your headers

```
#include "my_header.h" // Related header

#include <string> // Standard library headers
#include <vector>

#include <cuda.h> // Other libraries headers
#include <Windows.h>

#include "some_util.h" // Your headers
...
```

This order is not always possible, in exceptional cases make sure to document reasons for breaking the style.

# Naming

## Directory Names

**Do not use spaces**

Use **lower_snake_case** for naming directories.

## File Names

Use **lower_snake_case** for naming files.

C++ files should use the `.cpp` extension and header files should use the `.h` extension.

Do not use file names that exist in the standard libraries or operating system includes such as `/usr/include`.

## Macro Names

Macros should use **UPPER_SNAKE_CASE**.

```
MY_MACRO(...) 
```

## Namespace Names

Namespace names are in **lower_snake_case**.

When creating an embedded namespace scope, prefer the more modern `::` approche.

```
// Prefered
namespace hello::reader
{
...
}

// Also OK
namespace hello 
{
...
namespace reader 
{
...
}
}

```

## Type Names

Type names are in **PascalCasing**. This applies for everything: classes, structs, typedefs & aliases.

```
class MyClass;
struct MyStruct;

typedef std::string MyString;

using MyMap = std::map<std::string, std::string>;
```

Not required for compliance with other libraries such as the standard.

## Variables Names

The names variables are in **lower_snake_case**.

Also applies to static variables and const variables that do not have a fixed value for the duration of the program.

```
int today_week_day = 5;

const int tomorrow_week_day = (today + 1) % 7;
```

### Struct Data Members

The struct data member names are in **lower_snake_case**.

```
struct Node
{
  Node* next;
  int held_data;
};
```

### Class Data Memebers

Class data members are in **lower_snake_case** and have an additional underscore at the end of the name.

```
class Account
{
...
private:
  int type_;
  double total_amount_;
};
```

## Constants

Constants, declared constexpr or const, where the value is fixed for the duration of the program, are named with
leading 'c' and followed by **PascalCasing**.

```
class Account
{
...
constexpr int cDaysInAWeek = 7;
const int cWeeksInAYear = 52;
```

## Function Names

Functions use **PascalCasing**.

```
PushBack()
Delete()
OpenFile()
```

## Enumerator Names

Enumerators are named with **PascalCasing**.

Naming enums like macros is also permitted but should be avoided because this may cause collisions between enums and
macros.

```
// Prefered
enum class Color
{
  White,
  Black
  LimeGreen
  ...
}

// Not recommended, but allowed
enum LogSeverity
{
  DEBUG_VERBOSE,
  INFO,
  ERROR,
  ...
}
```
