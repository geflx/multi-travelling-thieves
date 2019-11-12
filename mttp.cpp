#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

// Variaveis globais recebidas do arquivos de leitura 
// São utilizadas em varias partes do codigo por isto estão aqui

string instancia,tipo, tipoDistancia;
    
int dimensao, nItem, capacidade, nMochileiros;
double vMin, vMax, aluguel;
long double v; //Constante da função Objetivo

// Guarda as informacoes referente a cada ladrão  
struct Mochileiro{
    vector<int> caminho; // Cidades em que o mochleiro passa 
    vector< vector<int> > mochila; // Contem os itens de cada cidade que o mochileiro passou
    int pesoMochila; // Peso da mochila com todos os itens que ele coletou

    // Construtor:
    Mochileiro(){
        mochila.resize(dimensao); //A primeira casa possui index 1. A origem zero.
        pesoMochila = 0;
    }

    void operator= (Mochileiro a);
};

// Guarda as informacoes referente a cada Item que pode ser roubado
struct Item{
  
    int index;  // Indice do item
    int lucro;  // Valor daquele determinado item
    int peso;   // Peso daquele item
    int ondeTo; // Em qual cidade este item se encontra 

    void operator= (Item a);
};

// Guarda as informacoes referente a cada Cidade incluindo os que estão nela
struct Casa{
    int idCasa; // Indice da casa
    long double x; // Posição do indice x no plano cartesiano 
    long double y; // Posição do indice y no plano cartesiano
    vector<Item> itemCasa; // Todos os itens que estão na casa 
    vector<bool> visited; // Verifica se o item daquela cidade já foi roubado por algum ladrão

    void operator= (Casa a);
};

void Mochileiro::operator= (Mochileiro a){

    caminho.resize( a.caminho.size() );

    caminho = a.caminho;    
    
    mochila.resize( a.mochila.size() );

    for(int i=0;i<mochila.size();i++){
        mochila[i].resize( a.mochila[i].size() );
        mochila[i] =  a.mochila[i];
    }
    pesoMochila = a.pesoMochila;
}

void Item::operator= (Item a){

    index = a.index;
    lucro = a.lucro;
    peso = a.peso;
    ondeTo = a.ondeTo;
}

void Casa::operator= (Casa a){

    idCasa = a.idCasa;
    x = a.x;
    y = a.y;
    itemCasa = a.itemCasa;
    visited = a.visited;
}

int dist(Casa a, Casa b){
    return ceil( sqrt(((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y))) );
}

//Imprime instancia para o visualizador
void imprimeInstancia( const vector<Mochileiro> &ladroes, const vector<Casa> &cidade, ofstream& saida){

    for(int i=0; i<ladroes.size(); i++){
        
        saida<<"[";
        
        for(int j=0; j<ladroes[i].caminho.size(); j++){
            
            //Imprime as cidades que o ladrao "i" passou
            saida<< ladroes[i].caminho[j]+1;
            
            if( j!= ladroes[i].caminho.size()-1 ){
                saida<<",";
            }
            
        }
        
        saida<<"]\n";
        saida<<"["; 
        
        //Contando quantas virgulas teremos que imprimir
        
        int virgulas=0;

        for(int j=0; j< ladroes[i].mochila.size(); j++){
            if(ladroes[i].mochila[j].size()!=0){
                virgulas++;
            }
        }
        virgulas-=1;
        
        for(int j=0; j< ladroes[i].mochila.size(); j++){
            
            if(ladroes[i].mochila[j].size()==0){ // Sem itens roubados pelo ladrao i na cidade j
                continue;
            }
            
            for(int k=0; k< ladroes[i].mochila[j].size(); k++){
                saida<< ladroes[i].mochila[j][k]+1;
                
                if(k!=ladroes[i].mochila[j].size()-1)
                    saida<<",";
            }
            
            if( virgulas>0 ){ //Se ainda posso imprimir uma virgula
                virgulas--;
                saida<<",";
            }       
        }
        saida<<"]\n";
    }
}

void imprimir( vector<Mochileiro>&ladroes, string &instancia){

    cout << "Instancia : " << instancia << "\n"; 
    for(int i=0;i<ladroes.size();i++){
        
        if(! ladroes[i].caminho.size()){
            cout << "\nO Mochileiro " << i+1 << " pegou nenhum item \n\n";
        }
        else{
            cout << "\nO Mochileiro " << i+1 << " pegou " << ladroes[i].caminho.size()-1  << " itens e passou nas cidades: \n";
        
            for(int j=0; j<ladroes[i].caminho.size()-1 ;j++){
                cout << '[' << ladroes[i].caminho[j]+1 << "] ";
            }

            cout << '[' << ladroes[i].caminho[ ladroes[i].caminho.size()-1 ] << "] ";
        
            cout << "\nE pegou os itens:\n";

            for(int j=0;j<ladroes[i].caminho.size();j++){
                
                int qualCasa = ladroes[i].caminho[j];

                for(int k=0; k< ladroes[i].mochila[qualCasa].size();k++)
                    cout << '[' << ladroes[i].mochila[qualCasa][k]+1 << "] ";
                
            }
            cout << "\n\n";
        }
    }
}

void imprimiCasas(vector<Casa> &cidade){
    cout<<"Casas:\n";
    for(int i=0;i<dimensao;i++){
        cout<<"Id casa, x e y:\n";
        cout << cidade[i].idCasa+1 << " , " << cidade[i].x << " " << cidade[i].y << endl;
        cout<<"Id dos itens na casa:\n";
        for(int j=0;j<cidade[i].itemCasa.size();j++){
            cout<<cidade[i].itemCasa[j].index+1<<" ";
        }
        cout << "\n\n";
    }
}

void imprimiItens(vector<Item> &itens){
    cout <<"Itens:\n";
    for(int i=0;i<nItem;i++)
        cout << itens[i].index+1 << " " << itens[i].lucro << " " << itens[i].peso << " " << itens[i].ondeTo << endl;
    cout << endl;
}

void limpeza(vector<Casa> &cidade){

    for(int i=0;i<dimensao;i++)
        for(int j=0;j<cidade[i].visited.size();j++)
                cidade[i].visited[j] = false;
}

void leitura(){
  
    string lixo;    
    cin >> lixo >> lixo >> instancia;   
    cin >> lixo >> lixo >> lixo;
    cin.ignore();
    getline(cin,tipo);
    cin >> lixo >> dimensao;
    cin >> lixo >> lixo >> lixo >> nItem;
    cin >> lixo >> lixo >> lixo >> capacidade;
    cin >> lixo >> lixo >> vMin;    
    cin >> lixo >> lixo >> vMax; 
    cin >> lixo >> lixo >> aluguel;
    cin >> lixo >> tipoDistancia;

    getline(cin,lixo);
    getline(cin,lixo);
}
  
