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
        Texture wall;
        Texture death_screen_bg;
        Texture heart;
        Texture turtle;
        Text ScoreDisplay;
        Timer seed_generator;

        Mix_Chunk* death_sound = 0;
        Mix_Chunk* damage_sound = 0;
        Mix_Chunk* button_hover_sound = 0;
        uint32_t platforms_created = 0;


        PEntity platform_default;
        PEntity platform_ice;
        
        std::vector<PEntity> platforms;
        bool running = false;
        bool quit_app = false;

        Player player;
        int score = 0;
        int lives = 3;


        Canon canon;
        Entity cloud;

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
        int TopPlatformPosition();
        inline int GetGoodSeed(){return seed_generator.PassedTime().count();}
        void RepositionPlatformRandomly(PEntity& platform);
        
    };
