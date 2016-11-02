#include <SDL.h>
#include "tabuleiro15puzzle.h"
#include <stdio.h>
#include <SDL_ttf.h>
#include <string>



class PUZZLE15
{
private:
    Tabuleiro15puzzle jogo;
    SDL_Renderer* layoutRender;

    //parts
    SDL_Texture *slotsTexture;
    SDL_Surface *surface;
    SDL_Rect parts[3][3];

    //backgrounds
    SDL_Rect backtable;

    //menus
    SDL_Rect backmenu;
    SDL_Rect menuBack;
    SDL_Rect menuShuffle;
    SDL_Rect menuSolve;

    //text
    TTF_Font *font;
    SDL_Surface *message;
    SDL_Texture *text;
    SDL_Texture *backMenuText;
    SDL_Texture *shuffleMenuText;
    SDL_Texture *solveMenuText;

    SDL_Rect backText;
    SDL_Rect shuffleText;
    SDL_Rect solveText;

    SDL_Color textColor;

public:
    PUZZLE15(SDL_Renderer*);
    void Run();

//methods for intern use of the class
private:
    bool moveWithAnimation(int direction);
    void loadImagesFromFile();
    void loadTextFeatures();
    void positioningElements();
    void reactToEvent(bool &running, int &qtdMoves, SDL_Event &event);
    void renderElements(int &qtdMoves);
    void freeAll();
    void defineCut(SDL_Rect &cut, int number);
};





//constructor of the game
PUZZLE15::PUZZLE15(SDL_Renderer* layoutRender)
{
    this->layoutRender = layoutRender;
    jogo.shuffle();
}





//class that execute the game
void PUZZLE15::Run()
{
    //store the quantity of moves until the solution
    int qtdMoves = 0;

    //shuffle the game
    jogo.shuffle();

    //load the images of numbers for parts
    loadImagesFromFile();

    //load the text
    loadTextFeatures();

    //boolean to control game loop
    bool running = true;

    //variables for position
    const int leftx = 50;
    const int lefty = 50;

    //constants for sizes
    const int PART_SIZE = 130;

    //backgrounds
    backtable = {leftx-5, lefty-5, 405, 412};
    backmenu = {523, lefty-5, 253, 412};

    //menu button
    menuBack = {525, lefty,                 PART_SIZE*2 -10, PART_SIZE};
    menuShuffle = {525, PART_SIZE+2+lefty,   PART_SIZE*2 -10, PART_SIZE};
    menuSolve = {525, 2*(PART_SIZE+2)+lefty, PART_SIZE*2 -10, PART_SIZE};


    //create parts
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            parts[i][j] = {0,0,PART_SIZE,PART_SIZE};
            parts[i][j].x = i*(PART_SIZE + 2) + leftx;
            parts[i][j].y = j*(PART_SIZE + 2) + lefty;
        }
    }

    //game loop
    SDL_Event event;
    while(running)
    {
        //if(jogo.isSolved())
        //    printf("CONGRATULATIONS!\n You did %d moves\n", qtdMoves);

        //reaction to the events

        while(SDL_PollEvent(&event))
        {
            reactToEvent(running, qtdMoves, event);
        }

        renderElements(qtdMoves);


    }
}





