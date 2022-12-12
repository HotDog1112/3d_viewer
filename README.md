# About 3D-Viewer
3D Model Viewer written in C and Qt/C++

- Athenian transformations(rotate, scale, move along the axes X, Y, Z);
- Display settings - colors, size, vertex/polygon types
- Saving in `.jpeg`, `.bmp` formats are available
- Saving settings
- Rendering works with models with a million vertices without freezing

## `Makefile` commands
- `make all` - run library build, tests, application build and unit test coverage report
- `make clean` - remove generated files
- `make install` - build app with cmake
- `make dvi` - generate manual
- `make check` - cppcheck, style tests (clang-format)