void leitura2(vector<Casa> &cidade){
    
    int aux;
    
    for(int i=0;i<dimensao;i++){
        cin >> aux; 
        cidade[i].idCasa = aux-1;
        cin >> cidade[i].x;
        cin >> cidade[i].y;
    }
}

void leitura3(vector<Item> &itens){
  
    string lixo;

    getline(cin,lixo);
    getline(cin,lixo);
    
    int aux;
    for(int i=0;i<nItem;i++){
        cin >> aux;
        itens[i].index = aux-1;
        cin >> itens[i].lucro;
        cin >> itens[i].peso;
        cin >> aux;
        itens[i].ondeTo = aux-1;
    }
}

void prenche(vector<Casa> &cidade, vector<Item> &itens){
    
    for(int i=0;i<itens.size();i++){
        cidade[ itens[i].ondeTo ].itemCasa.push_back(itens[i]); 
        cidade[ itens[i].ondeTo ].visited.push_back(false); 
    }
}

void calculaDistCasas(const vector<Casa> &cidade, vector<vector<int> > &distCasas){
    
    for(int i=0;i<dimensao;i++){
        for(int j=i;j<dimensao;j++){
            int repetirSimetria=dist(cidade[i],cidade[j]);
            distCasas[i][j]=repetirSimetria;
            distCasas[j][i]=repetirSimetria;
        }
    }    
}

double fObj(const vector<Mochileiro> &ladroes, const vector<Item> &itens, const vector<Casa> &cidade, 
    const vector<vector<int> > &distCasas, int W, int i){ 
    /*Funcao objetivo considera que o caminho de cada mochileiro possui IMPLICITAMENTE a saida da Origem
    e a chegada na Origem. Ex: Um caminho 0->1->2->0 sera representado no vector caminho por {1,2} */
    double resultado=0;
    int pesoDaMochila=0;

    if(!ladroes[i].caminho.size()){ // Se o tamanho da rota for 0, 
        return 0;
    }
        
    //Calculo da primeira parte da funcao objetivo para o ladrao em questao: Lucro!

    double somatorioParte2=0;

    //Da origem ate o primeiro...
    double parteSuperiorFracaoInicial = distCasas[0][ladroes[i].caminho[0]];
    double parteInferiorFracaoInicial = vMax- (( ( vMax-vMin ) / W*(1.0) ) * pesoDaMochila );
    
    somatorioParte2+= parteSuperiorFracaoInicial/parteInferiorFracaoInicial; //Da ultima casa ate a origem

    int tamanhoCaminhoLadrao= ladroes[i].caminho.size();

    for(int j=0;j<tamanhoCaminhoLadrao;j++){

        int qualCidade= ladroes[i].caminho[ j ];

        for(int k=0;k<ladroes[i].mochila[ qualCidade ].size();k++){
            
            int qualItem = ladroes[i].mochila[qualCidade][k];
            
            resultado+= itens[qualItem].lucro;
            pesoDaMochila+= itens[qualItem].peso;
        }

        if(j<tamanhoCaminhoLadrao-1){
           
            double parteSuperiorFracao = distCasas[ladroes[i].caminho[j]][ladroes[i].caminho[j+1]];
            double parteInferiorFracao= vMax- ( ( ( vMax-vMin ) / W*(1.0) ) * pesoDaMochila );
           
            somatorioParte2+= parteSuperiorFracao/parteInferiorFracao ;
        }

        

    }
    /* Casa [ 0 ] e a origem */
    //Do ultimo ate a origem...
    double parteSuperiorFracaoFinal= distCasas[ladroes[i].caminho[tamanhoCaminhoLadrao-1]][0];

    double parteInferiorFracaoFinal= vMax-( ((vMax-vMin )/ W*(1.0)) * pesoDaMochila) ;
    somatorioParte2+= parteSuperiorFracaoFinal/parteInferiorFracaoFinal; //Da ultima casa ate a origem
    

    resultado-= (aluguel*somatorioParte2) ;
    return resultado;
}

double fObj(const vector<Mochileiro> &ladroes, const vector<Item> &itens, const vector<Casa> &cidade, 
    const vector<vector<int> > &distCasas, int W){ 
    
    double resultado=0;
    
    for(int i=0;i<ladroes.size();i++) // Para cada mochileiro...
        resultado+= fObj(ladroes,itens,cidade,distCasas,W,i);
    
    return resultado; 
}

//Olha se uma cidade aparece duas vezes na rota de um mochileiro
void consertaRota(vector<Mochileiro> &ladroes, int k){

    for(int i=0;i<ladroes[k].caminho.size();i++){ // Para cada cidade daquela caminho
        for(int j=i+1;j<ladroes[k].caminho.size();j++){ // Ve se a cidade se repete na sua frente
            if(ladroes[k].caminho[i] == ladroes[k].caminho[j]){ // Se repetir ela faz swap com a proxima cidade 
                                                            // até deixar ela na ultima posicao , depois faz um pop_back
                for(int l=j;l<ladroes[k].caminho.size()-1;l++)
                    swap(ladroes[k].caminho[l],ladroes[k].caminho[l+1]);

                ladroes[k].caminho.pop_back();
                j--;
            }
        }
    }
}

