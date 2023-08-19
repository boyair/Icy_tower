#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

class Window
{
    SDL_Window* wnd;
  friend class Entity;
  public:
    SDL_Renderer* Renderer;
    SDL_Rect CameraView;
    int width,height;


    Window(const char* title,const SDL_Rect& rect,Uint32 flags);

    void Show();
    void Clear();
    SDL_Point GetMousePos();
    void DrawLine(SDL_Point A,SDL_Point B,SDL_Color color);
    SDL_Point GetRelativePoint(SDL_Point original);
    SDL_Rect GetRelativeRect(SDL_Rect original);
    void SetBGColor(unsigned char red,unsigned char green,unsigned char blue);
    //camera management
    void MoveCamera(int dx, int dy);
    void RepositionCamera(int x,int y);
    void ResizeCamera(int x,int y);
    ~Window();


  };
