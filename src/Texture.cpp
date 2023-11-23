#include "Texture.h"
#include "Window.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>


Texture::Texture(SDL_Rect rect,const Window& wnd)
:window(&wnd),
texture(nullptr),
rect{rect}
{
}



Texture::Texture(const std::string& file,SDL_Rect rect,const Window& wnd)
:window (&wnd),
texture ( IMG_LoadTexture(wnd.Renderer,file.c_str())),
rect(rect)
{
    if(!texture)
        throw std::runtime_error(std::string("Error: Failed to load file:") + file + std::string("\nsuggestion: make sure SDL2 has been initiallized and the file path leads to an existing file."));
}

Texture::Texture(const Texture& other)
:window(other.window)
,rect(other.rect)
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


void Texture::operator= (SDL_Surface* other)
{
    if (texture)
        SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(window->Renderer,other);
}


void Texture::operator= (SDL_Texture* other)
{
    if (texture)
        SDL_DestroyTexture(texture);
    texture = other;
}

Texture::Texture(Texture&&  other)
:window(other.window),
texture (other.texture)
,rect(other.rect)
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
rect = other.rect;
    SDL_Point size;
    Uint32 format;
    SDL_QueryTexture(other.texture,&format,0,&size.x,&size.y);
    if (texture)
        SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture( window->Renderer, format, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
    SDL_SetRenderTarget(window->Renderer,texture);
    SDL_Rect TexRect = {0,0,size.x,size.y};
    SDL_RenderCopy(window->Renderer, other.texture, nullptr, &TexRect);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(window->Renderer,0);

}

void Texture::operator= (const std::string& file)
{
    if (texture)
        SDL_DestroyTexture(texture);
    texture = IMG_LoadTexture(window->Renderer, file.c_str());
}


void Texture::operator= (Texture&& other)
{
    if (texture)
        SDL_DestroyTexture(texture);


window = other.window;
texture = other.texture;
other.texture = nullptr;

}


Vec2 Texture::Rotate(float angle,SDL_RendererFlip flip, bool resize)
{

    SDL_Point size;
    Uint32 format;
    SDL_QueryTexture(texture,&format,0,&size.x,&size.y);
    int W = size.x;
    int H = size.y;


    if(angle!=0 && size.x!= size.y)
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
    //resize the texture rect such that the rotated texture will look the same size and wont shrink in order to fit in the rect. 
    if (!resize)
    {
        rect.x -= (W-size.x)/2;
        rect.y -= (H-size.y)/2;
        rect.w *= float(W)/size.x;
        rect.h *= float(H)/size.y;
    }
    return {(float)W/size.x,(float)H/size.y};
}



Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}
