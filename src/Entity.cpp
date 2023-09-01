#include "Entity.h"
#include "Texture.h"
#include "Utils.h"
#include <vector>
Entity::Entity(const std::string& texture, SDL_Rect rect,Window& wnd)
:texture(texture,wnd,rect),
position      (Vec2((float)rect.x,(float)rect.y)),
velocity      (Vec2(0 , 0)),
acceleration  (Vec2(0 , 0)),
hitbox(rect)
{



}

Entity::Entity(const Entity& other)
:texture(other.texture),
position     (other.position    ),
velocity     (other.velocity    ),
acceleration (other.acceleration),
hitbox(other.hitbox),
standing(other.standing)
{




}
Entity::Entity(Entity&& other)
:texture(std::move(other.texture)),
position     (other.position    ),
velocity     (other.velocity    ),
acceleration (other.acceleration),
hitbox(other.hitbox),
standing(other.standing)

{
}
Entity::Entity(const Texture& texture, SDL_Rect rect)
:texture(texture),
position     {(float)rect.x,(float)rect.y},
velocity     {0,0},
acceleration {0,0},
hitbox(rect),
standing(false)
{



}


void Entity::operator=(const Entity& other)
{
    texture = other.texture;
    position = other.position;
    velocity = other.velocity;
    acceleration = other.acceleration;
    hitbox = other.hitbox;
    standing = other.standing;


}


void Entity::DrawEX(float angle,SDL_RendererFlip flip)
{
    texture.DrawEX(texture.rect,angle, flip);
}


std::ostream& operator <<(std::ostream& out, Vec2 vector)
{
    out<<vector.x<<" , "<<vector.y;
    return out;
}



void Entity::Draw()
{
  texture.Draw();
}


void Entity::Update(unsigned int microseconds)
{
    float milliseconds = microseconds/1000.0f;
    velocity += acceleration * milliseconds;
    position += velocity * milliseconds;
    hitbox.x = position.x; hitbox.y = position.y;
    Utils::FitCenter(hitbox, texture.rect);
    standing = false;

}


void Entity::Resize(SDL_Point size)
{
    texture.rect.w *= size.x/hitbox.w; 
    texture.rect.h *= size.y/hitbox.h; 
    position.y += hitbox.h - size.y; //moves the Entity such that his bottom part will stay in the same hight instead of its top.
    hitbox.y = position.y;
    hitbox.w = size.x;
    hitbox.h = size.y;
    texture.rect.x = hitbox.x + hitbox.w/2 - texture.rect.w/2;
    texture.rect.y = hitbox.y + hitbox.h/2 - texture.rect.h/2;
//    texture.rect.w = hitbox.w;
//    texture.rect.h = hitbox.h;
    //if (hitbox.h == 120)
    
    
}
void Entity::Stop()
{
    velocity.x = 0;
    velocity.y = 0;
}

  void Entity::Repos(SDL_Point pos)
{
    position.x = pos.x;
    hitbox.x = pos.x;
    position.y = pos.y;
    hitbox.y = pos.y;
    Utils::FitCenter(hitbox, texture.rect);

}

  void Entity::Move(SDL_Point delta)
{
  position.x += delta.x;
  hitbox.x += delta.x;
  position.y += delta.y;
  hitbox.y = delta.y;
    Utils::FitCenter(hitbox, texture.rect);
}


  void Entity::Resize(int w,int h)
{
     
     
    hitbox.y = position.y;
    texture.rect.w *= float(w)/hitbox.w;
    texture.rect.h *= float(h)/hitbox.h;
    hitbox.w = w;
    hitbox.h = h;
    Utils::FitCenter(hitbox, texture.rect);
}
  void Entity::Repos(int x,int y)
{
    position.x = x;
    hitbox.x = x;
    position.y = y;
    hitbox.y = y;
    Utils::FitCenter(hitbox, texture.rect);
}


  void Entity::Move(int x,int y)
{
  position.x += x;
  hitbox.x += x;
  position.y += y;
  hitbox.y = y;
}

  SDL_Point Entity::GetSize()
{
  return SDL_Point{hitbox.w,hitbox.h};
}
const bool Entity::Standing() const
{
  return standing;
}
Side Entity::CheckCollision(const SDL_Rect& other)
{
    float interX = ((position.x+hitbox.w)-(other.x +other.w)) + (position.x - other.x);

    bool  interX_left=  interX < 0;

    float interY =  ((position.y+hitbox.h)-(other.y +other.h)) + (position.y - other.y);

    bool  interY_up=  interY < 0;
      if(interX < (hitbox.w+other.w) && interX > 0 - (hitbox.w+other.w) &&
        interY < (hitbox.h+other.h) && interY > 0 - (hitbox.h+other.h))
        {
                if((hitbox.w+other.w)-abs(interX)>(hitbox.h+other.h)-abs( interY ))
                {
                    if(interY_up)
                        return Side::top;
                    return Side::bottom;


                }
                else
                {
                    if(interX_left)
                        return Side::left;
                    return Side::right;

                }


        }

    return Side::none;
}


Side Entity::AvoidCollision(const SDL_Rect& other)
{

    float interX = ((position.x+hitbox.w)-(other.x +other.w)) + (position.x - other.x);

    bool  interX_left=  interX < 0;

    float interY =  ((position.y+hitbox.h)-(other.y +other.h)) + (position.y - other.y);

    bool  interY_up=  interY < 0;

    if (( interY + (hitbox.h+other.h)) == 0 && interX < (hitbox.w+other.w) && interX > 0 -(hitbox.w+other.w))
        standing = true;

    // if there is a collision
    if(interX < (hitbox.w+other.w) && interX > 0 - (hitbox.w+other.w) &&
        interY < (hitbox.h+other.h) && interY > 0 - (hitbox.h+other.h))
     {
        if((hitbox.w+other.w)-abs(interX)>(hitbox.h+other.h)-abs( interY ))
            {

                if(interY_up)
                    {

                        position.y = other.y-hitbox.h;
                        standing = true;
                    }
                else
                {
                    position.y = other.y+other.h;
                }
                hitbox.y =position.y;
                texture.rect.y = position.y;
                velocity.y = 0;
                if(interY_up)
                    return Side::top;
                return Side::bottom;
            }
        else
        {

            if(interX_left)
                {
                    position.x = other.x-hitbox.w;

                }
            else
            {
                position.x = other.x+other.w;
            }
            hitbox.x = position.x;
            texture.rect.x = position.x;
            velocity.x = 0;
                if(interX_left)
                    return Side::left;
                return Side::right;
 
        }
    }
    return Side::none;
}

void Entity::ChangeTexture(const Texture& texture)
{
    this->texture = texture;

}
void Entity::ChangeTexture(Texture&& texture)
{
    this->texture = ((Texture&&) texture);
}


























  Entity::~Entity()
{

}

void SortByHeight(std::vector<Entity*>& entitys)
{
    for (long unsigned int i=1;i<entitys.size();i++) 
    {
        for(int j = i;j > 0 && entitys[j]->hitbox.y > entitys[j-1]->hitbox.y;j--)
        {
            Entity* temp = entitys[j];
            entitys[j] = entitys[j-1];
            entitys[j-1] = temp;

        }
    }
    
}
