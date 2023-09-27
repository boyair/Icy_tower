#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>


class Sound
{
    Mix_Chunk* sound;

    int volume;
    int channel;
    

    public:
    static void CutAll();
    Sound();
    Sound(const std::string& file,int volume);
    void Play(int loops);
    void Play(int loops,int volume);
    void ChangeVolume(int newVolume,bool instant);
    void Cut();
    void operator = (const std::string& file);
    ~Sound();
};
