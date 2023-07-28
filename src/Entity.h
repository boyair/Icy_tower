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

class Entity
{
    public:
        Texture texture;
        Vec2 position;
        Vec2 velocity;
        Vec2 acceleration;
        SDL_Rect hitbox;
 

    protected: 
        bool standing;

 
        public:
        Entity(const Texture& texture, SDL_Rect rect);
        Entity(const std::string& texture, SDL_Rect rect,Window& wnd);
        Entity(const Entity& other);

        Entity(Entity&& other);
        ~Entity();

        //Texture related functions
        void Draw();
        void Draw(Window& wnd);
        void ChangeTexture(const Texture& texture);
        void ChangeTexture(Texture&& texture);

        //space modification
        void Resize(SDL_Point size);
        void Resize(int w,int h);
        void Repos(SDL_Point pos);
        void Move(SDL_Point delta);
        void Repos(int x,int y);
        void Move(int x,int y);

        //physics
        void Update(unsigned int microseconds);
        Side AvoidCollision(const SDL_Rect& other);
       

        //Information
        Side CheckCollision(const SDL_Rect& other);
        bool IsStanding(){return standing;}
        const bool Standing() const;
        SDL_Point GetSize();
  };


void SortByHeight(std::vector<Entity*>& entitys);
