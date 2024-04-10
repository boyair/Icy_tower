#pragma once
#include "Drawable.h"
#include "Texture.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

class Text : public Drawable {

public:
  // constructors
  Text(Window &window, SDL_Rect rect, const std::string &text = "",
       SDL_Color color = {255, 255, 255, 255},
       const std::string &font_file_path = "../fonts/font.ttf");
  Text(const Text &other);
  std::unique_ptr<Drawable> Clone() const;

  void Draw();
  void DrawEX(float angle, SDL_RendererFlip flip);
  void DrawOnWindow(bool scale);
  void DrawEXOnWindow(bool scale, float angle, SDL_RendererFlip flip);

  // mofication functions:
  void ChangeColor(SDL_Color new_color); // requires recreation of texture
                                         // (slow)
  void
  ChangeText(const std::string &text); // requires recreation of texture (slow)
  void ChangeFont(
      const std::string &filepath); // requires recreation of texture (slow)

  // space modifications (dosent require texture recreation.):
  void Reposition(SDL_Point position);
  void Resize(SDL_Point size);

  SDL_Rect GetRect() { return rect; };

  void RecreateTexture(); // recreates the texture to apply changes after
                          // modification functions
  const SDL_Texture *Texture() const;
  std::string text;
  SDL_Color color;
  TTF_Font *font;

private:
  void HandleFontLoadingError(const std::string &filepath);

protected:
  SDL_Texture *texture = nullptr;
};
