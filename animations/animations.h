#include <SDL.h>
#include <vector>

class menuAnimation
{
private:

    SDL_Renderer * laytouRender;
    void moveWithoutAnimation(int movement);
    void moveWithAnimation(int direction);

    SDL_Texture * texture;
    SDL_Surface * surface;

    int mat[3][4];
    int voidi;
    int voidj;
    SDL_Point corner[12];
    SDL_Rect parts[3][4];

public:
    menuAnimation(SDL_Renderer * laytouRender);
    void run();

};

menuAnimation::menuAnimation(SDL_Renderer * a)
{
    laytouRender = a;


    for(int i=0; i<3; i++)
        for(int j=0; j<4; j++)
            mat[i][j] = i*4+j+1;

    voidi = 2;
    voidj = 3;

}




void menuAnimation::run()
{

    surface = SDL_LoadBMP("images/principal.bmp");
    texture = SDL_CreateTextureFromSurface(laytouRender,surface);
    SDL_FreeSurface(surface);


    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            parts[i][j] = {j*201,i*201,200,200};
        }
    }


    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            corner[i*4+j] = {j*200,i*200};
        }
    }


    // scramble the screen
    std::vector<int> moves = {1,1,1,-2,-1,-1,-2,-1,2,2,1,1,1,-2,-1,-1,-2,1,1,2,-1,-1,2,1,-2,-1,-2,1,2};

    for(int i=0; i<(int)moves.size(); i++)
        moveWithoutAnimation(moves[i]);

    SDL_SetRenderDrawColor(laytouRender, 255, 255, 255, 255);
    SDL_RenderClear(laytouRender);
    SDL_Rect cut = {0,0,200,200};

    int number;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<4; j++)
        {
            number = mat[i][j]-1;
            if(number != 11)
            {
                cut.x = corner[number].x;
                cut.y = corner[number].y;
                SDL_RenderCopy(laytouRender,texture,&cut,&parts[i][j]);
            }
        }
    }
    SDL_RenderPresent(laytouRender);
    SDL_Delay(250);

    for(int i=moves.size()-1; i>=0; i--)
        moveWithAnimation(moves[i]*-1);


    /*
    int qtdMoves = 0;
    bool running = true;
    while(running)
    {



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
                    //count only valid moves
                    moveWithAnimation(-2);
                }
                else if(event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    moveWithAnimation(2);
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                {
                    moveWithAnimation(1);
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                {
                    moveWithAnimation(-1);
                }

            }
            break;
            }
        }

        SDL_SetRenderDrawColor(laytouRender, 255, 255, 255, 255);
        SDL_RenderClear(laytouRender);
        SDL_Rect cut = {0,0,200,200};

        int number;
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<4; j++)
            {
                number = mat[i][j]-1;
                if(number != 11)
                {
                    cut.x = corner[number].x;
                    cut.y = corner[number].y;
                    SDL_RenderCopy(laytouRender,texture,&cut,&parts[i][j]);
                }
            }
        }
        SDL_RenderPresent(laytouRender);
        SDL_Delay(10);
    }
    */

}

void menuAnimation::moveWithoutAnimation(int direction)
{

    int aux;
    switch(direction)
    {
    case +1:
        if(voidj != 0)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi][voidj - 1];
            mat[voidi][voidj - 1] = aux;
            voidj--;
        }

        break;

    case -1:
        if(voidj != 3)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi][voidj + 1];
            mat[voidi][voidj + 1] = aux;
            voidj++;
        }


        break;
    case +2:
        if(voidi != 2)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi+1][voidj];
            mat[voidi+1][voidj] = aux;
            voidi++;
        }

        break;
    case -2:
        if(voidi != 0)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi-1][voidj];
            mat[voidi-1][voidj] = aux;
            voidi--;
        }

        break;
    }

}

void menuAnimation::moveWithAnimation(int direction)
{

    int fromi;
    int fromj;
    int toi = voidi;
    int toj = voidj;


    int aux;
    switch(direction)
    {
    case +1:
        if(voidj != 0)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi][voidj - 1];
            mat[voidi][voidj - 1] = aux;
            voidj--;
        }

        break;

    case -1:
        if(voidj != 3)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi][voidj + 1];
            mat[voidi][voidj + 1] = aux;
            voidj++;
        }


        break;
    case +2:
        if(voidi != 2)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi+1][voidj];
            mat[voidi+1][voidj] = aux;
            voidi++;
        }

        break;
    case -2:
        if(voidi != 0)
        {
            aux = mat[voidi][voidj];
            mat[voidi][voidj] = mat[voidi-1][voidj];
            mat[voidi-1][voidj] = aux;
            voidi--;
        }

        break;
    }

    fromi = voidi;
    fromj = voidj;

    int num = mat[toi][toj];

    int di = toi - fromi;
    int dj = toj - fromj;

    SDL_Rect toMove = {0,0,200,200};
    toMove.x = fromj * 201;
    toMove.y = fromi * 201;


    bool done = false;
    while(!done)
    {

        SDL_SetRenderDrawColor(laytouRender, 255, 255, 255, 255);
        SDL_RenderClear(laytouRender);
        SDL_Rect cut = {0,0,200,200};

        int number;
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<4; j++)
            {
                number = mat[i][j];
                //printf("valor de number = %d\n",number);
                //getchar();
                if(number != 12 && number!= num)
                {
                    //printf("passou aqui2\n");
                    cut.x = corner[number-1].x;
                    cut.y = corner[number-1].y;
                    SDL_RenderCopy(laytouRender,texture,&cut,&parts[i][j]);
                }
                else if(number == 12)
                {
                    //printf("passou aqui\n");
                    cut.x = corner[num-1].x;
                    cut.y = corner[num-1].y;
                    SDL_RenderCopy(laytouRender,texture,&cut,&toMove);
                    toMove.x+=dj*3;
                    toMove.y+=di*3;
                    if(toMove.x == parts[toi][toj].x && toMove.y == parts[toi][toj].y)
                        done = true;
                }
            }
        }
        SDL_Delay(1);
        SDL_RenderPresent(laytouRender);

    }

}








