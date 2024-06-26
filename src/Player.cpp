#include "Player.h"
#include "Animation.h"
#include "PEntity.h"
#include <SDL2/SDL_render.h>
#include <memory>

extern std::string animationfolder;
extern std::string texturefolder;
extern std::string soundfolder;

Player::Player(SDL_Rect rect, Window &window)
    : PEntity(std::make_shared<Animation>(4, rect, window,
                                          animationfolder + "player")),
      direction(SDL_FLIP_NONE), jump_sound(soundfolder + "jump.wav", 15) {
  acceleration.y = 0.004;
  friction_cof = 1;
  mass = 1;
}

void Player::Draw() { PEntity::DrawEX(0, direction); }

void Player::Update(uint32_t microseconds) {
  if (pressing_up && standing) {
    if (jump_sound.PlayTime() > 500000 || jump_sound.PlayTime() == 0) {
      jump_sound.Cut();
      jump_sound.Play(0);
    }
    velocity.y = (std::abs(velocity.x) + 1.3) * jump_strength * -1;
  }
  PEntity::Update(microseconds);
}

void Player::SetLookDiraction() {
  if (velocity.x > 0)

    direction = SDL_FLIP_NONE;

  else if (velocity.x < 0)

    direction = SDL_FLIP_HORIZONTAL;
}
void Player::LimitXSpeed(float speed) {
  if (velocity.x < -1 * speed)
    velocity.x = -1 * speed;
  if (velocity.x > speed)
    velocity.x = speed;
}

void Player::LimitXpos(int min, int max) {
  if (position.x < min) {
    position.x = min;
    velocity.x *= -0.7f;
  }
  if (position.x > max) {
    position.x = max;
    velocity.x *= -0.7f;
  }
}

void Player::HandleInput(const SDL_Event &event) {
  const auto &key = event.key.keysym.scancode;
  // handle key press
  if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
    switch (key) {
    case SDL_SCANCODE_D:

      pressing_right = true;
      if (pressing_left)
        acceleration.x = 0;
      else
        acceleration.x = 0.002;

      break;

    case SDL_SCANCODE_A:

      pressing_left = true;
      if (pressing_right)
        acceleration.x = 0;
      else
        acceleration.x = -0.002;

      break;
    case SDL_SCANCODE_W:

      pressing_up = true;

      break;
    default:
      break;
    }
  }
  // handle key release
  else if (event.type == SDL_KEYUP) {
    switch (key) {
    case SDL_SCANCODE_D:
      pressing_right = false;
      if (!pressing_left)
        acceleration.x = 0;
      else
        acceleration.x = -0.002;

      break;

    case SDL_SCANCODE_A:

      pressing_left = false;
      if (!pressing_right)
        acceleration.x = 0;
      else
        acceleration.x = 0.002;

      break;
    case SDL_SCANCODE_W:
      pressing_up = false;

      break;
    default:
      break;
    }
  }
}
