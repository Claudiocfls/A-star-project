#include <string>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include "tree.h"
#include <stdio.h>
#include <stack>

#define DIMENSION_PUZZLE 3


//class that represents the game
class Tabuleiro15puzzle
{
private:
    int matrix[3][3];
    int voidi;
    int voidj;


public:
    Tabuleiro15puzzle();
    void shuffle();
    bool doMove(std::string command);
    std::stack<char> solve();
    bool isSolved();
    int getPosition(int i, int j);

private:
    void setPosition(int i, int j, int k);
    int heuristic(int temp[DIMENSION_PUZZLE][DIMENSION_PUZZLE]);
    bool isSolvable(std::vector<int> &vect);

    bool evitarDuplicatas(node* no);
    bool solutionSaved(std::stack<char> &sol);

};


//define a number for a position
void Tabuleiro15puzzle::setPosition(int i, int j, int k)
{
    matrix[i][j] = k;
}

//return de number at a position
int Tabuleiro15puzzle::getPosition(int i, int j)
{
    return matrix[i][j];
}

//constructor
Tabuleiro15puzzle::Tabuleiro15puzzle()
{

    //initiate the table at right order
    int number= 1;
    for(int i=0; i<DIMENSION_PUZZLE; i++)
    {
        for(int j=0; j<DIMENSION_PUZZLE; j++)
        {
            matrix[i][j] = number;
            number++;
        }
    }

    //set the blank space
    matrix[DIMENSION_PUZZLE-1][DIMENSION_PUZZLE-1] = -1;

    //set the position of blank space
    voidi = DIMENSION_PUZZLE-1;
    voidj = DIMENSION_PUZZLE-1;

}



unsigned int g_seed;

// Used to seed the generator.
inline void fast_srand(int seed)
{
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void)
{
    g_seed = clock();
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}


//shuffle the puzzle
void Tabuleiro15puzzle::shuffle()
{
    int tempo = clock();
    std::vector<int> vect;
    std::vector<int> vectShuffle;
    int number;
    do{
       //create the list
       for(int i=1;i<=8;i++)
            vect.push_back(i);
       vect.push_back(-1);

       vectShuffle.erase(vectShuffle.begin(), vectShuffle.end());

       std::vector<int>::iterator it;

       //random the order
       for(int i=1;i<=9;i++){
           number = (int)(fast_rand())%vect.size();
           it = vect.begin();
           it += number;
           vectShuffle.push_back( *it );
           vect.erase(it);
       }
        //if(!isSolvable(vectShuffle))
        //    printf("caso em que nao ha solucao\n");
    }while(!isSolvable(vectShuffle));



    number = 0;
    for(int i=0;i<DIMENSION_PUZZLE;i++){
        for(int j=0;j<DIMENSION_PUZZLE;j++){
            matrix[i][j] = vectShuffle[number];
            if(matrix[i][j] == -1){
                voidi = i;
                voidj = j;
            }
            number++;
        }
    }

    /*
    //do N = 12 random moves
    int number;
    int cont = 0;
    while(heuristic(matrix) < 10)
    {
        number = fast_rand()%100;
        //if odd and greater than 50
        if(number % 2 == 1 && number > 50 )
        {
            if(doMove("RIGHT")) cont++;
            //if odd and less than 50
        }
        else if(number %2 ==1 && number <= 50)
        {
            if(doMove("LEFT")) cont++;
            //if even and greater than 50
        }
        else if(number>50)
        {
            if(doMove("UP")) cont++;
        }
        else
        {
            if(doMove("DOWN")) cont++;
        }
    }
    */



    printf("embaralhado em %d ms\n",(int)(clock() - tempo) );
}

//verifies if a state can be solved
bool Tabuleiro15puzzle::isSolvable(std::vector<int> &vect){
    //count number of inversions
    int cont = 0;
    for(int i=0;i<9-1;i++){
        for(int j=i+1;j<9;j++){
            if(vect[i]!=-1 && vect[j]!=-1 && vect[i]>vect[j]){
                cont++;
            }
        }
    }
    return (cont%2 == 0);
}


//realizes a move
//return true if the move is possible and false case not
bool Tabuleiro15puzzle::doMove(std::string command)
{

    //"translate" the move for a convention code
    int direction;
    if(command == "LEFT")
    {
        direction = -1;
    }
    else if(command == "RIGHT")
    {
        direction = +1;
    }
    else if(command == "DOWN")
    {
        direction = -2;
    }
    else if(command == "UP")
    {
        direction = +2;
    }
    else
    {
        printf("UNKNOWN MOVE!");
        return false;
    }

    //realizes, if possible, the move required
    int aux;
    switch(direction)
    {
    case +1:
        if(voidj != 0)
        {
            aux = matrix[voidi][voidj];
            matrix[voidi][voidj] = matrix[voidi][voidj - 1];
            matrix[voidi][voidj - 1] = aux;
            voidj--;

        }
        else
            return false;
        break;

    case -1:
        if(voidj != DIMENSION_PUZZLE-1)
        {
            aux = matrix[voidi][voidj];
            matrix[voidi][voidj] = matrix[voidi][voidj + 1];
            matrix[voidi][voidj + 1] = aux;
            voidj++;

        }
        else
            return false;

        break;
    case +2:
        if(voidi != DIMENSION_PUZZLE-1)
        {
            aux = matrix[voidi][voidj];
            matrix[voidi][voidj] = matrix[voidi+1][voidj];
            matrix[voidi+1][voidj] = aux;
            voidi++;
        }
        else
            return false;
        break;
    case -2:
        if(voidi != 0)
        {
            aux = matrix[voidi][voidj];
            matrix[voidi][voidj] = matrix[voidi-1][voidj];
            matrix[voidi-1][voidj] = aux;
            voidi--;
        }
        else
            return false;
        break;
    }

    return true;
}


