#include <math.h>
#include <stdlib.h>
#include <deque>
#include <windows.h>
#include <vector>
#include <queue>

//using namespace std;


enum elements
{
    VAZIO = 0,
    WALL,
    HUNTER,
    GOAL,
    VISITED
};

class position
{
public:
    int i,j;
};

class son
{
public:
    position current;
    position father;
    float value;
    float f;
    float h;
};



class MAZEtab
{
private:
    int tab[24][24];
    //position of the hunter and goal
    position hunter,goal,origin;

public:
    MAZEtab();
    void construct(int from[24][24]);
    int getPosition(int i, int j);
    bool setHunter(int i, int j);
    bool setGoal(int i, int j);
    position getHunter();
    position getGoal();
    bool moveHunter(char movement);
    void imprimir();
    bool solveMaze();

private:
    float heuristic(int, int, int, int);
    float cost(int, int , int, int);
    float tryMoveHunter(char movement);

    std::queue<position> possibilitiesToMove(int i, int j);

};

//default constructor
MAZEtab::MAZEtab()
{
    //define the maze as void
    for(int i=0; i<24; i++)
    {
        for(int j=0; j<24; j++)
        {
            tab[i][j] = 0;
        }
    }

    //default values for position
    hunter.i = -1;
    hunter.j = -1;
    goal.i = -1;
    goal.j = -1;
}

//construct the maze based on a matrix
void MAZEtab::construct(int from[24][24])
{
    for(int i=0; i<24; i++)
    {
        for(int j=0; j<24; j++)
        {
            tab[i][j] = from[i][j];
        }
    }
}

//return the value of a position
int MAZEtab::getPosition(int i, int j)
{
    return tab[i][j];
}

//set and get positions of hunter and goal
bool MAZEtab::setHunter(int i, int j)
{
    if(tab[i][j] != VAZIO)
    {
        return false;
    }
    else
    {
        tab[i][j] = HUNTER;
        hunter.i = i;
        hunter.j = j;
        origin.i = i;
        origin.j = j;
        return true;
    }
}

bool MAZEtab::setGoal(int i, int j)
{
    if(tab[i][j] != VAZIO)
    {
        return false;
    }
    else
    {
        tab[i][j] = GOAL;
        goal.i = i;
        goal.j = j;
        return true;
    }
}

position MAZEtab::getHunter()
{
    return hunter;
}
position MAZEtab::getGoal()
{
    return goal;
}

//method for move the hunter in valid positions
bool MAZEtab::moveHunter(char movement)
{

    //identifies the move type
    int di = 0, dj = 0;
    switch(movement)
    {
    case 'U':
        di = -1;
        break;
    case 'D':
        di = +1;
        break;
    case 'R':
        dj = 1;
        break;
    case 'L':
        dj = -1;
        break;
    default:
        printf("tentando movimento invalido");
        return -1;
    }
    int i = hunter.i + di;
    int j = hunter.j + dj;

    //if the position is out of the matrix
    if(i<0 || i>23 || j<0 || j>23)
    {
        return false;
    }
    //if the position is a wall
    else if(tab[i][j] == WALL)
    {
        return false;
    }
    //do move
    else
    {
        tab[hunter.i][hunter.j] = VAZIO;
        tab[i][j] = HUNTER;
        hunter.i = i;
        hunter.j = j;
        return true;
    }
}

//defines the heuristics used for the search algorithm
float MAZEtab::heuristic(int currentI, int currentJ, int goalI, int goalJ)
{
    //use the manhattan distance between two points
    return sqrt( (currentI - goalI)*(currentI - goalI))+sqrt((currentJ - goalJ)*(currentJ-goalJ) );
}

//defines the cost function
float MAZEtab::cost(int currentI, int currentJ, int originI, int originJ)
{
    //use the euclidean distance between two points
    return sqrt( (currentI - originI)*(currentI - originI))+sqrt((currentJ - originJ)*(currentJ-originJ) );

}

//evaluate if a move is valid and, if possible, return the cost of that position
//method for move the hunter in valid positions
float MAZEtab::tryMoveHunter(char movement)
{

    //identifies the move type
    int di = 0, dj = 0;
    switch(movement)
    {
    case 'U':
        di = -1;
        break;
    case 'D':
        di = +1;
        break;
    case 'R':
        dj = 1;
        break;
    case 'L':
        dj = -1;
        break;

    }
    int i = hunter.i + di;
    int j = hunter.j + dj;

    //if the position is out of the matrix
    if(i<0 || i>23 || j<0 || j>23)
    {
        return -1;
    }
    //if the position is a wall or other obstacle
    else if(tab[i][j] != GOAL && tab[i][j] != VAZIO)
    {
        return -1;
    }
    //do move
    else
    {

        float H = heuristic(i,j,goal.i,goal.j);
        float C = cost(i,j,origin.i, origin.j);
        return (H + C);
    }
}

