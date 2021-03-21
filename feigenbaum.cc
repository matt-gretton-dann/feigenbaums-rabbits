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
#include <iostream>

int main(int argc, char **argv) {
  // Range of values for k.
  double start = 0.0;
  double end = 4.0;

  // How many iterations to do.  We do warmup number of steps to try and get to
  // the steady state and then plot `points` iterations.
  unsigned warmup = 1000;
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

  int width = 0;
  int height = 0;
  if (SDL_GetRendererOutputSize(renderer, &width, &height) < 0) {
    std::clog << "Window size can not be determined! SDL_Error: "
              << SDL_GetError() << "\n";
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 1;
  }

  // Clear the screen.
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  double step = (end - start) / width;
  double k = 0;

  // For each value of k (i is horizontal position).
  for (unsigned i = 0; i < width; ++i) {
    double x = 0.5;

    for (unsigned j = 0; j < warmup; ++j) {
      x = k * x * (1 - x);
    }

    for (unsigned j = 0; j < points; ++j) {
      x = k * x * (1 - x);
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