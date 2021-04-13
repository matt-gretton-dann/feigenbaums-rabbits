/** \file      feigenbaum.cc
 *  \brief     Feigenbaum's Rabbits
 *  \author    Matthew Gretton-Dann
 *  \copyright 2021, Matthew Gretton-Dann
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Draws the traditional "Feigenbaum's Rabbits" picture.
 *
 * That is the values of x[n + 1] = k * x[n] * (1 - x[n]).
 *
 */

#include <SDL.h>
#include <cassert>
#include <cstdint>
#include <iostream>

class FixedPoint {
public:
  constexpr FixedPoint(uint64_t units, uint64_t frac)
      : num_((units << frac_bits_) | frac_bits_) {
    assert(units < (std::uint64_t(1) << unit_bits_));
    assert(frac < (std::uint64_t(1) << frac_bits_));
  }

  FixedPoint(FixedPoint const &) = default;
  FixedPoint(FixedPoint &&) = default;
  FixedPoint &operator=(FixedPoint const &) = default;
  FixedPoint &operator=(FixedPoint &&) = default;
  ~FixedPoint() = default;
  static constexpr unsigned unit_bits_ = 3;
  static constexpr unsigned frac_bits_ = 61;

  constexpr FixedPoint &operator+=(FixedPoint rhs) noexcept {
    assert((num_ + rhs.num_) > num_);
    num_ += rhs.num_;
    return *this;
  }

  constexpr FixedPoint &operator-=(FixedPoint rhs) noexcept {
    assert(rhs.num_ < num_);
    num_ -= rhs.num_;
    return *this;
  }

  constexpr FixedPoint &operator/=(uint32_t rhs) noexcept {
    num_ /= rhs;
    return *this;
  }

  constexpr FixedPoint &operator*=(FixedPoint rhs) noexcept {
    unsigned __int128 res = __int128(num_) * rhs.num_;
    num_ = res >> frac_bits_;
    return *this;
  }

  constexpr uint32_t operator*(uint32_t rhs) noexcept {
    unsigned __int128 res = __int128(num_) * rhs;
    return res >> frac_bits_;
  }

  constexpr bool operator==(FixedPoint rhs) noexcept {
    return num_ == rhs.num_;
  }

private:
  std::uint64_t num_;
};

constexpr FixedPoint operator+(FixedPoint lhs, FixedPoint rhs) noexcept {
  lhs += rhs;
  return lhs;
}

constexpr FixedPoint operator-(FixedPoint lhs, FixedPoint rhs) noexcept {
  lhs -= rhs;
  return lhs;
}

constexpr FixedPoint operator/(FixedPoint lhs, unsigned rhs) noexcept {
  lhs /= rhs;
  return lhs;
}

constexpr FixedPoint operator*(FixedPoint lhs, FixedPoint rhs) noexcept {
  lhs *= rhs;
  return lhs;
}

static constexpr FixedPoint fp4 = FixedPoint(4, 0);
static constexpr FixedPoint fp0 = FixedPoint(0, 0);
static constexpr FixedPoint fp1 = FixedPoint(1, 0);
static constexpr FixedPoint fp0_5 = fp1 / 2U;

int main(int argc, char **argv) {
  // Range of values for k.
  FixedPoint start(fp0);
  FixedPoint end(fp4);

  // How many iterations to do.  We do warmup number of steps to try and get to
  // the steady state and then plot `points` iterations.
  unsigned warmup = 10000;
  unsigned points = 100;

  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
    std::clog << "Unable to initialise SDL\n";
    return 1;
  }

  // Set up SDL.  Pick the size of the display mode as the default window size.
  // Then use the Renderer to give us the window size - so that we handle cases
  // (normally HiDPI) where the size we request is not what we want to use.
  SDL_DisplayMode dm;
  if (SDL_GetCurrentDisplayMode(0, &dm) < 0) {
    std::clog << "Unable to get display stats.\n";
    SDL_Quit();
    return 1;
  }

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  if (SDL_CreateWindowAndRenderer(
          dm.w, dm.h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN, &window,
          &renderer) < 0) {
    std::clog << "Window could not be created! SDL_Error: " << SDL_GetError()
              << "\n";
    SDL_Quit();
    return 1;
  }

  int w = 0;
  int h = 0;
  if (SDL_GetRendererOutputSize(renderer, &w, &h) < 0) {
    std::clog << "Window size can not be determined! SDL_Error: "
              << SDL_GetError() << "\n";
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 1;
  }

  assert(w > 0);
  uint32_t width(w);
  assert(h > 0);
  uint32_t height(h);

  // Clear the screen.
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  FixedPoint step = (end - start) / width;
  FixedPoint k(start);

  // For each value of k (i is horizontal position).
  for (unsigned i = 0; i < width; ++i) {
    FixedPoint x(fp0_5);

    for (unsigned j = 0; j < warmup; ++j) {
      x = k * x * (fp1 - x);
    }

    for (unsigned j = 0; j < points; ++j) {
      x = k * x * (fp1 - x);
      SDL_RenderDrawPoint(renderer, i, (height - 1) - x * height);
    }

    k += step;
  }

  SDL_RenderPresent(renderer);

  bool quit = false;
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}