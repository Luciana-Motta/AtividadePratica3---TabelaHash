#include <iostream>
#include <fstream>
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int num_colisoesLinear = 0;
int num_colisoesDupla = 0;

class Data
{
private:
    int info;
public:
    int getInfo(){
        return info;
    }
    void setInfo(int informacao){
        info = informacao;
    }
};

class Hash
{
    private:
        int tamanhoTabela;
        Data *vetor;
        int tipo;
        int (*h)(int , int);

    public:
        void criar(int m , int (*escolha_hash)(int , int)){
            tamanhoTabela = m;
            vetor = new Data[tamanhoTabela];
            for(int i = 0 ; i < tamanhoTabela ; i++){
                Data *aux = new Data();
                vetor[i] = *aux;
                vetor[i].setInfo(-2);
            }
            h = escolha_hash;
        }

        Data *procurar(int chave , Data *data){
            int i = 0;
            int aux;
            while(i < tamanhoTabela){
                aux = h(chave, i)%tamanhoTabela;
                if(vetor[aux].getInfo() == -2 || vetor[aux].getInfo() == chave){
                    break;
                } i++;
            }
            if(i == tamanhoTabela || vetor[aux].getInfo() == -2){
                return NULL;
            }
            return &vetor[aux];
        }

        Data *inserir(int chave , Data *data){
            int aux;
            Data *pont = procurar(chave , data);
            if ( pont != NULL){
                cout << "data já inserida anteriormente" << endl;
                return pont;
            }
            int i = 0;
            while(i < tamanhoTabela){
                aux = h(chave, i)%tamanhoTabela;
                if(vetor[aux].getInfo() == -2){
                    vetor[aux] = *data;
                    return &(vetor[aux]);
                }i++;
            }
            if(i == tamanhoTabela){
                cout << "tabela cheia" << endl;
            }
            return data;
        }

        void destroi(){
            Data *p;
            for (int i = 0 ; i < tamanhoTabela ; i++){
                *p = vetor[i];
                delete p;
            }
            delete []vetor;
        }

        void imprime(){
            for(int i = 0 ; i < tamanhoTabela ; i++){
                cout << vetor[i].getInfo();
                cout << "  ";
            }
            cout << endl;
        }


};

int hash_linear(int chave , int i){
    if(i != 0){
        num_colisoesLinear++;
    }
    return (3*chave + i);
}

int hash_aux_quadratica(int chave){
    return ( pow(chave,2) - 5*chave + 7);
}

int hash_dupla(int chave , int i){
    if(i != 0)
        num_colisoesDupla++;
    int h1 = hash_linear(chave , 0);
    int h2 = hash_aux_quadratica(chave);
    return (h1 + i*h2);
}

int transformaInteiro(string str){
    int inteiro = 0;
    int n = str.size();
    for(int i = 0 , j = n  ; i < n ; i++){
        j--;
        inteiro += (str[j] - '0')*pow(10, i);
    }
    return inteiro;
}


int main()
{

    fstream arq("arqInfo.txt");
    if(arq.is_open()){
        string str;
        getline(arq , str);
        int tam_str = str.size();

        int i = 0; string aux_tamanho;
        while (str[i] != ' '){
            aux_tamanho += str[i];
            i++;
        }
        int tamanho = transformaInteiro(aux_tamanho);

        i++; string aux_numEntradas;
        while (str[i] != ' '){
            aux_numEntradas += str[i];
            i++;
        }
        int numEntradas = transformaInteiro(aux_numEntradas);

        i++; string aux_tipoHash;
        while (i < tam_str){
            aux_tipoHash += str[i];
            i++;
        }
        int tipoHash = transformaInteiro(aux_tipoHash);

        Hash *h = new Hash();
        int m = tamanho;
        int escolha_hash = tipoHash;
        switch (escolha_hash){
            case 1:
                h->criar(m , hash_linear);
                break;

            case 2:
                h->criar(m , hash_dupla);
                break;

            default:
                cout << "valor incorreto" << endl;
        }
        for (int i = 0 ; i < numEntradas ; i++){
            getline(arq , str);
            Data *d = new Data();
            int info_data = transformaInteiro(str);
            d->setInfo(info_data);
            h->inserir(info_data , d);
        }
        h->imprime();
        if(tipoHash == 1)
            cout << num_colisoesLinear << endl;
        if(tipoHash == 2)
            cout << num_colisoesDupla << endl;
       
        arq.close();


    } else {
        cout << "ERRO:  O arquivo nao pode ser aberto!" << endl;
    }

    

    return 0;
}
