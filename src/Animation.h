#pragma once

#include "Texture.h"
#include "Timer.h"
#include <SDL2/SDL_stdinc.h>
#include <cstddef>
#include <cstdint>
#include <vector>
class Animation
{
    public:
    Animation(size_t size,const Window& wnd,SDL_Rect rect);
    Animation(size_t size,const Window& wnd,SDL_Rect rect, const std::string& folder_path);
    void SetTexture(size_t index, const std::string& file);

    

    void Draw();
    void DrawOnWindow();
    void DrawEX(float angle,SDL_RendererFlip flip);
    void DrawEXOnWindow(float angle,SDL_RendererFlip flip);
    void Pause(int index);
    void Resume(int index);
    

    void SetTimePerImage(size_t MicroSeconds);
    
    void SetBackAndForth(bool back_and_forth);





    private:
    std::vector<Texture> images;   
    Timer timer;
    uint32_t TPI = 100000; //"time per image" the time for each image to be desplayed.
    int pause_index = -1;
    bool back_and_forth = false;
    uint32_t ChooseTexture();
    uint32_t time_diff = 0;
    public:
    SDL_Rect rect;
};
