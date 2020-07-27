#include "Core.h"
#include "LocalSearch.h"
#include "Heuristics.h"

void mttp(vector<Casa>& cidade, vector<Item>& itens, vector<vector<int> >& distCasas, int i, ofstream& saida, int esc,
    int qualGRASP)
{

    nMochileiros = i;

    vector<Mochileiro> ladroes(nMochileiros);

    //-----------GRASP_VND----------------
    if (esc == 0) {
        GRASP(cidade, itens, ladroes, distCasas, qualGRASP);
        double atual = fObj(ladroes, itens, cidade, distCasas);
        imprimeInstancia3(ladroes, cidade, atual);
    }
  
    //-----------VND----------------
    if (esc == 1) {

        Greedy1(cidade, itens, ladroes, distCasas);

        VND(cidade, itens, ladroes, distCasas);

        double atual = fObj(ladroes, itens, cidade, distCasas);

        imprimeInstancia3(ladroes, cidade, atual);
    }

    // ! Caution: Greedy 1 retornará sempre a mesma solucao?
    //----------- ILS_VND ----------------
    if (esc == 2) {

        double S_Best_Value = -1;
        vector<Mochileiro> S_Best;

        for (int i = 0; i < 30; i++) {
            vector<Mochileiro> ladroes(nMochileiros);

            ILS(cidade, itens, ladroes, distCasas);

            double newobj = fObj(ladroes, itens, cidade, distCasas);

            if (newobj > S_Best_Value) {
                S_Best_Value = newobj;
                S_Best = ladroes;
            }
        }
        imprimeInstancia3(S_Best, cidade, S_Best_Value);
    }
}

int main(int argc, char** argv)
{

    srand(time(NULL));
    
    vector<Casa> cidade;
    vector<Item> itens;
    readInput(cidade, itens);

    listItems(cidade, itens);

    vector<vector<int> > distCasas(dimensao, vector<int>(dimensao, 0));

    calculaDistCasas(cidade, distCasas);

    ofstream saida("saida.txt");

    v = (vMax - vMin) / capacidade * (1.0); //Constante da função Objetivo

    //Primeiro argumento: Número de Mochileiros
    //Segundo argumento: Heuristica escolhida
    //Terceiro argumento: GRASP escolhida

    start = time(NULL);

    contIter();

    // Primeira ILS 40 / 60
    // Segunda  ILS 30 / 40
    
    mttp(cidade, itens, distCasas, atoi(argv[1]), saida, atoi(argv[2]), atoi(argv[3]));

    saida.close();
}
