#include "Animation.h"
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <strings.h>


Animation::Animation(size_t size,SDL_Rect rect,const Window& wnd)
: rect(rect)
{
    if(size < 1)
        size = 1;

    images.reserve(size);
    for (int i = 0;i < size; i++)
        images.emplace_back(rect,wnd);
    timer.Start();
}
Animation::Animation(size_t size,SDL_Rect rect,const Window& wnd, const std::string& folder_path)
: rect(rect)

{
    if(size < 1)
        size = 1;
    
    images.reserve(size);
    for (int i = 0;i < size; i++)
        images.emplace_back(folder_path + "/" + std::to_string(i) + ".png" ,rect,wnd);
    timer.Start();


}

    std::vector<Texture> images;   
    Timer timer;
    uint32_t TPI = 100000; //"time per image" the time for each image to be desplayed.
    int pause_index = -1;
    bool back_and_forth = false;
    uint32_t ChooseTexture();
    uint32_t time_diff = 0;

Animation::Animation(const Animation& other)
:
images(other.images),
rect(other.rect),
TPI(other.TPI),
back_and_forth(other.back_and_forth),
time_diff(other.time_diff + other.timer.PassedTime().count())

{
    timer.Start();
}

Animation::Animation(Animation&& other)
:
images(std::move(other.images)),
rect(other.rect),
TPI(other.TPI),
back_and_forth(other.back_and_forth),
time_diff(other.time_diff + other.timer.PassedTime().count())

{
    timer.Start();
}


void Animation::operator = (const Animation& other)
{
    images = other.images;
    rect  = other.rect;
    TPI = other.TPI;
    back_and_forth = other.back_and_forth;
    time_diff = other.time_diff + other.time_diff + other.timer.PassedTime().count();
    timer.Start();
}

void Animation::operator = (Animation&& other)
{

    images = std::move(other.images);
    rect  = other.rect;
    TPI = other.TPI;
    back_and_forth = other.back_and_forth;
    time_diff = other.time_diff + other.time_diff + other.timer.PassedTime().count();
    timer.Start();


}

void Animation::Draw()
{

        images[ChooseTexture()].rect = rect;
        images[ChooseTexture()].Draw();
}



void Animation::DrawOnWindow()
{
        images[ChooseTexture()].rect = rect;
        images[ChooseTexture()].DrawOnWindow();
}

void Animation::DrawEX(float angle,SDL_RendererFlip flip)
{
        images[ChooseTexture()].rect = rect;
        images[ChooseTexture()].DrawEX(angle,flip);
}



void Animation::DrawEXOnWindow(float angle,SDL_RendererFlip flip)
{
        images[ChooseTexture()].rect = rect;
        images[ChooseTexture()].DrawEXOnWindow(angle,flip);
}
void Animation::Pause(int index)
{
    if(pause_index == -1)
    {
        pause_index = ChooseTexture();
        time_diff = timer.PassedTime().count() + time_diff;
    }
}
void Animation::Resume(int index)
{
    if(pause_index != -1)
    {
        pause_index = -1;
        timer.Start();
    }

}



void Animation::SetTimePerImage(size_t MicroSeconds)
{
    time_diff = (timer.PassedTime().count() + time_diff) * MicroSeconds/TPI;

    TPI = MicroSeconds;
    timer.Start();
}

void Animation::SetBackAndForth(bool back_and_forth)
{
    this->back_and_forth = back_and_forth;
}

void Animation::SetTexture(size_t index, const std::string& file)
{
    if(index < images.size())
        images[index] = file;

}

int lastindex = -1;

uint32_t Animation::ChooseTexture()
{
    if (images.size() == 1) 
        return 0;
    if(pause_index >= 0 )
        return pause_index;

/*
NOTE:
the calculation for back and forth is done by calculating the index of a vector twice
the size and than if the index surpass the array size calculate it by reducing it from
twice the array size.
 */


    int time = timer.PassedTime().count() + time_diff;

    if (back_and_forth)
    {

        
         int index = (time % (TPI * (images.size() * 2 - 2))) / TPI ;
         if (index >= images.size())
             index = (images.size() - 1) * 2 - index;
         return index;
    }
    else
        return (time % (TPI * images.size())) / TPI;
}


