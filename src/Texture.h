#pragma once
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
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
        Texture(const std::string& file,SDL_Rect rect,const Window& wnd);
        Texture(SDL_Rect rect,const Window& wnd);
        Texture(const Texture& other);
        Texture(Texture&& other);

        inline void Draw()
        {
            if(SDL_HasIntersection(&rect ,&window->CameraView))
            {
                if(SDL_HasIntersection(&rect, &window->CameraView))
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
        }

        inline bool OnScreen() {return SDL_HasIntersection(&rect ,&window->CameraView);}
        inline void DrawOnWindow(bool scale)
        {
            if (scale)
            {
                SDL_Rect scaled_rect = window->ScaleRect(rect);
                SDL_RenderCopy(window->Renderer,texture,0,&scaled_rect);
            }
            else
            SDL_RenderCopy(window->Renderer,texture,0,&rect);
        }
       


        void Tint(unsigned char r,unsigned char g,unsigned char b){SDL_SetTextureColorMod(texture,r,g,b);}
        inline void DrawEXOnWindow(bool scale,float angle,SDL_RendererFlip flip)
        {

            if (scale)
            {
                SDL_Rect scaled_rect = window->ScaleRect(rect);
                SDL_RenderCopyEx(window->Renderer,texture,0,&scaled_rect,angle,0,flip);
            }
            else
            SDL_RenderCopyEx(window->Renderer,texture,0,&rect,angle,0,flip);
        }

        inline void DrawEX(float angle,SDL_RendererFlip flip)
        {
            if(SDL_HasIntersection(&rect, &window->CameraView))
            {
                SDL_Rect rect_on_cam = rect;
                rect_on_cam.x -= window->CameraView.x;
                rect_on_cam.y -= window->CameraView.y;
                rect_on_cam.x *= static_cast<float>(window->width)/window->CameraView.w;
                rect_on_cam.y *= static_cast<float>(window->height)/window->CameraView.h;
                rect_on_cam.w *= static_cast<float>(window->width)/window->CameraView.w;
                rect_on_cam.h *= static_cast<float>(window->height)/window->CameraView.h;


                SDL_RenderCopyEx(window->Renderer,texture,0,&rect_on_cam,angle,0,flip);
            }
        }
        Vec2 Rotate(float angle , SDL_RendererFlip flip , bool resize);
        void operator= (const Texture& other);
        void operator= (SDL_Surface* other);
        void operator= (SDL_Texture* other);
        void operator= (Texture&& other);
        void operator= (const std::string& file);
        
        SDL_Texture* s_tex() const;
        ~Texture();


};