void roubo(int qualMochileiro, vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
    int cap, const vector<pair<double,pair<int,int > > > &custoBeneficio, vector<vector<int>> &distCasas){
    
    int contIteracao=0;
    bool coloqueiAlguem= true;
    int contcB =0;
   
    vector<bool> casaVisitada(cidade.size(), false);
    while(coloqueiAlguem){
        
        priority_queue< pair< double, pair<int,int> > > cB;
        coloqueiAlguem = false;
        
        if(contIteracao==0){ 
            contcB =-1;
            for(int j=1;j<cidade.size();j++){ //Ignora a cidade 0 (origem)

                if(casaVisitada[j]){

                    contcB += cidade[j].itemCasa.size(); //Gambiarra para caminhar o contcB mesmo pulando essa cidade.
                    continue;

                }

                for(int k=0;k<cidade[j].itemCasa.size();k++){  //Olha os  itens de todas cidades
                    
                    contcB++;

                    if(/*!cidade[j].visited[k] &&*/ cidade[j].itemCasa[k].peso <= cap-ladroes[qualMochileiro].pesoMochila ){
                        
                        cB.push(make_pair(custoBeneficio[contcB].first/( 1 /(distCasas[0][custoBeneficio[contcB].second.first]+1) ) /*CB / DIST */, 
                                make_pair(custoBeneficio[contcB].second.first /*ID CIDADE*/, custoBeneficio[contcB].second.second /*ID ITEM ARTIFICIAL. de 0 a numItens*/)));        
                    }                   
                }
            }
            
            if(!cB.empty()){
                
                pair< double,pair<int,int> > escolhida = cB.top();


                int city = escolhida.second.first;
                int item = cidade[city].itemCasa[escolhida.second.second].index;
                
                casaVisitada[ city ] = true;

                ladroes[qualMochileiro].caminho.push_back( city );
                /* ladroes[qualMochileiro].mochila[escolhido.second.first].push_back( escolhido.second.second ); Essa push back ta estranho, deveria
                 dar push back no index do item, como considera a funcao objetivo, nao em um k de 0 ao numItens na Casa gerados no custoBeneficio. */
                
                ladroes[qualMochileiro].mochila[city].push_back( item );

                //cout<<"CidadeEscolhida: "<<city<<". ItemIndex roubado: "<<item<<" durante a iteracao "<<contIteracao<<".\n";
                
                cidade[ city ].visited[ escolhida.second.second ] = true; //VISITED: ID Item artificial
                coloqueiAlguem = true;

                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

            }            
            contIteracao++;

        }else{
            // Continuar para quando não estamos na origem
            contcB =-1;
            
            for(int j=1;j<cidade.size();j++){
                
                if(casaVisitada[j]){

                    contcB += cidade[j].itemCasa.size(); //Gambiarra para caminhar o contcB mesmo pulando essa cidade.
                    continue;

                }
                for(int k=0;k<cidade[j].itemCasa.size();k++){
                    
                    contcB++;
                    
                    if(/*!cidade[j].visited[k] &&*/ cidade[j].itemCasa[k].peso <= cap-ladroes[qualMochileiro].pesoMochila ){
                        
                        int ultimaVitima = ladroes[qualMochileiro].caminho[ladroes[qualMochileiro].caminho.size()-1];
                        
                        // cout<<"Estou na iteracao "<<contIteracao<<"e tenho candidatos a ser roubados.\n";
                        cB.push(make_pair(custoBeneficio[contcB].first/(1/ (distCasas[ultimaVitima][custoBeneficio[contcB].second.first]+1) ), 
                            make_pair(custoBeneficio[contcB].second.first, custoBeneficio[contcB].second.second)));
                   
                    }                   
                }
            }

            if(!cB.empty()){
                
                pair< double,pair<int,int> > escolhido = cB.top();
                
                 //Pega o valor da funcao objetivo sem o item
                long double melhorFObj =fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);
                
                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                // Ve se a cidade ja foi visitada
                bool visiteiAntes = casaVisitada[ city ];
                
                //cout<<"CidadeEscolhida: "<<city<<". ItemIndex roubado: "<<item<<" durante a iteracao "<<contIteracao<<".\n";

                casaVisitada[ city ] = true;

                ladroes[qualMochileiro].caminho.push_back(city);
                
                /* ladroes[qualMochileiro].mochila[escolhido.second.first].push_back( escolhido.second.second ); Essa push back ta estranho, deveria
                 dar push back no index do item, como considera a funcao objetivo, nao em um k de 0 ao numItens na Casa gerados no custoBeneficio. */
                ladroes[qualMochileiro].mochila[ city ].push_back( item ); 

                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                cidade[ city ].visited[ escolhido.second.second ] = true;
                
                coloqueiAlguem = true;
                
                if(contIteracao>3 && fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro) < melhorFObj){
                 coloqueiAlguem = false;
                 cidade[ city ].visited[ escolhido.second.second ] = false;
                 ladroes[qualMochileiro].pesoMochila -= itens[item].peso;
                 ladroes[qualMochileiro].caminho.pop_back();
                 ladroes[qualMochileiro].mochila[ city ].pop_back(); 
                 casaVisitada[ city ] = visiteiAntes;
                }
            }            
            contIteracao++;
        }
    }
    return;
}

double greedyOne(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){
    
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
    
        vector<pair<double,pair<int,int>>> custoBeneficio; //Custo beneficio de todos itens
    
        for(int j=1;j<cidade.size();j++){ //Ignoramos a origem pois nao possui itens
            
            for(int k=0;k<cidade[j].itemCasa.size();k++){                
                double calculoCb = (cidade[j].itemCasa[k].lucro) / ((cidade[j].itemCasa[k].peso)) ;
                // custoBeneficio.push_back(make_pair( -1* calculoCb,make_pair( j, k ))); //Gambiarra: CB.second.first == cidade , .second= item
                custoBeneficio.push_back(make_pair( -1* calculoCb,make_pair( j, k ))); //Gambiarra: CB.second.first == cidade , .second= item

            }
        }
        roubo(i,cidade,itens,ladroes, cap, custoBeneficio, distCasas);
        consertaRota(ladroes,i);
    }
    
    return fObj(ladroes, itens, cidade, distCasas, capacidade);
}


double greedyGrasp(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int qualMochileiro, int cap, int randow){

    int iteracao = 0;
    bool coloqueiAlguem = true;

    while(coloqueiAlguem){

        priority_queue<pair<double,pair<int,int>>> custoBeneficio; //Custo beneficio dos itens
        
        coloqueiAlguem = false;

        for(int j=1;j<cidade.size();j++){ //Ignoramos a origem pois nao possui itens
            if(!iteracao){

                for(int k=0;k<cidade[j].itemCasa.size();k++){ //Para todos os itens daquela cidade
                    int pesoItem = cidade[j].itemCasa[k].peso;  

                    if(!cidade[j].visited[k] && pesoItem <= cap-ladroes[qualMochileiro].pesoMochila){
                        long double lucro = cidade[j].itemCasa[k].lucro;
                        long double tempo = distCasas[0][j]/(vMax- v*ladroes[qualMochileiro].pesoMochila);
                        long double gastos =  aluguel*tempo;
                        long double cb = lucro-gastos;

                        //cout << "Cidade: " << j << " Item: " << k << " Lucro: " << lucro << " Gastos: " << gastos << endl;
                        
                        custoBeneficio.push(make_pair(cb,make_pair(j,k))); 
                    }
                }
            }
            else{
                for(int k=0;k<cidade[j].itemCasa.size();k++){ //Para todos os itens daquela cidade
                    int pesoItem = cidade[j].itemCasa[k].peso;  

                    if(!cidade[j].visited[k] && pesoItem <= cap-ladroes[qualMochileiro].pesoMochila){
                        long double lucro = cidade[j].itemCasa[k].lucro;
                        
                        int qualCidade = ladroes[qualMochileiro].caminho[ ladroes[qualMochileiro].caminho.size()-1 ]; 
                        
                        long double tempo = distCasas[qualCidade][j]/ (vMax- v *ladroes[qualMochileiro].pesoMochila);
                        long double gastos =  aluguel*tempo;
                        long double cb = lucro-gastos;

                        //cout << "Cidade: " << j << " Item: " << k << " Lucro: " << lucro << " Gastos: " << gastos << endl;
                        
                        custoBeneficio.push(make_pair(cb,make_pair(j,k))); 
                    }
                }
            }
        }

        iteracao++;
        
        if(!custoBeneficio.empty()){
            int qualVouPegar = rand()%randow;

            //cout << "Vou pegar o : " << qualVouPegar << "  fila de tamanho: " << custoBeneficio.size() << endl;
            while(qualVouPegar-- && custoBeneficio.size()>1)
                custoBeneficio.pop();

            auto escolhido = custoBeneficio.top();
            
            //cout << escolhido.first << " " <<  escolhido.second.first << " " << escolhido.second.second << endl;
            
            if(escolhido.first >0){
                coloqueiAlguem = true;
                //if(iteracao>2)
                    //cout << "Valor da FObj ate agora: " << fObj(ladroes, itens, cidade, distCasas,capacidade,qualMochileiro) << endl;

                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[ city ].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                //cerr << "Adicionando a cidade: " << city << " e o item: " << item << " com o peso: " << itens[item].peso << endl;

                cidade[city].visited[ escolhido.second.second ] = true;
            }
        }
    }
}

