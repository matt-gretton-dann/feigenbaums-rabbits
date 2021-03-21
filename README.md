# Feigenbaum's Rabbits

A quick project to show how the iteration `x[n + 1] = k * x[n] * (1 - x[n])` shows chaotic
behaviour as `k` is varied.

This is a quick hack so the code is horrible!

# Building

This has been tested on macOS with Homebrew only.

Requires CMake and SDL2 to be installed:

```sh
cmake -Bbuild -S.
cmake --build build
./build/fs-bunnies
```

# Copyright & License

Copyright 2020, Matthew Gretton-Dann
Licese: Apache-2.0.  See [LICENSE](./LICENSE.md)