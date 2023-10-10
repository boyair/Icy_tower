#pragma once
#include <SDL2/SDL_mixer.h>
#include "Animation.h"
#include "Player.h"
#include "Sound.h"
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
       
        //window and window info
        Window window;
        float cameraheight = 0;
        float cameraspeed = 0.0001;

        //entities
        Player player;
        Canon canon;
        Entity cloud;
        
        //menu buttons
        Button start_button;
        Button restart_button;
        Button quit_button;
        
        //backgrounds
        Texture bg;
        Texture death_screen_bg;

        //textures / animations presented throughout the game.
        Texture wall;
        Texture heart;
        Animation wind;
        
        //score displays
        Text score_display;
        Text death_score_display;

        
        Timer seed_generator;
        
        //sounds
        Sound wind_sound;
        Sound death_sound;
        Sound damage_sound;
        Sound button_hover_sound;
        Sound wood_crack;
        Mix_Music* bg_music;


        //platform types
        PEntity platform_default;
        PEntity platform_ice;
        PEntity platform_gum;
        PEntity platform_wood;

        //progress tracking variables 
        uint32_t platforms_created = 0;
        uint32_t platform_level = 1; //the level of the top platform (used to decide what platform should be created).
        uint32_t player_level = 1;   //the level of the player. (used for knowing when to start the windused for knowing when to start the wind)
        int score = 0;
        int lives = 3;
       

        static constexpr int platforms_per_level = 20;


        std::vector<PEntity> platforms;
        bool running = false     ;
        bool quit_app = false;




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
        void FitPlatformToLevel(PEntity& platform);
        
    };