double greedyGrasp(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int randow){

    // Defina a capacidade da mochila de cada ladrão 
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
        greedyGrasp(cidade,itens,ladroes,distCasas,i,cap,randow);
        consertaRota(ladroes,i); // Conserta Rota 
    }

    return fObj(ladroes, itens, cidade, distCasas, capacidade);
}

double greedyGrasp2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int qualMochileiro, int cap, int randow){

    int iteracao = 0;
    bool coloqueiAlguem = true;

    while(coloqueiAlguem){

        priority_queue<pair<double,pair<int,int>>> custoBeneficio; //Custo beneficio dos itens
        
        coloqueiAlguem = false;

        for(int j=1;j<cidade.size();j++){ //Ignoramos a origem pois nao possui itens
            if(!iteracao){

                for(int k=0;k<cidade[j].itemCasa.size();k++){ //Para todos os itens daquela cidade
                    int pesoItem = cidade[j].itemCasa[k].peso;  

                    if(!cidade[j].visited[k] && pesoItem <= cap-ladroes[qualMochileiro].pesoMochila){
                        long double lucro = cidade[j].itemCasa[k].lucro;
                        long double tempo = distCasas[0][j]/(vMax- v*ladroes[qualMochileiro].pesoMochila);
                        long double gastos =  aluguel*tempo;
                        long double cb = lucro-gastos;

                        //cout << "Cidade: " << j << " Item: " << k << " Lucro: " << lucro << " Gastos: " << gastos << endl;
                        
                        custoBeneficio.push(make_pair(cb,make_pair(j,k))); 
                    }
                }
            }
            else{
                for(int k=0;k<cidade[j].itemCasa.size();k++){ //Para todos os itens daquela cidade
                    int pesoItem = cidade[j].itemCasa[k].peso;  

                    if(!cidade[j].visited[k] && pesoItem <= cap-ladroes[qualMochileiro].pesoMochila){
                        long double lucro = cidade[j].itemCasa[k].lucro;
                        
                        int qualCidade = ladroes[qualMochileiro].caminho[ ladroes[qualMochileiro].caminho.size()-1 ]; 
                        
                        long double tempo = distCasas[qualCidade][j]/ (vMax- v *ladroes[qualMochileiro].pesoMochila);
                        long double gastos =  aluguel*tempo;
                        long double cb = lucro-gastos;

                        //cout << "Cidade: " << j << " Item: " << k << " Lucro: " << lucro << " Gastos: " << gastos << endl;
                        
                        custoBeneficio.push(make_pair(cb,make_pair(j,k))); 
                    }
                }
            }
        }

        iteracao++;
        
        if(!custoBeneficio.empty()){
            int qualVouPegar = rand()%randow;

            //cout << "Vou pegar o : " << qualVouPegar << "  fila de tamanho: " << custoBeneficio.size() << endl;
            while(qualVouPegar-- && custoBeneficio.size()>1)
                custoBeneficio.pop();

            auto escolhido = custoBeneficio.top();
            
            //cout << escolhido.first << " " <<  escolhido.second.first << " " << escolhido.second.second << endl;
            
            if(iteracao < 5 || escolhido.first >0){
                coloqueiAlguem = true;
                //if(iteracao>2)
                    //cout << "Valor da FObj ate agora: " << fObj(ladroes, itens, cidade, distCasas,capacidade,qualMochileiro) << endl;

                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[ city ].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                //cerr << "Adicionando a cidade: " << city << " e o item: " << item << " com o peso: " << itens[item].peso << endl;

                cidade[city].visited[ escolhido.second.second ] = true;
            }
        }
    }
}

double greedyGrasp2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int randow){

    // Defina a capacidade da mochila de cada ladrão 
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
        greedyGrasp2(cidade,itens,ladroes,distCasas,i,cap,randow);
        consertaRota(ladroes,i); // Conserta Rota 
    }

    return fObj(ladroes, itens, cidade, distCasas, capacidade);
}

double greedy(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, string &tipo, 
    vector<vector<int>> &distCasas){

    //"bounded strongly corr"
    if(tipo.size() == 22  || tipo.size() == 21){ // os valores dos itens estão fortemente relacionados a seus pesos.
        return greedyGrasp(cidade, itens, ladroes, distCasas,1);
    }
    //"uncorrelated" 
    else if(tipo.size() == 13 || tipo.size() == 12 ){ //os valores dos itens não estão relacionados a seus pesos. 
        return greedyGrasp(cidade, itens, ladroes, distCasas,1);
    }
    //"uncorrelated, similar weights"
    else if(tipo.size() == 30 || tipo.size() == 29 ){ // os valores dos itens não estão relacionados a seus pesos, mas os
        return greedyGrasp(cidade, itens, ladroes, distCasas,1); // pesos de todos os itens são similares 
    }
}

