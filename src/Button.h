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

        
        public:
        Button (Window& window);
        Button(const std::string& text,SDL_Rect rect ,unsigned int thickness, Window& window);
        Button(const std::string& text,const std::string& font_file_path,SDL_Rect rect ,unsigned int thickness, Window& window);
        Button(const std::string& text,SDL_Color color,SDL_Rect rect ,unsigned int thickness, Window& window);
        Button(const std::string& text,SDL_Color color,const std::string& font_file_path,SDL_Rect rect ,unsigned int thickness, Window& window);
 
        void HandleEvent(const SDL_Event& event);//handle the click event by running Onclick 
        bool Hovered();
        void ChangeRectColor(SDL_Color color);
        void Draw();
        ~Button();

        private:
        unsigned int frame_thickness;// how thick (in pixels) is the farme of the button
        SDL_Color frame_color{255,255,255,255};// the color of the frame.
        public:
        std::function<void()> on_click; //the function that runs when button is clicked. can be accessed publicly.

    };
