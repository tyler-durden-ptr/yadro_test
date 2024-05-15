## Requirements

- CMake 3.27 or higher
- vcpkg (latest version)
- A C++ compiler (GCC, Clang, MSVC, etc.)

## Build
`cmake --build ./build --config Debug --target yadro_test -j 8 --`

Run: `./build/yadro_test file.txt`

## Build & run tests
`cmake --build ./build --config Debug --target tests -j 8 --`

Run: `./build/tests`