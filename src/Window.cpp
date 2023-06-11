#include "Window.h"
#include <SDL2/SDL_video.h>
#include <stdexcept>
Window::Window(const char* title,const SDL_Rect& rect,Uint32 flags)
{
// Initialize SDL2
  #ifndef _WIN32
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");
  #endif
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL2: %s", SDL_GetError());
        throw;
    }

  wnd = SDL_CreateWindow(title, rect.x, rect.y,rect.w,rect.h, SDL_WINDOW_SHOWN|flags);
    if (!wnd)
        std::cout << "Error creating window: " << SDL_GetError() << std::endl;


   Renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED );
  if (!Renderer)
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
}


void Window::Show()
{
  SDL_RenderPresent(Renderer);
}


void Window::SetBGColor(unsigned char red,unsigned char green,unsigned char blue)
{

SDL_SetRenderDrawColor(Renderer,red,green,blue,255);

}

    void Window::Maximize()
{
    SDL_MaximizeWindow(wnd); 


}



void Window::Clear()
{
  SDL_RenderClear(Renderer);
}











Window::~Window()
{
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(wnd);
}
