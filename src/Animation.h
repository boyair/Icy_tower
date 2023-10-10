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
    Animation(size_t size,SDL_Rect rect,const Window& wnd);
    Animation(size_t size,SDL_Rect rect,const Window& wnd, const std::string& folder_path);
    Animation(const Animation& other);
    Animation(Animation&& other);
    void SetTexture(size_t index, const std::string& file);

    

    void Draw() ;
    void DrawEX(float angle,SDL_RendererFlip flip);
    void DrawOnWindow(bool scale) ;
    void DrawEXOnWindow(bool scale,float angle,SDL_RendererFlip flip);
    void Pause(int index);
    void Resume(int index);
     
    
    void SetTimePerImage(size_t MicroSeconds);
    
    void SetBackAndForth(bool back_and_forth);

    void operator = (const Animation& other);
    void operator = (Animation&& other);



    bool IsRunning();
    uint32_t CurrentImageIndex();


    private:
    std::vector<Texture> images;   
    Timer timer;
    uint32_t TPI = 100000; //"time per image" the time for each image to be desplayed.
    int pause_index = -1;
    bool back_and_forth = false;
    uint32_t ChooseTexture();
    uint64_t time_diff = 0;
    public:
    SDL_Rect rect;
};
