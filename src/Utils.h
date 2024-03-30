#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <vector>
namespace Utils {

inline int RandInRange(int min, int max, uint32_t seed) {
  // swaps min and max in case min is bigger.
  if (min > max) {
    int temp = min;
    min = max;
    max = temp;
  }

  uint32_t state = seed * 747796405u + 2891336453u;
  uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
  uint32_t randuint = (word >> 22u) ^ word;
  return randuint % (max - min) + min;
}

inline void FitCenter(const SDL_Rect &source, SDL_Rect &rect) {
  rect.x = source.x + source.w / 2 - rect.w / 2;
  rect.y = source.y + source.h / 2 - rect.h / 2;
}

inline SDL_Point GetCenter(const SDL_Rect &rect) {
  return {rect.x + rect.w / 2, rect.y + rect.h / 2};
}

}; // namespace Utils
struct Vec2 {

  friend std::ostream &operator<<(std::ostream &out, Vec2 vector);

  float x, y;

  explicit Vec2() : x(0), y(0) {}
  Vec2(float x, float y) : x(x), y(y) {}

  Vec2(const Vec2 &other) : x(other.x), y(other.y) {}
  explicit Vec2(const SDL_Point &point) : x(point.x), y(point.y) {}

  void operator=(const Vec2 &other) {
    x = other.x;
    y = other.y;
  }

  void operator+=(Vec2 other) {
    x += other.x;
    y += other.y;
  }
  void operator-=(Vec2 other) {
    x -= other.x;
    y -= other.y;
  }
  Vec2 operator+(Vec2 other) { return {x + other.x, y + other.y}; }
  Vec2 operator-(Vec2 other) { return {x - other.x, y - other.y}; }
  float GetAngle(Vec2 other) {
    float dot_product = x * other.x + y * other.y;
    return (180 / M_PI) *
           acos(dot_product / ((((float)*this)) * ((float)other)));
  }

  void operator/=(Vec2 other) {
    x /= other.x;
    y /= other.y;
  }
  void operator/=(float num) {
    x /= num;
    y /= num;
  }
  void operator*=(Vec2 other) {
    x *= other.x;
    y *= other.y;
  }
  void operator*=(float num) {
    x *= num;
    y *= num;
  }

  Vec2 operator/(Vec2 other) { return {x / other.x, y / other.y}; }
  Vec2 operator*(Vec2 other) { return {x * other.x, y * other.y}; }
  Vec2 operator/(float num) { return {x / num, y / num}; }
  Vec2 operator*(float num) { return {x * num, y * num}; }
  explicit operator float() const { return sqrt(pow(x, 2) + pow(y, 2)); }
  void rotate(float degrees) {
    float radians = degrees * 3.14159265 / 180;
    float W = x * cos(radians) + y * sin(radians);
    float H = y * cos(radians) - x * sin(radians);
    x = W;
    y = H;
  }

  bool operator==(const Vec2 &other) const {
    return x == other.x && y == other.y;
  }
};

inline std::ostream &operator<<(std::ostream &out, const Vec2 vector) {
  out << vector.x << " , " << vector.y;
  return out;
}
inline bool operator==(const SDL_Rect &r1, const SDL_Rect &r2) {
  return !std::memcmp(&r1, &r2, sizeof(SDL_Rect));
}

enum class Side { left = -2, bottom = -1, none = 0, top = 1, right = 2 };
