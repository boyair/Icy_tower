#pragma once
#include "Drawable.h"
#include "Text.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <memory>
#include <string>

class Button {

public:
  Button(std::shared_ptr<Drawable> visual);
  Button(const Button &other);

  void HandleEvent(
      const SDL_Event &event); // handle the click event by running Onclick
  bool Hovered();
  void Draw();
  ~Button();

private:
  std::shared_ptr<Drawable> visual;

public:
  std::function<void()> on_click =
      nullptr; // the function that runs when button is
               // clicked. can be accessed publicly.
};