//verifies if the puzzle is solved
bool Tabuleiro15puzzle::isSolved()
{

    int cont = 1;
    for(int i=0; i<DIMENSION_PUZZLE; i++)
    {
        for(int j=0; j<DIMENSION_PUZZLE; j++)
        {
            if(matrix[i][j] == cont)
            {
                cont++;
            }
            else if(i == DIMENSION_PUZZLE-1 && j == DIMENSION_PUZZLE-1)
            {
                //do nothing
            }
            else
            {
                return false;
            }
        }
    }

    return true;

}


//calculate the heuristic for A* algorithm
/*
Is based on manhattan distance from a actual position of a number
to the correct position
*/
int Tabuleiro15puzzle::heuristic(int temp[DIMENSION_PUZZLE][DIMENSION_PUZZLE])
{
    int distance = 0;
    int a,b;


    for(int i=0; i<DIMENSION_PUZZLE; i++)
    {
        for(int j=0; j<DIMENSION_PUZZLE; j++)
        {
            int number = temp[i][j];
            if(number != -1)
            {
                for(int k=0; k<DIMENSION_PUZZLE; k++)
                {
                    for(int l=0; l<DIMENSION_PUZZLE; l++)
                    {
                        if(k*DIMENSION_PUZZLE+l+1 == number)
                        {
                            a = k;
                            b = l;
                            k = 5;
                            l = 5;
                        }
                    }
                }
                distance+= abs(i-a) + abs(j-b);
            }
        }
    }

    return distance;

}

//defines a new way for sort method on priority queue declared on solve() method
class compare
{
public:
    bool operator()(node* a, node* b)
    {
        if (a->heuristica + a->passos > b->heuristica + b->passos)
            return true;
        return false;

    }

};

