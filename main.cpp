#include <SDL.h>
#include "8puzzle/source/puzzle8.h"
#include "maze/MAZE.h"
#include "animations/animations.h"
#include <SDL_ttf.h>


int main(int argc, char **argv)
{

    //initialize features of SDL
    SDL_Init( SDL_INIT_VIDEO );
    TTF_Init();

    //controller for gameloop
    bool running = true;


    //create the window where load drawings
    SDL_Window * window = SDL_CreateWindow("AIGames - a try!", 100, 100, 800, 600, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    //declaration of the rects for principal screen
    SDL_Point corner[12];
    SDL_Rect parts[3][4];

    //define rects to cut the texture
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            corner[i*4+j] = {j*200,i*200};
        }
    }

    //define parts
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            parts[i][j] = {j*201,i*201,200,200};
        }
    }

    //load textures from file to render
    SDL_Texture * texture;
    SDL_Surface * surface;
    surface = SDL_LoadBMP("images/principal.bmp");
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("images/menu1.bmp");
    SDL_Texture * texMenu1 = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("images/menu2.bmp");
    SDL_Texture * texMenu2 = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP("images/menu3.bmp");
    SDL_Texture * texMenu3 = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    SDL_Rect menu1,menu2,menu3;

    menu1 = parts[0][3];
    menu2 = parts[1][3];
    menu3 = parts[2][3];

    /*
    menuAnimation * menu = new menuAnimation(renderer);
    menu->run();
    delete menu;
    */



    while(running)
    {


        //Reaction to events
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
            {
                //quit the game
                running = false;
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                SDL_Point mouseClick = {event.button.x, event.button.y};
                if(event.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mouseClick, 1, &menu1, nullptr) )
                {
                    PUZZLE15 * puzzle;
                    puzzle = new PUZZLE15(renderer);
                    puzzle->Run();
                    delete puzzle;
                }
                if(event.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mouseClick, 1, &menu2, nullptr) )
                {
                    MAZE * maze;
                    maze = new MAZE(renderer,window);
                    maze->Run();
                    delete maze;
                }
                if(event.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mouseClick, 1, &menu3, nullptr) )
                {
                    //implement snake
                }
            }
            case SDL_KEYDOWN:
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_P)
                {
                    PUZZLE15 * puzzle;
                    puzzle = new PUZZLE15(renderer);
                    puzzle->Run();
                    delete puzzle;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_S)
                {



                }
                if(event.key.keysym.scancode == SDL_SCANCODE_M)
                {

                    MAZE * maze;
                    maze = new MAZE(renderer,window);
                    maze->Run();
                    delete maze;

                }
                if(event.key.keysym.scancode == SDL_SCANCODE_A)
                {

                    menuAnimation * menu = new menuAnimation(renderer);
                    menu->run();
                    delete menu;

                }


                break;
            }
            }
        }


        //rendering drawings
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_Rect cut = {0,0,200,200};

        int number;
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<4; j++)
            {
                number = i*4+j+1;
                if(number != 12)
                {
                    cut.x = j*200;
                    cut.y = i*200;
                    SDL_RenderCopy(renderer,texture,&cut,&parts[i][j]);
                }
            }
        }

        SDL_RenderCopy(renderer,texMenu1,NULL,&menu1);
        SDL_RenderCopy(renderer,texMenu2,NULL,&menu2);
        SDL_RenderCopy(renderer,texMenu3,NULL,&menu3);

        SDL_Delay(10);
        SDL_RenderPresent(renderer);
    }

    SDL_free(renderer);
    SDL_free(window);

    return 0;

}
