#include "Entity.h"
#include "Texture.h"
#include "Utils.h"
#include <memory>
#include <stdexcept>
#include <vector>

Entity::Entity(const Entity &other)
    : visual(other.visual->Clone()), position(other.position),
      velocity(other.velocity), acceleration(other.acceleration),
      hitbox(other.hitbox), standing(other.standing) {}

Entity::Entity(Entity &&other)
    : visual(other.visual), position(other.position), velocity(other.velocity),
      acceleration(other.acceleration), hitbox(other.hitbox),
      standing(other.standing) {}

Entity::Entity(std::shared_ptr<Drawable> visual, SDL_Rect rect)
    : visual(visual), position{(float)rect.x, (float)rect.y}, velocity{0, 0},
      acceleration{0, 0}, hitbox(rect), standing(false) {
  if (rect == SDL_Rect{0}) {
    rect = visual->rect;
    hitbox = rect;
    position = {(float)rect.x, (float)rect.y};
  }
}

void Entity::operator=(const Entity &other) {
  visual = other.visual->Clone();
  position = other.position;
  velocity = other.velocity;
  acceleration = other.acceleration;
  hitbox = other.hitbox;
  standing = other.standing;
}

bool Entity::operator==(const Entity &other) {
  return position == other.position && velocity == other.velocity &&
         acceleration == other.acceleration && hitbox == other.hitbox &&
         standing == other.standing;
}

void Entity::DrawEX(float angle, SDL_RendererFlip flip) {
  visual->DrawEX(angle, flip);
}

void Entity::Draw() { visual->Draw(); }

void Entity::Update(uint32_t microseconds) {
  float milliseconds = microseconds / 1000.0f;
  velocity += acceleration * milliseconds;
  position += velocity * milliseconds;
  hitbox.x = position.x;
  hitbox.y = position.y;
  Utils::FitCenter(hitbox, visual->rect);
  standing = false;
}

void Entity::Resize(SDL_Point size) { Resize(size.x, size.y); }
void Entity::Stop() {
  velocity.x = 0;
  velocity.y = 0;
}

void Entity::Repos(SDL_Point pos) { Repos(pos.x, pos.y); }

void Entity::Move(SDL_Point delta) { Move(delta.x, delta.y); }

void Entity::Resize(int w, int h) {
  visual->rect.w *= (float)w / hitbox.w;
  visual->rect.h *= (float)h / hitbox.h;
  hitbox.y = position.y;
  hitbox.w = w;
  hitbox.h = h;

  Utils::FitCenter(hitbox, visual->rect);
}

void Entity::Repos(int x, int y) {
  position.x = x;
  hitbox.x = x;
  position.y = y;
  hitbox.y = y;

  Utils::FitCenter(hitbox, visual->rect);
}

void Entity::Move(int x, int y) {
  position.x += x;
  hitbox.x += x;
  position.y += y;
  hitbox.y = y;
  Utils::FitCenter(hitbox, visual->rect);
}

SDL_Point Entity::GetSize() { return SDL_Point{hitbox.w, hitbox.h}; }

bool Entity::Standing() const { return standing; }
Side Entity::CheckCollision(const SDL_Rect &other) {
  float interX =
      ((position.x + hitbox.w) - (other.x + other.w)) + (position.x - other.x);

  bool interX_left = interX < 0;

  float interY =
      ((position.y + hitbox.h) - (other.y + other.h)) + (position.y - other.y);

  bool interY_up = interY < 0;
  if (interX < (hitbox.w + other.w) && interX > 0 - (hitbox.w + other.w) &&
      interY < (hitbox.h + other.h) && interY > 0 - (hitbox.h + other.h)) {
    if ((hitbox.w + other.w) - abs(interX) >
        (hitbox.h + other.h) - abs(interY)) {
      if (interY_up)
        return Side::top;
      return Side::bottom;

    } else {
      if (interX_left)
        return Side::left;
      return Side::right;
    }
  }

  return Side::none;
}

Side Entity::AvoidCollision(const SDL_Rect &other) {

  float interX =
      ((position.x + hitbox.w) - (other.x + other.w)) + (position.x - other.x);

  bool interX_left = interX < 0;

  float interY =
      ((position.y + hitbox.h) - (other.y + other.h)) + (position.y - other.y);

  bool interY_up = interY < 0;

  if ((interY + (hitbox.h + other.h)) == 0 && interX < (hitbox.w + other.w) &&
      interX > 0 - (hitbox.w + other.w))
    standing = true;

  // if there is a collision
  if (interX < (hitbox.w + other.w) && interX > 0 - (hitbox.w + other.w) &&
      interY < (hitbox.h + other.h) && interY > 0 - (hitbox.h + other.h)) {
    if ((hitbox.w + other.w) - abs(interX) >
        (hitbox.h + other.h) - abs(interY)) {

      if (interY_up) {

        position.y = other.y - hitbox.h;
        standing = true;
      } else {
        position.y = other.y + other.h;
      }
      hitbox.y = position.y;
      visual->rect.y = position.y;
      velocity.y = 0;
      if (interY_up)
        return Side::top;
      return Side::bottom;
    } else {

      if (interX_left) {
        position.x = other.x - hitbox.w;

      } else {
        position.x = other.x + other.w;
      }
      hitbox.x = position.x;
      visual->rect.x = position.x;
      velocity.x = 0;
      if (interX_left)
        return Side::left;
      return Side::right;
    }
  }
  return Side::none;
}

void SortByHeight(std::vector<Entity *> &entitys) {
  for (uint64_t i = 1; i < entitys.size(); i++) {
    for (int j = i; j > 0 && entitys[j]->hitbox.y > entitys[j - 1]->hitbox.y;
         j--) {
      Entity *temp = entitys[j];
      entitys[j] = entitys[j - 1];
      entitys[j - 1] = temp;
    }
  }
}
