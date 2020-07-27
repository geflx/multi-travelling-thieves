#ifndef HEURISTICS_H
#define HEURISTICS_H

void greedy1(vector<Casa>& cidade, vector<Item>& itens, vector<Mochileiro>& ladroes, vector<vector<int> >& distCasas,
    int qualMochileiro, int cap, int randow)
{

    int iteracao = 0;
    bool coloqueiAlguem = true;

    while (coloqueiAlguem) {

        priority_queue<pair<double, pair<int, int> > > custoBeneficio; //Custo beneficio dos itens

        coloqueiAlguem = false;

        for (int j = 1; j < cidade.size(); j++) { //Ignoramos a origem pois nao possui itens

            long double lucro, tempo, gastos, cb;

            if (!iteracao) {

                for (int k = 0; k < cidade[j].itemCasa.size(); k++) { //Para todos os itens daquela cidade

                    int pesoItem = cidade[j].itemCasa[k].peso;

                    if (!cidade[j].visited[k] && pesoItem <= cap - ladroes[qualMochileiro].pesoMochila) {


                        lucro = cidade[j].itemCasa[k].lucro;
                        tempo = distCasas[0][j] / (vMax - v * ladroes[qualMochileiro].pesoMochila);
                        gastos = aluguel * tempo;
                        cb = lucro - gastos;

                        custoBeneficio.push(make_pair(cb, make_pair(j, k)));
                    }
                }

            }
            else {

                for (int k = 0; k < cidade[j].itemCasa.size(); k++) { //Para todos os itens daquela cidade
                    
                    int pesoItem = cidade[j].itemCasa[k].peso;

                    if (!cidade[j].visited[k] && pesoItem <= cap - ladroes[qualMochileiro].pesoMochila) {

                        int qualCidade = ladroes[qualMochileiro].caminho[ladroes[qualMochileiro].caminho.size() - 1];
                        
                        lucro = cidade[j].itemCasa[k].lucro;
                        tempo = distCasas[qualCidade][j] / (vMax - v * ladroes[qualMochileiro].pesoMochila);
                        gastos = aluguel * tempo;
                        cb = lucro - gastos;

                        custoBeneficio.push(make_pair(cb, make_pair(j, k)));
                    }
                }
            }
        }

        iteracao++;

        if (!custoBeneficio.empty()) {

            int qualVouPegar = rand() % randow;

            while (qualVouPegar-- && custoBeneficio.size() > 1)
                custoBeneficio.pop();

            auto escolhido = custoBeneficio.top();

            if (escolhido.first > 0) {
                coloqueiAlguem = true;

                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[city].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                cidade[city].visited[escolhido.second.second] = true;
            }
        }
    }
}

double greedy1(vector<Casa>& cidade, vector<Item>& itens, vector<Mochileiro>& ladroes, vector<vector<int> >& distCasas,
    int randow)
{

    // Defina a capacidade da mochila de cada ladrão
    int cap = capacidade / (ladroes.size());

    for (int i = 0; i < ladroes.size(); i++) { //Para cada ladrao...

        // ! Caution: checar se melhorou "randow % 25 + 5" ou somente "randow".
        greedy1(cidade, itens, ladroes, distCasas, i, cap, randow % 25 + 5); 
        consertaRota(ladroes, i); // Conserta Rota
    }

    return fObj(ladroes, itens, cidade, distCasas);
}

void Greedy1(vector<Casa>& cidade, vector<Item>& itens, vector<Mochileiro>& ladroes, vector<vector<int> >& distCasas)
{

    vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;

    cidadeOtima = cidade;
    ladroesOtima = ladroes;

    long double melhorFObj = numeric_limits<double>::lowest();

    for (int i = 0; i < nIteracoes; i++) {

        if (!temTempo()) {
            cidade = cidadeOtima;
            ladroes = ladroesOtima;
            return;
        }
        vector<Casa> cidadeClone;
        vector<Mochileiro> ladroesClone;

        cidadeClone = cidade;
        ladroesClone = ladroes;

        greedy1(cidadeClone, itens, ladroesClone, distCasas, i + 1);

        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);

        if (atualFObj > melhorFObj) {

            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone;
            melhorFObj = atualFObj;
        }
    }

    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}

