#include "Game.h"
#include "Button.h"
#include "PEntity.h"
#include "Sound.h"
#include "Utils.h"
#include "Window.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <string>

extern std::string EXEPath;
extern std::string texturefolder;
extern std::string animationfolder;
extern std::string soundfolder;
extern std::string fontfolder;

Game::Game()
    : window("2D Game!",
             {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1000},
             SDL_WINDOW_RESIZABLE),

      // entities
      player({300, 640, 60, 100}, window),
      canon({1400, 500, 170, 100}, SDL_FLIP_NONE, window),
      cloud(texturefolder + "cloud_platform.png", {1400, 600, 200, 100},
            window),

      // menu buttons
      start_button("start", SDL_Color{255, 255, 0, 255},
                   fontfolder + "button.ttf", {700, 400, 200, 100}, 0, window),
      restart_button("restart", SDL_Color{255, 255, 0, 255},
                     fontfolder + "button.ttf", {675, 400, 250, 100}, 0,
                     window),
      quit_button("Quit!", SDL_Color{255, 255, 0, 255},
                  fontfolder + "button.ttf", {700, 550, 200, 100}, 0, window),

      // backgrounds
      bg(texturefolder + "sky.png", {0, 0, window.width, window.height},
         window),
      death_screen_bg(2, {0, 0, window.width, window.height}, window,
                      animationfolder + "cry"),
      start_menu_bg(texturefolder + "jerusalem.png",
                    {0, 0, window.width, window.height}, window),

      // textures / animations presented throughout the game.
      wall(texturefolder + "wall.png", {200, 0, 100, window.height}, window),
      heart(texturefolder + "heart.png", {1050, 30, 100, 100}, window),
      wind(4, {1200, 0, 400, 400}, window, animationfolder + "wind2"),
      pray(2, {200, 500, 300, 300}, window, animationfolder + "pray"),
      mouse(texturefolder + "mouse.png", {0, 0, 50, 50}, window),

      // score displays
      score_display("0", {255, 190, 70, 255}, "../fonts/font.ttf", window,
                    {20, 20, 100, 150}),
      death_score_display(SDL_Color{190, 0, 0, 255}, window,
                          {320, 200, 900, 200}),

      // sounds
      click_sound(soundfolder + "horn.wav", 50),
      wind_sound(soundfolder + "wind.wav", 30),
      damage_sound(soundfolder + "damage.wav", 80),
      button_hover_sound(soundfolder + "buttonhover.wav", 50),
      wood_crack(soundfolder + "wood_crack.wav", 80),

      // platform types
      platform_default(texturefolder + "grass.png",
                       SDL_Rect{300, 700, 1000, 300}, window),
      platform_ice(texturefolder + "ice_platform.png",
                   SDL_Rect{300, 1200, 1000, 300}, window),
      platform_gum(texturefolder + "gum_platform.png",
                   SDL_Rect{300, 700, 1000, 300}, window),
      platform_wood(SDL_Rect{300, 700, 1000, 300}, window) {

  player.animation.emplace(4, SDL_Rect{300, 640, 120, 120}, window,
                           animationfolder + "player");
  SDL_ShowCursor(false);
  player.animation->SetBackAndForth(true);
  Canon::LoadSounds();
  bg_music = Mix_LoadMUS((soundfolder + "music.wav").c_str());
  Mix_PlayMusic(bg_music, -1);
  Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

  // set properties for each platform type.
  platform_default.SetRoughness(0.2f);
  platform_ice.SetRoughness(0.0f);
  platform_gum.SetRoughness(0.31);
  platforms.reserve(10);
  platform_wood.animation = Animation(5, {300, 1200, 1000, 300}, window,
                                      animationfolder + "wood_platform");
  platform_wood.animation->SetTimePerImage(250000);
  platform_wood.SetRoughness(0.21f);
  platform_wood.animation->Pause(0);
  platform_wood.animation->rect.h = 600;

  platform_levels = {&platform_default, &platform_ice, &platform_gum,
                     &platform_wood, &platform_default};

  // create and place all platfroms randomly.
  for (int i = 0; i < 10; i++) {
    platforms.emplace_back(platform_default);
    RepositionPlatformRandomly(platforms[i]);
  }
  platforms_created = 10;
  wind.SetBackAndForth(true);
  wind.SetTimePerImage(70000);
  pray.SetTimePerImage(800000);
  cloud.acceleration.y = -0.0002;
  canon.acceleration.y = 0.04;

  seed_generator.Start();
  canon.Change_Power(2.0f);

  death_screen_bg.SetTimePerImage(1000000);

  // setting functionalities for buttons.
  start_button.on_click = [this]() {
    running = true;
    Mix_PauseMusic();
  };
  restart_button.on_click = [this]() {
    running = true;
    Reset();
    Mix_PauseMusic();
  };
  quit_button.on_click = [this]() {
    quit_app = true;
    running = true;
  };

  // set buttons frame to dark red.
  start_button.ChangeRectColor({190, 0, 0, 255});
  restart_button.ChangeRectColor({190, 0, 0, 255});
  quit_button.ChangeRectColor({190, 0, 0, 255});
}

