#include "aco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lista.h"
#include "util.h"

instancia_t instancia;

/* PARÂMETROS */
double alfa;
double beta;
double rho;
double q0;       // Ferômonio inicial

int n_formigas;
int n_ciclos;

formiga_t *lista_formigas;
formiga_t melhor_formiga;
double *feromonio;          // vetor do feromonio depositado em cada coluna

void inicializarFormiga(formiga_t *formiga){
    formiga->colunas = lista_criar();
    formiga->qtd_colunaCobreLinha = (int*)(calloc(instancia.l, sizeof(int)));
}

void inicializarVariaveis(){
    lista_formigas = (formiga_t*)(malloc(n_formigas * sizeof(formiga_t)));

    int i;
    for (i = 0; i < n_formigas; ++i){
        inicializarFormiga(&lista_formigas[i]);
    }

    inicializarFormiga(&melhor_formiga);

    feromonio = (double*)(malloc(instancia.c * sizeof(double)));
}

void inicializarFeromonio(){
    int i;
    for (i = 0; i < instancia.c; i++){
        feromonio[i] = q0;
    }
}

double heuristica(int coluna, formiga_t *formiga, lista_t *linhasDescobertas){
    int tam_intersec = tam_intersecao(instancia.coluna[coluna], linhasDescobertas->elem, instancia.nlinhas[coluna], linhasDescobertas->tam);
    double visibilidade = (double) tam_intersec / instancia.custo[coluna];
    return visibilidade;
}

double calcularProbabilidade(int coluna, formiga_t *formiga, lista_t *linhasDescobertas){
    if (lista_contem(formiga->colunas, coluna)){
        return 0.0;
    }

    double h = heuristica(coluna, formiga, linhasDescobertas);
    double numerador = pow(feromonio[coluna], alfa) + pow(h, beta);

    
}

int maximizarProbabilidade(int linha, lista_t *linhasDescobertas, formiga_t *formiga){
    int i, coluna, custo;
    double probabilidade;
    double maior = 0.0;
    int melhorColuna = -1;
    for (i = 0; i < instancia.ncolunas[linha]; i++){
        coluna = instancia.linha[linha][i];

        probabilidade = calcularProbabilidade(coluna, formiga, linhasDescobertas);
        if (probabilidade > maior){
            maior = probabilidade;
            melhorColuna = coluna;
        }
    }

    return melhorColuna;
}

void construirSolucao(formiga_t *formiga){
    int i;
    lista_t *linhasDescobertas = lista_criarTam(instancia.l);
    for (i = 0; i < instancia.l; i++){
        lista_insere(linhasDescobertas, i);
    }

    while (!lista_vazia(linhasDescobertas)){
        int rand_int = random_int(linhasDescobertas->tam);
        int linha = lista_obter(linhasDescobertas, rand_int);

        int melhorColuna = maximizarProbabilidade(linha, linhasDescobertas, formiga);
    }
    
}

void eliminarRedundancia(formiga_t *formiga){

}

void construirSolucoesFormigas(){
    int i;
    for (i = 0; i < n_formigas; ++i){
        construirSolucao(&lista_formigas[i]);
        eliminarRedundancia(&lista_formigas[i]);
    }
}

void ant_colony(instancia_t inst){
    instancia = inst;
    inicializarVariaveis();
    inicializarFeromonio();

    int i;
    for (i = 0; i < n_ciclos; ++i){
        construirSolucoesFormigas();
        //atualizarFeromonio();
    }
}
