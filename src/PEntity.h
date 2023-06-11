#pragma once
#include "Entity.h"
#include <math.h>
#include <iostream>

class PEntity: public Entity
{

    Vec2 prevposition;
    float mass = 1;
    int friction_cof = 500;
    //Vec2 centerofmass = position + Vec2{(float)Rect.w/2,(float)Rect.h/2};
    
   public:
        using Entity::AvoidCollision ;
        PEntity(const Texture& texture, SDL_Rect rect);
        PEntity(const std::string& texture, SDL_Rect rect,Window& wnd);

        void Update(unsigned int time);
        inline void ApplyForce (Vec2 force){velocity += force / mass;}; 
        inline float GetDistance (const Vec2& point){return sqrtf(pow(position.x-point.x,2)+pow(position.y-point.y,2));}
        void Gravitate(Vec2 point, float strength);
        void AvoidCollision(PEntity& other);
        //void AvoidCollision(const SDL_Rect& other);
        void ChangeMass(float NewMass);
        void LimitSpeed(float Limit);
        
};
