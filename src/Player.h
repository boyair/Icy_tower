#pragma once
#include "PEntity.h"
#include "Window.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>


class Player : public PEntity
{
    public:
    Player(SDL_Rect rect,Window& window);
    void HandleInput(const SDL_Event& event);
    void Draw();
    void SetLookDiraction();
    void LimitXSpeed(float speed);
    void LimitXpos(int min,int max);
    private: 
    SDL_RendererFlip direction;
    Mix_Chunk* jumpsound = 0;
    
};
