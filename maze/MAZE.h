#include <SDL.h>
#include "mazelib.h"

class MAZE
{
private:
    MAZEtab game;
    SDL_Renderer* layoutRender;
    SDL_Window* window;

public:
    MAZE(SDL_Renderer * a, SDL_Window* b);
    void Run();

};

MAZE::MAZE(SDL_Renderer * a,SDL_Window* b)
{
    this->layoutRender = a;
    this->window = b;
}

void MAZE::Run()
{

    //read the maze from file
    int leitura[24][24];
    char a;
    FILE * entrada = fopen("maze/mazeTxt/maze1.txt","r");
    for(int i=0; i<24; i++)
        for(int j=0; j<24; j++)
        {
            fscanf(entrada,"%c ",&a);
            leitura[i][j] = a - '0';
        }
    fclose(entrada);

    //construct the maze on the object
    game.construct(leitura);
    game.setGoal(23,23);
    game.setHunter(0,0);
    //game.imprimir();


    SDL_Rect original = {0,0,100,100};


    //load images
    //load the texture images for the mae
    SDL_Texture *mazeTexture[5];
    SDL_Surface *surface;
    for(int i=0; i<5; i++)
    {
        switch(i)
        {
        case 0:
            surface = SDL_LoadBMP("maze/images/grass.bmp");
            break;
        case 1:
            surface = SDL_LoadBMP("maze/images/wall.bmp");
            break;
        case 2:
            surface = SDL_LoadBMP("maze/images/hunter.bmp");

            break;
        case 3:
            surface = SDL_LoadBMP("maze/images/goal.bmp");
            break;
        case 4:
            surface = SDL_LoadBMP("maze/images/clues.bmp");
            break;
        }

        //this comand defines the color that will be transparent
        //SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 255, 255, 255));

        mazeTexture[i] = SDL_CreateTextureFromSurface(layoutRender, surface);
        SDL_FreeSurface(surface);

    }

    //boolean to control game loop
    bool running = true;

    //variables for position
    int leftx = 0;
    int lefty = 0;

    //constants for sizes
    const int PART_SIZE = 25;


    //create parts
    SDL_Rect parts[24][24];
    for(int i=0; i<24; i++)
    {
        for(int j=0; j<24; j++)
        {
            parts[i][j] = {0,0,PART_SIZE,PART_SIZE};
            parts[i][j].x = i*25 + leftx;
            parts[i][j].y = j*25 + lefty;
        }
    }

    //game loop
    while(running)
    {

        //reaction to the events
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    int tempo = clock();
                    game.solveMaze();
                    tempo = clock() - tempo;
                    printf("solucao em %d ms\n",tempo);
                    game.setGoal(23,23);
                    game.setHunter(0,0);
                    //game.imprimir();

                }

            }
            break;
            }
        }

        //render the background

        SDL_SetRenderDrawColor(layoutRender, 176, 224, 230, 255);
        SDL_RenderClear(layoutRender);

        original.y++;
        //render the numbered parts
        for(int i=0; i<24; i++)
        {
            for(int j=0; j<24; j++)
            {

                if(game.getPosition(j,i) == 0)
                {
                    SDL_RenderCopy(layoutRender, mazeTexture[0], NULL, &parts[i][j]);
                }
                else if(game.getPosition(j,i) == 1)
                {
                    SDL_RenderCopy(layoutRender, mazeTexture[1], NULL, &parts[i][j]);
                }
                else if(game.getPosition(j,i) == 2)
                {
                    SDL_RenderCopy(layoutRender, mazeTexture[2], NULL, &parts[i][j]);
                }
                else if(game.getPosition(j,i) == 3)
                {
                    SDL_RenderCopy(layoutRender, mazeTexture[3], NULL, &parts[i][j]);
                }
                else
                {
                    SDL_RenderCopy(layoutRender, mazeTexture[4], NULL, &parts[i][j]);
                }
            }
        }
        //render start
        SDL_RenderPresent(layoutRender);

        //set default delay :60fps
        SDL_Delay(10);

    }


}
