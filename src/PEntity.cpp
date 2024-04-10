#include "PEntity.h"
#include "Entity.h"
#include "Utils.h"
#include <algorithm>
#include <cmath>
#include <memory>

PEntity::PEntity(std::shared_ptr<Drawable> visual, SDL_Rect rect)
    : Entity(visual, rect) {}
PEntity::PEntity(const PEntity &other)
    : Entity(other), mass(other.mass), friction_cof(other.friction_cof),
      elasticity(other.elasticity) {}

PEntity::PEntity(PEntity &&other)
    : Entity(std::move(other)), mass(other.mass),
      friction_cof(other.friction_cof), elasticity(other.elasticity)

{}

void PEntity::operator=(const PEntity &other)

{
  Entity::operator=(other);
  mass = other.mass;
  friction_cof = other.friction_cof;
  elasticity = other.elasticity;
}
bool PEntity::EqualProperties(const PEntity &other) const {
  return elasticity == other.elasticity && friction_cof == other.friction_cof &&
         mass == other.mass;
}
void PEntity::ChangeMass(float NewMass) { mass = NewMass; }
void PEntity::SetRoughness(float roughness) {
  friction_cof = roughness;
  if (friction_cof > 1.0f)
    friction_cof = 1.0f;
  if (friction_cof < 0.0f)
    friction_cof = 0.0f;
}
void PEntity::SetElasticity(float elasticity) { this->elasticity = elasticity; }

void PEntity::Gravitate(Vec2 point, float strength) {
  SDL_Point center = Utils::GetCenter(hitbox);
  Vec2 diraction =
      (point - Vec2((float)center.x, (float)center.y)) / GetDistance(point);
  velocity += {strength * diraction.x / mass, strength * diraction.y / mass};
}

Side PEntity::PhysicsCollision(const SDL_Rect &other, float friction_cof,
                               float elasticity) {

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
    if ((hitbox.w + other.w) - abs((int)interX) >
        (hitbox.h + other.h) - abs(interY)) {

      // caculcaltes friction force according to normal force created in the y
      // coordinate
      float fricForce =
          velocity.x > 0 ? std::min(velocity.x * mass,
                                    std::min(friction_cof, this->friction_cof) *
                                        (mass * abs(velocity.y))) *
                               -1
                         : std::min(abs(velocity.x * mass),
                                    std::min(friction_cof, this->friction_cof) *
                                        (mass * abs(velocity.y)));
      if (interY_up) {
        position.y = other.y - hitbox.h;
        standing = true;
      } else {
        position.y = other.y + other.h;
      }

      position.y = position.y;
      visual->rect.y = position.y;

      ApplyForce({fricForce, 0});
      if ((velocity.y > 0) == interY_up)
        velocity.y *= -1 * std::max(elasticity, this->elasticity);
      if (interY_up)
        return Side::top;
      return Side::bottom;

    } else {

      // caculcaltes friction force according to normal force created in the x
      // coordinate
      float fricForce =
          velocity.y > 0 ? std::min(velocity.y * mass,
                                    std::min(friction_cof, this->friction_cof) *
                                        (mass * abs(velocity.x))) *
                               -1
                         : std::min(abs(velocity.y * mass),
                                    std::min(friction_cof, this->friction_cof) *
                                        (mass * abs(velocity.x)));

      if (interX_left) {
        position.x = other.x - hitbox.w;
      } else {
        position.x = other.x + other.w;
      }

      hitbox.x = position.x;
      visual->rect.x = position.x;

      ApplyForce({0, fricForce});
      velocity.x *= -1 * std::max(elasticity, this->elasticity);

      if (interX_left)
        return Side::left;
      return Side::right;
    }
  }
  return Side::none;
}
Side PEntity::GhostPhysicsCollision(const PEntity &otherentity) {
  const SDL_Rect &other = otherentity.hitbox;

  float interX =
      ((position.x + hitbox.w) - (otherentity.position.x + other.w)) +
      (hitbox.x - other.x);
  bool interX_left = interX < 0;

  float interY =
      ((position.y + hitbox.h) - (otherentity.position.y + other.h)) +
      (hitbox.y - other.y);
  bool interY_up = interY < 0;

  if ((interY + (hitbox.h + other.h)) > -1 &&
      (interY + (hitbox.h + other.h)) < 1 && interX < (hitbox.w + other.w) &&
      interX > 0 - (hitbox.w + other.w))
    standing = true;

  // if there is a collision
  if (interX < (hitbox.w + other.w) && interX > 0 - (hitbox.w + other.w) &&
      interY < (hitbox.h + other.h) && interY > 0 - (hitbox.h + other.h)) {
    if ((hitbox.w + other.w) - abs(interX) >
        (hitbox.h + other.h) - abs(interY)) {
      float system_force = ((velocity.y - otherentity.velocity.y) * mass);
      system_force +=
          system_force * std::max(elasticity, otherentity.elasticity);
      float fricForce = FricCalcX(otherentity, system_force);

      if (interY_up) {
        position.y = otherentity.position.y - hitbox.h;
        standing = true;
        if (system_force < 0)
          system_force = 0;
      } else {
        position.y = otherentity.position.y + other.h;
        if (system_force > 0)
          system_force = 0;
      }
      hitbox.y = position.y;
      visual->rect.y = hitbox.y;
      ApplyForce({fricForce, system_force * -1});
      if (interY_up)
        return Side::top;
      return Side::bottom;
    } else {

      float system_force = ((velocity.x - otherentity.velocity.x) * mass);
      system_force +=
          system_force * std::max(elasticity, otherentity.elasticity);

      float fricForce = FricCalcY(otherentity, system_force);
      if (interX_left) {
        position.x = otherentity.position.x - hitbox.w;

        if (system_force < 0)
          system_force = 0;
      }

      else {
        position.x = otherentity.position.x + other.w;
        if (system_force > 0)
          system_force = 0;
      }
      hitbox.x = position.x;
      visual->rect.x = hitbox.x;

      ApplyForce({system_force * -1, fricForce});
      if (interX_left)
        return Side::left;
      return Side::right;
    }
  }
  return Side::none;
}

