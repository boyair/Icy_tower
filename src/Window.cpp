#include "Window.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdexcept>
Window::Window(const char* title,const SDL_Rect& rect,Uint32 flags)
:width(rect.w),
height(rect.h),
CameraView{0,0,rect.w,rect.h}
{
  wnd = SDL_CreateWindow(title, rect.x, rect.y,rect.w,rect.h, SDL_WINDOW_SHOWN|flags);
    if (!wnd)
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;


   Renderer = SDL_CreateRenderer(wnd, -1,SDL_RENDERER_ACCELERATED);
  if (!Renderer)
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
    SDL_SetRenderDrawBlendMode(Renderer,SDL_BLENDMODE_BLEND);
}

SDL_Point Window::GetRelativePoint(SDL_Point original)
{
    original.x -= CameraView.x;
    original.y -= CameraView.y;
    original.x *= static_cast<float>(width) /CameraView.w;
    original.y *= static_cast<float>(height)/CameraView.h;
    return original; 
}

SDL_Rect Window::GetRelativeRect(SDL_Rect original)
{
    original.x -= CameraView.x;
    original.y -= CameraView.y;
    original.x *= static_cast<float>(width)/CameraView.w;
    original.y *= static_cast<float>(height)/CameraView.h;
    original.w *= static_cast<float>(width)/CameraView.w;
    original.h *= static_cast<float>(height)/CameraView.h;
    return original;

}

void Window::Show()
{
  SDL_RenderPresent(Renderer);
}


void Window::SetBGColor(unsigned char red,unsigned char green,unsigned char blue)
{

SDL_SetRenderDrawColor(Renderer,red,green,blue,255);

}
void Window::HandleEvent(const SDL_Event& event)
{
    //testing if the window event came from the right window
    SDL_Window* eventwindow = SDL_GetWindowFromID(event.window.windowID);
    if(wnd != eventwindow)
        return;

    //handeling window resize by changing width and height
    if(event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
    {
        SDL_GetWindowSize(wnd, &width, &height);
    }
}
SDL_Rect Window::ScaleRect(SDL_Rect rect) const
{

                return {(int)(rect.x * ((float)width/CameraView.w)),(int)(rect.y * ((float)height/CameraView.h)),
                    (int)(rect.w * ((float)width/CameraView.w)),(int)(rect.h * ((float)height/CameraView.h)) };


}
void Window::DrawLine(SDL_Point A,SDL_Point B,SDL_Color color)
{
            A.x -= CameraView.x;
            A.y -= CameraView.y;
            A.x *= static_cast<float>(width) /CameraView.w;
            A.y *= static_cast<float>(height)/CameraView.h;
            B.x -= CameraView.x;
            B.y -= CameraView.y;
            B.x *= static_cast<float>(width) /CameraView.w;
            B.y *= static_cast<float>(height)/CameraView.h;

    SDL_Color temp;
    SDL_GetRenderDrawColor(Renderer, &temp.r, &temp.g, &temp.b, &temp.a);

    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawLine(Renderer, A.x, A.y, B.x, B.y);
    SDL_SetRenderDrawColor(Renderer, temp.r, temp.g, temp.b, temp.a);
  
}


    SDL_Point Window::GetMousePos()
{
    SDL_Point Pos;
    SDL_GetMouseState(&Pos.x,&Pos.y);
    Pos.x /= static_cast<float>(width)/CameraView.w;
    Pos.y /= static_cast<float>(height)/CameraView.h;
 
    Pos.x += CameraView.x;
    Pos.y += CameraView.y;

    return Pos;
}

void Window::Clear()
{
  SDL_RenderClear(Renderer);
}
//camera management

void Window::ResizeCamera(int x,int y)
{
    CameraView.w = x;
    CameraView.h = y;
}

void Window::RepositionCamera(int x,int y)
{
    CameraView.x = x;
    CameraView.y = y;


}

void Window::MoveCamera(int dx, int dy)
{
    CameraView.x += dx;
    CameraView.y += dy;
}








Window::~Window()
{
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(wnd);
}
