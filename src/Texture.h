#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Window.h"
class Texture
{
    private:
        friend class Entity;
        const Window *window;
        SDL_Texture *texture;

    public:
        Texture(const std::string& file,const Window& wnd);
        Texture(const Texture& other);
        Texture(Texture&& other);

        inline void Draw(const SDL_Rect& rect) {SDL_RenderCopy(window->Renderer,texture,0,&rect);}
        void DraxEX(const SDL_Rect& rect,float angle,SDL_RendererFlip flip){SDL_RenderCopyEx(window->Renderer,texture,0,&rect,angle,0,flip);};
        void Rotate(float angle , SDL_RendererFlip flip , bool resize);
        void operator= (const Texture& other);
        void operator= (SDL_Texture* other);
        void operator= (Texture&& other);
        
        SDL_Texture* s_tex() const;
        ~Texture();


};

