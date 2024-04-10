#include "Button.h"
#include "Window.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

Button::Button(std::shared_ptr<Drawable> visual) : visual(visual) {}
Button::Button(const Button &other) : visual(other.visual.get()->Clone()) {}

bool Button::Hovered() {
  int mx, my;
  SDL_GetMouseState(&mx, &my);
  SDL_Rect rect = visual->window->ScaleRect(visual->rect);
  return mx > rect.x && mx < rect.w + rect.x && my > rect.y &&
         my < rect.h + rect.y;
}

void Button::HandleEvent(const SDL_Event &event) {
  if (on_click && event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.button == SDL_BUTTON_LEFT) {
    if (Hovered()) {
      on_click();
    }
  }
}

void Button::Draw() {
  // creates temporary rectangle to draw the frame of the button

  // get the original renderer color so that it can be reverted after drawing
  // the frame.
  visual->DrawOnWindow(true);
}

Button::~Button() {}
