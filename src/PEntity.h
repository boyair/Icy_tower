#pragma once
#include "Entity.h"
#include "Utils.h"
#include <SDL2/SDL_rect.h>
#include <math.h>
#include <iostream>

class PEntity: public Entity
{
    protected:
    float mass = 1;
    float friction_cof = 0.5f;
    float elasticity = 0;
   public:
        using Entity::AvoidCollision ;
        PEntity(const Texture& texture, SDL_Rect rect);
        PEntity(const std::string& texture, SDL_Rect rect,Window& wnd);
        PEntity(const PEntity& other);
        PEntity(PEntity&& other);
        void operator=(const PEntity& other); 
        inline void ApplyForce (Vec2 force){velocity += force / mass;}; 
        inline float GetDistance (const Vec2& point){SDL_Point center = Utils::GetCenter(hitbox); return sqrtf(pow(center.x-point.x,2)+pow(center.y-point.y,2));}
        void Gravitate(Vec2 point, float strength);
        Side PhysicsCollision(PEntity& otherentity);
        Side GhostPhysicsCollision(const PEntity& otherentity);
        Side PhysicsCollision(const SDL_Rect& other,float friction_cof,float elasticity);
        void Limit (const SDL_Rect& border);
        void ChangeMass(float NewMass);
        void LimitSpeed(float Limit);
        void SetRoughness(float Roughness);
        void SetElasticity(float elasticity);
    private:
        float FricCalcX(const PEntity& otherentity,float system_force);
        float FricCalcY(const PEntity& otherentity,float system_force);
};

void SortByHeight(std::vector<PEntity*>& entitys);
