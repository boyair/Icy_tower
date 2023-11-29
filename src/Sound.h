#pragma once
#include <SDL2/SDL_mixer.h>
#include <cstdint>
#include <string>
#include "Timer.h"
class Sound
{

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
        

    private:
        Timer playtime;
        bool played_once = false;
        Mix_Chunk* sound;
        int volume;
        int channel = 0;

};


class Music
{
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



    private:
        Mix_Music* music;
        int volume;
        int channel;
};


