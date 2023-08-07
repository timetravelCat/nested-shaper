# nested-shaper

## Features
- **header only** library
- Requires C++ compiler only. (libstdc++ is not necessary)

## Getting Started
### Prerequisites
* **CMake v3.15+** if you want to install system-wide library.
* **C++ Compiler** - needs to support at least the **C++17** standard, i.e. *MSVC*,
*GCC*, *Clang*
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

## License
This project is licensed under the [Unlicense](https://unlicense.org/) - see the
[LICENSE](LICENSE) file for details

## Contact
timetravelCat - timetraveler930@gmail.com