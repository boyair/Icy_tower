#pragma once
#include "Entity.h"
#include "PEntity.h"
#include "Window.h"
#include <vector>
class Game
    {
    private:
        const std::string texturefolder = "../textures/";
        const std::string soundfolder = "../sounds/";
        
        Window window;
        std::vector<Entity> platforms;
        PEntity player;
        std::vector<PEntity> Boxes;
        bool quit = false;

    public:
        Game();
        void Draw();
        void InputAndPhysics();
    };