//solve the puzzle using A* algorithm
std::stack<char> Tabuleiro15puzzle::solve()
{
    std::priority_queue<node*, std::vector<node*>, compare> openList;
    std::stack<char> moves;

    int tempo = clock();

    //search for a solution saved
    FILE * archive = fopen("solutions.puzzle","r");
    if(archive != NULL)
    {
        int num;
        while(!feof(archive))
        {
            bool existSolution = true;
            for(int i=0; i<DIMENSION_PUZZLE; i++)
            {
                for(int j=0; j<DIMENSION_PUZZLE; j++)
                {
                    fscanf(archive,"%d ",&num);
                    if(num != matrix[i][j])
                        existSolution = false;
                }
            }
            char movements;
            if(existSolution)
            {
                while(true)
                {
                    fscanf(archive,"%c",&movements);
                    if(movements == '\n')
                        break;

                    moves.push(movements);
                }
                std::stack<char> stackReturn;
                while(!moves.empty())
                {
                    stackReturn.push(moves.top());
                    moves.pop();
                }
                printf("Solution obtained from file at %d ms\n",clock() - tempo);
                printf("solution in %d moves\n",stackReturn.size());
                return stackReturn;
            }
            else
            {
                while(true)
                {
                    fscanf(archive,"%c",&movements);
                    if(movements == '\n')
                        break;
                }

            }
        }
    }
    fclose(archive);

    int cont = 0;

    node * first = new node;
    first->setEstado(matrix);
    first->heuristica = heuristic(matrix);
    openList.push(first);

    node* second;
    bool searchSolution = true;






    while(searchSolution)
    {

        //take the state with minimun cost
        second = openList.top();
        openList.pop();
        cont++;


        char previousMovement = second->getMovimento();

        //load the state in the matrix of the current object
        int** tempMatrix = second->getEstado();
        for(int i=0; i<DIMENSION_PUZZLE; i++)
        {
            for(int j=0; j<DIMENSION_PUZZLE; j++)
            {
                this->setPosition(i,j,tempMatrix[i][j]);

                //update the void position
                if(matrix[i][j] == -1)
                {
                    voidi = i;
                    voidj = j;
                }
            }
        }
        if(second->getPassos() > 100)
        {
            printf("Solution not found in less than 100 moves");
            return moves;
        }



        if(isSolved())
        {
            printf("Solution on %d ms\n",(int)(clock() - tempo));
            printf("%d nodes verified\n",cont);
            int** tempMatrix = first->getEstado();
            for(int i=0; i<DIMENSION_PUZZLE; i++)
            {
                for(int j=0; j<DIMENSION_PUZZLE; j++)
                {
                    this->setPosition(i,j,tempMatrix[i][j]);
                    //update the void position
                    if(matrix[i][j] == -1)
                    {
                        voidi = i;
                        voidj = j;
                    }
                }
            }
            //print moves on console screen

            while(second->getMovimento() != 'x')
            {
                moves.push(second->getMovimento());
                second = second->getPai();
            }
            printf("solution in %d moves\n",moves.size());

            //save solution if needed
            if(moves.size() > 15)
            {
                printf("<<< solution saved >>>\n");
                archive = fopen("solutions.puzzle","a");
                for(int i=0; i<DIMENSION_PUZZLE; i++)
                {
                    for(int j=0; j<DIMENSION_PUZZLE; j++)
                    {
                        fprintf(archive,"%d ",matrix[i][j]);
                    }
                }
                std::stack<char> aux = moves;

                while(!aux.empty())
                {
                    fprintf(archive,"%c",aux.top());
                    aux.pop();
                }
                fprintf(archive,"\n");
                fclose(archive);

            }
            delete(first);
            return moves;
            /*
            while(!moves.empty())
            {
                printf("%c\n",moves.top());
                moves.pop();
            }

            //quit the function
            return;
            */
        }


        //try moves in the current state

        //move to right
        if(previousMovement!='L')
        {
            if(doMove("RIGHT"))
            {
                node * temp = new node;
                temp->setEstado(matrix);
                temp->setPai(second);
                temp->setHeuristica( heuristic(matrix) );
                temp->setPassos( second->getPassos() + 1);
                temp->setMovimento('R');
                if(!evitarDuplicatas(first))
                {
                    openList.push(temp);
                    second->adicionaFilho(temp);
                }
                temp = NULL;

                //undo the move
                doMove("LEFT");
            }
        }

        if(previousMovement!='R')
        {
            //move to left
            if(doMove("LEFT"))
            {
                node * temp = new node;
                temp->setEstado(matrix);
                temp->setPai(second);
                temp->setHeuristica( heuristic(matrix) );
                temp->setPassos( second->getPassos() + 1);
                temp->setMovimento('L');

                if(!evitarDuplicatas(first))
                {
                    openList.push(temp);
                    second->adicionaFilho(temp);
                }
                temp = NULL;

                //undo the move
                doMove("RIGHT");
            }
        }

        if(previousMovement!='D')
        {
            //move to UP
            if(doMove("UP"))
            {
                node * temp = new node;
                temp->setEstado(matrix);
                temp->setPai(second);
                temp->setHeuristica( heuristic(matrix) );
                temp->setPassos( second->getPassos() + 1);
                temp->setMovimento('U');

                if(!evitarDuplicatas(first))
                {
                    openList.push(temp);
                    second->adicionaFilho(temp);
                }
                temp = NULL;

                //undo the move
                doMove("DOWN");
            }
        }

        if(previousMovement!='U')
        {
            //move to DOWN
            if(doMove("DOWN"))
            {
                node * temp = new node;
                temp->setEstado(matrix);
                temp->setPai(second);
                temp->setHeuristica( heuristic(matrix) );
                temp->setPassos( second->getPassos() + 1);
                temp->setMovimento('D');

                if(!evitarDuplicatas(first))
                {
                    openList.push(temp);
                    second->adicionaFilho(temp);
                }
                temp = NULL;

                //undo the move
                doMove("UP");
            }
        }

    }

    //return just to avoid warning
    return moves;
}
//return true if there is a equal state on the tree
bool Tabuleiro15puzzle::evitarDuplicatas(node* no)
{
    //ignorar a função
    return false;
    int contador = 0;
    int **mat = no->getEstado();
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(matrix[i][j] == mat[i][j])
                contador++;
        }
    }
    mat = NULL;
    if(contador == 9)
    {
        return true;
    }
    node * aux;

    aux = no->getFilhoEsquerdo();
    while(aux!=NULL)
    {
        if(evitarDuplicatas(aux))
        {
            return true;
        }
        aux = aux->getIrmaoDireito();
    }

    return false;

}

bool Tabuleiro15puzzle::solutionSaved(std::stack<char> &sol)
{

    FILE * archive = fopen("solutions.puzzle","r");

    if(archive != NULL)
    {
        int num;
        while(!feof(archive))
        {
            bool existSolution = true;
            for(int i=0; i<DIMENSION_PUZZLE; i++)
            {
                for(int j=0; j<DIMENSION_PUZZLE; j++)
                {
                    fscanf(archive,"%d ",&num);
                    if(num != matrix[i][j])
                        existSolution = false;
                }
            }
            char movements;
            if(existSolution)
            {
                while(true)
                {
                    fscanf(archive,"%c",&movements);
                    if(movements == '\n')
                        break;

                    sol.push(movements);
                }
                fclose(archive);
                return true;
            }
            else
            {
                while(true)
                {
                    fscanf(archive,"%c",&movements);
                    if(movements == '\n')
                        break;
                }

            }
        }
    }
    fclose(archive);

    return false;



}


