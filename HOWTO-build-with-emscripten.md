In `external/`:

```sh
python fetch_sources.py
```

In `external/zlib/src/`:

```sh
emconfigure ./configure
```

Then in the repository root:

```sh
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/PATH/TO/emsdk_portable/emscripten/VERSION/cmake/Modules/Platform/Emscripten.cmake
```
