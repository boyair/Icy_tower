#include "Entity.h"

Entity::Entity(const std::string& texture, SDL_Rect rect,Window& wnd)
:texture(texture,wnd),
position      ({float(rect.x),float(rect.y)}),
velocity      ({0,0}),
acceleration  ({0,0}),
Rect(rect)
{



}

Entity::Entity(const Entity& other)
:texture(other.texture),
position     (other.position    ),
velocity     (other.velocity    ),
acceleration (other.acceleration),
Rect(other.Rect),
standing(other.standing)
{

}

Entity::Entity(const Texture& texture, SDL_Rect rect)
:texture(texture),
position     {(float)rect.x,(float)rect.y},
velocity     {0,0},
acceleration {0,0},
Rect(rect),
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
  Rect(other.Rect),
standing(other.standing)

{
}

void Entity::Draw()
{
  texture.Draw(Rect);
}


void Entity::Update(unsigned int time)
{
    velocity += acceleration * time;
    position += velocity * time;
    Rect.x = position.x; Rect.y = position.y;
    standing = false;

}


  void Entity::Resize(SDL_Point size)
{

position.y += Rect.h - size.y; //moves the Entity such that his bottom part will stay in the same hight instead of its top.
Rect.y = position.y;
Rect.w = size.x;
Rect.h = size.y;
}

  void Entity::Repos(SDL_Point pos)
{
  position.x = pos.x;
  Rect.x = pos.x;
  position.y = pos.y;
  Rect.y = pos.y;
}


  void Entity::Move(SDL_Point delta)
{
  position.x += delta.x;
  Rect.x += delta.x;
  position.y += delta.y;
  Rect.y = delta.y;
}


  void Entity::Resize(int w,int h)
{
if(h > 0)
  {
    position.y += Rect.h - h;//moves the Entity such that his bottom part will stay in the same hight instead of its top
    Rect.y = position.y;
    Rect.h = h;
  }
if(w>0)
    Rect.w = w;
}
  void Entity::Repos(int x,int y)
{
  position.x = x;
  Rect.x = x;
  position.y = y;
  Rect.y = y;
}


  void Entity::Move(int x,int y)
{
  position.x += x;
  Rect.x += x;
  position.y += y;
  Rect.y = y;
}

  SDL_Point Entity::GetSize()
{
  return SDL_Point{Rect.w,Rect.h};
}
const bool Entity::Standing() const
{
  return standing;
}

  void Entity::AvoidCollision(const SDL_Rect& other)
{

    int interX = (Rect.w+other.w) -
        (((position.x+Rect.w)-(other.x +other.w)) + (position.x - other.x));
    bool  interX_left=  interX < (Rect.w+other.w) * 2 && interX > (Rect.w+other.w)  ;
    //bool  interX_right=  interX >0 && interX < (Rect.w+other.w) ;

    int interY = (Rect.h+other.h) -
        (((position.y+Rect.h)-(other.y +other.h)) + (position.y - other.y));

    bool  interY_up=  interY < (Rect.h+other.h)*2 && interY > (Rect.h+other.h)  ;
    //bool  interY_down=  interY >0 && interY < (Rect.h+other.h) ;

    if(interX < (Rect.w+other.w)*2 && interX > 0&&
        interY < (Rect.h+other.h)*2 && interY > 0)
    {
        if((Rect.w+other.w)-abs(interX - (Rect.w+other.w))>(Rect.h+other.h)-abs( interY - (Rect.h+other.h)))
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
