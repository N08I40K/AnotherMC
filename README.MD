<img src="https://img.shields.io/static/v1?label=project status&message=in progress&color=orange&style=plastic" />
<img src="https://img.shields.io/badge/language-990099?logo=cplusplus&logoColor=FFFFFF&style=plastic" />
<img src="https://img.shields.io/badge/build_system-DA3434?logo=cmake&logoColor=FFFFFF&style=plastic" />
<img src="https://img.shields.io/badge/rendering_api-5686A5?logo=opengl&logoColor=FFFFFF&style=plastic" />

# AnotherMC

## Description

**AnotherMC** is a project that aims to create a Minecraft inspired game using the C++ programming language and the OpenGL library. The project is currently under development and is completely non-functional.

## Project Status

The project is in the early stages of development and is not intended for use. At the moment the focus is on building the basic architecture and exploring OpenGL features.

## Future plans

- Implementing basic rendering features.
- Adding a simple game world and basic mechanics.
- Building client-server interaction.

## Installation

The following tools are required to work on the project:

- C++ compiler (I use Clang from Visual Studio 2022)
- CMake to build the project
- CMake compatible project generator (I use [Ninja](https://github.com/ninja-build/ninja))

```bash
git clone https://github.com/N08I40K/another-mc.git
cd another-mc
mkdir build
cd build
cmake -S .. -B . -G Ninja
ninja
```

## Contribution

Your contribution is welcome! If you would like to help develop the project, please create a Pull Request or open an Issue with suggestions and corrections.

## License

This project is licensed under the MIT license. See the LICENSE file for details.