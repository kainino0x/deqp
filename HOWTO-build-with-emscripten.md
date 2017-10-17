In `external/`:

```sh
python fetch_sources.py
```

In `external/zlib/src/`:

```sh
emcmake cmake .
```

Then in the repository root:

```sh
mkdir build
cd build
emcmake cmake .. -DCMAKE_BUILD_TYPE=Debug
```
