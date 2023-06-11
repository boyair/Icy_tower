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
void PEntity::Update(unsigned int time)
{
    prevposition = position;  
    velocity += acceleration * time;
    position += velocity * time;
    Rect.x = position.x; Rect.y = position.y;
    standing = false;

}

//  void PEntity::AvoidCollision(const SDL_Rect& other)
//{
//
//   int interX = (Rect.w+other.w) -
//    (((position.x+Rect.w)-(other.x +other.w)) + (position.x - other.x));
//  bool  interX_left=  interX < (Rect.w+other.w) * 2 && interX > (Rect.w+other.w)  ;
//  //bool  interX_right=  interX >0 && interX < (Rect.w+other.w) ;
//
// int interY = (Rect.h+other.h) -
//    (((position.y+Rect.h)-(other.y +other.h)) + (position.y - other.y));
//
//  bool  interY_up=  interY < (Rect.h+other.h)*2 && interY > (Rect.h+other.h)  ;
//  //bool  interY_down=  interY >0 && interY < (Rect.h+other.h) ;
//
//
//    if(interX < (Rect.w+other.w)*2 && interX > 0&&
//        interY < (Rect.h+other.h)*2 && interY > 0)
//
//    {
//        if((Rect.w+other.w)-abs(interX - (Rect.w+other.w))>(Rect.h+other.h)-abs( interY - (Rect.h+other.h)))
//            {
//                if(interY_up)
//                    {
//                        position.y = other.y-Rect.h;
//                        Rect.y =position.y;
//                        standing = true;
//                    }
//                else
//                {
//                    position.y = other.y+other.h;
//                    Rect.y =position.y;
//                }
//
//            }
//        else
//        {
//
//            if(interX_left)
//                {
//                    position.x = other.x-Rect.w;
//                    Rect.x = position.x;
//
//                }
//
//
//            else
//            {
//                position.x = other.x+other.w;
//                Rect.x = position.x;
//            }
//
//
//        }
//    }
//}


  void PEntity::AvoidCollision(PEntity& otherentity)
{

    const SDL_Rect& other = otherentity.Rect;
    int width_sum = Rect.w + other.w, height_sum = Rect.h+other.h;
    int interX = width_sum -
        (((position.x+Rect.w)-(other.x +other.w)) + (position.x - other.x));
    bool  interX_left=  interX < width_sum * 2 && interX > width_sum  ;
    //bool  interX_right=  interX >0 && interX < (Rect.w+other.w) ;

    int interY = height_sum -
        (((position.y+Rect.h)-(other.y +other.h)) + (position.y - other.y));


    bool  interY_up=  interY < height_sum * 2 && interY > height_sum;
    //bool  interY_down=  interY >0 && interY < (Rect.h+other.h) ;


    if(interX < width_sum * 2 && interX > 0 &&
        interY < height_sum * 2 && interY > 0)
    {
        if(width_sum - abs(interX - width_sum) > height_sum - abs(interY - height_sum))
            {
                if(interY_up)
                    {
                        position.y = other.y-Rect.h;
                        Rect.y =position.y;
                        standing = true;
                    }
                else
                {
                    position.y = other.y+other.h;
                    Rect.y =position.y;
                }
                float system_force = ((velocity.y - otherentity.velocity.y) *std::min(otherentity.mass , mass));
                ApplyForce({0,system_force *-1});
                otherentity.ApplyForce({0,system_force});
            }
        else
        {

            if(interX_left)
                {
                    position.x = other.x-Rect.w;
                    Rect.x = position.x;

                }


            else
            {
                position.x = other.x+other.w;
                Rect.x = position.x;



            }

            Vec2 system_force = {(velocity.x - otherentity.velocity.x) *std::min(otherentity.mass , mass),std::min(friction_cof,otherentity.friction_cof)/1000.0f*(velocity.y - otherentity.velocity.y)};
            ApplyForce(system_force *-1);
            otherentity.ApplyForce(system_force);
            if(other.h == 120)
            {    
            
                std::cout<<"vel: "<<velocity.x*mass<<std::endl;
                std::cout<<"other.vel"<<otherentity.velocity.x<<std::endl;
                //std::cout<<system_force<<std::endl;
            }
        }
    }
}


void PEntity::LimitSpeed(float limit)
{
    if((float)velocity>limit)
    {
        velocity /= (float)velocity/limit; 
    }
}