std::queue<position> MAZEtab::possibilitiesToMove(int i, int j)
{
    std::queue<position> answer;

    for(int k=1; k<=4; k++)
    {
        //identifies the move type
        int di = 0, dj = 0;
        switch(k)
        {
        case 1:
            di = -1;
            break;
        case 2:
            di = +1;
            break;
        case 3:
            dj = 1;
            break;
        case 4:
            dj = -1;
            break;

        }
        int nexti = i + di;
        int nextj = j + dj;

        //if the position is out of the matrix
        if(nexti<0 || nexti>23 || nextj<0 || nextj>23)
        {
            //invalid position
        }
        //if the position is a wall or other obstacle
        else if( tab[nexti][nextj] == WALL || tab[nexti][nextj] == VISITED)
        {
            //invalid position
        }
        else
        {
            position aux;
            aux.i = nexti;
            aux.j = nextj;
            answer.push(aux);

        }
    }

    return answer;
}

class compare2
{
public:
    bool operator() (son a,son b)
    {
        if(a.value > b.value)
            return true;
        return false;
    }
};

void clearscreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);

}
//solve
bool MAZEtab::solveMaze()
{


    son matrix[24][24];
    for(int i=0; i<=23; i++)
    {
        for(int j=0; j<=23; j++)
            matrix[i][j].value = 1000;
    }

    matrix[hunter.i][hunter.j].value = 0;
    matrix[hunter.i][hunter.j].father = hunter;



    std::priority_queue<son, std::vector<son>, compare2> openList;

    std::deque<char> movementsToDo;

    bool isPossible = true;



    //add the current position to the open list
    son aux;
    aux.father.i = -1;
    aux.father.j = -1;
    aux.current.i = hunter.i;
    aux.current.j = hunter.j;
    aux.f = 0;
    aux.h = heuristic(hunter.i, hunter.j, goal.i, goal.j);
    aux.value = aux.f + aux.h;
    openList.push(aux);

    while(isPossible)
    {

        //go to the position with minimum cost value
        while(true)
        {
            if(openList.empty())
            {
                printf("nao existe caminho!\n");
                for(int i=0; i<24; i++)
                {
                    for(int j=0; j<24; j++)
                    {
                        if(tab[i][j] == 4)
                        {
                            tab[i][j] = VAZIO;
                        }
                    }

                }
                isPossible = false;
                return false;
                break;
            }
            aux = openList.top();
            openList.pop();
            if(tab[aux.current.i][aux.current.j] != VISITED)
                break;
        }

        matrix[aux.current.i][aux.current.j] = aux;



        //move hunter to that position
        hunter = aux.current;

        //add current position to the closed position
        tab[hunter.i][hunter.j] = VISITED;




        std::queue<position> possibilitie;

        //if hunter is in the goal
        if(hunter.i == goal.i && hunter.j == goal.j)
        {


            //clear map
            for(int i=0; i<24; i++)
            {
                for(int j=0; j<24; j++)
                {
                    if(tab[i][j] == 4)
                    {
                        tab[i][j] = VAZIO;
                    }
                }
            }

            aux = matrix[goal.i][goal.j];
            while(aux.father.i != -1)
            {
                tab[aux.current.i][aux.current.j] = 9;
                aux = matrix[aux.father.i][aux.father.j];
            }

            isPossible = false;


        }
        else
        {
            //verifies the possibilites of next positions

            possibilitie = possibilitiesToMove(hunter.i, hunter.j);

        }


        //calculate the cost of the possible positions
        while(!possibilitie.empty())
        {
            position analyse = possibilitie.front();
            possibilitie.pop();
            if(tab[analyse.i][analyse.j] == VAZIO || tab[analyse.i][analyse.j] == GOAL)
            {
                //just calculate the value and add to the open list
                son aux2;
                aux2.father = hunter;
                aux2.f = aux.f + 1;
                aux2.h = heuristic(analyse.i, analyse.j, goal.i, goal.j);
                aux2.value = aux2.f + aux2.h;
                aux2.current = analyse;
                openList.push(aux2);
            }
            else
            {
                //to implements

            }
        }


    }

    return true;

}

void MAZEtab::imprimir()
{

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    for(int i=0; i<24; i++)
    {
        for(int j=0; j<24; j++)
        {
            if(tab[i][j] == 4)
            {
                SetConsoleTextAttribute(hConsole, 192);
                printf("%d ",tab[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            else if(tab[i][j] == 1)
            {
                SetConsoleTextAttribute(hConsole, 10);
                printf("%d ",tab[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            else if(tab[i][j] == 9)
            {
                SetConsoleTextAttribute(hConsole, 132);
                printf("%d ",tab[i][j]);
                SetConsoleTextAttribute(hConsole, saved_attributes);
            }
            else
                printf("%d ",tab[i][j]);
        }
        printf("\n");
    }
    SetConsoleTextAttribute(hConsole, saved_attributes);

}




