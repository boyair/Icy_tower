#include "Texture.h"
#include <SDL2/SDL_render.h>
#include <iostream>
#include <ostream>

Texture::Texture(const std::string& file,const Window& wnd)
:window (&wnd),
texture (IMG_LoadTexture(wnd.Renderer,file.c_str()))
{
}

Texture::Texture(const Texture& other)
:window(other.window)
{
    SDL_Point size;
    Uint32 format;
    SDL_QueryTexture(other.texture,&format,0,&size.x,&size.y);
    texture = SDL_CreateTexture( window->Renderer, format, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
    SDL_SetRenderTarget(window->Renderer,texture);
    SDL_Rect TexRect = {0,0,size.x,size.y};
    SDL_RenderCopy(window->Renderer, other.texture, nullptr, &TexRect);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(window->Renderer,0);

}
void Texture::operator= (SDL_Texture* other)
{
texture = other;
}

Texture::Texture(Texture&&  other)
:window(other.window),
texture (other.texture)

{
    other.texture = nullptr;

}

SDL_Texture* Texture::s_tex()const
{
  return texture;
}

void Texture::operator= (const Texture& other)
{
window = other.window;

    SDL_Point size;
    Uint32 format;
    SDL_QueryTexture(other.texture,&format,0,&size.x,&size.y);
    texture = SDL_CreateTexture( window->Renderer, format, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
    SDL_SetRenderTarget(window->Renderer,texture);
    SDL_Rect TexRect = {0,0,size.x,size.y};
    SDL_RenderCopy(window->Renderer, other.texture, nullptr, &TexRect);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(window->Renderer,0);

}
void Texture::operator= (Texture&& other)
{
window = other.window;
texture = other.texture;
other.texture = nullptr;

}


void Texture::Rotate(float angle,SDL_RendererFlip flip, bool resize)
{

    SDL_Point size;
    Uint32 format;
    SDL_QueryTexture(texture,&format,0,&size.x,&size.y);
    int W = size.x;
    int H = size.y;
 
    
    if(angle!=0 && size.x!= size.y && resize)
    {
        float radians = angle*3.14159265/180;
        W = size.x * abs(cos (radians)) + size.y * abs(sin (angle*3.14159265/180));
        H = size.x * abs(sin (radians)) + size.y * abs(cos (angle*3.14159265/180));
    }
    
    SDL_Texture* flipped = SDL_CreateTexture( window->Renderer, format, SDL_TEXTUREACCESS_TARGET, W, H);
    SDL_SetRenderTarget(window->Renderer,flipped);
    SDL_Rect TexRect = {(W-size.x)/2,(H-size.y)/2,size.x,size.y};
    SDL_RenderCopyEx(window->Renderer, texture, nullptr, &TexRect,angle,0,flip);
    SDL_SetTextureBlendMode(flipped, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(window->Renderer,0);
    SDL_DestroyTexture(texture);
  texture = flipped;


}



Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}
