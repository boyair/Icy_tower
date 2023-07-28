#pragma once
#include <SDL2/SDL_rect.h>
#include <cmath>
#include <string>
#include <SDL2/SDL.h>
#include <math.h>
#include <SDL2/SDL_image.h>
#include "Window.h"
#include "Utils.h"



class Texture
{
    private:
        friend class Entity;
    public:
        const Window *window;
        SDL_Texture *texture;

        SDL_Rect rect;
        Texture(const std::string& file,const Window& wnd,SDL_Rect rect);
        Texture(const Window& wnd, SDL_Rect rect);
        Texture(const Texture& other);
        Texture(Texture&& other);

        inline void Draw(const SDL_Rect& rect)
        {
            if(SDL_HasIntersection(&rect ,&window->CameraView))
            {
                SDL_Rect rect_on_cam = rect;
                rect_on_cam.x -= window->CameraView.x;
                rect_on_cam.y -= window->CameraView.y;
                rect_on_cam.x *= static_cast<float>(window->width)/window->CameraView.w;
                rect_on_cam.y *= static_cast<float>(window->height)/window->CameraView.h;
                rect_on_cam.w *= static_cast<float>(window->width)/window->CameraView.w;
                rect_on_cam.h *= static_cast<float>(window->height)/window->CameraView.h;
                SDL_RenderCopy(window->Renderer,texture,0,&rect_on_cam);
            }
        }
         inline void Draw()
        {
            Draw(rect);
        }
        void Tint(unsigned char r,unsigned char g,unsigned char b){SDL_SetTextureColorMod(texture,r,g,b);}
        void DrawEX(const SDL_Rect& rect,float angle,SDL_RendererFlip flip){SDL_RenderCopyEx(window->Renderer,texture,0,&rect,angle,0,flip);}
        Vec2 Rotate(float angle , SDL_RendererFlip flip , bool resize);
        void operator= (const Texture& other);
        void operator= (SDL_Surface* other);
        void operator= (SDL_Texture* other);
        void operator= (Texture&& other);
        
        SDL_Texture* s_tex() const;
        ~Texture();


};
