# Genebits Style Guide

Welcome to the genebits style guide. This style guide is heavily influences by the **Google C++ Style Guide** and can be found [here](https://google.github.io/styleguide/cppguide.html).

# Table of Contents

* [Project Structure](#project-structure)
  * [Dependancies](#dependancies)
    * [Package Manager](#package-manager)
    * [Other Third Parties](#other-third-parties)
  * [Engine Module](#engine-module)
    * [Low Coupling](#low-coupling)
  * [Define Guard](#define-guard)
  * [Minimalist Includes](#minimalist-includes)
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

# Project Stucture

The project structure allows us to more easily scale the project, therefor is quite important for the long term.

## Dependancies

Both external and internal dependancies should be keapt to a minimum. There are various methods to adding third party dependancies to the project, but conan is encouraged when possible.

### Package Manager

We use the **Conan Package Manager** for most of our dependancies. Always prefer uses the package manager. We have cmake utilites for making conan easier to use. 

### Other Third Parties

Prefer putting the dependancy within its own folder the in the `thirdparty` folder at the root of the project.

If this is not possible, make sure sufficient cmake code is used to properly handle the dependancy.

## Engine Module

The engine is separated into `Modules`. Various cmake utilies facilitate the creation and manament of these modules.

### Low Coupling

In order to expose a bare minimum, engine modules must have a `public` and `private` directory. Only headers that are to be exposes should be in the public folder.

Every project should try not to expose thier internal dependancies. Using a `Private Implementation Pattern (Pimpl)` is encouraged to achieve this.

## Define Guard

All header files should have `#define` guards. Do not use `#pragma once`. The name should be `<PROJECT>_<PATH/SCOPE>_<FILE>_H_`.

The path/scope should be somewhat based on the projects source tree.

```
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_

...

#endif // FOO_BAR_BAZ_H_
```

## Minimalist Includes

You should only include what you use. Frequently clean up headers to reduce the bloat and keep everything included to a minimum.

## Includes Order

Includes should be in the following order:

* Related Header
* System Headers
* Standard Library Headers
* Other Libraries Headers
* Your headers

```
#include "my_header.h"

#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <cuda.h>

#include "some_util.h"
```

There may be execeptions where conditional includes are needed, prefer putting these at the bottom.

# Naming

The most important consistency rules are those that govern naming. The style informs us of the sort thing we are looking at without needing to search for the declaration.

There may be exceptions to naming something, usually if its something thats analogous to an exisiting `C` or `C++` entity.

## Folder Names

**Do not use spaces**

Naming is quite flexible, but prefer a minimalist all lowercase name.

## File Names

File names should all be lowercase and can include underscores.

C++ files should use the `.cpp` extension and header files should use the `.h` extension.

Do not use file names that exist in the standard libraries or operating system includes such as `/usr/include`.

## Macro Names

Macros should use **UPPER_SNAKE_CASE**. 

```
MY_MACRO(...) 
```

## Namespace Names

Namespace names are in **snake_case**. 

When creating an embedded namespace scope, prefer the more modern `::` approche.

```
// Prefered
namespace genebits::engine
{
...
}

// Also OK
namespace genebits 
{
...
namespace engine 
{
...
}
}

```

## Type Names

Type names are in **PascalCasing**. This applies for classes, structs, typedefs & aliases.

```
class MyClass;
struct MyStruct;

typedef std::string MyString;

using MyMap = std::map<std::string, std::string>;
```

## Variables Names

The names variables are in **snake_case**. This also applies to static data members.

This also applies to const variables that do not have a fixed value for the duration of the program.

```
int today_week_day = 5;

const int tomorrow_week_day = (today + 1) % 7;
```

### Struct Data Members

```
struct Node
{
  Node* next;
  int held_data;
};
```

### Class Data Memebers

Class data members has an additional underscore at the end of the name.

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

Constants, declared constexpr or const, and whoses value is fixed for the duration of the program are named with leading "c" and followed by **PascalCasing**.

```
class Account
{
...
constexpr int cDaysInAWeek = 7;
const int cWeeksInAYear = 52;
```

## Function Names

Regular functions use **PascalCasing**.

```
PushBack()
Delete()
OpenFile()
```

## Enumerator Names

Enumerators are named with **PascalCasing**.

Naming enums like macros is also permitted but should be avoid because this may cause collisions between enums and macros.

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
