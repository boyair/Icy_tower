#include "Player.h"
#include "PEntity.h"
#include <SDL2/SDL_render.h>


const std::string texturefolder = "../textures/";
const std::string soundfolder = "../sounds/";


Player::Player(SDL_Rect rect,Window& window)
    :PEntity(std::string( texturefolder+ "player.png"),rect,window),
    direction(SDL_FLIP_NONE)
{
    jumpsound   = Mix_LoadWAV((soundfolder+"jump.wav" ).c_str());
    acceleration.y = 0.004;
    friction_cof = 0.2;
    mass = 70;
}

void Player::Draw()
{
    PEntity::DrawEX(0, direction);

}
void Player::SetLookDiraction()
{
    if(velocity.x>0)

        direction = SDL_FLIP_NONE;

    else if(velocity.x<0) 

        direction = SDL_FLIP_HORIZONTAL;



}
void Player::LimitXSpeed(float speed)
{
    if (velocity.x<-1 * speed)
        velocity.x = -1 * speed;
    if (velocity.x>speed)
        velocity.x = speed;

}

void Player::LimitXpos(int min,int max)
{
    if(position.x<min)
    {
        position.x =min;
        velocity.x *= -0.7;
    }
    if(position.x>max)
    {
        position.x = max;
        velocity.x *= -0.7;
    }


}




void Player::HandleInput(const SDL_Event& event)
{
    const auto& key = event.key.keysym.scancode;
    if(event.type == SDL_KEYDOWN)
    {
        if (key == SDL_SCANCODE_D&& event.key.repeat == 0)
        {

            acceleration.x += 0.002;

        }
        if (key == SDL_SCANCODE_A && event.key.repeat == 0)
        {
            acceleration.x += -0.002;

        }
        if (key == SDL_SCANCODE_S && event.key.repeat == 0)
        {

        }

        if ((key == SDL_SCANCODE_W || key == SDL_SCANCODE_SPACE) && event.key.repeat == 0 && standing)
        {

            int channel = Mix_PlayChannel( -1, jumpsound, 0 );
            Mix_Volume( channel, 15);
            velocity.y = std::abs(velocity.x) * -2 - 0.5;
        }

    }
    else if(event.type == SDL_KEYUP)
    {
        if (key == SDL_SCANCODE_D && event.key.repeat == 0)
        {
            acceleration.x += -0.002;
        }
        if (key == SDL_SCANCODE_A && event.key.repeat == 0)
        {
            acceleration.x += 0.002;
        }
    }

}
