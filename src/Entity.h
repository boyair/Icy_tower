#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include "Texture.h"
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

};


class Entity
{
  
    public:
        SDL_Rect Rect;
        Vec2 position;
        bool standing;

        Texture texture;
        Vec2 velocity;
        Vec2 acceleration;
  

        Entity(const Texture& texture, SDL_Rect rect);
        Entity(const std::string& texture, SDL_Rect rect,Window& wnd);
        Entity(const Entity& other);

        Entity(Entity&& other);
        ~Entity();


        void Draw();
        void Draw(Window& wnd);
        void Update(unsigned int time);
        void Resize(SDL_Point size);
        void Repos(SDL_Point pos);
        void Move(SDL_Point delta);


        void Resize(int w,int h);
        void Repos(int x,int y);
        void Move(int x,int y);
        void AvoidCollision(const SDL_Rect& other);
        void ChangeTexture(const Texture& texture);
        void ChangeTexture(Texture&& texture);
        inline bool Collides(const SDL_Rect& other){return position.x<other.x + other.w && position.x + Rect.w> other.x&&position.y<other.y + other.h && position.y + Rect.h> other.y;}

        const bool Standing() const;
        SDL_Point GetSize();
  };
