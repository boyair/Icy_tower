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
        Text(const std::string& text,TTF_Font* font,Window& window,SDL_Rect rect);
        void Draw();
        void ChangeColor(SDL_Color new_color); // recreates the texture (slow) 
        void ChangeText(const std::string& text);  // recreates the texture (slow)
        void ChangeFont(const std::string& filepath);
        void Reposition(SDL_Point position);
        void Resize(SDL_Point size);
        SDL_Rect GetRect(){return texture.rect;};

        ~Text();
    private:

        void RecreateTexture();

};
