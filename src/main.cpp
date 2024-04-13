#include "Game.h"
#include "Timer.h"
#include <cstdint>
#include <string>
#define FPS 250
#define FRAME_TIME 1000000 / FPS

int InitSDL2();
void SetPaths(const char *argv_0);

// paths for accessing resources.
std::string EXEpath;
std::string texturefolder;
std::string animationfolder;
std::string soundfolder;
std::string fontfolder;

int main(int argc, char **argv) {

  SetPaths(argv[0]);

  // enclose game object inside a scope so that the destructors of game members
  // are called before quitting SDL.
  {
    // initialize SDL2
    int SDLInitLog = InitSDL2();
    if (SDLInitLog)
      return SDLInitLog;
    Game game;
    Timer timer;

    // start menu loop before game is running.
    uint32_t last_iteration_time = 2000;

    // full game loop including death screen.
    while (!game.AppQuit()) {
      timer.Start();
      game.Run(last_iteration_time);
      timer.WaitUntilPassed(FRAME_TIME);
      last_iteration_time = timer.PassedTime().count();
    }
  }
  // quiting game.
  SDL_Log("game ended.");
  SDL_Quit();
  SDL_Log("quit sdl!");

  return 0;
}

void SetPaths(const char *argv_0) {
  EXEpath = argv_0;
  // find last slash or backslash.
  size_t last_slash_index = std::string::npos;
  last_slash_index = EXEpath.find_last_of('/');
  if (last_slash_index == std::string::npos)
    last_slash_index = EXEpath.find_last_of('\\');
  if (last_slash_index == std::string::npos)
    EXEpath = "./";
  else
    EXEpath = EXEpath.substr(0, last_slash_index) + '/';

  texturefolder = EXEpath + "../textures/";
  animationfolder = EXEpath + "../animations/";
  soundfolder = EXEpath + "../sounds/";
  fontfolder = EXEpath + "../fonts/";
}

int InitSDL2() {
  // if this is the linux build prefer wayland naitive windows.
#ifndef _WIN32
  SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");

#endif // !_WIN32
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("Failed to initialize SDL2: %s", SDL_GetError());
    return -1;
  }

  if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 4, 1024) != 0) {
    SDL_Log("Failed to initialize audio %s", SDL_GetError());
    return -2;
  }
  if (TTF_Init() != 0) {
    SDL_Log("Failed to initialize fonts %s", SDL_GetError());
    return -3;
  }
  return 0;
}
