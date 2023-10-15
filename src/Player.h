#pragma once
#include "PEntity.h"
#include "Window.h"
#include "Sound.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <cstdint>


class Player : public PEntity
{
    public:
    Player(SDL_Rect rect,Window& window);
    void HandleInput(const SDL_Event& event);
    void Draw();
    void SetLookDiraction();
    void LimitXSpeed(float speed);
    void LimitXpos(int min,int max);
    bool pressing_up = false;
    bool pressing_left = false;
    bool pressing_right = false;
    
    void Update(uint32_t microseconds) override;
    int highest_platform_passed = 700;
    
    float jump_strength = 1.0f;
    

    private: 
    SDL_RendererFlip direction;
    Sound jump_sound; 
};
