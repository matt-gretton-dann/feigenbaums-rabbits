# Feigenbaum's Rabbits

A quick project to show how the iteration `x[n + 1] = k * x[n] * (1 - x[n])` shows chaotic
behaviour as `k` is varied.

This is a quick hack so the code is horrible!

# Building

## Install Dependencies:

On macOS using Homebrew install CMake and SDL2:

```sh
brew install cmake sdl2
```

On Ubuntu install build tools including CMake and SDL2:

```sh
sudo apt install build-essential cmake libsdl2-dev
```

## Build

```sh
cmake -Bbuild -S.
cmake --build build
./build/fs-bunnies
```

# Copyright & License

Copyright 2020, Matthew Gretton-Dann

Licese: Apache-2.0.  See [LICENSE](./LICENSE)