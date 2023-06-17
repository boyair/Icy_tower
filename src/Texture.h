#pragma once
#include <SDL2/SDL_rect.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Window.h"

struct Vec2
{

    friend std::ostream& operator <<(std::ostream& out, Vec2 vector);


    float x ,y;
    void operator +=(Vec2 other)
    {
        x += other.x;
        y += other.y;
    }
    void operator -=(Vec2 other)
    {
        x -= other.x;
        y -= other.y;
    }
    Vec2 operator +(Vec2 other)
    {
        return {x + other.x , y + other.y};   
    }
    Vec2 operator -(Vec2 other)
    {
        return {x - other.x , y - other.y};   
    }

    void operator /= (Vec2 other)
    {
        x /= other.x;
        y /= other.y;
    }
    void operator /= (float num)
    {
        x /= num;
        y /= num;
    }
    void operator *= (Vec2 other)
    {
        x *= other.x;
        y *= other.y;
    }
    void operator *= (float num)
    {
        x *= num;
        y *= num;
    }



    Vec2 operator /(Vec2 other)
    {
        return {x / other.x , y / other.y};   
    }
    Vec2 operator *(Vec2 other)
    {
        return {x * other.x , y * other.y};   
    }
    Vec2 operator /(float num)
    {
        return {x / num, y / num};   
    }
    Vec2 operator *(float num)
    {
        return {x *  num, y * num};   
    }
    explicit operator float() const {return sqrt(pow(x, 2)+pow(y,2));}
    void rotate(float degrees)
    {
        float radians = degrees*3.14159265/180;
        float W = x * cos (radians) + y * sin (radians);
        float H = y * cos (radians) - x * sin (radians);
        x=W; y=H; 
    }

};



class Texture
{
    private:
        friend class Entity;
        const Window *window;
        SDL_Texture *texture;

    public:
        SDL_Rect rect;
        Texture(const std::string& file,const Window& wnd,SDL_Rect rect);
        Texture(const Texture& other);
        Texture(Texture&& other);

        inline void Draw(const SDL_Rect& rect) {SDL_RenderCopy(window->Renderer,texture,0,&rect);}
        inline void Draw() {SDL_RenderCopy(window->Renderer,texture,0,&rect);}
        void Tint(unsigned char r,unsigned char g,unsigned char b){SDL_SetTextureColorMod(texture,r,g,b);}
        void DraxEX(const SDL_Rect& rect,float angle,SDL_RendererFlip flip){SDL_RenderCopyEx(window->Renderer,texture,0,&rect,angle,0,flip);}
        Vec2 Rotate(float angle , SDL_RendererFlip flip , bool resize);
        void operator= (const Texture& other);
        void operator= (SDL_Texture* other);
        void operator= (Texture&& other);
        
        SDL_Texture* s_tex() const;
        ~Texture();


};

