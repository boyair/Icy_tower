#pragma once
#include "Window.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
class Drawable {
public:
  Drawable(SDL_Rect rect, const Window &wnd) : rect(rect), window(&wnd) {}
  Drawable(const Drawable &other) : rect(other.rect), window(other.window) {}
  virtual void Draw(){};
  virtual void DrawEX(float angle, SDL_RendererFlip flip){};
  virtual void DrawOnWindow(bool scale){};
  virtual void DrawEXOnWindow(bool scale, float angle, SDL_RendererFlip flip){};
  virtual ~Drawable(){};

public:
  const Window *window;
  SDL_Rect rect;
};
