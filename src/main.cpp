#include "Game.h"

//function declarations
PEntity* ChooseBox(std::vector<PEntity>& boxes,int mousex,int mousey);



// Create a window


Entity  canon(texturefolder+"canon.png",{1000,600,200,100},window);
Entity  grass(texturefolder+"grass.png",{0,700,1700,200},window);
Entity  wall(texturefolder+"grass.png",{900,470,200,230},window);
PEntity player(texturefolder + "player.png",{300,500,60,60},window);
PEntity canon_ball(texturefolder+"canon_ball.png", {1000,600,25,25},window);
PEntity player2(texturefolder+"bot.png",{200,200,180,230},window);
bool dpressed= false,apressed = false;
std::vector<PEntity> Boxes;
std::vector<PEntity*> Boxes_p;
std::vector<PEntity> gems;
bool leftpressed = false;
bool rightpressed = false;
bool crouching = false;
Button button(window);
bool shot = false;
bool quit = false;
PEntity* right_clicked_box = nullptr;
PEntity* left_clicked_box  = nullptr;
void physics()
{
    Timer loopcontrol;
    Mix_Chunk* jumpsound  = Mix_LoadWAV((soundfolder+"jump.wav" ).c_str());
    Mix_Chunk* canonsound = Mix_LoadWAV((soundfolder+"canon.wav").c_str());
    uint32_t passed = 0;
    while (!quit)
    {



        //capture time
        loopcontrol.Start();
        for(auto& box : Boxes)
        {
            box.Update(passed);
        }
        player.Update(passed);
        player2.Update(passed);
        canon_ball.Update(passed);

        
        SortByHeight(Boxes_p);
        //calculate all collisions
        for(auto* box : Boxes_p)
        {   
            player.PhysicsCollision(*box);
        }
       player.PhysicsCollision(canon.hitbox,0.01,0.2);
        for(auto* box : Boxes_p)
        {
        for(auto* box_to_avoid : Boxes_p)
            {
                if(box_to_avoid  > box)
                {
                    box->PhysicsCollision(*box_to_avoid);
                    

                }
            }
        } 

        if(player.PhysicsCollision(wall.hitbox,0.1,0.2) != Side::none)
            ;
        player.PhysicsCollision(grass.hitbox,0.1,0.2);
        
        for(auto& box : Boxes)
        {
            box.PhysicsCollision(grass.hitbox,1,0.1);
            box.PhysicsCollision(wall.hitbox,1,0.1);
            //box.LimitSpeed(0.3);
        }
 
        player2.PhysicsCollision(grass.hitbox,0.01,0.2);
        canon_ball.AvoidCollision(grass.hitbox);
        player.PhysicsCollision(canon_ball);
        player.PhysicsCollision(player2.hitbox,0.01,0.2);
         if(left_clicked_box)
        {
            if(!player.Standing())
                player.Gravitate((Vec2)(Utils::GetCenter(left_clicked_box->hitbox)),0.0025);
            left_clicked_box->Gravitate((Vec2)(Utils::GetCenter(player.hitbox)),0.0025);
        }

         //handle window events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            button.HandleEvent(event); 
            if (event.type == SDL_QUIT)
            { quit = true;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mousepos = window.GetMousePos(); 
                
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    leftpressed = true;
                    left_clicked_box = ChooseBox(Boxes, mousepos.x,mousepos.y);
                }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    rightpressed = true;
                    right_clicked_box = ChooseBox(Boxes,mousepos.x,mousepos.y);
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    leftpressed = true;
                    left_clicked_box = nullptr;
                }
                if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    rightpressed = true;
                    right_clicked_box = nullptr;
                }

            }
            


            SDL_Scancode& key = event.key.keysym.scancode;
            if(event.type == SDL_KEYDOWN)
                {
                    if (key == SDL_SCANCODE_D&& event.key.repeat == 0)
                    {
                        player.velocity.x += 0.1;
                        dpressed = true;


                    }
                    if (key == SDL_SCANCODE_A && event.key.repeat == 0)
                    {
                        player.velocity.x -= 0.1;
                        apressed = true;

                    }
                    if (key == SDL_SCANCODE_S && event.key.repeat == 0)
                    {
                        crouching = true;

                    }

                    if(key == SDL_SCANCODE_Q)
                    {
                        quit = true;
                        break;
                    }
                    if ((key == SDL_SCANCODE_W || key == SDL_SCANCODE_SPACE) && event.key.repeat == 0 && (player.IsStanding()))
                    {
                        if(!crouching)
                        {
                            int channel = Mix_PlayChannel( -1, jumpsound, 0 );
                            Mix_Volume( channel, 5);
                            player.velocity.y = -1.0;
                            if(!shot)
                            {
                                canon_ball.velocity ={-0.2,-0.08};
                                canon_ball.acceleration.y = 0.0005;
                                shot = true;
                                int channel = Mix_PlayChannel( -1, canonsound, 0 );
                                Mix_Volume( channel, 5);

                            }
                        }
                    }

                }
            else if(event.type == SDL_KEYUP)
            {
                if (key == SDL_SCANCODE_D && event.key.repeat == 0)
                {
                    player.velocity.x-=0.1;
                    dpressed = false;
                }
                if (key == SDL_SCANCODE_A && event.key.repeat == 0)
                {
                    player.velocity.x+=0.1;
                    apressed = false; 
                }
                if (key == SDL_SCANCODE_S && event.key.repeat == 0)
                    crouching = false;
            }
        }
        // if(player.IsStanding())
        // {
        //     if(!(dpressed||apressed && abs(player.velocity.x)<0.1))
        //         player.velocity.x *=0.995;
        // }
       //choose the texture of player

        if(player.velocity.x > 0.001 )
            player.texture = lookright.s_tex();
        else if(player.velocity.x < -0.001)
            player.texture = lookleft.s_tex();


        if(dpressed)
            player.velocity.x = 0.1;
        else if (apressed)
          player.velocity.x = -0.1;

        loopcontrol.WaitUntilPassed(1000);
        passed = loopcontrol.PassedTime().count();
   }
}


