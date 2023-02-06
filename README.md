# CeltreSoftware's CPUID Library

Library to use the __cpuid extensions in a cross platform manner.

## How To Use

This library does <b>not</b> have installation targets, which is intentional, at least for now.

To use this library, download the sources, preferably using your favourite package manager, such as
`vcpkg` or CMake's `FetchContent`.

Add the `include` directory to your taget's include directories and link the name
`CELTRE_SOFTWARE_CPUID` to your linked libraries. This will also import the compilation requirements
given by the library.

You should also mark the binary directory as the link directory in your target to be able to link to
the library properly.

For example:
```cmake
cmake_minimum_required(VERSION 3.23)
project(MyProject)

include(FetchContent)

FetchContent_Declare(
  celtresoft_cpuid
  GIT_REPOSITORY https://github.com/CeltreSoftware/cpuid.git
  GIT_TAG main)

FetchContent_MakeAvailable(celtresoft_cpuid)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${celtresoft_cpuid_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE CELTRE_SOFTWARE_CPUID)
target_link_directories(${PROJECT_NAME} PRIVATE ${celtresoft_cpuid_BINARY_DIR})
```

## Functionality

This library contains a single class, called `cpuid` in `include/celtresoft/cpuid`. You may use this
class as a subsitute for the `__cpuid` function, since it is implemented differently in different
compilers.

If no implementation of `__cpuid` is found by CMake during configuration, the library will attempt
to configure an assembly implementation of `__cpuid` and, was this to fail, the library will not be
usable.

The assembly if all fails is as follows, given that `CELTRESOFT_CPUID_64BIT` will be defined on
64-bit machines:

```cpp
#ifndef CELTRESOFT_CPUID_64BIT
# define __cpuid(level, a, b, c, d)                                         \
   do {                                                                     \
     if (level && level != 1) {                                             \
       asm volatile("cpuid"                                                 \
                    : "=a"(a), "=b"(b), "=c"(c), "=d"(d)                    \
                    : "0"(level));                                          \
     } else {                                                               \
       asm volatile("cpuid"                                                 \
                    : "=a"(a), "=b"(b), "=c"(c), "=d"(d)                    \
                    : "0"(level), "1"(0), "2"(0));                          \
     }                                                                      \
   } while (0)
#else // CELTRESOFT_CPUID_64BIT
# define __cpuid(level, a, b, c, d)                                         \
   asm volatile("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "0"(level))
#endif // CELTRESOFT_CPUID_64BIT
```

To use the class, you may do:

```cpp
#include <celtresoft/cpuid.hpp>

auto main() -> int {
  celtresoft::cpuid id(0x80000000U);
  auto id_eax = id.eax();
  auto id_ebx = id.ebx();
  auto id_ecx = id.ecx();
  auto id_edx = id.edx();
}
```

The `operator=` is overloaded in the `cpuid` class so that, if you need to change the information
type, you may do so without re-instantiating the class:

```cpp
#include <celtresoft/cpuid.hpp>

auto main() -> int {
  celtresoft::cpuid id(0x80000000U);
  // Need a different type
  id = 0x80000001U; // Equivalent to id = celtresoft::cpuid(0x80000001U);
}
```

This class simply receives register information given an information type (or level) and saves it in
a private array, which then exposes the %eax, %ebx, %ecx and %edx values through getter functions.
