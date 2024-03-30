#include "Animation.h"
#include <SDL2/SDL_ttf.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

Animation::Animation(size_t size, SDL_Rect rect, const Window &wnd)
    : Drawable(rect, wnd) {
  if (size < 1)
    size = 1;

  images.reserve(size);
  for (size_t i = 0; i < size; i++)
    images.emplace_back(rect, wnd);
  timer.Start();
}
Animation::Animation(size_t size, SDL_Rect rect, const Window &wnd,
                     const std::string &folder_path)
    : Drawable(rect, wnd) {
  if (size < 1)
    size = 1;

  images.reserve(size);
  for (size_t i = 0; i < size; i++) {
    try {

      images.emplace_back(folder_path + "/" + std::to_string(i) + ".png", rect,
                          wnd);
    } catch (std::runtime_error &e) {
      // allow placement of empty textures in animation but warn the user with
      // logs.
      std::cerr << "failed to load image number " << i << " in " << folder_path
                << std::endl;
      std::cerr << "loading an empty texture instead..." << std::endl;
      images.emplace_back(rect, wnd);
    }
  }
  timer.Start();
}

std::vector<Texture> images;
Timer timer;
uint32_t TPI =
    100000; //"time per image" the time for each image to be desplayed.
int pause_index = -1;
bool back_and_forth = false;
uint32_t CurrentImageIndex();
uint32_t time_diff = 0;

Animation::Animation(const Animation &other)
    : images(other.images), TPI(other.TPI), pause_index(other.pause_index),
      back_and_forth(other.back_and_forth),
      time_diff(other.time_diff + other.timer.PassedTime().count()),
      Drawable((Drawable)other)

{
  timer.Start();
}

Animation::Animation(Animation &&other)
    : images(std::move(other.images)), TPI(other.TPI),
      pause_index(other.pause_index), back_and_forth(other.back_and_forth),
      time_diff(other.time_diff + other.timer.PassedTime().count()),
      Drawable((Drawable)other)

{
  timer.Start();
}

void Animation::operator=(const Animation &other) {
  images = other.images;
  rect = other.rect;
  TPI = other.TPI;
  pause_index = other.pause_index;
  back_and_forth = other.back_and_forth;
  time_diff =
      other.time_diff + other.time_diff + other.timer.PassedTime().count();
  timer.Start();
}

void Animation::operator=(Animation &&other) {

  images = std::move(other.images);
  rect = other.rect;
  TPI = other.TPI;
  pause_index = other.pause_index;
  back_and_forth = other.back_and_forth;
  time_diff =
      other.time_diff + other.time_diff + other.timer.PassedTime().count();
  timer.Start();
}

void Animation::Draw() {
  int index = CurrentImageIndex();
  images[index].rect = rect;
  images[index].Draw();
}

void Animation::DrawOnWindow(bool scale) {
  int index = CurrentImageIndex();
  images[index].rect = rect;
  images[index].DrawOnWindow(scale);
}

void Animation::DrawEX(float angle, SDL_RendererFlip flip) {
  int index = CurrentImageIndex();
  images[index].rect = rect;
  images[index].DrawEX(angle, flip);
}

void Animation::DrawEXOnWindow(bool scale, float angle, SDL_RendererFlip flip) {
  int index = CurrentImageIndex();
  images[index].rect = rect;
  images[index].DrawEXOnWindow(scale, angle, flip);
}
void Animation::Pause(int index) {
  if (index == -1) {

    time_diff += timer.PassedTime().count();
    pause_index = CurrentImageIndex();
  } else {
    time_diff = 0;
    pause_index = index;
  }
}
void Animation::Resume(int index) {
  if (index >= 0) {
    time_diff = TPI * index;
  }
  if (pause_index >= 0) {
    pause_index = -1;
    timer.Start();
  }
}

uint32_t Animation::CurrentImageIndex() {
  if (images.size() == 1)
    return 0;
  if (pause_index >= 0)
    return pause_index;

  /*
  NOTE:
  the calculation for back and forth is done by calculating the index of a
  vector twice the size and than if the index surpass the array size calculate
  it by reducing it from twice the array size.
   */

  int time = timer.PassedTime().count() + time_diff;

  if (back_and_forth) {

    uint64_t index = (time % (TPI * (images.size() * 2 - 2))) / TPI;
    if (index >= images.size())
      index = (images.size() - 1) * 2 - index;
    return index;
  } else
    return (time % (TPI * images.size())) / TPI;
}

void Animation::SetTimePerImage(size_t MicroSeconds) {
  time_diff = (timer.PassedTime().count() + time_diff) * MicroSeconds / TPI;

  TPI = MicroSeconds;
  timer.Start();
}

void Animation::SetBackAndForth(bool back_and_forth) {
  this->back_and_forth = back_and_forth;
}

void Animation::SetTexture(size_t index, const std::string &file) {
  if (index < images.size())
    images[index] = file;
}

bool Animation::IsRunning() { return pause_index < 0; }
