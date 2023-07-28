#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <functional>
#include "Texture.h"
#include "Window.h"


class Button
    {
        std::string text;
        TTF_Font* font;
        unsigned int thickness;
        SDL_Color text_color{255,255,255,255};
        SDL_Color rect_color{255,255,255,255};
    public:

        SDL_Rect rect;
        Texture texture;
        std::function<void()> OnClick;
        Button (Window& wnd);
        Button(std::string text,SDL_Rect rect ,unsigned int thickness, Window& wnd);
        void HandleEvent(const SDL_Event& event);
        bool Hovered();
        void Resize(SDL_Point size);
        void ChangeTextColor(SDL_Color color);
        void ChangeRectColor(SDL_Color color);
        void Reposition(SDL_Point position);
        void SetFunctionality(std::function<void()> function);
        void Draw();
    };
