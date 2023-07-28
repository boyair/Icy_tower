#include "Button.h"
#include "Texture.h"
#include "Window.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cstring>
#include <iostream>
Button::Button(Window& wnd)
    :
    rect{100,100,100,100},
    text("Hello World"),
    font(TTF_OpenFont("../fonts/Hey_Comic.ttf", 100)),
    texture(wnd,rect)
{
    if(!font)
    {
        if(strcmp(SDL_GetError(), "Library not initialized") == 0)
        {
            TTF_Init();
            font = TTF_OpenFont("../fonts/Hey_Comic.ttf", 100);
        }
        else
            std::cout<<"font not loaded: "<<SDL_GetError()<<std::endl;

    }
    SDL_Surface* tempsurf = TTF_RenderText_Solid(font, text.c_str(), text_color);
    texture = (tempsurf);

    SDL_FreeSurface(tempsurf);
}

bool Button::Hovered()
{
    int mx,my;
    SDL_GetMouseState(&mx, &my);
    return mx > rect.x && mx < rect.w + rect.x && my > rect.y && my < rect.h + rect.y;     
    
}

void Button::HandleEvent(const SDL_Event& event)
{
    if (OnClick && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT )
    {
      if(Hovered())
        {
            OnClick();
        }
    }
}



void Button::Reposition(SDL_Point position)
{

    rect.x = position.x;
    rect.y = position.y;
    texture.rect.x = rect.x;
    texture.rect.y = rect.y;
}



void Button::Resize(SDL_Point size)
{

    rect.x -= (size.x - rect.w)/2;
    rect.y -= (size.y - rect.h)/2;
    rect.w = size.x;
    rect.h = size.y;
    texture.rect = rect;
}


void Button::ChangeTextColor(SDL_Color color)
{

    text_color = color;
    SDL_Surface* tempsurf = TTF_RenderText_Solid(font, text.c_str(), text_color);
    texture = (tempsurf);
    SDL_FreeSurface(tempsurf);
}
void Button::ChangeRectColor(SDL_Color color)
{
    rect_color = color;

}





void Button::Draw()
{
    SDL_Rect temp = rect;
    temp.x -= ((rect.w)/10);
    temp.y -= ((rect.h)/10);
    temp.w += ((rect.w)/5);
    temp.h += ((rect.h)/5);
    SDL_Color original_color;
    SDL_GetRenderDrawColor(texture.window->Renderer, &original_color.r, &original_color.g, &original_color.b, &original_color.a);

    SDL_SetRenderDrawColor(texture.window->Renderer, text_color.r,text_color.g,text_color.b,text_color.a);
    for (int i=0;i<20;i++)
    {
        SDL_RenderDrawRect(texture.window->Renderer,&temp);
        temp.x--;
        temp.y--;
        temp.w +=2;
        temp.h +=2;
    }

    SDL_SetRenderDrawColor(texture.window->Renderer, text_color.r, text_color.g, text_color.b, text_color.a);
    texture.Draw();
    SDL_SetRenderDrawColor(texture.window->Renderer, original_color.r, original_color.g, original_color.b, original_color.a);
}


void Button::SetFunctionality(std::function<void()> function)
{
    OnClick = function; 
}


