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
    //constructors
    Animation(size_t size,SDL_Rect rect,const Window& wnd);
    Animation(size_t size,SDL_Rect rect,const Window& wnd, const std::string& folder_path);
    Animation(const Animation& other);
    Animation(Animation&& other);
    void SetTexture(size_t index, const std::string& file);

    
    //drawing functions
    void Draw();
    void DrawEX(float angle,SDL_RendererFlip flip);
    void DrawOnWindow(bool scale) ;
    void DrawEXOnWindow(bool scale,float angle,SDL_RendererFlip flip);


    //time mangement
    void Pause(int index);
    void Resume(int index);
    void SetTimePerImage(size_t MicroSeconds);
    void SetBackAndForth(bool back_and_forth);

    
    //assingment operator to copy one animation to another.
    void operator = (const Animation& other);
    void operator = (Animation&& other);


    //information recivers.
    bool IsRunning();
    uint32_t CurrentImageIndex();


    private:
    //service functions
    uint32_t ChooseTexture();

    private:
    std::vector<Texture> images; //all the images in the texture in a vector.   
    Timer timer; //timer to manage the timing of the displayed images.
    uint32_t TPI = 100000; //"time per image" the time for each image to be desplayed.
    int pause_index = -1; //the image the user paused on when calling "Pause();" (-1 when running)
    bool back_and_forth = false; //whether the animation restart from the begining when reaching the end or going backwards.
    uint64_t time_diff = 0; //used to save the time difference when timer needs to restart.
    public:
    SDL_Rect rect;
};
