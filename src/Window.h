#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class Window {

public:
  Window(const char *title, const SDL_Rect &rect, Uint32 flags);

  // rendering
  void Show();
  void Clear();

  // event handlind
  SDL_Point GetMousePos();
  void HandleEvent(const SDL_Event &event);

  // drawing
  void DrawLine(SDL_Point A, SDL_Point B, SDL_Color color);
  void SetBGColor(unsigned char red, unsigned char green, unsigned char blue);

  // camera management
  SDL_Point GetRelativePoint(SDL_Point original) const;
  SDL_Rect GetRelativeRect(SDL_Rect original) const;
  SDL_Rect ScaleRect(SDL_Rect rect) const;
  ~Window();

private:
  SDL_Window *wnd;
  friend class Entity;

public:
  SDL_Renderer *Renderer;
  int width, height;   // the width and height of window.
  SDL_Rect CameraView; // the camera view of the window (if changed in relation
                       // to the window, scaling applied automatically).
};