bool removeItem( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas
    , int W){

    /*Encontra o melhor item removido para cada mochileiro
      Estrategia BEST IMPROVEMENT */
    bool melhoreiAlgo = false;
    for(int i=0; i< ladroes.size(); i++){

        pair<int,int> bItemCidade;
        double antigaFObj = fObj( ladroes, itens, cidade, distCasas, W, i);
        double atualFObj= antigaFObj;



        for(int j=0; j<ladroes[i].caminho.size(); j++){ 

            /* Para cada ladrao, tentaremos tirar cada item um por vez de sua mochila e verificaremos
                se isto ajuda no lucro da função objetivo (devido ao peso). */

            int qualCidade = ladroes[i].caminho[j];
            int numItensPossuidos= ladroes[i].mochila[ qualCidade ].size();

            if( numItensPossuidos > 1){ //So tira item de rota que carrega mais de um

                for(int k=0; k< numItensPossuidos; k++){
                    /* Removendo um item da mochila */
                    int removi= ladroes[i].mochila[ qualCidade ][ k ];
                    swap( ladroes[i].mochila[ qualCidade ][ k ], ladroes[i].mochila[ qualCidade ][ numItensPossuidos-1 ] );
                    ladroes[i].mochila[ qualCidade ].pop_back();

                    /*Calculando se foi bom ou nao a remocao do item*/
                    double tempFObj = fObj( ladroes, itens, cidade, distCasas, W, i);

                    if(tempFObj > atualFObj){
                        melhoreiAlgo = true;
                        bItemCidade = make_pair( removi, qualCidade );
                        atualFObj = tempFObj;
                    }
                    ladroes[i].mochila[ qualCidade ].push_back( removi );

                   }    
            }
     

            /* Se a funcao objetivo melhorou, entao iremos remover o item da mochila e tambem
                marca-lo como nao visitado no vetor de casas*/
            if(atualFObj > antigaFObj){

                for(int k=0; k<ladroes[i].mochila[ bItemCidade.second ].size(); k++){
                    if( ladroes[i].mochila[ bItemCidade.second ][k] == bItemCidade.first){
                        swap( ladroes[i].mochila[ bItemCidade.second ][k], ladroes[i].mochila[ bItemCidade.second ][ ladroes[i].mochila[ bItemCidade.second ].size()-1] );
                        ladroes[i].mochila[ bItemCidade.second ].pop_back();
                        break;
                    }
                }

                for(int k=0; k< cidade[ bItemCidade.second ].itemCasa.size();k++){

                    if( cidade[ bItemCidade.second ].itemCasa[k].index == bItemCidade.first ){
                        cidade[ bItemCidade.second ].visited[k]=false;
                        break;
                    }

                }

                ladroes[i].pesoMochila -= itens[ bItemCidade.first].peso;
            }
        }
    }
    if( melhoreiAlgo )
        return true;
    else
        return false;
}

bool addItemCidadeRota( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas
    , int W ){

    //FIRST IMPROVEMENT.

    int totalPeso=0;
    for(int i=0;i<ladroes.size();i++){
        totalPeso += ladroes[i].pesoMochila;
    }

     bool melhoreiAlgo = false;
    for(int i=0; i< ladroes.size(); i++){

        double antigaFObj = fObj( ladroes, itens, cidade, distCasas, W, i);


        for(int j=0; j<ladroes[i].caminho.size(); j++){ 

            /* Para cada ladrao, tentaremos adicionar itens disponiveis nas cidades.*/

            int qualCidade = ladroes[i].caminho[j];

            for(int z=0;z < cidade[ qualCidade ].visited.size() ; z++){

                if( ! cidade[ qualCidade ].visited[z] && capacidade >= (totalPeso + cidade[ qualCidade ].itemCasa[z].peso) ){
                    //Item disponivel!

                    ladroes[i].mochila[qualCidade].push_back(  cidade[ qualCidade ].itemCasa[ z ].index);

                    double tempFObj = fObj( ladroes, itens, cidade, distCasas, W, i);

                    if(tempFObj > antigaFObj){
             
                        ladroes[i].pesoMochila += cidade[ qualCidade ].itemCasa[z].peso;
                        cidade[ qualCidade ].visited[z] = true;

                        return true;

                    }else{
                        ladroes[i].mochila[qualCidade].pop_back();
                        //Nao melhorou, remove o item inserido.
                    }

                }

            }

        }

    }
    return false;
}

bool moveUmaCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int qualMochileiro, int verticeCaminha){

    int melhorVertice = -1;
    
    double melhorFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);

    bool melhorou = false;
    bool subiu;
    
    //Move a cidade para uma posicao a frente dela na solução 
    for(int i=verticeCaminha;i<ladroes[qualMochileiro].caminho.size()-1;i++){
        //cerr << ladroes[qualMochileiro].caminho[i] << " " << ladroes[qualMochileiro].caminho[i+1] << endl; 
        
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);
        
        if( atualFObj > melhorFObj){
            //cout << atualFObj << " " << melhorFObj << endl;
            subiu = false;
            melhorVertice = i+1;
            melhorFObj = atualFObj; 
            melhorou = true; 
        } 
    }
    
    for(int i=ladroes[qualMochileiro].caminho.size()-2;i>=verticeCaminha;i--)
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
    
    //Move a cidade para uma posicao atras dela na solução 
    for(int i=verticeCaminha;i>0;i--){

        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i-1]);
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);
        
        if( atualFObj > melhorFObj){
            //cout << atualFObj << " " << melhorFObj << endl;
            subiu = true;
            melhorVertice = i-1;
            melhorFObj = atualFObj; 
            melhorou = true;

        } 
    }
    
    for(int i=0;i<verticeCaminha;i++)
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
        
    if(melhorou){
        if(!subiu){
            for(int i=verticeCaminha; i<melhorVertice;i++){
                swap(ladroes[qualMochileiro].caminho[i], ladroes[qualMochileiro].caminho[i+1]);
            }
        }else{
            for(int i=verticeCaminha; i>melhorVertice;i--){
                swap(ladroes[qualMochileiro].caminho[i], ladroes[qualMochileiro].caminho[i-1]);
            }
        }
   
        return true;
    }
    return false;
}

bool moveUmaCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){
    
    int cont = 0;
    for(int i=0;i<nMochileiros;i++){
        //cerr << "\nladrao  " << i << endl;
        
        if(ladroes[i].caminho.size() > 1){
            for(int j=0;j<ladroes[i].caminho.size()-2;j++){
               if(moveUmaCidade(cidade,itens, ladroes, distCasas,i,j)) cont++;
            }
        }
    }
    return cont;
}

