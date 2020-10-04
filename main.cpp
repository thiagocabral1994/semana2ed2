/**
 * Prof: Marcelo Caniato Renhe
 * Aluno: Thiago do Vale Cabral
 * Matrícula: 201965220A
 * Email: thiago.cabral@ice.ufjf.br
 * */
#include <iostream>
#include <ctime>
#include <cmath>


using namespace std;

typedef long long (*HashFunction)(long long, int);


/**
 * Funções Hash
 * */
long long div(long long chave, int capacidade)
{
    return abs(chave % capacidade);
}

long long mult(long long chave, int capacidade)
{
    double result = chave * 0.6317981;
    long long parteInteira = result;
    result -= parteInteira;
    return abs(floor(capacidade * result));
}

long long minhaHash(long long chave, int capacidade)
{
    return (long long)sqrt(chave)%capacidade;
}

/**
 * Classe Registro
 * */
template <class T>
class Registro
{
public:
    long long chave;
    T valor;
    Registro<T> *proximo = NULL;
};

/**
 * Classe TabelaHash
 * */
template <class T>
class tabelaHash
{
private:
    Registro<T> **elementos;
    int capacidade;

public:
    int colisoes;
    HashFunction hash;

    tabelaHash<T>(HashFunction hash, int cap = 100)
    {
        this->elementos = new Registro<T> *[cap];
        this->hash = hash;
        this->capacidade = cap;
        this->colisoes = 0;
        for (int i = 0; i < cap; i++)
        {
            elementos[i] = NULL;
        }
    }

    ~tabelaHash()
    {
        for (int i = 0; i < this->capacidade; ++i)
        {
            Registro<T> *atual = this->elementos[i];
            while (atual != NULL)
            {
                Registro<T> *aux = atual;
                atual = atual->proximo;
                delete aux;
            }
        }
        delete[] this->elementos;
    }

    void insere(long long chave, T valor)
    {
        long long hash = this->hash(chave, capacidade);
        try
        {
            if (this->elementos[hash] == NULL)
            {
                this->elementos[hash] = new Registro<T>;
                this->elementos[hash]->chave = chave;
                this->elementos[hash]->valor = valor;
            }
            else
            {
                this->colisoes++;
                Registro<T> *atual = this->elementos[hash];
                while (atual->proximo != NULL)
                {
                    atual = atual->proximo;
                }
                Registro<T> *novo = new Registro<T>;
                novo->chave = chave;
                novo->valor = valor;
                atual->proximo = novo;
            }
        }
        catch (const char &e)
        {
            cerr << e << endl;
        }
    }

    T recupera(long long chave)
    {
        long long hash = this->hash(chave, this->capacidade);
        if (this->elementos[hash] != NULL and this->elementos[hash]->chave == chave)
        {
            return this->elementos[hash]->valor;
        }
        else
        {
            Registro<T> *atual = this->elementos[hash];

            while (atual != NULL and atual->chave != chave)
            {
                atual = atual->proximo;
            }

            if (atual != NULL and atual->chave == chave)
            {
                return atual->valor;
            }
            else
            {
                throw "CHAVE NÃO ENCONTRADA";
            }
        }
    }

    void altera(long long chave, T valor)
    {
        long long hash = this->hash(chave, capacidade);
        if (this->elementos[hash] != NULL and this->elementos[hash]->chave == chave)
        {
            this->elementos[hash]->valor = valor;
        }
        else
        {
            Registro<T> *atual = elementos[hash];
            while (atual != NULL and atual->chave != chave)
            {
                atual = atual->proximo;
            }
            if (atual != NULL and atual->chave == chave)
            {
                atual->valor = valor;
            }
            else
            {
                cerr << "ERRO NA ALTERAÇÃO" << endl;
            }
        }
    }

    void remove(long long chave)
    {
        long long hash = this->hash(chave, capacidade);
        if (this->elementos[hash] != NULL and this->elementos[hash]->chave == chave)
        {
            Registro<T> *aux = elementos[hash];
            this->elementos[hash] = this->elementos[hash]->proximo;
            delete aux;
        }
        else
        {
            Registro<T> *atual = this->elementos[hash];
            Registro<T> *anterior;
            while (atual != NULL and atual->chave != chave)
            {
                anterior = atual;
                atual = atual->proximo;
            }
            if (atual != NULL and atual->chave == chave)
            {
                anterior->proximo = atual->proximo;
                delete atual;
            }
            else
            {
                cerr << "ERRO NA REMOÇÃO" << endl;
            }
        }
    }

    void percorre()
    {
        Registro<T> *atual;
        cout << "!--------------------!" << endl;
        for (int i = 0; i < this->capacidade; ++i)
        {
            atual = this->elementos[i];
            while (atual != NULL)
            {
                cout << i << ":" << " {CHAVE:[" << atual->chave << "] VALOR:[" << atual->valor << "]} -->";
                atual = atual->proximo;
                if (atual == NULL)
                {
                    cout << endl;
                }
            }
            
        }
        cout << "!--------------------!" << endl;
    }
};

int main()
{
    srand((int)time(0));

    int tam;
    std::cout << "Digite o tam: ";
    std::cin >> tam;

    tabelaHash<int> tabelaDiv(div, tam);
    tabelaHash<int> tabelaMult(mult, tam);
    tabelaHash<int> tabelaMinha(minhaHash, tam);


    for(int i = 0; i < tam; i++) {
        long long num = (rand() % (tam*tam))+1;

        tabelaDiv.insere(num,num);
        tabelaMult.insere(num,num);
        tabelaMinha.insere(num,num);
    }
    //tabelaDiv.percorre();
    //tabelaMult.percorre();
    //tabelaMinha.percorre();

    cout << "!==COLISOES==!" <<endl;
    cout << "DIVISAO: " << tabelaDiv.colisoes << endl;
    cout << "MULTIPLICACAO: " << tabelaMult.colisoes << endl;
    cout << "MINHA HASH: " << tabelaMinha.colisoes << endl;
    return 0;
}