
/*
Tecnicas Heuristicas para resolucao do Problema do Caixeiro Viajante
Autor: Marcone Jamilson Freitas Souza
Local: Departamento de Computacao - Universidade Federal de Ouro Preto
Homepage: www.decom.ufop.br/prof/marcone
Data: 21-05-2007
Atualizado por Puca Huachi em ago/2019
*/

//---------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <ctime>
#include <iomanip>
#include <vector>
#include "Util.h"
#include "Arquivos.h"
#include "Construcao.h"
#include "Menus.h"
#include "Descida.h"
#include "MS.h"
#include "SA.h"
#include "GRASP.h"

//---------------------------------------------------------------------------
using namespace std;


int main(int argc, char* argv[])
{
  int n;                    // numero de cidades
  vector<int> s;            // vetor solucao corrente
  float **d,                // matriz de distancias entre as cidades
        fo,                 // funcao objetivo corrente
        melhor_fo_lit;      // melhor fo da literatura
  clock_t inicio_CPU,       // clock no inicio da aplicacao do metodo
          fim_CPU;          // clock no final da aplicacao do metodo
  int iterMax;

  int nind;
  float max_desvio;
  double prob_crossover;
  double prob_mutacao;
  float alpha = 0;
  int samax;

  float temp;

  //srand(1000); // pega o numero 1000 como semente de numeros aleatorios
  srand((unsigned) time(NULL)); // pega a hora do relogio como semente

  obter_parametros_pcv((char*)"data/c50info.txt", &n, &melhor_fo_lit);
  d = cria_matriz_float(n, n); // matriz de distancias
  le_arq_matriz((char*)"data/c50.txt", n, d);


  int escolha = 0;
  do {
    escolha = menu_principal();
    switch (escolha) {
    case 0:
            printf("\n\nBye bye!!!\n\n");
        break;
    case 1: /* Geracao de uma solucao inicial */
           switch(menu_solucao_inicial()) {
           case 1: /* Geracao gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 constroi_solucao_gulosa_vizinho_mais_proximo(n, s, d);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma gulosa (Vizinho Mais Proximo):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 2: /* Geracao parcialmente gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 alpha = 0;
                 constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n, s, d, alpha);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma parcialmente gulosa (Vizinho Mais Proximo):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 3: /* Geracao gulosa de uma solucao inicial via metodo da insercao mais barata */
                 constroi_solucao_gulosa_insercao_mais_barata(n,&s,d);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma gulosa (Insercao Mais Barata):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 4: /* Geracao parcialmente gulosa de uma solucao inicial via insercao mais barata */
                 alpha = 0.1;
                 constroi_solucao_parcialmente_gulosa_insercao_mais_barata(n, s ,d, alpha);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma parcialmente gulosa (Insercao Mais Barata):\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
              //   printf("Nao implementado\n"); break;
           case 5: /* Geracao aleatoria de uma solucao inicial */
                 constroi_solucao_aleatoria(n, s, d);
                 fo = calcula_fo(n, s, d);
                 printf("\nSolucao construida de forma aleatoria:\n");
                 imprime_rota(s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           }
           break;
    case 2: /* Descida com estrategia best improvement*/
           inicio_CPU = clock();
           fo = descida_best_improvement(n,s,d);
           fim_CPU = clock();
           printf("\nSolucao obtida usando a estrategia Best Improvement do Metodo da Descida:\n");
           imprime_rota(s,n);
           printf("Funcao objetivo = %f\n",fo);
           printf("Tempo de CPU = %f segundos:\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 3: /* Descida Randomica */
           printf("Nao implementado\n"); break;

    case 4: /* Descida com primeiro de melhora */
           inicio_CPU = clock();
           fo = descida_first_improvement(n, s, d);
           fim_CPU = clock();
           printf("\nSolucao obtida usando a estrategia First Improvement do Metodo da Descida:\n");
           imprime_rota(s, n);
           printf("Funcao objetivo = %f\n",fo);
           printf("Tempo de CPU = %f segundos:\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 5: /* Multi-Start */
            inicio_CPU = clock();
            fo = MS(n, s, d, n*2);
            fim_CPU = clock();
            printf("\nSolucao obtida usando a meta-heuristica Multi-Start:\n");
            imprime_rota(s, n);
            printf("Funcao Objetivo = %f\n", fo);
            printf("Tempo de CPU = %f segundos: \n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 6: /* Simulated Annealing */
            constroi_solucao_aleatoria(n, s, d);
            inicio_CPU = clock();
            fo = SA(n, s, d, 0.95, 100*n, 300, 0.01);
            fim_CPU = clock();
            printf("\nSolucao obtida usando a meta-heuristica Simulated Annealing:\n");
            imprime_rota(s, n);
            printf("Funcao Objetivo = %f\n", fo);
            printf("Tempo de CPU = %f segundos: \n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 7: /* Busca Tabu */
           printf("Nao implementado\n");
           break;

    case 8: /* Iterated Local Search */
           printf("Nao implementado\n");
           break;

    case 9: /* GRASP */
           alpha = 0.3;
           inicio_CPU = clock();
           fo = GRASP(n, s, d, alpha, 700);
           fim_CPU = clock();
           printf("\nSolucao obtida usando a meta-heuristica GRASP:\n");
           imprime_rota(s, n);
           printf("Funcao Objetivo = %f\n", fo);
           printf("Tempo de CPU = %f segundos: \n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 10: /* VND */
           printf("Nao implementado\n");
           break;

    case 11: /* VNS */
           printf("Nao implementado\n");
           break;

    case 12: /* Algoritmos Geneticos */
           switch(menu_AG()) {
           case 1: /* Algoritmos Geneticos usando operador OX */
                   printf("Nao implementado\n");
                   break;
           case 2: /* Algoritmos Geneticos usando operador ERX */
                 printf("Nao implementado\n");
                 break;
           }
           break;

    case 13: /* LAHC */
           printf("LAHC nao implementado\n");break;

    case 14: /* Algoritmos Memeticos */
           printf("\n Algoritmos Memeticos ainda nao implementado ... \n");
           break;

    case 15: /* Colonia de Formigas */
           printf("\n Colonia de Formigas ainda nao implementado ... \n");
           break;

    default:
        printf("\n Opcao invalida ... \n");
        break;
    }
  } while (escolha != 0);


  //libera_vetor(s);
  libera_matriz_float(d, n);
  return 0;
}
//---------------------------------------------------------------------------
