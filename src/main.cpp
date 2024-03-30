#include "Game.h"
#include "Timer.h"
#include <cstdint>
#include <functional>
#include <string>
#include <thread>
#define FPS 250
#define FRAME_TIME 1000000 / FPS

void RunPhysics(Game &game);
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
    while (!game.IsRunning() && !game.AppQuit()) {
      timer.Start();
      game.StartMenu();
      timer.WaitUntilPassed(FRAME_TIME);
    }
    uint32_t last_iteration_time = 2000;

    // full game loop including death screen.
    while (!game.AppQuit()) {
      std::thread physics_thread(RunPhysics, std::ref(game));
      // game loop while alive
      while (game.IsRunning()) {
        timer.Start();
        game.HandleInput();
        game.HandleLogic(last_iteration_time);
        game.Draw();
        timer.WaitUntilPassed(FRAME_TIME);
        last_iteration_time = timer.PassedTime().count();
      }
      physics_thread.join();
      // death screen loop
      while (!game.IsRunning() && !game.AppQuit()) {
        timer.Start();
        game.DeathScreen();
        timer.WaitUntilPassed(FRAME_TIME);
      }
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

void RunPhysics(Game &game) {

  Timer timer;
  uint32_t last_iteration_time = 500;

  while (game.IsRunning()) {
    timer.Start();
    game.RunPhysics(last_iteration_time);
    timer.WaitUntilPassed(500);
    last_iteration_time = timer.PassedTime().count();
  }
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
// fix the first platform being drawn with both frass and wood anf make git
// commit