bool moveUmaCidade2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int qualMochileiro, int verticeCaminha, double &melhorFObj){

    int melhorVertice = -1;
    
    bool melhorou = false;
    bool subiu;
    
    //Move a cidade para uma posicao a frente dela na solução 
    for(int i=verticeCaminha;i<ladroes[qualMochileiro].caminho.size()-1;i++){
        //cerr << ladroes[qualMochileiro].caminho[i] << " " << ladroes[qualMochileiro].caminho[i+1] << endl; 
        
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);
        
        if( atualFObj > melhorFObj){
            //cout << atualFObj << " " << melhorFObj << endl;
            subiu = false;
            melhorVertice = i+1;
            melhorFObj = atualFObj; 
            melhorou = true; 
        } 
    }
    
    for(int i=ladroes[qualMochileiro].caminho.size()-2;i>=verticeCaminha;i--)
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
    
    //Move a cidade para uma posicao atras dela na solução 
    for(int i=verticeCaminha;i>0;i--){

        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i-1]);
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);
        
        if( atualFObj > melhorFObj){
            //cout << atualFObj << " " << melhorFObj << endl;
            subiu = true;
            melhorVertice = i-1;
            melhorFObj = atualFObj; 
            melhorou = true;

        } 
    }
    
    for(int i=0;i<verticeCaminha;i++)
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
        
    if(melhorou){
        if(!subiu){
            for(int i=verticeCaminha; i<melhorVertice;i++){
                swap(ladroes[qualMochileiro].caminho[i], ladroes[qualMochileiro].caminho[i+1]);
            }
        }else{
            for(int i=verticeCaminha; i>melhorVertice;i--){
                swap(ladroes[qualMochileiro].caminho[i], ladroes[qualMochileiro].caminho[i-1]);
            }
        }
   
        return true;
    }
    return false;
}

bool addItemCidadeNaoRota( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas
    , int W ){

    //FIRST IMPROVEMENT.

    int totalPeso=0;
    for(int i=0;i<ladroes.size();i++){
        totalPeso += ladroes[i].pesoMochila;
    }

    for(int i=0; i< ladroes.size(); i++){

        double antigaFObj = fObj( ladroes, itens, cidade, distCasas, W, i);

        unordered_set<int> cidadesVisi;
        for(int city: ladroes[i].caminho){
        	cidadesVisi.insert( city );
        }

        //Olhando as cidades nao visitadas

        for(int k=1; k< cidade.size(); k++){

            int qualCidade = k;

            if( cidadesVisi.find( qualCidade )==cidadesVisi.end()){
                //A cidade ainda nao esta na rota

                //Inserimos a cidade
                ladroes[i].caminho.push_back( qualCidade );

                for(int z=0;z < cidade[ qualCidade ].visited.size() ; z++){

                    if( !cidade[ qualCidade ].visited[z] && 
                        capacidade >= (totalPeso + cidade[ qualCidade ].itemCasa[z].peso) ){

                        int qualItem = cidade[qualCidade].itemCasa[z].index;
                        ladroes[i].mochila[ qualCidade ].push_back( qualItem);

                        double antigoFObj = antigaFObj;

						moveUmaCidade2(cidade,itens,ladroes,distCasas,i,ladroes[i].caminho.size()-1,antigoFObj);

                        double novoObj= fObj( ladroes, itens, cidade, distCasas, capacidade,i );
           
                        if( novoObj > antigaFObj){
                            ladroes[i].pesoMochila+=cidade[ qualCidade ].itemCasa[z].peso;
                            cidade[ qualCidade ].visited[z] = true;

                            // cout<<" MELHOREI "<<novoObj-antigaFObj<<endl;
                            return true;

                        }else{
                            //Desfazendo
                            ladroes[i].mochila[ qualCidade ].pop_back();
                        }
                    }
                }
                ladroes[i].caminho.pop_back();
            }

        }
    }
    return false;
}

bool trocaCidadeEntreLadroes( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas
    , int W ){

    //FIRST IMPROVEMENT.

    if(ladroes.size()==1)
    	return false;

    // cout<<"vizi inteira "<<fObj( ladroes, itens, cidade, distCasas, capacidade)<<endl;

    vector<unordered_set<int>>  jafui( ladroes.size());
    for(int i=0;i<ladroes.size();i++){
    	for(int j=0;j<ladroes[i].caminho.size();j++){

    		jafui[i].insert( ladroes[i].caminho[j]);
    	}
    }

    for(int i=0; i<ladroes.size()-1;i++){

        double funcaoEsquerda= fObj( ladroes, itens, cidade, distCasas, capacidade, i);

    	for(int j=i+1; j<ladroes.size(); j++){

        	double funcaoDireita= fObj( ladroes, itens, cidade, distCasas, capacidade, j);

    		for(int k=0; k<ladroes[i].caminho.size();k++){

    			for(int l=0; l<ladroes[j].caminho.size(); l++){

    				int cidadeEsquerda = ladroes[i].caminho[k];
    				int cidadeDireita = ladroes[j].caminho[l];


    				if( !(jafui[i].find(cidadeDireita) == jafui[i].end() 
    					&& jafui[j].find(cidadeEsquerda) == jafui[j].end()) ) {
    					continue;
    				}
    				vector<int> tempLadraoEsquerda = ladroes[i].mochila[ cidadeEsquerda ];
    				vector<int> tempLadraoDireita = ladroes[j].mochila[ cidadeDireita ];

    				swap(ladroes[i].caminho[k], ladroes[j].caminho[l]);

    				ladroes[i].mochila[ cidadeEsquerda ].clear();
    				ladroes[j].mochila[ cidadeDireita ].clear();

    				ladroes[i].mochila[ cidadeDireita ] = tempLadraoDireita;
    				ladroes[j].mochila[ cidadeEsquerda ] = tempLadraoEsquerda;


        			double novaFEsq= fObj( ladroes, itens, cidade, distCasas, capacidade, i);
        			double novaFDir= fObj( ladroes, itens, cidade, distCasas, capacidade, j);

        			if( (funcaoEsquerda+funcaoDireita) < (novaFEsq+novaFDir)  ){

        				// cout<<"Melhorei Troca: "<< (novaFDir+novaFEsq)-(funcaoEsquerda+funcaoDireita)<<endl;
    					
        				return true;
        			}else{

        				swap(ladroes[i].caminho[k], ladroes[j].caminho[l]);

	    				ladroes[i].mochila[ cidadeDireita ].clear();
	    				ladroes[j].mochila[ cidadeEsquerda ].clear();

	    				ladroes[i].mochila[ cidadeEsquerda ] = tempLadraoEsquerda;
	    				ladroes[j].mochila[ cidadeDireita ] = tempLadraoDireita;
        			}

    			}

    		}
    	}
    }   
    return false;
}

