#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <cmath>
#include <math.h>
#include <optional>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <memory>
#include "Animation.h"
#include "Texture.h"
#include "Window.h"

class Entity
{
 
        public:
        Entity(const Texture& texture, SDL_Rect rect);
        Entity(const std::string& texture, SDL_Rect rect,Window& wnd);
        Entity(SDL_Rect rect,Window& wnd);
        Entity(const Entity& other);
        Entity(Entity&& other);

        void operator=(const Entity& other); 
        bool operator==(const Entity& other); 


        //Texture related functions
        void Draw();
        void DrawEX(float angle,SDL_RendererFlip flip);
        void ChangeTexture(const Texture& texture);
        void ChangeTexture(Texture&& texture);

        //space modification
        void Resize(SDL_Point size);
        void Resize(int w,int h);
        void Repos(SDL_Point pos);
        void Repos(int x,int y);
        void Move(SDL_Point delta);
        void Move(int x,int y);
        void Stop();

        //physics
        virtual void Update(uint32_t microseconds);
        Side AvoidCollision(const SDL_Rect& other);
       

        //Information
        Side CheckCollision(const SDL_Rect& other);
        bool IsStanding(){return standing;}
        bool Standing() const;
        SDL_Point GetSize();
        

        ~Entity();
    

        public:
        Texture texture;
        std::optional<Animation> animation;// optional animation for player will overwrite the texture.
        Vec2 position;
        Vec2 velocity;
        Vec2 acceleration;
        SDL_Rect hitbox; // player hitbox fits top left to position each update.
 

    protected: 
        bool standing;//check if entity is standing on enother entity (applies in collision and resets on update.)

  };


// when placing multiple entitys on top of each other its necessary to
// calculate collisions from bottom to top to prevent small visual glitches
void SortByHeight(std::vector<Entity*>& entitys); 
