#include "Entity.h"

Entity::Entity(const std::string& texture, SDL_Rect rect,Window& wnd)
:texture(texture,wnd,rect),
position      ({float(rect.x),float(rect.y)}),
velocity      ({0,0}),
acceleration  ({0,0}),
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

Entity::Entity(const Texture& texture, SDL_Rect rect)
:texture(texture),
position     {(float)rect.x,(float)rect.y},
velocity     {0,0},
acceleration {0,0},
hitbox(rect),
standing(false)
{



}

std::ostream& operator <<(std::ostream& out, Vec2 vector)
{
    out<<vector.x<<" , "<<vector.y;
    return out;
}

Entity::Entity(Entity&& other)
:
texture((Texture&&)(other.texture)),
position     (other.position    ),
velocity     (other.velocity    ),
acceleration (other.acceleration),
  hitbox(other.hitbox),
standing(other.standing)

{
}

void Entity::Draw()
{
  texture.Draw();
}


void Entity::Update(unsigned int time)
{
    velocity += acceleration * time;
    position += velocity * time;
    hitbox.x = position.x; hitbox.y = position.y;
    texture.rect.x = hitbox.x + hitbox.w/2 - texture.rect.w/2;
    texture.rect.y = hitbox.y + hitbox.h/2 - texture.rect.h/2;
//    texture.rect.w = hitbox.w;
//    texture.rect.h = hitbox.h;
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

  void Entity::Repos(SDL_Point pos)
{
  position.x = pos.x;
  hitbox.x = pos.x;
  position.y = pos.y;
  hitbox.y = pos.y;
}


  void Entity::Move(SDL_Point delta)
{
  position.x += delta.x;
  hitbox.x += delta.x;
  position.y += delta.y;
  hitbox.y = delta.y;
}


  void Entity::Resize(int w,int h)
{
     
     
    position.y += hitbox.h - h; //moves the Entity such that his bottom part will stay in the same hight instead of its top.
    hitbox.y = position.y;
    texture.rect.w *= float(w)/hitbox.w;
    texture.rect.h *= float(h)/hitbox.h;
    hitbox.w = w;
    hitbox.h = h;
    texture.rect.x = hitbox.x + hitbox.w/2 - texture.rect.w/2;
    texture.rect.y = hitbox.y + hitbox.h/2 - texture.rect.h/2;

    //texture.rect = hitbox;
}
  void Entity::Repos(int x,int y)
{
  position.x = x;
  hitbox.x = x;
  position.y = y;
  hitbox.y = y;
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

  void Entity::AvoidCollision(const SDL_Rect& other)
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


        }
    }
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