bool trocaCidadeRepetidaEntreLadroes( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas
    , int W ){

    //FIRST IMPROVEMENT.

    if(ladroes.size()==1)
    	return false;

    // cout<<"vizi inteira "<<fObj( ladroes, itens, cidade, distCasas, capacidade)<<endl;

    int rep=0;
    for(int i=0; i<ladroes.size()-1;i++){

        double funcaoEsquerda= fObj( ladroes, itens, cidade, distCasas, capacidade, i);

    	for(int j=i+1; j<ladroes.size(); j++){

        	double funcaoDireita= fObj( ladroes, itens, cidade, distCasas, capacidade, j);

    		for(int k=0; k<ladroes[i].caminho.size();k++){

    			for(int l=0; l<ladroes[j].caminho.size(); l++){

    				int cidadeEsquerda = ladroes[i].caminho[k];
    				int cidadeDireita = ladroes[j].caminho[l];


    				if( cidadeEsquerda!=cidadeDireita ) {

    					//Nao passaram na mesma cidade. Continua.
    					continue;
    				}

    				rep++;
    				int qualCidade = cidadeEsquerda; //ou direita, sao iguais.

    				vector<int> tempLadraoEsquerda = ladroes[i].mochila[ cidadeEsquerda ];
    				vector<int> tempLadraoDireita = ladroes[j].mochila[ cidadeDireita ];

    				vector<int> tempCaminhoEsq = ladroes[i].caminho;
    				vector<int> tempCaminhoDir = ladroes[j].caminho;

    				//Tenta passar pro dar esquerda
    				for(int u=0; u<ladroes[j].mochila[qualCidade].size(); u++){
    					ladroes[i].mochila[qualCidade].push_back( ladroes[j].mochila[qualCidade][u]);
    				}

    				ladroes[j].mochila[ qualCidade ].clear();

    				for(int u=l; u<ladroes[j].caminho.size()-1;u++){
    					ladroes[j].caminho[u] = ladroes[j].caminho[u+1];
    				}
    				ladroes[j].caminho.pop_back();

	    			double novaFEsq= fObj( ladroes, itens, cidade, distCasas, capacidade, i);
        			double novaFDir= fObj( ladroes, itens, cidade, distCasas, capacidade, j);

        			if( (funcaoEsquerda+funcaoDireita) < (novaFEsq+novaFDir)  ){
        				// cout<<"Melhorei "<< (novaFEsq+novaFDir)-(funcaoEsquerda+funcaoDireita)<<" P ESQUERDA\n";

        				return true;

        			}else{

        				ladroes[i].mochila[ qualCidade ]= tempLadraoEsquerda;
        				ladroes[j].mochila[ qualCidade ]= tempLadraoDireita;
        				ladroes[j].caminho = tempCaminhoDir;
        			}

        			//Tenta passar pro dar esquerda
    				for(int u=0; u<ladroes[i].mochila[qualCidade].size(); u++){
    					ladroes[j].mochila[qualCidade].push_back( ladroes[i].mochila[qualCidade][u]);
    				}
    				ladroes[i].mochila[ qualCidade ].clear();

    				for(int u=k; u<ladroes[i].caminho.size()-1;u++){
    					ladroes[i].caminho[u] = ladroes[i].caminho[u+1];
    				}
    				ladroes[i].caminho.pop_back();

	    			novaFEsq= fObj( ladroes, itens, cidade, distCasas, capacidade, i);
        			novaFDir= fObj( ladroes, itens, cidade, distCasas, capacidade, j);

        			if( (funcaoEsquerda+funcaoDireita) < (novaFEsq+novaFDir)  ){

        				// cout<<"Melhorei "<< (novaFEsq+novaFDir)-(funcaoEsquerda+funcaoDireita)<<" P DIREITA\n";
        				return true;

        			}else{
        				// cout<<"nn melhorei\n";
        				ladroes[j].mochila[ qualCidade ]= tempLadraoDireita;
        				ladroes[i].mochila[ qualCidade ]= tempLadraoEsquerda;
        				ladroes[i].caminho = tempCaminhoEsq;
        			}
    			}
    		}
    	}
    }   
  
    return false;
}

bool trocaDuasCidades(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> distCasas,
    int qualMochileiro){

    int melhorVertice = -1;
    int melhorVertice2 = -1;
    double melhorFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);

    bool melhorou = false;

    for(int i=0;i<ladroes[qualMochileiro].caminho.size()-1;i++){
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
        
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);
        
        if( atualFObj > melhorFObj){
           // cout << atualFObj << " " << melhorFObj << endl;
            melhorVertice = i;
            melhorVertice2 = i+1;
            melhorFObj = atualFObj; 
            melhorou = true;
        }
        
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]); 
    }

    if(melhorou){
        swap(ladroes[qualMochileiro].caminho[melhorVertice], ladroes[qualMochileiro].caminho[melhorVertice2]);
        return true;
    }
    return false;
} 

bool trocaDuasCidades(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){
    
    int cont = 0;
    for(int i=0;i<nMochileiros;i++){

        if(ladroes[i].caminho.size() > 1){
            while(trocaDuasCidades(cidade,itens, ladroes, distCasas,i)) cont++;
        }
    }
    return cont;
}

bool removeCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> distCasas,
    int qualMochileiro){

    double melhorFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);

    int qualCidade =-1;
    int posicaoCidade =-1;
    
    vector<int> itensCidade;
    vector<int> itensCid;
    
    bool melhorou = false;

    for(int i=0;i<ladroes[qualMochileiro].caminho.size();i++){

        int qualCid = ladroes[qualMochileiro].caminho[i];
        int posicaoCid = i;

        //cout << qualCid << " " << i << endl;
        
        itensCid = ladroes[qualMochileiro].mochila[qualCid];

        auto it = ladroes[qualMochileiro].caminho.begin()+i;

        ladroes[qualMochileiro].caminho.erase(it);
        
        ladroes[qualMochileiro].mochila[qualCid].clear();

        long double atualFObj = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);

        //cout << atualFObj << " " <<  melhorFObj << endl;
        if(atualFObj > melhorFObj){
            
            melhorFObj = atualFObj;
            itensCidade = itensCid;
            posicaoCidade = posicaoCid;
            qualCidade = qualCid;
        }

        auto it2 = ladroes[qualMochileiro].caminho.begin()+i;

        ladroes[qualMochileiro].caminho.insert(it,qualCid);
        ladroes[qualMochileiro].mochila[qualCid] = itensCid;
    }

    if(melhorou){
        
        auto it = ladroes[qualMochileiro].caminho.begin()+posicaoCidade; 
        
        //Retiro aquela cidade da rota
        ladroes[qualMochileiro].caminho.erase(it);
        
        //Calculo o peso que irá sair da mochla do ladrão
        int diminuiPeso=0;
        for(int i=0;i<ladroes[qualMochileiro].mochila[qualCidade].size();i++){
            diminuiPeso += itens[ ladroes[qualMochileiro].mochila[qualCidade][i] ].peso;
        }

        //Tiro o peso q vai sair da mochila do ladrao
        ladroes[qualMochileiro].pesoMochila-= diminuiPeso;

        //Retiro aqueles itens que o ladrao pegou nessa cidade da rota dele
        ladroes[qualMochileiro].mochila[qualCidade].clear();

        double melhorFObj2 = fObj(ladroes,itens,cidade,distCasas,capacidade,qualMochileiro);

        return true;
    }
    return false;
} 

