#include "Text.h"
#include "Drawable.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <memory>
#include <ostream>
SDL_Texture **start_button_texture = nullptr;

Text::Text(Window &window, SDL_Rect rect, const std::string &text,
           SDL_Color color, const std::string &font_file_path)
    : Drawable(rect, window), text(text), color(color),
      font(TTF_OpenFont(font_file_path.c_str(), 100)) {
  HandleFontLoadingError(font_file_path);
  if (text != "")
    RecreateTexture();
}

Text::Text(const Text &other)
    : Drawable(static_cast<Drawable>(other)), text(other.text),
      color(other.color), font(other.font) {
  RecreateTexture();
}

std::unique_ptr<Drawable> Text::Clone() const {
  return std::make_unique<Text>(*this);
}
void Text::ChangeColor(SDL_Color new_color) { color = new_color; }
// Drawing
// --------------------------------------------------------------------------------------------------------------------------
void Text::Draw() {
  if (SDL_HasIntersection(&rect, &window->CameraView)) {
    SDL_Rect rect_on_cam = window->GetRelativeRect(rect);
    SDL_RenderCopy(window->Renderer, texture, 0, &rect_on_cam);
  }
}
void Text::DrawOnWindow(bool scale) {
  SDL_Rect scaled_rect = scale ? window->ScaleRect(rect) : rect;
  // if (!texture)
  //   std::cout << text << std::endl;
  //  std::cout << text << ": " << scaled_rect.x << ", " << scaled_rect.y << ",
  //  "
  //            << scaled_rect.w << ", " << scaled_rect.h << std::endl;
  SDL_RenderCopy(window->Renderer, texture, 0, &scaled_rect);
}
void Text::DrawEX(float angle, SDL_RendererFlip flip) {
  if (SDL_HasIntersection(&rect, &window->CameraView)) {
    SDL_Rect rect_on_cam = window->GetRelativeRect(rect);
    SDL_RenderCopyEx(window->Renderer, texture, 0, &rect_on_cam, angle, 0,
                     flip);
  }
}
void Text::DrawEXOnWindow(bool scale, float angle, SDL_RendererFlip flip) {

  SDL_Rect scaled_rect = scale ? window->ScaleRect(rect) : rect;
  SDL_RenderCopyEx(window->Renderer, texture, 0, &scaled_rect, angle, 0, flip);
}
//--------------------------------------------------------------------------------------------------------------------------
void Text::ChangeFont(const std::string &filepath) {
  if (font)
    TTF_CloseFont(font);
  font = TTF_OpenFont(filepath.c_str(), 100);
}

void Text::ChangeText(const std::string &text) { this->text = text; }

void Text::Reposition(SDL_Point position) {
  rect.x = position.x;
  rect.y = position.y;
}
void Text::Resize(SDL_Point size) {
  rect.w = size.x;
  rect.h = size.y;
}

void Text::RecreateTexture() {
  SDL_Surface *tempsurf = TTF_RenderText_Solid(font, text.c_str(), color);
  texture = SDL_CreateTextureFromSurface(window->Renderer, tempsurf);
  SDL_FreeSurface(tempsurf);
}

const SDL_Texture *Text::Texture() const { return texture; }

void Text::HandleFontLoadingError(const std::string &filepath) {
  if (!font) {
    if (strcmp(SDL_GetError(), "Library not initialized") == 0) {
      font = TTF_OpenFont(filepath.c_str(), 100);
    } else
      std::cout << "font not loaded: " << SDL_GetError() << std::endl;
  }
}
