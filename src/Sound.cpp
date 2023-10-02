#include "Sound.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

void Sound::CutAll()
{
    Mix_HaltChannel(-1);
}
Sound::Sound()
:sound(0),
volume(0)
{
}

Sound::Sound(const std::string& file,int volume)
    :volume(volume)

{

    try{
        sound = Mix_LoadWAV(file.c_str());
        if(sound == 0)
            throw file;
    }
    catch(const std::string& str)
    {
        std::cerr<<"sound file could not be opened: "<<file<<std::endl; 
    }

}
void Sound::operator = (const std::string& file)
{
    if(sound)
        Mix_FreeChunk(sound);
    try{
        sound = Mix_LoadWAV(file.c_str());
        if(sound == 0)
            throw file;
    }
    catch(const std::string& str)
    {
        std::cerr<<"sound file could not be opened: "<<file<<std::endl; 
    }

}

bool Sound::IsPlaying()
{
    return Mix_Playing(channel);
}



void Sound::Play(int loops,int volume)
{
    if(sound)
    {
        channel = Mix_PlayChannel(-1, sound, loops);
        Mix_Volume(channel,volume);
    }
    else 
    {
        std::cerr<<"Invalid sound cant be played!"<<std::endl;
    }
}
void Sound::Play(int loops) {Play(loops,volume );}

void Sound::ChangeVolume(int newVolume,bool instant)
{
    volume = newVolume;
    if(instant)
        Mix_Volume(channel, volume);
}
void Sound::Cut()
{
    Mix_HaltChannel(channel);
}


Sound::~Sound()
{
    if(sound)
        Mix_FreeChunk(sound);
}