void Foo()
{
    button.ChangeRectColor({255,0,0,255});
    button.ChangeRectColor({0,255,0,255});

    button.Reposition({1000,100});
    std::cout<<"im Foo func"<<std::endl;
}

int mainold(int argc, char* argv[])
{ 
    //initiallize audio
    Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2, 1024 );
    TTF_Init();
    //Create Textures
    window.SetBGColor(120,120,255);
    canon_ball.SetRoughness(0.005);
    player.ChangeMass(50);
    Boxes.reserve(3);
    
    Boxes.emplace_back(texturefolder+"Box.png",SDL_Rect{100,100,50,50},window);
    Boxes.emplace_back(texturefolder+"Box.png",SDL_Rect{200,100,60,60},window);
    Boxes.emplace_back(texturefolder+"Box.png",SDL_Rect{300,100,70,70},window);


    for (auto& box : Boxes)
    {
        box.acceleration.y = 0.0015;
        //box.acceleration.x = 0.0015;
        box.ChangeMass(1);
        Boxes_p.push_back(&box);
        box.SetRoughness(1);
    }
    lookleft.Rotate(0, SDL_FLIP_HORIZONTAL, false);
    Timer loopcontrol;
    player.acceleration.y = 0.0025;
    //player.acceleration.x = 0.0025;
    int mousex,mousey;
    canon_ball.ChangeMass(5555);
    button.SetFunctionality(Foo);
    std::function<void()> testf = [](){std::cout<<"asda"<<std::endl;};
    button.OnClick = Foo;//[](){player.velocity.y = -0.5;};
   // Event loop
    std::thread physicsthread (physics);
    while (!quit)
    {
        //        //capture time
        loopcontrol.Start();
        window.Clear();
        if(left_clicked_box)
        {
            SDL_Point PC = Utils::GetCenter(player.hitbox) , BC = Utils::GetCenter(left_clicked_box->hitbox); 
            window.DrawLine(PC, BC, {255,255,255,255});
        }

        //        //handle rendering
        canon_ball.Draw();
        player.Draw();
        button.Draw();
        wall.Draw();
        for(auto& box : Boxes)
        {
            box.Draw();
        }
        player2.Draw();
        grass.Draw();
        canon.Draw();
        window.Show();

        loopcontrol.WaitUntilPassed(2000);
    }
    physicsthread.join();
    return 0;
}

PEntity* ChooseBox(std::vector<PEntity>& boxes,int mousex,int mousey)
{
    for (auto& box : boxes) 
    {
        if(mousex>box.hitbox.x && mousex<box.hitbox.x+box.hitbox.w
            && mousey>box.hitbox.y && mousey<box.hitbox.y+box.hitbox.h
            && player.GetDistance(Vec2(Utils::GetCenter( box.hitbox)))<400    )
        {
            return &box; 
        }
    }

    return nullptr;
}

int main()
{
   // Game game;
   // while(!game.quit)
   // {
   //     game.Draw();
   // }
}

