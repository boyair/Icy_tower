#pragma once
#include "Animation.h"
#include "Button.h"
#include "Canon.h"
#include "Entity.h"
#include "PEntity.h"
#include "Player.h"
#include "ScoresDB.h"
#include "Sound.h"
#include "Text.h"
#include "Texture.h"
#include "Timer.h"
#include "Window.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <array>
#include <cstdint>
#include <thread>
#include <vector>

class Game {

public:
  enum class Screen { start = 1, game, death, score_board };
  Game();
  void Run(uint32_t last_iteration_time);
  void Draw();
  void StartMenu();
  void HandleInput();
  void HandleLogic(uint32_t LastIterationTime);
  void RunPhysics(unsigned int LastIterationTime);
  void DeathScreen();
  void ScoreBoard();
  void Reset();
  Screen CurrentScreen();
  void SetScreen(Screen screen);
  bool AppQuit();
  ~Game();

private:
  void ResizeButtonCorrectly(Button &button, int norm_height);
  int TopPlatformPosition();
  inline int GetGoodSeed() { return seed_generator.PassedTime().count(); }
  void RepositionPlatformRandomly(PEntity &platform);
  void FitPlatformToLevel(PEntity &platform);

private:
  // window and window info
  Window window;
  float cameraheight = 0;
  float cameraspeed = 0.0001;

  // entities
  Player player;
  Canon canon;
  Entity cloud;

  // menu buttons
  Button start_button;
  Button restart_button;
  Button quit_button;
  Button score_board_button;
  Button back_button;

  // backgrounds
  Texture bg;
  Animation death_screen_bg;
  Texture start_menu_bg;

  // textures / animations presented throughout the game.
  Texture wall;
  Texture heart;
  Animation wind;
  Animation pray;
  Texture mouse;

  // score displays
  Text score_display;
  Text death_score_display;

  Timer seed_generator;
  std::thread physics_thread;

  // sounds
  Sound click_sound;
  Sound wind_sound;
  Sound damage_sound;
  Sound button_hover_sound;
  Sound wood_crack;
  Mix_Music *bg_music;

  // platform types
  PEntity platform_default;
  PEntity platform_ice;
  PEntity platform_gum;
  PEntity platform_wood;
  std::array<PEntity *, 5> platform_levels;

  // progress tracking variables
  Screen current_screen = Screen::start;
  Screen prev_screen = Screen::start;
  uint32_t platforms_created = 0;
  int score = 0;
  int lives = 3;

  ScoreDB scoresdb;
  std::vector<Text> score_textures_cache;
  static constexpr int platforms_per_level = 20;

  std::vector<PEntity> platforms;
  bool quit_app = false;
};