Side PEntity::PhysicsCollision(PEntity &otherentity) {
  SDL_Rect &other = otherentity.hitbox;

  float interX =
      ((position.x + hitbox.w) - (otherentity.position.x + other.w)) +
      (hitbox.x - other.x);
  bool interX_left = interX < 0;

  float interY =
      ((position.y + hitbox.h) - (otherentity.position.y + other.h)) +
      (hitbox.y - other.y);
  bool interY_up = interY < 0;

  if ((interY + (hitbox.h + other.h)) > -1 &&
      (interY + (hitbox.h + other.h)) < 1 && interX < (hitbox.w + other.w) &&
      interX > 0 - (hitbox.w + other.w))
    standing = true;

  // if there is a collision
  if (interX < (hitbox.w + other.w) && interX > 0 - (hitbox.w + other.w) &&
      interY < (hitbox.h + other.h) && interY > 0 - (hitbox.h + other.h)) {
    if ((hitbox.w + other.w) - abs(interX) >
        (hitbox.h + other.h) - abs(interY)) {
      float system_force = ((velocity.y - otherentity.velocity.y) *
                            std::min(otherentity.mass, mass));
      system_force +=
          system_force * std::max(elasticity, otherentity.elasticity);
      float fricForce = FricCalcX(otherentity, system_force);
      float saveypos = position.y;
      if (interY_up) {
        position.y = otherentity.position.y - hitbox.h;
        otherentity.position.y = saveypos + hitbox.h;
        standing = true;
        if (system_force < 0)
          system_force = 0;
      } else {
        position.y = otherentity.position.y + other.h;
        otherentity.position.y = saveypos - other.h;
        if (system_force > 0)
          system_force = 0;
        otherentity.standing = true;
      }
      hitbox.y = position.y;
      other.y = otherentity.position.y;
      visual->rect.y = hitbox.y;
      otherentity.visual->rect.y = otherentity.position.y;
      ApplyForce({fricForce, system_force * -1});
      otherentity.ApplyForce({fricForce * -1, system_force});
      if (interY_up)
        return Side::top;
      return Side::bottom;
    } else {

      float system_force = ((velocity.x - otherentity.velocity.x) *
                            std::min(otherentity.mass, mass));
      system_force +=
          system_force * std::max(elasticity, otherentity.elasticity);
      float fricForce = FricCalcY(otherentity, system_force);
      float savexpos = position.x;
      if (interX_left) {
        position.x = otherentity.position.x - hitbox.w;
        otherentity.position.x = savexpos + hitbox.w;

        if (system_force < 0)
          system_force = 0;
      }

      else {
        position.x = otherentity.position.x + other.w;
        otherentity.position.x = savexpos - other.w;
        if (system_force > 0)
          system_force = 0;
      }
      hitbox.x = position.x;
      other.x = otherentity.position.x;
      visual->rect.x = hitbox.x;
      otherentity.visual->rect.x = otherentity.position.x;
      ApplyForce({system_force * -1, fricForce});
      otherentity.ApplyForce({system_force, fricForce * -1});
      if (interX_left)
        return Side::left;
      return Side::right;
    }
  }
  return Side::none;
}

void PEntity::Limit(const SDL_Rect &border) {
  if (position.x + hitbox.w > border.x + border.w) {
    position.x = border.x + border.w - hitbox.w;
    hitbox.x = position.x;
    velocity.x = 0;
  }

  if (position.x < border.x) {
    position.x = border.x;
    hitbox.x = position.x;
    velocity.x = 0;
  }
  if (position.y + hitbox.h > border.y + border.h) {
    position.y = border.y + border.h - hitbox.h;
    hitbox.y = position.y;
    velocity.y = 0;
  }

  if (position.y < border.y) {
    position.y = border.y;
    hitbox.y = position.y;
    velocity.y = 0;
  }
}

void PEntity::LimitSpeed(float limit) {
  if ((float)velocity > limit) {
    velocity /= (float)velocity / limit;
  }
}
void SortByHeight(std::vector<PEntity *> &entitys) {
  for (uint64_t i = 1; i < entitys.size(); i++) {
    for (int j = i; j > 0 && entitys[j]->hitbox.y < entitys[j - 1]->hitbox.y;
         j--) {
      PEntity *temp = entitys[j];
      entitys[j] = entitys[j - 1];
      entitys[j - 1] = temp;
    }
  }
}

float PEntity::FricCalcX(const PEntity &otherentity, float system_force) {
  float maxfric =
      (otherentity.velocity.x - velocity.x) * std::min(mass, otherentity.mass);
  float neededfric =
      std::min(friction_cof, otherentity.friction_cof) * (abs(system_force));
  if ((otherentity.velocity.x - velocity.x) < 0)
    neededfric *= -1;
  if (abs(maxfric) > abs(neededfric)) {
    return neededfric;
  }
  return maxfric;
}

float PEntity::FricCalcY(const PEntity &otherentity, float system_force) {
  float maxfric =
      (otherentity.velocity.y - velocity.y) * std::min(mass, otherentity.mass);
  float neededfric =
      std::min(friction_cof, otherentity.friction_cof) * (abs(system_force));
  if ((otherentity.velocity.y - velocity.y) < 0)
    neededfric *= -1;
  if (abs(maxfric) > abs(neededfric)) {
    return neededfric;
  }
  return maxfric;
}
