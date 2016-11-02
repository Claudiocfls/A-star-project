


class node
{
public:
    node * pai;
    node * filhoEsquerdo;
    node * irmaoDireito;
    int** estado;
    char movimento;
    int heuristica;
    int passos;


public:
    node();
    ~node();
    node* getPai();
    node* getFilhoEsquerdo();
    node* getIrmaoDireito();
    int** getEstado();
    char getMovimento();
    int getHeuristica();
    int getPassos();

    void setPai(node* );
    void setFilhoEsquerdo(node* );
    void setIrmaoDireito(node* );
    void setEstado(int a[3][3]);
    void setMovimento(char);
    void setHeuristica(int);
    void setPassos(int);
    void adicionaFilho(node * add);
    int getAltura();

};

node::node()
{
    pai = NULL;
    filhoEsquerdo =NULL;
    irmaoDireito =NULL;
    heuristica = 0;
    passos = 0;
    movimento = 'x';
    estado = NULL;

}
node::~node()
{

    node* percorre;
    node* elimina;


    for(int i=0; i<3; i++)
        free(estado[i]);
    free(estado);

    percorre = this->filhoEsquerdo;

    while(percorre!=NULL)
    {
        elimina = percorre;
        percorre = percorre->irmaoDireito;
        delete(elimina);
    }

}

node* node::getPai()
{
    return pai;
}

void node::setEstado(int a[3][3])
{
    estado = (int**)malloc(3*sizeof(int));
    for(int i=0; i<3; i++)
        estado[i] = (int*)malloc(3*sizeof(int));

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            estado[i][j] = a[i][j];
        }
    }


}

node* node::getFilhoEsquerdo()
{
    return filhoEsquerdo;
}

node* node::getIrmaoDireito()
{
    return irmaoDireito;
}

int** node::getEstado()
{
    return estado;
}

char node::getMovimento()
{
    return movimento;
}
int node::getHeuristica()
{
    return heuristica;
}
int node::getPassos()
{
    return passos;
}

void node::setPai(node* par)
{
    pai = par;
}
void node::setFilhoEsquerdo(node* par )
{
    filhoEsquerdo = par;
}
void node::setIrmaoDireito(node* par)
{
    irmaoDireito = par;
}


void node::setMovimento(char move)
{
    movimento = move;
}
void node::setHeuristica(int h)
{
    heuristica = h;
}
void node::setPassos(int p)
{
    passos = p;
}

void node::adicionaFilho(node * add)
{
    if(getFilhoEsquerdo() == NULL)
    {
        setFilhoEsquerdo(add);

    }
    else
    {
        node * filho = this->getFilhoEsquerdo();
        while(filho->getIrmaoDireito()!=NULL)
        {
            filho = filho->getIrmaoDireito();
        }
        filho->setIrmaoDireito(add);
    }
    add->setPai(this);

}

int node::getAltura()
{
    int alt = 0;
    node * percorre = this;
    percorre = percorre->getFilhoEsquerdo();
    while(percorre!=NULL)
    {
        percorre = percorre->getFilhoEsquerdo();
        alt++;

    }
    return alt;
}

