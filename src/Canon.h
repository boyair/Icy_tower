#pragma once
#include "Entity.h"
#include "PEntity.h"
#include "Texture.h"
#include <SDL2/SDL_render.h>
class Canon: public PEntity
{

    static Mix_Chunk* shotsound;
    static Mix_Chunk* reloadsound;
    static Texture texture;
    float power = 1.0f;
    SDL_RendererFlip direction = SDL_FLIP_NONE;
    bool loaded = true;
    PEntity ball;
    



    

    public:
    static float gravity ;

    Canon(SDL_Rect rect,SDL_RendererFlip direction,Window& window);
    void Update(unsigned int microseconds);
    void Reload();
    void Shot();
    void Change_Power(float power);

    Side PhysicsCollision(PEntity& other);
    Side PhysicsCollision(const SDL_Rect& other,float friction_cof,float elasticity);
    void Draw();
    bool InTrajectory(SDL_Rect rect);
    bool BallLeftScreen();
    inline bool IsLoaded(){return loaded;}
    
    private:
    
    void FitBall();
};
