#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <windows.h>
#include "Tad.h"

void InicializarProcessadores(TpProcessador p[], int n){
    for(int i=0;i<n;i++){
        p[i].ocupado = 0;
        p[i].tempo_resta = 0;
        p[i].qtd_processos = 0;
        p[i].tempo_total = 0;
    }
}
//Barras

void BarraProgresso(int total, int restante){
    int tam = 10;
    if(total == 0) total = 1;

    int preenchido = ((total - restante) * tam) / total;

    printf("[");
    for(int i=0;i<tam;i++){
        if(i < preenchido) printf("%c",219);
        else printf("%c",176);
    }
    printf("]");
}

//Moldura

void Moldura(int CI, int LI, int CF, int LF, int Frente) {
    textcolor(Frente);
    gotoxy(CI, LI); printf("%c", 201);
    gotoxy(CF, LI); printf("%c", 187);
    gotoxy(CI, LF); printf("%c", 200);
    gotoxy(CF, LF); printf("%c", 188);
    for (int a = CI + 1; a < CF; a++) {
        gotoxy(a, LI); printf("%c", 205);
        gotoxy(a, LF); printf("%c", 205);
    }
    for (int a = LI + 1; a < LF; a++) {
        gotoxy(CI, a); printf("%c", 186);
        gotoxy(CF, a); printf("%c", 186);
    }
    textcolor(7);
}

void FormPrincipal(void) {
    system("cls");
    Moldura(10, 5, 110, 29, 15);
    Moldura(11, 6, 109, 8, 4);
    Moldura(11, 9, 109, 28, 4);
}

//Interface atividades
void MostrarProcessadores(TpProcessador p[], int n, int tempo){
    int x = 13;
    int y = 12;

    gotoxy(x, y-2);
    textcolor(11);
    printf("PROCESSADORES - Tempo: %d", tempo);

    for(int i=0;i<n;i++){
        gotoxy(x, y + i);
        printf("CPU %d: ", i+1);
        if(p[i].ocupado){
            printf("%d - %s - %d  ", p[i].atual.cod,p[i].atual.nome, p[i].atual.prio);
            BarraProgresso(p[i].atual.tempo, p[i].tempo_resta);
            printf(" %d ut             ", p[i].tempo_resta);
        }
        else{
            textcolor(8);
            printf("LIVRE                                              ");
            textcolor(7);
        }
    }
}

void MostrarEntrada(TpTarefas t,int linha){
    int aux = 60;
    gotoxy(aux, linha);
    textcolor(14);
    printf("ULTIMA ENTRADA: %-20s (P%d)", t.nome, t.prio);
    textcolor(7);
}

void ExibirFilaSlots(TpFila f, int linha, int tempo){
    int pos = f.inicio;
    int max = 12;
    int x =70; 
    gotoxy(x, linha);
    textcolor(11);
    printf("FILA:");

    for(int i=0;i<max;i++){
        gotoxy(x, linha+2+i);
        printf("                                ");
        gotoxy(x, linha+2+i);

        if(i < f.qtd){
            if(i==0) printf("-> ");
            else printf("   ");
            printf("%d - %-15s - %d",f.fila[pos].cod, f.fila[pos].nome,f.fila[pos].prio);
            int espera = tempo - f.fila[pos].tempochegada;
            pos = (pos+1)%MAXFILA;
        }
        else{
            textcolor(8);
            printf("VAZIA  ");
            textcolor(7);
        }
    }
    if(f.qtd > max){
        gotoxy(x, linha+2+max);
        printf("+%d...", f.qtd - max);
    }
}

void CarregaArquivo(FILE *arq, TpFila &f, int tempoatual,int intervalo){
    TpTarefas t;
    if(tempoatual % intervalo == 0){
        if (fscanf(arq, "%d;%[^;];%d\n", &t.cod, t.nome, &t.tipo) == 3){
               t.prio = t.tipo;
            switch(t.tipo){
                case 1: t.tempo = 5; break;
                case 2: t.tempo = 8; break;
                case 3: t.tempo = 4; break;
                case 4: t.tempo = 3;

            }
            t.tempochegada = tempoatual;
            if(!FilaCheia(f.qtd))
                Inserir(f,t);;
            
            MostrarEntrada(t,7);
        }
    }
} 

void Simular(int qtdcpu, int tempoduracaoexecucao){
    TpFila fila;
    Inicializar(fila);
    TpProcessador cpu[15]; 
    InicializarProcessadores(cpu,qtdcpu);
    int intervalo = 4;
    int flag = 0;
    int tempoGlobal = 0;
    FILE *arq = fopen("tarefas.txt", "r");
    if (arq == NULL) 
        printf("Erro: Arquivo tarefas.txt nao encontrado!\n");
        
    FormPrincipal(); 
    int algumOcupado = 0;
    gotoxy(35,27);
    printf("Clique +/- para mudar o intervalo e ESC para sair!");
    do {
        //Controla o intervalo
        if(kbhit()){
            char tecla = getch();
            if(tecla == '+')
               intervalo++;
            else if(tecla == '-' && intervalo > 1)
                intervalo--;
            if(tecla == 27)
                flag = 1;
        }
        //Carrega dados
        if(tempoGlobal < tempoduracaoexecucao)
           CarregaArquivo(arq, fila, tempoGlobal, intervalo);

        //Logica das cpus
        algumOcupado = 0;
        for(int i = 0; i < qtdcpu; i++) { 
        //Esta fazendo
        if (cpu[i].ocupado) {
        cpu[i].tempo_resta--;
        cpu[i].tempo_total++; 
                    
            if (cpu[i].tempo_resta <= 0) 
                cpu[i].ocupado = 0; 
            
        }
        //Esta Livre
        if (!cpu[i].ocupado && !FilaVazia(fila.qtd)) {
            cpu[i].atual = Retirar(fila);
            cpu[i].tempo_resta = cpu[i].atual.tempo;
            cpu[i].ocupado = 1;
            cpu[i].qtd_processos++;
        }

        if(cpu[i].ocupado) 
            algumOcupado = 1;
        }
            
        gotoxy(12, 7);
        textcolor(7);
        printf("TEMPO: %d ut | TEMPO FINAL: %d ut Intervalo: %d" , tempoGlobal, tempoduracaoexecucao, intervalo);
        MostrarProcessadores(cpu, qtdcpu, tempoGlobal);
        ExibirFilaSlots(fila, 10, tempoGlobal);

        Sleep(500); 
        tempoGlobal++;
            
    } while (tempoGlobal < tempoduracaoexecucao && flag == 0);
    gotoxy(50,107); textcolor(7);
    printf("Clique +/- para mudar o intervalo!");
    fclose(arq);
}



int main() {
    int qtd,tempo;
    FormPrincipal();
    gotoxy(35, 10);
    printf("CONFIGURACAO DA SIMULACAO");
    gotoxy(35, 12);
    printf("Quantidade de CPUs (1-15): ");
    scanf("%d", &qtd);
    gotoxy(35, 14);
    printf("Tempo execucao da simulacao ");
    scanf("%d", &tempo);
     if(qtd == 15)
        qtd = 15;
    if(qtd == 0)
        qtd = 1;
    if(tempo <= 0)
        tempo = 20;
    Simular(qtd, tempo);
    system("cls");
    printf("Simulacao encerrada com sucesso.\n");
    return 0;
}