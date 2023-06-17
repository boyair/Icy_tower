#include "PEntity.h"
#include "Entity.h"
#include <algorithm>
#include <cmath>
#include <iostream>
PEntity::PEntity(const std::string& texture, SDL_Rect rect,Window& wnd)
:Entity(texture, rect, wnd)
{



}


PEntity::PEntity(const Texture& texture, SDL_Rect rect)
:Entity(texture , rect)
{



}
    void PEntity::ChangeMass(float NewMass)
{
mass = NewMass;
}

    void PEntity::Gravitate(Vec2 point, float strength)
{
    Vec2 diraction= (point - position)/GetDistance(point);
    velocity +={strength*diraction.x,strength*diraction.y};
}
//void PEntity::Update(unsigned int time)
//{
//    prevposition = position;  
//    velocity += acceleration * time;
//    position += velocity * time;
//    hitbox.x = position.x; hitbox.y = position.y;
//    texture.rect.x = hitbox.x + hitbox.w/2 - texture.rect.w/2;
//    texture.rect.y = hitbox.y + hitbox.h/2 - texture.rect.h/2;
//
//    standing = false;
//
//}
  void PEntity::PhysicsCollision(const SDL_Rect& other)
{

    int interX = (hitbox.w+other.w) -
        (((position.x+hitbox.w)-(other.x +other.w)) + (position.x - other.x));
    bool  interX_left=  interX < (hitbox.w+other.w) * 2 && interX > (hitbox.w+other.w)  ;
    //bool  interX_right=  interX >0 && interX < (hitbox.w+other.w) ;

    int interY = (hitbox.h+other.h) -
        (((position.y+hitbox.h)-(other.y +other.h)) + (position.y - other.y));

    bool  interY_up=  interY < (hitbox.h+other.h)*2 && interY > (hitbox.h+other.h)  ;
    //bool  interY_down=  interY >0 && interY < (hitbox.h+other.h) ;

    if(interX < (hitbox.w+other.w)*2 && interX > 0&&
        interY < (hitbox.h+other.h)*2 && interY > 0)
    {
        if((hitbox.w+other.w)-abs(interX - (hitbox.w+other.w))>(hitbox.h+other.h)-abs( interY - (hitbox.h+other.h)))
            {
                if(interY_up)
                    {
                        position.y = other.y-hitbox.h;
                        hitbox.y =position.y;
                        standing = true;
                    }
                else
                {
                    position.y = other.y+other.h;
                    hitbox.y =position.y;
                }
            velocity.y *= -1;
            }
        else
        {

            if(interX_left)
                {
                    position.x = other.x-hitbox.w;
                    hitbox.x = position.x;

                }


            else
            {
                position.x = other.x+other.w;
                hitbox.x = position.x;



            }

        velocity.x *= -1;
        }
    }
}
  void PEntity::PhysicsCollision(PEntity& otherentity)
{

    const SDL_Rect& other = otherentity.hitbox;
    int width_sum = hitbox.w + other.w, height_sum = hitbox.h+other.h;
    int interX = width_sum -
        (((position.x+hitbox.w)-(other.x +other.w)) + (position.x - other.x));
    bool  interX_left=  interX < width_sum * 2 && interX > width_sum  ;
    //bool  interX_right=  interX >0 && interX < (hitbox.w+other.w) ;

    int interY = height_sum -
        (((position.y+hitbox.h)-(other.y +other.h)) + (position.y - other.y));


    bool  interY_up=  interY < height_sum * 2 && interY > height_sum;
    //bool  interY_down=  interY >0 && interY < (hitbox.h+other.h) ;


    if(interX < width_sum * 2 && interX > 0 &&
        interY < height_sum * 2 && interY > 0)
    {
        if(width_sum - abs(interX - width_sum) > height_sum - abs(interY - height_sum))
            {
                if(interY_up)
                    {
                        position.y = other.y-hitbox.h;
                        hitbox.y =position.y;
                        standing = true;
                    }
                else
                {
                    position.y = other.y+other.h;
                    hitbox.y =position.y;
                }
                float system_force = ((velocity.y - otherentity.velocity.y) *std::min(otherentity.mass , mass));
                ApplyForce({0,system_force *-1});
                otherentity.ApplyForce({0,system_force});
            }
        else
        {

            if(interX_left)
                {
                    position.x = other.x-hitbox.w;
                    hitbox.x = position.x;

                }


            else
            {
                position.x = other.x+other.w;
                hitbox.x = position.x;



            }

            Vec2 system_force = {(velocity.x - otherentity.velocity.x) *std::min(otherentity.mass , mass),std::min(friction_cof,otherentity.friction_cof)/1000.0f*(velocity.y - otherentity.velocity.y)};
            ApplyForce(system_force *-1);
            otherentity.ApplyForce(system_force);
        }
    }
}

void PEntity::Limit (const SDL_Rect& border)
{
    if(position.x+hitbox.w>border.x+border.w)
    {
        position.x = border.x + border.w - hitbox.w;
        hitbox.x = position.x;
        velocity.x=0;
    }

    if(position.x<border.x)
    {
        position.x = border.x;
        hitbox.x = position.x;
        velocity.x=0;
    }
    if(position.y+hitbox.h>border.y+border.h)
    {
        position.y = border.y + border.h - hitbox.h;
        hitbox.y = position.y;
        velocity.y=0;
    }

    if(position.y<border.y)
    {
        position.y = border.y;
        hitbox.y = position.y;
        velocity.y=0;
    }
}

void PEntity::LimitSpeed(float limit)
{
    if((float)velocity>limit)
    {
        velocity /= (float)velocity/limit; 
    }
}

