# PLACEHOLDER TITLE

## Build Instructions

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 4
```
The name of the resulting executable is `placeholder_name`.

If desired, unit tests can be run with the command: `cmake --build . --target test`

## Building Doxygen Documentation

Run the following commands:

```
cd doc
doxygen
```

Then open `./doc/html/index.html` in your browser of choice. Internet Explorer may experience issues.