bool Game::IsRunning() { return running; }

void Game::DeathScreen() {

  ResizeButtonCorrectly(restart_button, {675, 400, 250, 100});
  ResizeButtonCorrectly(quit_button, {700, 550, 200, 100});

  window.Clear();
  death_screen_bg.DrawOnWindow(true);
  restart_button.Draw();
  quit_button.Draw();
  death_score_display.Draw();
  SDL_GetMouseState(&mouse.rect.x, &mouse.rect.y);
  mouse.DrawOnWindow(false);
  window.Show();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    if (event.type == SDL_QUIT) {
      running = false;
      quit_app = true;
      break;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
      click_sound.Play(0);
    }
    window.HandleEvent(event);
    restart_button.HandleEvent(event);
    quit_button.HandleEvent(event);
  }
}

void Game::StartMenu() {
  ResizeButtonCorrectly(start_button, {700, 400, 200, 100});
  ResizeButtonCorrectly(quit_button, {700, 550, 200, 100});

  window.Clear();
  start_menu_bg.DrawOnWindow(true);
  pray.DrawOnWindow(true);
  start_button.Draw();
  quit_button.Draw();
  SDL_GetMouseState(&mouse.rect.x, &mouse.rect.y);
  mouse.DrawOnWindow(false);
  window.Show();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
      quit_app = true;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
      click_sound.Play(0);
    }
    window.HandleEvent(event);
    start_button.HandleEvent(event);
    quit_button.HandleEvent(event);
  }
}

void Game::Reset() {
  player.Repos(800, 640);
  player.Stop();
  lives = 3;
  player.acceleration.x = 0;
  score = 0;
  player.highest_platform_passed = 700;
  score_display.Resize({100, 150});
  score_display.Reposition({20, 20});
  score_display.ChangeColor({255, 190, 70, 255});
  score_display.ChangeText("0");
  score_display.RecreateTexture();
  // set all platforms outside the screen so that the next update will
  // reposition them randomly.
  for (auto &platform : platforms) {
    platform = platform_default;
    platform.Repos(0, 10000);
  }

  platforms_created = 9;
  cameraheight = 0;
  cameraspeed = 0.0001;
  window.CameraView.x = 0;
  window.CameraView.y = 0;

  for (auto &platform : platforms) {
    RepositionPlatformRandomly(platform);
  }
  canon.Repos(1400, 500);
  canon.SetFlip(SDL_FLIP_NONE);
  cloud.Repos(canon.position.x, canon.position.y + canon.hitbox.h);
  canon.Reload(false);
}

SDL_RendererFlip direction = SDL_FLIP_HORIZONTAL;

