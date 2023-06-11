#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

class Window
{
    SDL_Window* wnd;
  friend class Entity;
  public:
    SDL_Renderer* Renderer;


    Window(const char* title,const SDL_Rect& rect,Uint32 flags);

    void Show();
    void Clear();
    void Maximize();

    void SetBGColor(unsigned char red,unsigned char green,unsigned char blue);
    ~Window();


  };
