#include <cstdlib>
#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include "Util.h"
#include "Construcao.h"

using namespace std;

void constroi_solucao(int n, vector<int>& s, float **distancia)
{
    s.clear(); // limpa solucao
    //insere todas cidades sequencialmente
    for (int j=0; j < n; j++) s.push_back(j);
}

/* Constroi uma solucao de forma gulosa, no caso,
   implementa o Metodo construtivo do vizinho mais proximo */
void constroi_solucao_gulosa_vizinho_mais_proximo(int n, vector<int> &s, float **d)
{
  vector<int> nao_visitada; //lista das cidades ainda nao visitadas
  // inserir um elemento no final de uma lista
  for(int i = 1; i < n; i++) nao_visitada.push_back(i);

  s.clear(); // limpa solucao
  s.push_back(0); // A cidade de origem é a cidade 0

  int mais_proxima; // armazena a cidade mais proxima para inserir na solucao
  float dist; // armazena a menor distancia

  int j = 1;
  while (j < n) {
    dist = INT64_MAX;
    int i = 0;
    int pos_i = 0;

    while (i < nao_visitada.size()) {
      if (d[s.at(j-1)][nao_visitada[i]] < dist) {
        dist = d[s.at(j-1)][nao_visitada[i]];
        mais_proxima = nao_visitada[i];
        pos_i = i;
      }
      i++;
    }
    s.push_back(mais_proxima);
    nao_visitada.erase(nao_visitada.begin() + pos_i);
    j++;
  }

/*
	Implementar o loop do metodo construtivo, 
	inserindo sempre a cidade mais proxima ainda
	nao visitada na solucao.
*/
	
}



/* Constroi uma solucao de forma aleatoria */
void constroi_solucao_aleatoria(int n, vector<int> &s, float **d)
{

    constroi_solucao(n, s, d);

    //Para c++ 11
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rand_seed(seed);
    shuffle ( s.begin(), s.end(), rand_seed );

    //Para c++ 98
    //random_shuffle ( s.begin(), s.end() );

}

/* Constroi uma solucao pela insercao mais barata */
void constroi_solucao_gulosa_insercao_mais_barata(int n, vector<int> *s, float **d)
{
    vector<int> nao_visitada;

    /* Inicio da Fase de Construcao de uma solucao */
    cout << "\n Construindo nova solucao ...\n";

    for (int i=1; i<n; i++){

        /* vou inserir um registro no final de uma lista  */
        nao_visitada.push_back(i);

    }
    s->clear();
    s->push_back(0);  /* A cidade origem é a cidade 0 */

    int j = 1;
    int mais_proxima;
    float dist;

    int cid_i, cid_j, cid_k, pos;
    int melhor_i, melhor_k;
    float sij, melhor_sij;


    /* Insere as duas proximas cidades pela heuristica do vizinho mais proximo */

    while ( j < 3){
        dist = INT64_MAX;
        int i = 0;
        int pos_i = 0;

        while (i < nao_visitada.size()) {
            if (d[s->at(j-1)][nao_visitada[i]] < dist){
                dist = d[s->at(j-1)][nao_visitada[i]];
                mais_proxima = nao_visitada[i];
                pos_i = i;
            }
            i++;
        }
        /* Insere a cidade mais proxima apos a ultima cidade inserida*/
        s->push_back(mais_proxima);
        /* Apaga a cidade mais_proxima da lista de nao visitadas */
        nao_visitada.erase(nao_visitada.begin() + pos_i);

        j++;
    }


   /* Formada uma subrota inicial com tres cidades, aplicar o metodo da
      insercao mais barata para decidir qual cidade inserir entre cada
      par de cidades i e j. A cidade k escolhida sera aquela que minimizar
      custo(k) = d(i,k) + d(k,j) - d(i,j) */

    while (j < n) {
      melhor_sij = INT64_MAX;

      int k = 0;
      int pos_k;
      /* Calcula os custos para cada cidade k ser inserida entre as cidades i e j */
      while (k < nao_visitada.size()) {
        cid_k = nao_visitada[k];
        for (int i = 0; i < j; i++) {
	      cid_i = s->at(i);
          if ((i+1) != j) cid_j = s->at(i+1);
          else cid_j = 0;

          sij = d[cid_i][cid_k] + d[cid_k][cid_j] - d[cid_i][cid_j];

          if (sij < melhor_sij) {
 	        melhor_i = cid_i;
            //melhor_j = cid_j;
            melhor_k = cid_k;
            melhor_sij = sij;
            pos = i + 1;
            pos_k = k;
          }
        }
        k++;
      }

      /* Adiciona cada nova cidade k entre as cidades i e j que produzir a insercao mais barata */
      //insere_meio_vetor(s,melhor_k,pos,j);
      s->insert(s->begin()+pos,melhor_k);

      //imprime_rota(s,n);

      /* Apaga a cidade mais barata da lista de nao visitadas */
        nao_visitada.erase(nao_visitada.begin() + pos_k);

      j++;
    }
}

