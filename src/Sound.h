#pragma once
#include <SDL2/SDL_mixer.h>
#include <cstdint>
#include <string>
class Sound
{

    Mix_Chunk* sound;

    int volume;
    int channel = 0;

    public:
    static void CutAll();
    Sound();
    Sound(const std::string& file,int volume);
    void Play(int loops);
    void Play(int loops,int volume);
    void ChangeVolume(int newVolume,bool instant);
    void Cut();
    bool IsPlaying();
    int Channel();
    void operator = (const std::string& file);
    ~Sound();
};


class Music
{
    Mix_Music* music;
    int volume;
    int channel;
    

    public:
    static void CutAll();
    Music();
    Music(const std::string& file,int volume);
    void Play(int loops);
    void Play(int loops,int volume);
    void Pause();
    void Resume();

    void ChangeVolume(int newVolume,bool instant);
    bool IsPlaying();
    void operator = (const std::string& file);
    ~Music();

};