bool removeCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){
    
    int cont = 0;

    for(int i=0;i<nMochileiros;i++){

        if(ladroes[i].caminho.size() > 0)
            if(removeCidade(cidade,itens, ladroes, distCasas,i)) cont++;
    }
    return cont;
}

void GRASP(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;

    cidadeOtima = cidade;
    ladroesOtima = ladroes;

    long double melhorFObj = 0;

    long double melhorFOne = 0;
    long double melhorFTwo = 0;

    int nIteracoes;

    if(nItem < 1000){
        nIteracoes = dimensao;
    }else if(nItem < 5000) {
        nIteracoes =  60;
    }else if(nItem < 10000){
        nIteracoes = 40;
    }else{
        nIteracoes = 40;
    }
    
    for(int i=0;i<nIteracoes;i++){
        vector<Casa> cidadeClone;

        vector<Mochileiro> ladroesClone;
        
        cidadeClone = cidade;
        ladroesClone = ladroes;
        
        if(nItem < 1000)
            greedyGrasp(cidadeClone,itens,ladroesClone,distCasas,((log10(i+10))/(log10(3))));
        else if(nItem < 5000)
            greedyGrasp(cidadeClone,itens,ladroesClone,distCasas, sqrt(i)+1);
        else if(nItem < 10000)
            greedyGrasp(cidadeClone,itens,ladroesClone,distCasas, i+1 );
        else
            greedyGrasp(cidadeClone,itens,ladroesClone,distCasas, (i+3)/3 );
        
        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas, capacidade);
        
        if( atualFObj > melhorFObj){
            
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
                
            melhorFObj = atualFObj;
        
            melhorFOne = atualFObj;
        }
    }

    for(int i=0;i<nIteracoes;i++){
        vector<Casa> cidadeClone;

        vector<Mochileiro> ladroesClone;
        
        cidadeClone = cidade;
        ladroesClone = ladroes;
        
        if(nItem < 1000)
            greedyGrasp2(cidadeClone,itens,ladroesClone,distCasas,((log10(i+10))/(log10(3))));
        else if(nItem < 5000)
            greedyGrasp2(cidadeClone,itens,ladroesClone,distCasas, sqrt(i)+1);
        else if(nItem < 10000)
            greedyGrasp2(cidadeClone,itens,ladroesClone,distCasas, i+1 );
        else
            greedyGrasp2(cidadeClone,itens,ladroesClone,distCasas, (i+3)/3 );
        
        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas, capacidade);
        

        melhorFTwo = max(atualFObj,melhorFTwo);

        if( atualFObj > melhorFObj){
            
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
            
            melhorFObj = atualFObj;
        }
    }
    cout << "Primeira: " << melhorFOne << " segunda: " << melhorFTwo << endl;

    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}

void VND(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    long double valorFOBJ;
    int cont=0;
    valorFOBJ= fObj( ladroes, itens, cidade, distCasas, capacidade );
    
    cout << "Comeca com: " << fixed << setprecision(2) << valorFOBJ << endl;

    while(true){
    	cout << "Estou com: " << fixed << setprecision(2) << fObj( ladroes, itens, cidade, distCasas, capacidade ) << endl;

        while(trocaDuasCidades(cidade,itens,ladroes,distCasas));
        
        if(moveUmaCidade(cidade,itens,ladroes,distCasas)) continue;
        
        if(removeItem(cidade,itens,ladroes,distCasas,capacidade)) continue;

        if(removeCidade(cidade,itens,ladroes,distCasas)) continue;

        if(addItemCidadeRota(cidade,itens,ladroes,distCasas,capacidade)) continue;

        if(addItemCidadeNaoRota(cidade,itens,ladroes,distCasas,capacidade)) continue;
        
        if(trocaCidadeEntreLadroes(cidade,itens,ladroes,distCasas,capacidade)) continue;
        
        if(trocaCidadeRepetidaEntreLadroes(cidade,itens,ladroes,distCasas,capacidade)) continue;

        break;
    }
    
    valorFOBJ = fObj( ladroes, itens, cidade, distCasas, capacidade );
    
    cout << "Termina com :" << fixed << setprecision(2) << valorFOBJ << "\n\n";
}

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> &distCasas, int i, ofstream& saida){
        
    nMochileiros =i;

    vector<Mochileiro> ladroes(nMochileiros);

    GRASP(cidade,itens,ladroes,distCasas);
    
    double grr = fObj(ladroes, itens, cidade, distCasas, capacidade);  

    cout<<"\nUtilizando " << i << " ladrao(ladroes) \n\nO resultado do Grasp: "<< grr << "\n";

    VND(cidade,itens,ladroes,distCasas);

    double atual = fObj( ladroes, itens, cidade, distCasas, capacidade );

    cout<<"\nUtilizando " << i << " ladrao(ladroes) \n\nO resultado atual: "<< atual << "\n";

    imprimir(ladroes, instancia);

    //cout << "\n\nImprimindo a instancia: \n";
    
    imprimeInstancia(ladroes,cidade,saida);
    
    limpeza(cidade);
}

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> &distCasas, ofstream& saida){

    for(int i=1;i<5;i++){
        mttp(cidade,itens,distCasas,i,saida);
        saida << "\n";
    }
    mttp(cidade,itens,distCasas,5,saida);   
}

int main( int argc, char** argv ){

    srand(time(NULL));

    leitura();
    
    vector<Casa> cidade(dimensao);
    leitura2(cidade);
  
    vector<Item> itens(nItem);
    leitura3(itens);

    prenche(cidade, itens); 
    
    //imprimiCasas(cidade);
    //imprimiItens(itens);
    
    vector<vector<int> > distCasas(dimensao,vector<int>(dimensao,0));
    
    calculaDistCasas(cidade,distCasas);
    
    ofstream saida("saida.txt");

    v = (vMax-vMin) / capacidade*(1.0); //Constante da função Objetivo
    
    if(argc <2 || argc >2 ){
        cout << "Quantidade de argumentos invalida!!" << endl;
        cout << "Coloca a quantidade de mochileiros OTÁRIO" << endl;
    }
    else if(atoi(argv[1]) == 6 )
        mttp(cidade, itens, distCasas,saida);
    else
        mttp(cidade, itens, distCasas, atoi(argv[1]),saida);
    saida.close();
}  