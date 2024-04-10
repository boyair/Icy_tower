#pragma once
#include "Window.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <algorithm>
#include <memory>
#include <utility>
class Drawable {
public:
  Drawable(SDL_Rect rect, const Window &wnd) : rect(rect), window(&wnd) {}
  Drawable(const Drawable &other) : rect(other.rect), window(other.window) {}
  virtual void Draw(){};
  virtual void DrawEX(float angle, SDL_RendererFlip flip){};
  virtual void DrawOnWindow(bool scale){};
  virtual void DrawEXOnWindow(bool scale, float angle, SDL_RendererFlip flip){};
  virtual ~Drawable(){};
  inline bool OnScreen() {
    return SDL_HasIntersection(&rect, &window->CameraView);
  }
  virtual std::unique_ptr<Drawable> Clone() const {
    return std::make_unique<Drawable>(*this);
  }; // makes copiyng unknown type base classes easier because
     // virtual constructors are not allowed

protected:
public:
  const Window *window;
  SDL_Rect rect;
};
