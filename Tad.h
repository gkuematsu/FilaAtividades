#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <windows.h>

#define MAXFILA 100

struct TpTarefas
{
    char nome[50];
    int cod,prio,tempo,tempochegada;
};

struct TpProcessador{
    TpTarefas atual;
    int ocupado,tempo_resta,qtd_processos,tempototal;
};

struct TpFila
{
    int fim,inicio,qtd;
    TpTarefas fila[MAXFILA];
};

void Inicializar(TpFila &f){
    f.fim = -1;
    f.qtd = 0;
    f.inicio = 0;
}

char FilaCheia(int qtd){
    return qtd == MAXFILA;
}

char FilaVazia(int qtd){
    return qtd == 0;
}

void Inserir(TpFila &f, TpTarefas elem){
    int pos, ant;
    TpTarefas aux;

    if(f.fim == MAXFILA - 1)
        f.fim = -1; 

    f.fila[++f.fim] = elem;  

 
    pos = f.fim;
    while(f.qtd > 0 && pos != f.inicio){
        if(pos == 0)
            ant = MAXFILA - 1; 
        else
            ant = pos - 1;

        if(f.fila[pos].prio < f.fila[ant].prio){
            aux = f.fila[ant];
            f.fila[ant] = f.fila[pos];
            f.fila[pos] = aux;
        } 

        pos = ant;
    }

    f.qtd++;
}


TpTarefas Retirar(TpFila &f ){
	TpTarefas elem;
    elem = f.fila[f.inicio]; 

    if(f.inicio == MAXFILA - 1)  
        f.inicio = 0;
    else
        f.inicio++;

    f.qtd--; 
    return elem;

}

TpTarefas Inicio(TpFila f){
    return f.fila[f.inicio];
}

TpTarefas Fim(TpFila f){
    return f.fila[f.fim];
}