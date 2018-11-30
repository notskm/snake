# Snake
A simple snake clone written in C++

![screenshot](images/snake.png?raw=true)

## Building

These instructrions show you how to build and install the program from source.

### Requirements:
* CMake 3.11+ https://cmake.org/download/
* Conan https://conan.io/downloads.html
* A local copy of this repo

```
git clone https://github.com/notskm/snake
```

### Multi Generators (Visual Studio):
```
mkdir build
cd ./build
```

Build only
```
cmake ..
cmake --build . --config Release
```

Build and install
```
cmake  -D CMAKE_INSTALL_PREFIX:PATH="install/path/snake" ..
cmake --build . --config Release --target install
```

### Other Generators (make):
```
mkdir build
mkdir build/release
cd ./build/release
```

Build only
```
cmake -DCMAKE_BUILD_TYPE=Release ../..
cmake --build .
```

Build and install
```
cmake -DCMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX:PATH="install/path/snake" ../..
cmake --build . --target install
```

## License

BSD 3-Clause License 2018 - [Notskm](https://github.com/notskm).
See [LICENSE](LICENSE.md) for more details.
