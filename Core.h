#ifndef CORE_H
#define CORE_H

#include <bits/stdc++.h>
using namespace std;

// Variaveis globais recebidas do arquivos de leitura
// São utilizadas em varias partes do codigo por isto estão aqui

string instancia, tipo, tipoDistancia;

int dimensao, nItem, capacidade, nMochileiros;
double vMin, vMax, aluguel;
long double v; //Constante da função Objetivo

int nIteracoes;

time_t start;

// Guarda as informacoes referente a cada ladrão
struct Mochileiro {
    vector<int> caminho; // Cidades em que o mochleiro passa
    vector<vector<int> > mochila; // Contem os itens de cada cidade que o mochileiro passou
    int pesoMochila; // Peso da mochila com todos os itens que ele coletou

    // Construtor:
    Mochileiro()
    {
        mochila.resize(dimensao); //A primeira casa possui index 1. A origem zero.
        pesoMochila = 0;
    }
};

// Guarda as informacoes referente a cada Item que pode ser roubado
struct Item {

    int index; // Indice do item
    int lucro; // Valor daquele determinado item
    int peso; // Peso daquele item
    int ondeTo; // Em qual cidade este item se encontra
};

// Guarda as informacoes referente a cada Cidade incluindo os que estão nela
struct Casa {
    int idCasa; // Indice da casa
    long double x; // Posição do indice x no plano cartesiano
    long double y; // Posição do indice y no plano cartesiano
    vector<Item> itemCasa; // Todos os itens que estão na casa
    vector<bool> visited; // Verifica se o item daquela cidade já foi roubado por algum ladrão
};

bool temTempo()
{
    if (time(NULL) - start > 600) {
        return false;
    }
    return true;
}

int dist(Casa a, Casa b)
{
    return ceil(sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y))));
}

//Imprime instancia para o visualizador
void imprimeInstancia(const vector<Mochileiro>& ladroes, const vector<Casa>& cidade, ofstream& saida)
{

    for (int i = 0; i < ladroes.size(); i++) {

        if (ladroes[i].caminho.empty())
            continue;

        saida << "[";

        for (int j = 0; j < ladroes[i].caminho.size(); j++) {

            //Imprime as cidades que o ladrao "i" passou
            saida << ladroes[i].caminho[j] + 1;

            if (j != ladroes[i].caminho.size() - 1) {
                saida << ",";
            }
        }

        saida << "]\n";
        saida << "[";

        //Contando quantas virgulas teremos que imprimir

        int virgulas = 0;

        for (int j = 0; j < ladroes[i].mochila.size(); j++) {
            if (ladroes[i].mochila[j].size() != 0) {
                virgulas++;
            }
        }
        virgulas -= 1;

        for (int j = 0; j < ladroes[i].mochila.size(); j++) {

            if (ladroes[i].mochila[j].size() == 0) { // Sem itens roubados pelo ladrao i na cidade j
                continue;
            }

            for (int k = 0; k < ladroes[i].mochila[j].size(); k++) {
                saida << ladroes[i].mochila[j][k] + 1;

                if (k != ladroes[i].mochila[j].size() - 1)
                    saida << ",";
            }

            if (virgulas > 0) { //Se ainda posso imprimir uma virgula
                virgulas--;
                saida << ",";
            }
        }
        saida << "]\n";
    }
}

//Usada
void imprimeInstancia3(const vector<Mochileiro>& ladroes, const vector<Casa>& cidade,
    const double valorFOBJ)
{

    cout << "FOBJ: " << valorFOBJ;

    string s2(tipo.begin(), tipo.end() - 1);

    int qual;

    if (cidade[1].itemCasa.size() == 10)
        qual = 7;
    else
        qual = 3;

    cout << " Instancia: " << instancia << "_n" << nItem << '_' << s2 << "_0" << qual << endl;
    cout << "Número de Mochileiros: " << nMochileiros << endl;
    for (int i = 0; i < ladroes.size(); i++) {

        if (ladroes[i].caminho.empty())
            continue;

        cout << "[";

        for (int j = 0; j < ladroes[i].caminho.size(); j++) {

            //Imprime as cidades que o ladrao "i" passou
            cout << ladroes[i].caminho[j] + 1;

            if (j != ladroes[i].caminho.size() - 1) {
                cout << ",";
            }
        }

        cout << "]\n";
        cout << "[";

        //Contando quantas virgulas teremos que imprimir

        int virgulas = 0;

        for (int j = 0; j < ladroes[i].mochila.size(); j++) {
            if (ladroes[i].mochila[j].size() != 0) {
                virgulas++;
            }
        }
        virgulas -= 1;

        for (int j = 0; j < ladroes[i].mochila.size(); j++) {

            if (ladroes[i].mochila[j].size() == 0) { // Sem itens roubados pelo ladrao i na cidade j
                continue;
            }

            for (int k = 0; k < ladroes[i].mochila[j].size(); k++) {
                cout << ladroes[i].mochila[j][k] + 1;

                if (k != ladroes[i].mochila[j].size() - 1)
                    cout << ",";
            }

            if (virgulas > 0) { //Se ainda posso imprimir uma virgula
                virgulas--;
                cout << ",";
            }
        }
        cout << "]\n";
    }
    cout << "\n" << endl;
}