void Game::Draw() {
  window.Clear();
  bg.DrawOnWindow(true);
  if ((score % 100) / 20 + 1 == 5) {
    int original_y = wind.rect.y;
    for (int j = 0; j < 4; j++) {

      int original_x = wind.rect.x;
      for (int i = 0; i < 5; i++) {
        wind.DrawOnWindow(true);
        wind.rect.x -= wind.rect.w;
      }
      wind.rect.x = original_x;
      wind.rect.y += wind.rect.h;
    }
    wind.rect.y = original_y;
  }
  cloud.Draw();

  // draws wall around the player.
  wall.rect.y = int(player.position.y / 1000) * 1000;
  wall.Draw();
  for (int i = 0; i < 2; i++) {
    wall.rect.y -= 1000;
    wall.Draw();
  }
  wall.rect.x = 1300;
  wall.rect.y = int(player.position.y / 1000) * 1000;
  wall.Draw();

  for (int i = 0; i < 2; i++) {
    wall.rect.y -= 1000;
    wall.Draw();
  }
  wall.rect.x = 200;

  for (auto &platform : platforms) {
    platform.Draw();
  }
  platform_default.Draw();
  score_display.Draw();
  canon.Draw();
  // draws the heart texture as many times as there are lives left and reset the
  // texture position for the next draw.
  for (int i = 0; i < lives; i++) {
    heart.DrawOnWindow(true);
    heart.rect.x -= 120;
  }
  heart.rect.x = 1450;

  player.Draw();
  platform_wood.Draw();
  window.Show();
}

void Game::HandleInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = false;
      quit_app = true;
    }
    player.HandleInput(event);
    window.HandleEvent(event);
  }
}

void Game::HandleLogic(uint32_t LastIterationTime) {

  // if speed is 0 or in air then pause player animation.
  if (!player.Standing()) {
    player.animation->Pause(-1);
  } else if (player.velocity.x == 0) {
    player.animation->Pause(0);
  } else {
    player.animation->Resume(-1);
  }

  player.animation->SetTimePerImage((int)abs(
      10000 / player.velocity.x)); // fits animation speed to player speed.

  int savescore =
      score; // saves score value to see if changed and redrawing is needed.
  for (auto &platform : platforms) {
    if (player.position.y + player.hitbox.h < platform.position.y &&
        player.highest_platform_passed > platform.position.y) {
      player.highest_platform_passed = platform.position.y;
      score++;

      // handle level update

      if (score % platforms_per_level == 0) {
        int level = (score % 100) / 20 + 1;

        if (level == 5)
          wind_sound.Play(-1);
        if (level == 1) {
          wind_sound.Cut();
          cameraspeed += 0.00005;
        }
      }
    }

    // handle platform recreation when it leaves the screen as the game goes.
    if (platform.position.y > window.CameraView.y + window.CameraView.h ||
        (platform.EqualProperties(platform_wood) && platform.animation &&
         platform.animation->CurrentImageIndex() == 4)) {
      platforms_created++;
      FitPlatformToLevel(platform);
      RepositionPlatformRandomly(platform);
    }

    if ((platform.EqualProperties(platform_wood) && platform.animation &&
         platform.animation->CurrentImageIndex() == 2) &&
        !wood_crack.IsPlaying()) {
      wood_crack.Play(0);
    }
  }

  // handle camera movement
  if (player.position.y - window.CameraView.y < 300)
    cameraheight = player.position.y - 300;
  if (score > 2)
    cameraheight -= LastIterationTime * cameraspeed;

  // handle player death
  if (!SDL_HasIntersection(&player.hitbox, &window.CameraView) || lives < 1) {
    Sound::CutAll();
    Mix_ResumeMusic();
    death_score_display.ChangeText("Final score: " + std::to_string(score));
    death_score_display.RecreateTexture();
    running = false;
    return;
  }
  if (canon.InTrajectory(player.hitbox))
    canon.Shot();
  if (canon.BallLeftScreen())
    canon.Reload(true);

  window.CameraView.y = cameraheight;
  wind.rect.x += LastIterationTime / 1500;
  if (wind.rect.x >= 1600)
    wind.rect.x -= 400;
  // recreate score texture if needed.
  if (savescore != score) {
    std::string score_text = std::to_string(score);
    score_display.ChangeText(score_text);
    score_display.Resize({(int)score_text.length() * 100, 150});
    score_display.RecreateTexture();
  }
  player.SetLookDiraction();
}

