#include "Entity.h"
#include "Texture.h"
#include "Utils.h"
#include <memory>
#include <stdexcept>
#include <vector>

Entity::Entity(SDL_Rect rect, Window &wnd)
    : visual(new Texture(rect, wnd)),
      position(Vec2((float)rect.x, (float)rect.y)), velocity(Vec2(0, 0)),
      acceleration(Vec2(0, 0)), hitbox(rect) {}

Entity::Entity(const std::string &texture, SDL_Rect rect, Window &wnd)
    : visual(new Texture(texture, rect, wnd)),
      position(Vec2((float)rect.x, (float)rect.y)), velocity(Vec2(0, 0)),
      acceleration(Vec2(0, 0)), hitbox(rect) {}

Entity::Entity(const Entity &other)
    : position(other.position), velocity(other.velocity),
      acceleration(other.acceleration), hitbox(other.hitbox),
      standing(other.standing) {
  if (visual != nullptr) {
    delete visual;
  }

  // handle copying of texture
  if (Texture *other_texture = dynamic_cast<Texture *>(other.visual);
      other_texture != nullptr) {
    visual = new Texture(*other_texture);
    return;
  }

  // handle copying of animation
  if (Animation *other_animation = dynamic_cast<Animation *>(other.visual);
      other_animation != nullptr) {
    visual = new Animation(*other_animation);
    return;
  }
  throw std::runtime_error(
      "invalid instatnce of visual representation of entity detected");
}

Entity::Entity(Entity &&other)
    : visual(other.visual), position(other.position), velocity(other.velocity),
      acceleration(other.acceleration), hitbox(other.hitbox),
      standing(other.standing) {
  other.visual = nullptr;
}

Entity::Entity(const Texture &texture, SDL_Rect rect)
    : visual(new Texture(texture)), position{(float)rect.x, (float)rect.y},
      velocity{0, 0}, acceleration{0, 0}, hitbox(rect), standing(false) {}

void Entity::operator=(const Entity &other) {
  if (visual) {
    delete visual;
    visual = nullptr;
  }
  if (Texture *other_texture = dynamic_cast<Texture *>(other.visual);
      other_texture != nullptr) {
    visual = new Texture(*other_texture);
  } else if (Animation *other_animation =
                 dynamic_cast<Animation *>(other.visual);
             other_animation != nullptr) {
    visual = new Animation(*other_animation);
  } else
    throw std::runtime_error(
        "invalid instatnce of visual representation of entity detected");
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
Animation *Entity::animation() { return dynamic_cast<Animation *>(visual); }
Texture *Entity::texture() { return dynamic_cast<Texture *>(visual); }

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

void Entity::ChangeTexture(const Texture &texture) {
  SDL_Rect save_rect = this->visual->rect;
  if (visual != nullptr)
    delete visual;
  this->visual = new Texture(texture);
  this->visual->rect = save_rect;
  std::cerr << save_rect.w << std::endl;
}
void Entity::ChangeTexture(Texture &&texture) {
  SDL_Rect save_rect = this->visual->rect;
  if (visual != nullptr)
    delete visual;
  this->visual = &texture;
  this->visual->rect = save_rect;
  std::cerr << save_rect.w << std::endl;
}
void Entity::SetAnimation(const Animation &animation) {
  if (visual != nullptr)
    delete visual;
  visual = new Animation(animation);
  visual->rect = hitbox;
  std::cout << visual->rect.w << std::endl;
}

Entity::~Entity() {
  if (visual != nullptr)
    delete visual;
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
