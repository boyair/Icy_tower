#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <functional>
#include "Window.h"
#include "Text.h"

class Button: public Text
    {
        TTF_Font* font;
        unsigned int frame_thickness;
        SDL_Color rect_color{255,255,255,255};
    public:
        //the function that runs when button is clicked. can be accessed publicly.
        std::function<void()> OnClick;
        
        public:
        Button (Window& window);
        Button(const std::string& text,SDL_Rect rect ,unsigned int thickness, Window& wnd);
        void HandleEvent(const SDL_Event& event);
        bool Hovered();
        void ChangeRectColor(SDL_Color color);
        void Draw();
        ~Button();
    };
