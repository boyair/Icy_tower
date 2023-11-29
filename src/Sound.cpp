#include "Sound.h"
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_mixer.h>
#include <cstring>
#include <iostream>

static constexpr uint32_t MAX_CHANNELS = 8;
static Mix_Chunk* available_channels[MAX_CHANNELS] = {nullptr};


void Sound::CutAll()
{
    Mix_HaltChannel(-1);
    memset(available_channels, 0, sizeof(Mix_Chunk*) * MAX_CHANNELS);
}

//sound class functions implemetations:

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
    
    return available_channels[channel] == sound;
}

int Sound::Channel()
{
    return channel;
}

void Sound::Play(int loops,int volume)
{
    if(sound)
    {
        playtime.Start();
        played_once = true;
        channel = Mix_PlayChannel(-1, sound, loops);
        Mix_Volume(channel,volume);
        available_channels[channel] = sound;
        Mix_ChannelFinished([](int channel){available_channels[channel] = nullptr;});
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
    if(available_channels[channel] == sound)
        Mix_HaltChannel(channel);
}


Sound::~Sound()
{
    if(sound)
        Mix_FreeChunk(sound);
}








//------------------------------------
//music class functions implemetations:

Music::Music()
:music(0),
volume(0)
{
}

Music::Music(const std::string& file,int volume)
    :volume(volume)

{

    try{
        music = Mix_LoadMUS(file.c_str());
        if(music == 0)
            throw file;
    }
    catch(const std::string& str)
    {
        std::cerr<<"music file could not be opened: "<<file<<std::endl; 
    }

}
void Music::operator = (const std::string& file)
{
    if(music)
        Mix_FreeMusic(music);
    try{
        music = Mix_LoadMUS(file.c_str());
        if(music == 0)
            throw file;
    }
    catch(const std::string& str)
    {
        std::cerr<<"music file could not be opened: "<<file<<std::endl; 
    }

}

bool Music::IsPlaying()
{
    return Mix_Playing(channel);
}



void Music::Play(int loops,int volume)
{
    if(music)
    {
        channel = Mix_PlayMusic(music, loops);
        Mix_Volume(channel,volume);
    }
    else 
    {
        std::cerr<<"Invalid music cant be played!"<<std::endl;
    }
}
void Music::Play(int loops) {Play(loops,volume );}

void Music::ChangeVolume(int newVolume,bool instant)
{
    volume = newVolume;
    if(instant)
        Mix_Volume(channel, volume);
}
void Music::Pause()
{
    Mix_PausedMusic();
}


Music::~Music()
{
    if(music)
        Mix_FreeMusic(music);
}

