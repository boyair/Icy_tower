#include "Text.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

Text::Text(SDL_Color color,Window& window,SDL_Rect rect)
:
    color(color),
    font(TTF_OpenFont("../fonts/font.ttf", 100)),
    texture(rect,window)
{


}

Text::Text(const std::string& text,const std::string& font_file_path,Window& window,SDL_Rect rect)
:
    text(text),
    color({255,255,255,255}),
    font(TTF_OpenFont(font_file_path.c_str(), 100)),
    texture(rect,window)
{
    HandleFontLoadingError(font_file_path);
   RecreateTexture();
}
Text::Text(SDL_Color color,const std::string& font_file_path,Window& window,SDL_Rect rect)
:
    color(color),
    font(TTF_OpenFont("../fonts/font.ttf", 100)),
    texture(rect,window)


{
    HandleFontLoadingError(font_file_path);
   RecreateTexture();


}


Text::Text(const std::string& text,Window& window,SDL_Rect rect)
:
text(text),
color({255,255,255,255}),
font(TTF_OpenFont("../fonts/font.ttf", 100)),
texture(rect,window)
{
    HandleFontLoadingError("../fonts/font.ttf");
    RecreateTexture();
}


Text::Text(const std::string& text,SDL_Color color,Window& window,SDL_Rect rect)
:
        text(text),
        color(color),
        font(TTF_OpenFont("../fonts/font.ttf", 100)),
        texture(rect,window)

{
    if(!font)
    {
        if(strcmp(SDL_GetError(), "Library not initialized") == 0)
        {
            TTF_Init();
            font = TTF_OpenFont("../fonts/font.ttf", 100);
        }
        else
            std::cout<<"font not loaded: "<<SDL_GetError()<<std::endl;

    }
    RecreateTexture();

}
Text::Text(const std::string& text,SDL_Color color,const std::string& font_file_path,Window& window,SDL_Rect rect)
:
text(text),
color(color),
font(TTF_OpenFont(font_file_path.c_str(), 100)),
texture(rect,window)

{
    if(!font)
    {
        if(strcmp(SDL_GetError(), "Library not initialized") == 0)
        {
            TTF_Init();
            font = TTF_OpenFont(font_file_path.c_str(), 100);
        }
        else
            std::cout<<"font not loaded: "<<SDL_GetError()<<std::endl;

    }
    RecreateTexture();

}




void Text::ChangeColor(SDL_Color new_color)
{
   color = new_color;
}



void Text::Draw()
{
    texture.DrawOnWindow();
}
void Text::ChangeFont(const std::string& filepath)
{
    TTF_CloseFont(this->font);
    this->font = TTF_OpenFont(filepath.c_str(), 100) ;

}


void Text::ChangeText(const std::string& text)
{
    this->text = text; 
}


void Text::Reposition(SDL_Point position)
{
    texture.rect.x = position.x;
    texture.rect.y = position.y;
}
void Text::Resize(SDL_Point size)
{
    texture.rect.w = size.x;
    texture.rect.h = size.y;
}




void Text::RecreateTexture()
{
    SDL_Surface* tempsurf = TTF_RenderText_Solid(font, text.c_str(), color);
    texture = tempsurf;
    SDL_FreeSurface(tempsurf);


}

void Text::HandleFontLoadingError(const std::string& filepath)
{
     if(!font)
    {
        if(strcmp(SDL_GetError(), "Library not initialized") == 0)
        {
            TTF_Init();
            font = TTF_OpenFont(filepath.c_str(), 100);
        }
        else
            std::cout<<"font not loaded: "<<SDL_GetError()<<std::endl;
    }
    
}

Text::~Text()
{
    TTF_CloseFont(font);

}
