//
// Created by Puca Penna on 03/10/19.
//

#include <cmath>
#include <iostream>
#include <ctime>
#include "GRASP.h"
#include "Construcao.h"
#include "Util.h"
#include "Descida.h"
#include "Arquivos.h"

float GRASP(int n, std::vector<int> &s, float **d, float alpha, int GRASP_max /*, int tipo_construcao*/)
{
    vector<int> s_star;
    int iter = 0;
    float fo_star = INT64_MAX;
    float fo;

    // por volta de 470 com parametros alpha = 0.3 e GRASPmax = 700 iteracoes
    // por volta de 465 com parametros alpha = 0.2 e GRASP_max = 1000 iteracoes
    for (int i = 0; i < GRASP_max; i++) {
        constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n ,s, d, alpha);
        fo = descida_best_improvement(n, s, d);

    }
    
    
    s = s_star;

    return fo_star;
}

