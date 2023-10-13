# Minesweeper

## How to build

### 🪟 win32

* Install `cmake`
* Install `vcpkg`
* Using `vcpkg` install sdl2

```
vcpkg.exe install sdl2 --triplet x64-windows
```

You can use either use cmake presets like so:

```sh
$ cmake --list-presets
$ cmake --preset win32_vs2022
```

*Warning: Don't forget to pinpoint to vcpkg.cmake in CMakePresets.json*

or just call cmake like so:

```sh
$ cmake -B build . -DCMAKE_TOOLCHAIN_FILE=c:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows
```

*Warning: Change CMAKE_TOOLCHAIN_FILE to your own path*.