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
  Button(Button &&other);

  void HandleEvent(
      const SDL_Event &event); // handle the click event by running Onclick
  bool Hovered();
  void Draw();
  void Scale(float scale);
  ~Button();

public:
  std::shared_ptr<Drawable> visual;
  bool was_hovered = false;
  std::function<void()> on_click = nullptr; // the function that runs when
                                            // button is clicked.
  // by default hovering scales up button by 1.2
  std::function<void()> on_start_hover = [this]() {
    Scale(1.2f);
  }; // runs when start hovering
  std::function<void()> on_stop_hover = [this]() {
    Scale(1 / 1.2f);
  }; // runs when stop hovering
};