void ILS(vector<Casa>& cidade, vector<Item>& itens, vector<Mochileiro>& ladroes, vector<vector<int> >& distCasas)
{

    // Criando solução inicial.
    for (int i = 0; i < cidade.size(); i++) {
        for (bool j : cidade[i].visited) {
            j = false;
        }
    }
    Greedy1(cidade, itens, ladroes, distCasas);
    
    vector<Mochileiro> bestRoubo = ladroes;
    vector<Casa> bestCidade = cidade;

    double bestRouboValor = fObj(bestRoubo, itens, cidade, distCasas);

    int nbiter = 10;

    if (dimensao > 3000) 
        nbiter = 10;
    else if (dimensao > 2000) 
        nbiter = 20;
    else if (dimensao > 1000) 
        nbiter = 50;
    else 
        nbiter = 100;
    

    while (nbiter--) {

        if (!temTempo()) {
            ladroes = bestRoubo;
            cidade = bestCidade;
        }

        for (int i = 0; i < ladroes.size(); i++) {

            //CASO ESPECIAL: Nao mexe com ladrao que passa so em uma cidade
            if (ladroes[i].caminho.size() == 1)
                continue;

            priority_queue<pair<double, int> > goodshit;

            int sizeCaminho = ladroes[i].caminho.size();
            vector<int> caminhoLadrao(sizeCaminho);

            //Random cities will be removed
            for (int j = 0; j < sizeCaminho; j++) {
                caminhoLadrao[j] = ladroes[i].caminho[j];
            }

            random_shuffle(caminhoLadrao.begin(), caminhoLadrao.end());

            int qtoBaguncar = ceil(sizeCaminho * 0.5);

            for (int j = 0; j < qtoBaguncar; j++) {

                int qualCidade = caminhoLadrao[j];
                int posiCidade = -1;
                for (int i1 = 0; i1 < ladroes[i].caminho.size(); i1++) {
                    if (ladroes[i].caminho[i1] == qualCidade) {
                        posiCidade = i1;
                        break;
                    }
                }
                if (posiCidade == -1)
                    continue; //Evitar erros

                int randomTiraCidade = rand() % 100;

                if (randomTiraCidade < 40) { //40% de chance de tirar a cidade

                    for (int u = posiCidade; u < ladroes[i].caminho.size() - 1; u++) {
                        ladroes[i].caminho[u] = ladroes[i].caminho[u + 1];
                    }
                    ladroes[i].caminho.pop_back();

                    unordered_set<int> itemDaCidade;
                    for (int u = 0; u < ladroes[i].mochila[qualCidade].size(); u++) {
                        int item = ladroes[i].mochila[qualCidade][u];
                        itemDaCidade.insert(item);
                    }
                    for (int u = 0; u < cidade[qualCidade].visited.size(); u++) {
                        if (itemDaCidade.find(cidade[qualCidade].itemCasa[u].index) != itemDaCidade.end()) {
                            cidade[qualCidade].visited[u] = false; //Tira da mochila
                            ladroes[i].pesoMochila -= itens[cidade[qualCidade].itemCasa[u].index].peso;
                        }
                    }

                    ladroes[i].mochila[qualCidade].clear();
                }
                else {

                    //Caso especial: Somente um item na cidade!!
                    // Nao vou tratar por enquanto

                    unordered_set<int> removidos;

                    vector<int> novaMochila;
                    novaMochila.reserve(ladroes[i].mochila[qualCidade].size());

                    for (int u = 0; u < ladroes[i].mochila[qualCidade].size(); u++) {

                        int randomTiraItem = rand() % 100;

                        if (randomTiraItem < 60) {
                            //Vou remover
                            removidos.insert(ladroes[i].mochila[qualCidade][u]);
                        }
                        else {
                            //Nao vou remover
                            novaMochila.push_back(ladroes[i].mochila[qualCidade][u]);
                        }
                    }

                    if (novaMochila.size() == 0) { //Nao remove a cidade que tem so um item
                        goodshit.pop();
                        continue;
                    }

                    if (removidos.size() != 0) {
                        ladroes[i].mochila[qualCidade] = novaMochila;

                        for (int z = 0; z < cidade[qualCidade].visited.size(); z++) {
                            int item = cidade[qualCidade].itemCasa[z].index;

                            if (removidos.find(item) != removidos.end()) {

                                ladroes[i].pesoMochila -= itens[item].peso;
                                cidade[qualCidade].visited[z] = false;
                            }
                        }
                    }
                }
            }
        }

        VND(cidade, itens, ladroes, distCasas);

        double novaObj = fObj(ladroes, itens, cidade, distCasas);

        if (novaObj > bestRouboValor) {

            bestRouboValor = novaObj;
            bestRoubo = ladroes;
            bestCidade = cidade;
        }
    }

    ladroes = bestRoubo;
    cidade = bestCidade;

    return;
}

void GRASP(vector<Casa>& cidade, vector<Item>& itens, vector<Mochileiro>& ladroes, vector<vector<int> >& distCasas,
    int qualGRASP)
{

    vector<Casa> cidadeOtima = cidade;
    vector<Mochileiro> ladroesOtima = ladroes;

    long double melhorFObj = numeric_limits<double>::lowest();

    int cont = 1;

    while (true) {

        if (!temTempo()) {
            cidade = cidadeOtima;
            ladroes = ladroesOtima;
            return;
        }

        vector<Casa> cidadeClone = cidade;
        vector<Mochileiro> ladroesClone = ladroesClone;

        cont++;

        greedy1(cidadeClone, itens, ladroesClone, distCasas, cont);

        VND(cidadeClone, itens, ladroesClone, distCasas);

        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);

        if (atualFObj > melhorFObj) {
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone;
            melhorFObj = atualFObj;
        }
    }
}

#endif