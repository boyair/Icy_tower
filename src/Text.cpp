#include "Text.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>


Text::Text(const std::string& text,const std::string& font_file_path,Window& window,SDL_Rect rect)
:
text(text),
font(TTF_OpenFont(font_file_path.c_str(), 100)),
texture(window,rect)
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


Text::Text(const std::string& text,Window& window,SDL_Rect rect)
:
text(text),
font(TTF_OpenFont("../TT", 100)),
texture(window,rect)
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



Text::~Text()
{
    TTF_CloseFont(font);

}
