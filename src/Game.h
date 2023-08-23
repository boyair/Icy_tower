#pragma once
#include <SDL2/SDL_mixer.h>
#include "Player.h"
#include "Timer.h"
#include "Button.h"
#include "Entity.h"
#include "PEntity.h"
#include "Texture.h"
#include "Window.h"
#include "Text.h"
#include "Canon.h"
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <vector>
class Game
    {
       

        float cameraheight = 0;
        Window window;
        uint32_t iterations = 0;
        Button start_button;
        Button restart_button;
        Button quit_button;
        Texture bg;
        Texture death_screen_bg;
        Text ScoreDisplay;

        Mix_Chunk* death_sound = 0;

        std::vector<Entity> platforms;
        std::vector<PEntity> Boxes;
        bool running = false;
        bool quit_app = false;

        Player player;
        int score = 0;



        Canon canon;

    int TopPlatformIndex();
    public:
        Game();
        void Draw();
        void StartMenu();
        void HandleInput();
        void HandleLogic(uint32_t LastIterationTime);
        void RunPhysics(unsigned int LastIterationTime);
        void DeathScreen();
        void Reset();
        bool IsRunning();
        bool AppQuit();
        ~Game();
    private:
        void ResizeButtonCorrectly(Button& button,SDL_Rect original_rect);
        
    };