//moves the parts with animations
bool PUZZLE15::moveWithAnimation(int direction)
{

    //varibles for origin and destination of the number
    int fromi;
    int fromj;
    int toi;
    int toj;

    //search for the position of the void
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(jogo.getPosition(i,j) == -1)
            {
                toi = i;
                toj = j;
            }
        }
    }

    switch(direction)
    {
    case +1:
        if(!jogo.doMove("RIGHT")) return false;
        break;
    case -1:
        if(!jogo.doMove("LEFT")) return false;
        break;
    case +2:
        if(!jogo.doMove("UP")) return false;
        break;
    case -2:
        if(!jogo.doMove("DOWN")) return false;
        break;
    }

    //search for the new position of the void
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(jogo.getPosition(i,j) == -1)
            {
                fromi = i;
                fromj = j;
            }
        }
    }

    int num = jogo.getPosition(toi,toj);

    int di = toi - fromi;
    int dj = toj - fromj;

    SDL_Rect toMove = {0,0,130,130};
    toMove.x = fromj * 132 + 50;
    toMove.y = fromi * 132 + 50;

    bool done = false;
    while(!done)
    {

        //render the background
        SDL_SetRenderDrawColor(layoutRender, 176, 224, 230, 255);
        SDL_RenderClear(layoutRender);

        //render the background parts
        SDL_SetRenderDrawColor(layoutRender, 128, 0, 0, 255);
        SDL_RenderFillRect(layoutRender, &backtable);

        //render menu background
    SDL_SetRenderDrawColor(layoutRender, 128, 0, 0,255);
    SDL_RenderFillRect(layoutRender, &backmenu);
    SDL_SetRenderDrawColor(layoutRender, 128, 255, 255, 255);
    SDL_RenderFillRect(layoutRender, &menuBack);
    SDL_SetRenderDrawColor(layoutRender, 128, 128, 255, 255);
    SDL_RenderFillRect(layoutRender, &menuShuffle);
    SDL_SetRenderDrawColor(layoutRender, 255, 255, 128, 255);
    SDL_RenderFillRect(layoutRender, &menuSolve);

        ///render menu

        SDL_SetRenderDrawColor(layoutRender, 255, 255, 255, 255);
        SDL_RenderCopy(layoutRender,shuffleMenuText,NULL,&shuffleText);
        SDL_RenderCopy(layoutRender,backMenuText,NULL,&backText);
        SDL_RenderCopy(layoutRender,solveMenuText,NULL,&solveText);
        SDL_Rect cut;

        int number;
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3; j++)
            {
                number = jogo.getPosition(j,i);

                if(number != -1 && number!= num)
                {
                    defineCut(cut,jogo.getPosition(j,i));
                    SDL_RenderCopy(layoutRender, slotsTexture, &cut, &parts[i][j]);
                }
                else if(number == -1)
                {
                    defineCut(cut,num);
                    SDL_RenderCopy(layoutRender, slotsTexture, &cut, &toMove);

                    toMove.x+=dj*12;
                    toMove.y+=di*12;
                    if(toMove.x == parts[toi][toj].y && toMove.y == parts[toi][toj].x)
                        done = true;
                }
            }
        }
        SDL_Delay(15);
        SDL_RenderPresent(layoutRender);
    }

    return true;
}


//load the images of numbers
void PUZZLE15::loadImagesFromFile()
{
        surface = SDL_LoadBMP("15puzzle/images/sonic.bmp");
        slotsTexture = SDL_CreateTextureFromSurface(layoutRender, surface);
        SDL_FreeSurface(surface);
}

//load the features needed for texts
void PUZZLE15::loadTextFeatures()
{

    //define color of the text
    textColor = {0,0,0};


    //define font
    font = TTF_OpenFont( "STRCMIT_.ttf", 48);

    //define messages
    message = TTF_RenderUTF8_Blended( font, "BACK", textColor );
    backMenuText = SDL_CreateTextureFromSurface(layoutRender,message);
    SDL_FreeSurface(message);

    message = TTF_RenderUTF8_Blended( font, "SOLVE", textColor );
    solveMenuText = SDL_CreateTextureFromSurface(layoutRender,message);
    SDL_FreeSurface(message);

    message = TTF_RenderUTF8_Blended( font, "SHUFFLE", textColor );
    shuffleMenuText = SDL_CreateTextureFromSurface(layoutRender,message);
    SDL_FreeSurface(message);

    int h = 0;
    int w = 0;


    //SDL_QueryTexture(text, NULL, NULL, &w, &h);
    SDL_QueryTexture(backMenuText, NULL, NULL, &w, &h);
    SDL_QueryTexture(shuffleMenuText, NULL, NULL, &w, &h);
    SDL_QueryTexture(solveMenuText, NULL, NULL, &w, &h);

    backText = {568, 110, w+20, h};
    shuffleText = {568, 227, w+20, h};
    solveText = {568, 343, w, h};


}

