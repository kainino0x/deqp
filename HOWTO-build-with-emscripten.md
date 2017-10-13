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
emcmake cmake ..
```
