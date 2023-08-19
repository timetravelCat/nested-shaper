# nested-shaper

## Features
- **header only** library
- Requires C++ compiler supports C++11 standard. (libstdc++/libc++ is not necessary)

## Limitations
- (WIP) Zero vel-acc-jerk... initialization for nested_shaper

## Getting Started
### Prerequisites
* **C++ Compiler** - needs to support at least the **C++11** standard, i.e. *MSVC*, *GCC*, *Clang*
* **CMake v3.15+** if you want to install system-wide library.
> ***Note:*** *You also need to be able to provide ***CMake*** a supported
[generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).*

### Installation from sources
- *nested-shaper* is header-only library.
    - Copy include/** to your project.
- If you want to install system-wide, requires **CMake v3.15+**
    ```bash
    cmake -Bbuild -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION)
    cmake --build build --config Release
    cmake --build build --target install --config Release
    ```
    
### Unit Test
- *nested-shaper* uses [Catch2](https://github.com/catchorg/Catch2) for using testing.
- Turn on the CMake **BUILD_UNIT_TEST** option

### Examples
- *nested-shaper* uses [Matplot++](https://github.com/alandefreitas/matplotplusplus) for examples (for algorithm visualization).
- Turn on the CMake **BUILD_EXAMPLES** option
- Install gnuplot and add bin folder to your PATH.

## License
This project is licensed under the [Unlicense](https://unlicense.org/) - see the
[LICENSE](LICENSE) file for details

## Contact
timetravelCat - timetraveler930@gmail.com