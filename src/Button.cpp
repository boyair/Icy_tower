#include "Button.h"
#include "Utils.h"
#include "Window.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

Button::Button(std::shared_ptr<Drawable> visual) : visual(visual) {}
Button::Button(const Button &other) : visual(other.visual.get()->Clone()) {}
Button::Button(Button &&other) : visual(other.visual) {}

bool Button::Hovered() {
  int mx, my;
  SDL_GetMouseState(&mx, &my);
  SDL_Rect rect = visual->window->ScaleRect(visual->rect);
  return mx > rect.x && mx < rect.w + rect.x && my > rect.y &&
         my < rect.h + rect.y;
}

void Button::HandleEvent(const SDL_Event &event) {
  if (Hovered()) {
    if (on_start_hover && !was_hovered)
      on_start_hover();
    if (on_click && event.type == SDL_MOUSEBUTTONDOWN &&
        event.button.button == SDL_BUTTON_LEFT) {
      on_click();
    }
    was_hovered = true;
  } else {
    if (was_hovered && on_stop_hover)
      on_stop_hover();
    was_hovered = false;
  }
}

void Button::Scale(float scale) {
  visual->rect = Utils::Scale(visual->rect, scale);
}

void Button::Draw() { visual->DrawOnWindow(true); }

Button::~Button() {}