void unmarkItems(vector<Casa>& cidade)
{
    for (int i = 0; i < dimensao; i++)
        for (int j = 0; j < cidade[i].visited.size(); j++)
            cidade[i].visited[j] = false;
}

void readInput(vector<Casa> &cidade, vector<Item> &itens)
{
    string lixo;
    cin >> lixo >> lixo >> instancia;
    cin >> lixo >> lixo >> lixo;
    cin.ignore();
    getline(cin, tipo);
    cin >> lixo >> dimensao;
    cin >> lixo >> lixo >> lixo >> nItem;
    cin >> lixo >> lixo >> lixo >> capacidade;
    cin >> lixo >> lixo >> vMin;
    cin >> lixo >> lixo >> vMax;
    cin >> lixo >> lixo >> aluguel;
    cin >> lixo >> tipoDistancia;

    getline(cin, lixo);
    getline(cin, lixo);

    // Alocating memory.
    cidade.resize(dimensao);
    itens.resize(nItem);

    int aux;

    for (int i = 0; i < dimensao; i++) {
        cin >> aux;
        cidade[i].idCasa = aux - 1;
        cin >> cidade[i].x >> cidade[i].y;
    }

    getline(cin, lixo);
    getline(cin, lixo);

    for (int i = 0; i < nItem; i++) {

        cin >> aux;
        itens[i].index = aux - 1;

        cin >> itens[i].lucro >> itens[i].peso >> aux;
        itens[i].ondeTo = aux - 1;
    }
}

void listItems(vector<Casa>& cidade, vector<Item>& itens)
{
    for (int i = 0; i < itens.size(); i++) {
        cidade[itens[i].ondeTo].itemCasa.push_back(itens[i]);
        cidade[itens[i].ondeTo].visited.push_back(false);
    }
}

void calculaDistCasas(const vector<Casa>& cidade, vector<vector<int> >& distCasas)
{

    for (int i = 0; i < dimensao; i++) {
        for (int j = i; j < dimensao; j++) {
            int repetirSimetria = dist(cidade[i], cidade[j]);
            distCasas[i][j] = repetirSimetria;
            distCasas[j][i] = repetirSimetria;
        }
    }
}

double fObj(const vector<Mochileiro>& ladroes, const vector<Item>& itens, const vector<Casa>& cidade,
    const vector<vector<int> >& distCasas, int i)
{

    /*Funcao objetivo considera que o caminho de cada mochileiro possui IMPLICITAMENTE a saida da Origem
    e a chegada na Origem. Ex: Um caminho 0->1->2->0 sera representado no vector caminho por {1,2} */

    double resultado = 0;
    int pesoDaMochila = 0;
    int W = capacidade;

    if (!ladroes[i].caminho.size()) { // Se o tamanho da rota for 0,
        return 0;
    }

    //Calculo da primeira parte da funcao objetivo para o ladrao em questao: Lucro!

    double somatorioParte2 = 0;

    //Da origem ate o primeiro...
    double parteSuperiorFracaoInicial = distCasas[0][ladroes[i].caminho[0]];
    double parteInferiorFracaoInicial = vMax - (((vMax - vMin) / W * (1.0)) * pesoDaMochila);

    somatorioParte2 += parteSuperiorFracaoInicial / parteInferiorFracaoInicial; //Da ultima casa ate a origem

    int tamanhoCaminhoLadrao = ladroes[i].caminho.size();

    for (int j = 0; j < tamanhoCaminhoLadrao; j++) {

        int qualCidade = ladroes[i].caminho[j];

        for (int k = 0; k < ladroes[i].mochila[qualCidade].size(); k++) {

            int qualItem = ladroes[i].mochila[qualCidade][k];

            resultado += itens[qualItem].lucro;
            pesoDaMochila += itens[qualItem].peso;
        }

        if (j < tamanhoCaminhoLadrao - 1) {

            double parteSuperiorFracao = distCasas[ladroes[i].caminho[j]][ladroes[i].caminho[j + 1]];
            double parteInferiorFracao = vMax - (((vMax - vMin) / W * (1.0)) * pesoDaMochila);

            somatorioParte2 += parteSuperiorFracao / parteInferiorFracao;
        }
    }

    /* Casa [ 0 ] e a origem */
    //Do ultimo ate a origem...

    double parteSuperiorFracaoFinal = distCasas[ladroes[i].caminho[tamanhoCaminhoLadrao - 1]][0];

    double parteInferiorFracaoFinal = vMax - (((vMax - vMin) / W * (1.0)) * pesoDaMochila);
    somatorioParte2 += parteSuperiorFracaoFinal / parteInferiorFracaoFinal; //Da ultima casa ate a origem

    resultado -= (aluguel * somatorioParte2);
    return resultado;
}

double fObj(const vector<Mochileiro>& ladroes, const vector<Item>& itens, const vector<Casa>& cidade,
    const vector<vector<int> >& distCasas)
{

    double resultado = 0;

    for (int i = 0; i < ladroes.size(); i++) // Para cada mochileiro...
        resultado += fObj(ladroes, itens, cidade, distCasas, i);

    return resultado;
}

void contIter()
{

    if (nItem < 1000) 
        nIteracoes = 100;
    else if (nItem < 2900) 
        nIteracoes = 5;
    else if (nItem < 10000) 
        nIteracoes = 3;
    else 
        nIteracoes = 1;
}

#endif