/* Constroi uma solucao parcialmente gulosa pelo metodo do vizinho mais proximo */
void constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(int n, vector<int> &s, float **d, float alpha)
{
    if (alpha < 0) alpha = 0;
    if (alpha > 1) alpha = 1;

    vector<int> nao_visitadas; 
    int tamanho_LC;

    /* Inicio da Fase de Construcao de uma solucao */
    for (int i=1; i<n; i++){

        /* vou inserir um registro no final de uma lista das cidades nao visitadas */
        nao_visitadas.push_back(i);

    }

    //limpa solucao corrente
    s.clear();
    s.push_back(0);  /* A cidade origem � a cidade 0 */
    
    //Ordena lista
    ordena_dist_crescente ordem;
    ordem.d = d; // fornece a matriz de distancia para usar na ordenacao
    ordem.index = s[0];

    /* Ordenando a lista de cidade nao visitadas */
    stable_sort(nao_visitadas.begin(), nao_visitadas.end(), ordem);

    int j = 1;
    int selected;
    vector<int> lrc;

    while (j < n) {
      float gmin = d[ordem.index][nao_visitadas[0]];
      float gmax = d[ordem.index][nao_visitadas[nao_visitadas.size()-1]];
      float g = gmin + alpha * (gmax - gmin);
      
      int k = 0;
      while (d[ordem.index][nao_visitadas[k]] <= g && k < nao_visitadas.size()) lrc.push_back(nao_visitadas[k++]);

      selected = rand() % lrc.size();

      s.push_back(lrc[selected]);

      nao_visitadas.erase(nao_visitadas.begin() + selected);

      lrc.clear();
      
      ordem.index = s[j];
      j++;

      stable_sort(nao_visitadas.begin(), nao_visitadas.end(), ordem);
    }
}

typedef struct visitedCities {
  int num_cid, pos_previous; // numero da cidade e posicao anterior pra inserir na solucao
  bool in_lcr; // checa se a cidade já está no lcr
} Visited;

/* Constroi uma solucao parcialmente gulosa com base no metodo da insercao mais barata */
void constroi_solucao_parcialmente_gulosa_insercao_mais_barata(int n, vector<int> &s, float **d, float alpha)
{
    if (alpha < 0) alpha = 0;
    if (alpha > 1) alpha = 1;

    vector<Visited> nao_visitadas;

    for (int i = 1; i < n; i++) {
      Visited v = {
        i,
        0,
        false
      };
      nao_visitadas.push_back(v);
    }

    s.clear();
    s.push_back(0);

    int j = 1, mais_proxima;
    float dist;
    int size, selected;

    vector<int> lcr; // o lcr guarda as posições do vector de nao visitadas

    while (j < 3) {

      int i, pos_i;

      size = round((n - j) * alpha);

      while (lcr.size() < size) {
        dist = INT64_MAX;
        i = 0;
        pos_i = 0;
        while (i < nao_visitadas.size()) {
          if (nao_visitadas[i].in_lcr == false && d[s.at(j-1)][nao_visitadas[i].num_cid] < dist){
            dist = d[s.at(j-1)][nao_visitadas[i].num_cid];
            mais_proxima = nao_visitadas[i].num_cid;
            pos_i = i;
          }
        i++;
        }
        lcr.push_back(pos_i);
        nao_visitadas[pos_i].num_cid = mais_proxima;
        nao_visitadas[pos_i].in_lcr = true;
      }

      selected = rand() % lcr.size();

      s.push_back(nao_visitadas[lcr[selected]].num_cid);
      nao_visitadas.erase(nao_visitadas.begin() + lcr[selected]);

      for (int i = 0; i < nao_visitadas.size(); i++) nao_visitadas[i].in_lcr = false; 

      lcr.clear();

      j++;
    }

    int cid_i, cid_j, cid_k;
    int melhor_i, melhor_k, pos;
    float sij, melhor_sij;

    while (j < n) {
      int k, pos_k;

      size = round((n - j) * alpha);
      if (size < 1) size = 1;

      while (lcr.size() < size) {
        melhor_sij = INT64_MAX;
        k = 0;
        while (k < nao_visitadas.size()) {
          if (nao_visitadas[k].in_lcr == false) {
            cid_k = nao_visitadas[k].num_cid;

            for (int i = 0; i < j; i++) {
              cid_i = s.at(i);
              if ((i + 1) != j) cid_j = s.at(i+1);
              else cid_j = 0;

              sij = d[cid_i][cid_k] + d[cid_k][cid_j] - d[cid_i][cid_j];

              if (sij < melhor_sij) {
                melhor_k = cid_k;
                melhor_sij = sij;
                pos = i+1;
                pos_k = k;
              }
            }
          }
          k++;
        }

        nao_visitadas[pos_k].num_cid = melhor_k;
        nao_visitadas[pos_k].pos_previous = pos;
        nao_visitadas[pos_k].in_lcr = true;
        lcr.push_back(pos_k);
      }

      selected = rand() % lcr.size();

      s.insert(s.begin() + nao_visitadas[lcr[selected]].pos_previous, nao_visitadas[lcr[selected]].num_cid);

      nao_visitadas.erase(nao_visitadas.begin() + lcr[selected]);

      for(int i = 0; i < nao_visitadas.size(); i++) nao_visitadas[i].in_lcr = false;
      lcr.clear();

      j++;
    }
}
