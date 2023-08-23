#pragma once
#include "Window.h"
#include "Texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>



class Text
{
        protected:
        std::string text;
        TTF_Font* font;
        SDL_Color color{255,255,255,255};
        Texture texture;


    public:
        Text(const std::string& text,Window& window,SDL_Rect rect);
        Text(const std::string& text,const std::string& font_file_path,Window& window,SDL_Rect rect);
        void Draw();
        
        //mofication functions:
        void ChangeColor(SDL_Color new_color); //requires recreation of texture (slow) 
        void ChangeText(const std::string& text);  //requires recreation of texture (slow) 
        void ChangeFont(const std::string& filepath);//requires recreation of texture (slow) 
        
        //space modifications (dosent require texture recreation.):
        void Reposition(SDL_Point position);
        void Resize(SDL_Point size);

        SDL_Rect GetRect(){return texture.rect;};
        
        void RecreateTexture(); //required after modification functions 

        ~Text();
    private:


};
