#include "Button.h"
#include "Texture.h"
#include "Window.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
Button::Button(Window& window)
:Text("Hello world!","../fonts/font.ttf",window,{0,0,200,100})
{
}


Button::Button(const std::string& text,SDL_Rect rect ,unsigned int thickness, Window& window)
:Text(text ,"../fonts/font.ttf",window,rect),
frame_thickness(thickness)
{
    
}

Button::Button(const std::string& text,const std::string& font_file_path,SDL_Rect rect ,unsigned int thickness, Window& window)
:Text(text ,font_file_path,window,rect),
frame_thickness(thickness)
{
    
}
Button::Button(const std::string& text,SDL_Color color,SDL_Rect rect ,unsigned int thickness, Window& window)
:Text(text ,color,"../fonts/font.ttf",window,rect),
frame_thickness(thickness)
{
    
}

Button::Button(const std::string& text,SDL_Color color,const std::string& font_file_path,SDL_Rect rect ,unsigned int thickness, Window& window)
:Text(text ,color,font_file_path,window,rect),
frame_thickness(thickness)
{
    
}
bool Button::Hovered()
{
    int mx,my;
    SDL_GetMouseState(&mx, &my);
    SDL_Rect rect = texture.window->ScaleRect(texture.rect);
    return mx > rect.x && mx < rect.w + rect.x &&
           my > rect.y && my < rect.h + rect.y;     
    
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


void Button::ChangeRectColor(SDL_Color color)
{
    rect_color = color;

}





void Button::Draw()
{
    //creates temporary rectangle to draw the frame of the button
    SDL_Rect temp = texture.window->ScaleRect( texture.rect);
    temp.x -= ((texture.rect.w)/10);
    temp.y -= ((texture.rect.h)/10);
    temp.w += ((texture.rect.w)/5);
    temp.h += ((texture.rect.h)/5);
    
    //get the original renderer color so that it can be reverted after drawing the frame.
    SDL_Color original_color;
    SDL_GetRenderDrawColor(texture.window->Renderer, &original_color.r, &original_color.g, &original_color.b, &original_color.a);

    SDL_SetRenderDrawColor(texture.window->Renderer, rect_color.r,rect_color.g,rect_color.b,rect_color.a);
    for (int i=0;i<frame_thickness * ((float)texture.window->height/texture.window->CameraView.h);i++)
    {
        SDL_RenderDrawRect(texture.window->Renderer,&temp);
        temp.x--;
        temp.y--;
        temp.w +=2;
        temp.h +=2;
    }

    Text::Draw();
    SDL_SetRenderDrawColor(texture.window->Renderer, original_color.r, original_color.g, original_color.b, original_color.a);
}




Button::~Button()
{
}
