
For build the project past next commands in terminal Linux/Windows.
on both ways you will need a CMake, Make, QT library installed:

Linux/Mac:

```bash
mkdir build
cd build
cmake ..
make

```

Windows (MSYS2):

```bash
# if not installed
pacman -S --needed mingw-w64-i686-toolchain

# build
mkdir build
cd build
cmake .. -G 'MSYS Makefiles'
```

in build dir run Visual Studio generated project and just build.