void PUZZLE15::reactToEvent(bool &running, int &qtdMoves, SDL_Event &event)
{

    switch(event.type)
    {
    case SDL_QUIT:
        running = false;
        freeAll();
        break;
    case SDL_MOUSEBUTTONDOWN:
    {
        SDL_Point mouseClick = {event.button.x, event.button.y};
        if(event.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mouseClick, 1, &menuBack, nullptr) )
        {
            freeAll();
            running = false;
        }
        if(event.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mouseClick, 1, &menuShuffle, nullptr) )
        {
            jogo.shuffle();
        }
        if(event.button.button == SDL_BUTTON_LEFT && SDL_EnclosePoints(&mouseClick, 1, &menuSolve, nullptr) )
        {
            std::stack<char> solution = jogo.solve();
            printf("solution in %d moves\n",solution.size());

            while(!solution.empty())
            {
                char tempMove = solution.top();
                solution.pop();
                //printf("move %c\n",tempMove);
                if(tempMove == 'R')
                {
                    moveWithAnimation(1);
                }
                else if(tempMove == 'L')
                {
                    moveWithAnimation(-1);
                }
                else if(tempMove == 'U')
                {
                    moveWithAnimation(2);
                }
                else if(tempMove == 'D')
                {
                    moveWithAnimation(-2);
                }
                //printf("%s\n",SDL_GetError());
                SDL_Delay(33);
                SDL_RenderClear(layoutRender);
            }

        }
    }
    case SDL_KEYDOWN:
    {
        if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
        {
            //count only valid moves
            //if(jogo.doMove("DOWN")) qtdMoves++;
            if(moveWithAnimation(-2) ) qtdMoves++;
        }
        else if(event.key.keysym.scancode == SDL_SCANCODE_UP)
        {
            //if(jogo.doMove("UP")) qtdMoves++;
            if(moveWithAnimation(2) ) qtdMoves++;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
        {
            //if(jogo.doMove("RIGHT")) qtdMoves++;
            if(moveWithAnimation(1) ) qtdMoves++;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
        {
            //if(jogo.doMove("LEFT")) qtdMoves++;
            if(moveWithAnimation(-1) ) qtdMoves++;
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_Q)
        {
            jogo.shuffle();
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_S)
        {
            std::stack<char> solution = jogo.solve();
            printf("solution in %d moves\n",solution.size());

            while(!solution.empty())
            {
                char tempMove = solution.top();
                solution.pop();
                //printf("move %c\n",tempMove);
                if(tempMove == 'R')
                {
                    moveWithAnimation(1);
                }
                else if(tempMove == 'L')
                {
                    moveWithAnimation(-1);
                }
                else if(tempMove == 'U')
                {
                    moveWithAnimation(2);
                }
                else if(tempMove == 'D')
                {
                    moveWithAnimation(-2);
                }
                //printf("%s\n",SDL_GetError());
                SDL_Delay(33);
                SDL_RenderClear(layoutRender);
            }

        }
        if(event.key.keysym.scancode == SDL_SCANCODE_G)
        {
            while(true)
            {
                std::stack<char> solution = jogo.solve();
                printf("solution in %d moves\n",solution.size());
                jogo.shuffle();
            }

        }

    }
    break;
    }

}

void PUZZLE15::renderElements(int &qtdMoves)
{
    //render the background
    SDL_SetRenderDrawColor(layoutRender, 176, 224, 230, 255);
    SDL_RenderClear(layoutRender);

    //render the background parts
    SDL_SetRenderDrawColor(layoutRender, 128, 0, 0, 255);
    SDL_RenderFillRect(layoutRender, &backtable);

    //render menu background
    SDL_SetRenderDrawColor(layoutRender, 128, 0, 0,255);
    SDL_RenderFillRect(layoutRender, &backmenu);
    SDL_SetRenderDrawColor(layoutRender, 128, 255, 255, 255);
    SDL_RenderFillRect(layoutRender, &menuBack);
    SDL_SetRenderDrawColor(layoutRender, 128, 128, 255, 255);
    SDL_RenderFillRect(layoutRender, &menuShuffle);
    SDL_SetRenderDrawColor(layoutRender, 255, 255, 128, 255);
    SDL_RenderFillRect(layoutRender, &menuSolve);

    ///render menu

    SDL_SetRenderDrawColor(layoutRender, 255, 255, 255, 255);
    SDL_RenderCopy(layoutRender,shuffleMenuText,NULL,&shuffleText);
    SDL_RenderCopy(layoutRender,backMenuText,NULL,&backText);
    SDL_RenderCopy(layoutRender,solveMenuText,NULL,&solveText);

    SDL_Rect cut = {0,0,130,130};

    //render the numbered parts
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(jogo.getPosition(j,i) == -1)
            {
                //do nothing
                //the void will not be drawed
                //SDL_RenderCopy(layoutRender, slotsTexture[0], NULL, &parts[i][j]);
            }
            else
            {
                defineCut(cut,jogo.getPosition(j,i));
                SDL_RenderCopy(layoutRender, slotsTexture, &cut, &parts[i][j]);
            }
        }
    }

    //render start
    SDL_RenderPresent(layoutRender);

    //set default delay :60fps
    SDL_Delay(10);



}

void PUZZLE15::freeAll(){


        SDL_DestroyTexture(slotsTexture);



    if(text!=NULL)
        SDL_DestroyTexture(text);

    if(backMenuText != NULL)
        SDL_DestroyTexture(backMenuText);

    SDL_DestroyTexture(shuffleMenuText);
    SDL_DestroyTexture(solveMenuText);

}

void PUZZLE15::defineCut(SDL_Rect &cut, int number){

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(j*3+i+1 == number){
                cut = {130*i, 130*j,130,130};
                break;
            }
        }
    }
}






