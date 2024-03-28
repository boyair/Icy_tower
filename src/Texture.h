#pragma once
#include "DrawAble.h"
#include "Utils.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <math.h>
#include <string>

class Texture : public DrawAble {
public:
  Texture(const std::string &file, SDL_Rect rect, const Window &wnd);
  Texture(SDL_Rect rect, const Window &wnd);
  Texture(const Texture &other);
  Texture(Texture &&other);

  inline void Draw() {
    if (SDL_HasIntersection(&rect, &window->CameraView)) {
      SDL_Rect rect_on_cam = window->GetRelativeRect(rect);
      SDL_RenderCopy(window->Renderer, texture, 0, &rect_on_cam);
    }
  }

  inline bool OnScreen() {
    return SDL_HasIntersection(&rect, &window->CameraView);
  }
  inline void DrawOnWindow(bool scale) {
    SDL_Rect final_rect = scale ? window->ScaleRect(rect) : rect;
    SDL_RenderCopy(window->Renderer, texture, 0, &final_rect);
  }

  void Tint(unsigned char r, unsigned char g, unsigned char b) {
    SDL_SetTextureColorMod(texture, r, g, b);
  }
  inline void DrawEXOnWindow(bool scale, float angle, SDL_RendererFlip flip) {

    if (scale) {
      SDL_Rect scaled_rect = window->ScaleRect(rect);
      SDL_RenderCopyEx(window->Renderer, texture, 0, &scaled_rect, angle, 0,
                       flip);
    } else
      SDL_RenderCopyEx(window->Renderer, texture, 0, &rect, angle, 0, flip);
  }

  inline void DrawEX(float angle, SDL_RendererFlip flip) {
    if (SDL_HasIntersection(&rect, &window->CameraView)) {
      SDL_Rect rect_on_cam = window->GetRelativeRect(rect);
      SDL_RenderCopyEx(window->Renderer, texture, 0, &rect_on_cam, angle, 0,
                       flip);
    }
  }
  Vec2 Rotate(float angle, SDL_RendererFlip flip, bool resize);
  void operator=(const Texture &other);
  void operator=(SDL_Surface *other);
  void operator=(SDL_Texture *other);
  void operator=(Texture &&other);
  void operator=(const std::string &file);

  SDL_Texture *s_tex() const;
  ~Texture();

private:
  friend class Entity;

public:
  const Window *window;
  SDL_Texture *texture;
  SDL_Rect rect;
};
