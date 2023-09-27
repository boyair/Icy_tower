#pragma once
#include "Entity.h"
#include "PEntity.h"
#include "Texture.h"
#include <SDL2/SDL_render.h>
#include "Timer.h"
#include "Sound.h"

class Canon: public PEntity
{

    static Sound shotsound;
    static Sound reloadsound;
    public:
    static void LoadSounds();

    private:
    static Texture texture;
    float power = 1.0f;
    SDL_RendererFlip direction = SDL_FLIP_NONE;
    bool on_reload_process = false;
    bool loaded = true;
    bool can_do_damage = true;
    Timer reload_timer; 



    

    public:
    PEntity ball;
    static float gravity ;

    Canon(SDL_Rect rect,SDL_RendererFlip direction,Window& window);
    void Update(unsigned int microseconds);
    void Reload(bool play_sound);
    void Shot();
    void Change_Power(float power);
    void DisableDamage();
    void Draw();
    void SetFlip(SDL_RendererFlip flip);
    bool InTrajectory(SDL_Rect rect);
    bool BallLeftScreen();
    bool CanDoDamage();
    inline bool IsLoaded(){return loaded;}
    
    private:
    
    void FitBall();
};