void Game::RunPhysics(unsigned int LastIterationTime) {
  player.Update(LastIterationTime);
  canon.Update(LastIterationTime);

  player.GhostPhysicsCollision(platform_default);
  player.GhostPhysicsCollision(platform_ice);
  if ((score % 100) / 20 + 1 == 5)
    player.ApplyForce({0.000001f * LastIterationTime, 0});
  canon.PhysicsCollision(cloud.hitbox, 0.5, 0);
  if (!canon.IsLoaded() &&
      std::abs(static_cast<int>(canon.ball.PhysicsCollision(player))) ==
          static_cast<int>(Side::right) &&
      canon.CanDoDamage()) {
    lives -= 1;
    canon.DisableDamage();
    damage_sound.Play(0);
  }
  if (canon.position.y >
      window.CameraView.y +
          window.CameraView.h) // if canon below camera reposition it higher and
                               // on the other side.
  {

    if (canon.position.x == 1400) {
      canon.Repos(0, Utils::RandInRange(canon.position.y - 4000,
                                        canon.position.y - 2000,
                                        LastIterationTime));
      canon.SetFlip(SDL_FLIP_HORIZONTAL);
    } else {
      canon.Repos(1400, Utils::RandInRange(canon.position.y - 3500,
                                           canon.position.y - 2000,
                                           LastIterationTime));
      canon.SetFlip(SDL_FLIP_NONE);
    }
    cloud.Repos(canon.position.x, canon.position.y + canon.hitbox.h);
  }
  for (auto &platform : platforms) {
    if (player.CheckCollision(platform.hitbox) == Side::top &&
        player.velocity.y > 0 &&
        player.hitbox.h + player.position.y < platform.hitbox.y + 5)
    // calculate collision if player is falling and his legs are at max 5 pixels
    // below platform
    {
      player.GhostPhysicsCollision(platform);
      if (platform.animation)
        platform.animation->Resume(-1);

    } else if (platform.animation)
      platform.animation->Pause(0);
  }

  player.LimitXpos(300, window.CameraView.w - 300 - player.hitbox.w);
  player.LimitXSpeed(1.2f);

  cloud.Update(LastIterationTime);
  if (cloud.velocity.y > 0.15f)
    cloud.acceleration.y = -0.0002;
  if (cloud.velocity.y < -0.15f)
    cloud.acceleration.y = 0.0002;
}

int Game::TopPlatformPosition() {
  int topposition = 1000;
  for (auto &platform : platforms) {
    if (topposition > platform.hitbox.y) {
      topposition = platform.hitbox.y;
    }
  }
  if (topposition > platform_default.hitbox.y)
    topposition = platform_default.hitbox.y;

  return topposition;
}

bool Game::AppQuit() { return quit_app; }

void Game::ResizeButtonCorrectly(Button &button, SDL_Rect original_rect) {

  if (button.Hovered() && button.GetRect().w == original_rect.w) {
    button.Resize({static_cast<int>(original_rect.w * 1.2f),
                   static_cast<int>(original_rect.h * 1.2f)});
    button.Reposition(
        {original_rect.x - static_cast<int>(original_rect.w * 0.1f),
         original_rect.y - static_cast<int>(original_rect.h * 0.1f)});
    button.ChangeColor({190, 190, 0, 255});
    button.RecreateTexture();
    button_hover_sound.Play(0);
  }
  if (!button.Hovered() && button.GetRect().w != original_rect.w) {
    button.Resize({original_rect.w, original_rect.h});
    button.Reposition({original_rect.x, original_rect.y});
    button.ChangeColor({255, 255, 0, 255});
    button.RecreateTexture();
  }
}

void Game::RepositionPlatformRandomly(PEntity &platform) {
  int TopPlatform = TopPlatformPosition();
  platform.Repos(Utils::RandInRange(300, window.CameraView.w - 900,
                                    TopPlatform + GetGoodSeed()),
                 Utils::RandInRange(TopPlatform - 350, TopPlatform - 150,
                                    TopPlatform + GetGoodSeed()));

  platform.Resize(Utils::RandInRange(300, 600, TopPlatform * GetGoodSeed()),
                  50);
}

void Game::FitPlatformToLevel(PEntity &platform) {

  if (!platform_levels[(platforms_created % 100) / 20]->EqualProperties(
          platform))
    platform = *platform_levels[(platforms_created % 100) / 20];
}
