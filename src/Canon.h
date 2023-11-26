#pragma once
#include "Entity.h"
#include "PEntity.h"
#include "Texture.h"
#include <SDL2/SDL_render.h>
#include "Timer.h"
#include "Sound.h"

class Canon: public PEntity
{



    

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
    
    static void LoadSounds();//function to load reload and shot sound that needs to be called after SDL2 initiallized.
    private:
    
    void FitBall();
private:
//sounds
static Sound shotsound;
static Sound reloadsound;
private:
float power = 1.0f;//how strong the canon shoots.
SDL_RendererFlip direction = SDL_FLIP_NONE;//where the canon points (no flip = right)
bool on_reload_process = false;// boolean to check if the canon is during reload.
bool loaded = true;//boolean to determine whether the canon is loaded.
bool can_do_damage = true;//boolean used to prevent multiple damage caused by a single shot.
Timer reload_timer;//the timer runs on reload to know how long its been since start of reload.
};
