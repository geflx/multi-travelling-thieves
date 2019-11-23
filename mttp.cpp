#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

// Variaveis globais recebidas do arquivos de leitura 
// São utilizadas em varias partes do codigo por isto estão aqui

string instancia,tipo, tipoDistancia;
    
int dimensao, nItem, capacidade, nMochileiros;
double vMin, vMax, aluguel;
long double v; //Constante da função Objetivo

int nIteracoes;

time_t start;

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
};

// Guarda as informacoes referente a cada Item que pode ser roubado
struct Item{
  
    int index;  // Indice do item
    int lucro;  // Valor daquele determinado item
    int peso;   // Peso daquele item
    int ondeTo; // Em qual cidade este item se encontra 
};

// Guarda as informacoes referente a cada Cidade incluindo os que estão nela
struct Casa{
    int idCasa; // Indice da casa
    long double x; // Posição do indice x no plano cartesiano 
    long double y; // Posição do indice y no plano cartesiano
    vector<Item> itemCasa; // Todos os itens que estão na casa 
    vector<bool> visited; // Verifica se o item daquela cidade já foi roubado por algum ladrão
};

bool temTempo(){
	if(time(NULL) - start > 5){
		return false;
	}
	return true;
}

int dist(Casa a, Casa b){
    return ceil( sqrt(((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y))) );
}

//Imprime instancia para o visualizador
void imprimeInstancia( const vector<Mochileiro> &ladroes, const vector<Casa> &cidade, ofstream& saida){

    for(int i=0; i<ladroes.size(); i++){

    	if(ladroes[i].caminho.empty()) continue;
        
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

//Debug, retirar antes de enviar
void imprimeInstancia2( const vector<Item> &itens, const vector<Mochileiro> &ladroes, const vector<Casa> &cidade){

    ofstream saida("saida2.txt");
	
	for(int i=0; i<ladroes.size(); i++){
        
        if(ladroes[i].caminho.size()) continue;

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
                saida<< ladroes[i].mochila[j][k]+1<<"_"<<itens[ ladroes[i].mochila[j][k] ].ondeTo+1 << " ";
                
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
    saida.close();
}

//Usada
void imprimeInstancia3( const vector<Mochileiro> &ladroes, const vector<Casa> &cidade, const int qualGRASP, 
	const double valorFOBJ){

	cout << "\nInstancia: " << instancia << "\n" << "Tipo: " << tipo << "\nnItens:" << nItem << endl;
	cout << "Número de mochileiros: " << ladroes.size() << endl;
	cout << "GRASP utilizada: " << qualGRASP << endl;
	cout << "Valor da fOBJ: " << valorFOBJ << endl;
    

    for(int i=0; i<ladroes.size(); i++){

    	if(ladroes[i].caminho.empty()) continue;
        
        cout<<"[";
        
        for(int j=0; j<ladroes[i].caminho.size(); j++){
            
            //Imprime as cidades que o ladrao "i" passou
            cout<< ladroes[i].caminho[j]+1;
            
            if( j!= ladroes[i].caminho.size()-1 ){
                cout<<",";
            }
            
        }
        
        cout<<"]\n";
        cout<<"["; 
        
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
                cout<< ladroes[i].mochila[j][k]+1;
                
                if(k!=ladroes[i].mochila[j].size()-1)
                    cout<<",";
            }
            
            if( virgulas>0 ){ //Se ainda posso imprimir uma virgula
                virgulas--;
                cout<<",";
            }       
        }
        cout<<"]\n";
    }
}

void imprimir(vector<Mochileiro>&ladroes){

    cout << "Instancia : " << instancia << "\n"; 
    for(int i=0;i<ladroes.size();i++){
        
        if(! ladroes[i].caminho.size()){
            cout << "\nO Mochileiro " << i+1 << " pegou nenhum item \n\n";
        }
        else{
            cout << "\nO Mochileiro " << i+1 << " pegou ";
            cout << ladroes[i].caminho.size()-1  << " itens e passou nas cidades: \n";
        
            for(int j=0; j<ladroes[i].caminho.size()-1 ;j++){
                cout << '[' << ladroes[i].caminho[j]+1 << "] ";
            }

            cout << '[' << ladroes[i].caminho[ ladroes[i].caminho.size()-1 ]+1 << "] ";
        
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
    const vector<vector<int> > &distCasas, int i){ 
    
    /*Funcao objetivo considera que o caminho de cada mochileiro possui IMPLICITAMENTE a saida da Origem
    e a chegada na Origem. Ex: Um caminho 0->1->2->0 sera representado no vector caminho por {1,2} */
    
    double resultado=0;
    int pesoDaMochila=0;
    int W = capacidade;

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
    const vector<vector<int> > &distCasas){ 
    
    double resultado=0;
    
    for(int i=0;i<ladroes.size();i++) // Para cada mochileiro...
        resultado+= fObj(ladroes,itens,cidade,distCasas,i);
    
    return resultado; 
}

//Olha se uma cidade aparece duas vezes na rota de um mochileiro
bool consertaRota(vector<Mochileiro> &ladroes, int k){

    bool resposta = false;

    for(int i=0;i<ladroes[k].caminho.size();i++){ // Para cada cidade daquela caminho
        for(int j=i+1;j<ladroes[k].caminho.size();j++){ // Ve se a cidade se repete na sua frente
            if(ladroes[k].caminho[i] == ladroes[k].caminho[j]){ // Se repetir ela faz swap com a proxima cidade 
                                                            // até deixar ela na ultima posicao , depois faz um pop_back
                for(int l=j;l<ladroes[k].caminho.size()-1;l++)
                    swap(ladroes[k].caminho[l],ladroes[k].caminho[l+1]);

               	resposta = true;
                
                ladroes[k].caminho.pop_back();
                j--;
            }
        }
    }

    if(!resposta)
    	return false;
    else
    	return true; 
}

bool removeItem( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    /*Encontra o melhor item removido para cada mochileiro
      Estrategia BEST IMPROVEMENT */
    bool melhoreiAlgo = false;

    for(int i=0; i< ladroes.size(); i++){

        pair<int,int> bItemCidade;
        double antigaFObj = fObj( ladroes, itens, cidade, distCasas, i);
        double atualFObj= antigaFObj;


        for(int j=0; j<ladroes[i].caminho.size(); j++){ 

            /* Para cada ladrao, tentaremos tirar cada item um por vez de sua mochila e verificaremos
                se isto ajuda no lucro da função objetivo (devido ao peso). */

            int qualCidade = ladroes[i].caminho[j];
            int numItensPossuidos= ladroes[i].mochila[ qualCidade ].size();

            if( numItensPossuidos > 1){ //So tira item de rota que carrega mais de um

            	vector<int> backupItensCidade= ladroes[i].mochila[qualCidade];
                for(int k=0; k< numItensPossuidos; k++){
                    /* Removendo um item da mochila */
                    int removi= ladroes[i].mochila[ qualCidade ][ k ];
                    swap( ladroes[i].mochila[ qualCidade ][ k ], ladroes[i].mochila[ qualCidade ][ numItensPossuidos-1]);
                    ladroes[i].mochila[ qualCidade ].pop_back();

                    /*Calculando se foi bom ou nao a remocao do item*/
                    double tempFObj = fObj( ladroes, itens, cidade, distCasas, i);

                    if(tempFObj > atualFObj){
                        melhoreiAlgo = true;
                        bItemCidade = make_pair( removi, qualCidade );
                        atualFObj = tempFObj;
                    }
                    ladroes[i].mochila[qualCidade] = backupItensCidade;
                }    
            }
     
            /* Se a funcao objetivo melhorou, entao iremos remover o item da mochila e tambem
                marca-lo como nao visitado no vetor de casas*/
            if(atualFObj > antigaFObj){

                for(int k=0; k<ladroes[i].mochila[ bItemCidade.second ].size(); k++){
                    if( ladroes[i].mochila[ bItemCidade.second ][k] == bItemCidade.first){ 
                        
                        swap( ladroes[i].mochila[ bItemCidade.second ][k], 
                            ladroes[i].mochila[ bItemCidade.second ][ ladroes[i].mochila[ bItemCidade.second ].size()-1] );
                        
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
                return true;
            }
        }
    }
    return false;
}

bool addItemCidadeRota( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
    vector<vector<int>> &distCasas){

    //FIRST IMPROVEMENT.

    int totalPeso=0;
    for(int i=0;i<ladroes.size();i++){
        totalPeso += ladroes[i].pesoMochila;
    }

     bool melhoreiAlgo = false;
    for(int i=0; i< ladroes.size(); i++){

        double antigaFObj = fObj( ladroes, itens, cidade, distCasas, i);


        for(int j=0; j<ladroes[i].caminho.size(); j++){ 

            /* Para cada ladrao, tentaremos adicionar itens disponiveis nas cidades.*/

            int qualCidade = ladroes[i].caminho[j];

            for(int z=0;z < cidade[ qualCidade ].visited.size() ; z++){

                int capDaMochila = (totalPeso + cidade[ qualCidade ].itemCasa[z].peso);
                if( cidade[qualCidade].visited[z]==false && capacidade >= capDaMochila ){
                    //Item disponivel!

                    ladroes[i].mochila[qualCidade].push_back(  cidade[ qualCidade ].itemCasa[ z ].index);
                    cidade[ qualCidade ].visited[z] = true;
					ladroes[i].pesoMochila += cidade[ qualCidade ].itemCasa[z].peso;

                    double tempFObj = fObj( ladroes, itens, cidade, distCasas, i);

                    if(tempFObj > antigaFObj){

                        return true;

                    }else{
                    	cidade[ qualCidade ].visited[z] = false;
						ladroes[i].pesoMochila -= cidade[ qualCidade ].itemCasa[z].peso;
                        ladroes[i].mochila[qualCidade].pop_back();
                        //Nao melhorou, remove o item inserido.
                    }
                }
            }
        }
    }
    return false;
}

bool moveUmaCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes,
	vector<vector<int>> &distCasas,int qualMochileiro, int verticeCaminha){

    int melhorVertice = -1;
    
    double melhorFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);

    bool melhorou = false;
    bool subiu;
    
    //Move a cidade para uma posicao a frente dela na solução 
    for(int i=verticeCaminha;i<ladroes[qualMochileiro].caminho.size()-1;i++){
        
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
        
        if( atualFObj > melhorFObj){
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
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
        
        if( atualFObj > melhorFObj){
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

bool moveUmaCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
	vector<vector<int>> &distCasas){
    
    int cont = 0;
    for(int i=0;i<nMochileiros;i++){
        if(ladroes[i].caminho.size() > 1){
            for(int j=0;j<ladroes[i].caminho.size()-2;j++){
               if(moveUmaCidade(cidade,itens, ladroes, distCasas,i,j)) cont++;
            }
        }
    }
    return cont;
}

bool moveUmaCidade2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
	vector<vector<int>> &distCasas, int qualMochileiro, int verticeCaminha, double &melhorFObj){

    int melhorVertice = -1;
    
    bool melhorou = false;
    bool subiu;
    
    //Move a cidade para uma posicao a frente dela na solução 
    for(int i=verticeCaminha;i<ladroes[qualMochileiro].caminho.size()-1;i++){
        
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
        
        if( atualFObj > melhorFObj){
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
    
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
        
        if( atualFObj > melhorFObj){
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

bool addItemCidadeNaoRota( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
    vector<vector<int>> &distCasas){

    //FIRST IMPROVEMENT.

    int totalPeso=0;
    for(int i=0;i<ladroes.size();i++){
        totalPeso += ladroes[i].pesoMochila;
    }

    for(int i=0; i< ladroes.size(); i++){

        double antigaFObj = fObj( ladroes, itens, cidade, distCasas, i);

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
                vector<int> caminhoAntigo = ladroes[i].caminho;
                ladroes[i].caminho.push_back( qualCidade );

                for(int z=0;z < cidade[ qualCidade ].visited.size() ; z++){

                    if( !cidade[ qualCidade ].visited[z] && 
                        capacidade >= (totalPeso + cidade[ qualCidade ].itemCasa[z].peso) ){

                        int qualItem = cidade[qualCidade].itemCasa[z].index;
                        ladroes[i].mochila[ qualCidade ].push_back( qualItem);

                        ladroes[i].pesoMochila+=cidade[ qualCidade ].itemCasa[z].peso;
                        cidade[ qualCidade ].visited[z] = true;


                        double antigoFObj = antigaFObj;

						moveUmaCidade2(cidade,itens,ladroes,distCasas,i,ladroes[i].caminho.size()-1,antigoFObj);

                        double novoObj= fObj( ladroes, itens, cidade, distCasas,i );
           
                        if( novoObj > antigaFObj){
                            return true;

                        }else{
                            //Desfazendo
                            ladroes[i].pesoMochila-=cidade[ qualCidade ].itemCasa[z].peso;
                        	cidade[ qualCidade ].visited[z] = false;
                            ladroes[i].mochila[ qualCidade ].pop_back();
                        }
                    }
                }
               ladroes[i].caminho = caminhoAntigo;
               //Antes era pop back
            }
        }
    }
    return false;
}

bool trocaCidadeEntreLadroes( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes,
    vector<vector<int>> &distCasas){

    //FIRST IMPROVEMENT.

    if(ladroes.size()==1)
    	return false;

    vector<unordered_set<int>>  jafui( ladroes.size());
    for(int i=0;i<ladroes.size();i++){
    	for(int j=0;j<ladroes[i].caminho.size();j++){

    		jafui[i].insert( ladroes[i].caminho[j]);
    	}
    }

    for(int i=0; i<ladroes.size()-1;i++){

        double funcaoEsquerda= fObj( ladroes, itens, cidade, distCasas, i);

    	for(int j=i+1; j<ladroes.size(); j++){

        	double funcaoDireita= fObj( ladroes, itens, cidade, distCasas, j);

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


        			double novaFEsq= fObj( ladroes, itens, cidade, distCasas, i);
        			double novaFDir= fObj( ladroes, itens, cidade, distCasas, j);

        			if( (funcaoEsquerda+funcaoDireita) < (novaFEsq+novaFDir)  ){    					
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

bool trocaCidadeRepetidaEntreLadroes( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
    vector<vector<int>> &distCasas){

    //FIRST IMPROVEMENT.

    if(ladroes.size()==1)
    	return false;

    int rep=0;

    for(int i=0; i<ladroes.size()-1;i++){

        double funcaoEsquerda= fObj( ladroes, itens, cidade, distCasas, i);

    	for(int j=i+1; j<ladroes.size(); j++){

        	double funcaoDireita= fObj( ladroes, itens, cidade, distCasas, j);

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

	    			double novaFEsq= fObj( ladroes, itens, cidade, distCasas, i);
        			double novaFDir= fObj( ladroes, itens, cidade, distCasas, j);

        			if( (funcaoEsquerda+funcaoDireita) < (novaFEsq+novaFDir)  ){
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

	    			novaFEsq= fObj( ladroes, itens, cidade, distCasas, i);
        			novaFDir= fObj( ladroes, itens, cidade, distCasas, j);

        			if( (funcaoEsquerda+funcaoDireita) < (novaFEsq+novaFDir)  ){
						return true;

        			}else{
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

bool trocaDuasCidades(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
	vector<vector<int>> distCasas, int qualMochileiro){

    int melhorVertice = -1;
    int melhorVertice2 = -1;
    double melhorFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);

    bool melhorou = false;

    for(int i=0;i<ladroes[qualMochileiro].caminho.size()-1;i++){
        swap(ladroes[qualMochileiro].caminho[i],ladroes[qualMochileiro].caminho[i+1]);
        
        double atualFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
        
        if( atualFObj > melhorFObj){
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

bool trocaDuasCidades(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
    vector<vector<int>> &distCasas){
    
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

    double melhorFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);

    int qualCidade =-1;
    int posicaoCidade =-1;
    
    vector<int> itensCidade;
    vector<int> itensCid;
    
    bool melhorou = false;

    for(int i=0;i<ladroes[qualMochileiro].caminho.size();i++){

        int qualCid = ladroes[qualMochileiro].caminho[i];
        int posicaoCid = i;
        
        itensCid = ladroes[qualMochileiro].mochila[qualCid];

        auto it = ladroes[qualMochileiro].caminho.begin()+i;

        ladroes[qualMochileiro].caminho.erase(it);
        
        ladroes[qualMochileiro].mochila[qualCid].clear();

        long double atualFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);

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

        double melhorFObj2 = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);

        return true;
    }
    return false;
} 

bool removeCidade(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
	vector<vector<int>> &distCasas){
    
    int cont = 0;

    for(int i=0;i<nMochileiros;i++){

        if(ladroes[i].caminho.size() > 0)
            if(removeCidade(cidade,itens, ladroes, distCasas,i)) cont++;
    }
    return cont;
}

double greedy1(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
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
                        
                        custoBeneficio.push(make_pair(cb,make_pair(j,k))); 
                    }
                }
            }
        }

        iteracao++;
        
        if(!custoBeneficio.empty()){
            int qualVouPegar = rand()%randow;

            while(qualVouPegar-- && custoBeneficio.size()>1)
                custoBeneficio.pop();

            auto escolhido = custoBeneficio.top();
            
            if(escolhido.first >0){
                coloqueiAlguem = true;
                
                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[ city ].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                cidade[city].visited[ escolhido.second.second ] = true;
            }
        }
    }
}

double greedy1(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int randow){

    // Defina a capacidade da mochila de cada ladrão 
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
        greedy1(cidade,itens,ladroes,distCasas,i,cap,randow);
        consertaRota(ladroes,i); // Conserta Rota 
    }

    return fObj(ladroes, itens, cidade, distCasas);
}

void Greedy1(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;

    cidadeOtima = cidade;
    ladroesOtima = ladroes;

    long double melhorFObj = numeric_limits<double>::lowest();

    for(int i=0;i<nIteracoes;i++){
    
    	if(!temTempo()){
	    	cidade = cidadeOtima;
    		ladroes = ladroesOtima;
    		return;
	    }
	    vector<Casa> cidadeClone;

        vector<Mochileiro> ladroesClone;
        
        cidadeClone = cidade;
        ladroesClone = ladroes;

        greedy1(cidadeClone,itens,ladroesClone,distCasas,i+1);

        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);
        
        if( atualFObj > melhorFObj){
            
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
                
            melhorFObj = atualFObj;
        }
    }
    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}

double greedy2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
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

                        custoBeneficio.push(make_pair(cb,make_pair(j,k))); 
                    }
                }
            }
        }

        iteracao++;
        
        if(!custoBeneficio.empty()){
            int qualVouPegar = rand()%randow;

            while(qualVouPegar-- && custoBeneficio.size()>1)
                custoBeneficio.pop();

            auto escolhido = custoBeneficio.top();
            
            if(iteracao < 5 || escolhido.first >0){
                coloqueiAlguem = true;

                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[ city ].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                cidade[city].visited[ escolhido.second.second ] = true;
            }
        }
    }
}

double greedy2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int randow){

    // Defina a capacidade da mochila de cada ladrão 
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
        greedy2(cidade,itens,ladroes,distCasas,i,cap,randow);
        consertaRota(ladroes,i); // Conserta Rota 
    }

    return fObj(ladroes, itens, cidade, distCasas);
}

void Greedy2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;

    cidadeOtima = cidade;
    ladroesOtima = ladroes;

    long double melhorFObj = numeric_limits<double>::lowest();

    for(int i=0;i<nIteracoes;i++){

    	if(!temTempo()){
	    	cidade = cidadeOtima;
    		ladroes = ladroesOtima;
    		return;
	    }

        vector<Casa> cidadeClone;

        vector<Mochileiro> ladroesClone;
        
        cidadeClone = cidade;
        ladroesClone = ladroes;
            
        greedy2(cidadeClone,itens,ladroesClone,distCasas,i+1);

        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);

        if( atualFObj > melhorFObj){
            
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
            
            melhorFObj = atualFObj;
        }
    }
    
    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}

double greedy3(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int qualMochileiro, int cap, int randow){

    int iteracao = 0;
    bool coloqueiAlguem = true;
    
    double melhorFObj = numeric_limits<double>::lowest();

    while(coloqueiAlguem){
       
        priority_queue<pair<double,pair<int,int>>> custoBeneficio; //Custo beneficio dos itens
       	
       	coloqueiAlguem = false;
       
        if( !iteracao){
  			
            iteracao++;
  			
			for(int j=1;j<cidade.size();j++){ //Ignoramos a origem pois nao possui itens
                
                for(int k=0;k<cidade[j].itemCasa.size();k++){ //Para todos os itens daquela cidade
                    int pesoItem = cidade[j].itemCasa[k].peso;  

                    if(!cidade[j].visited[k] && pesoItem <= cap-ladroes[qualMochileiro].pesoMochila){
                        
                        ladroes[qualMochileiro].caminho.push_back(j);
                        ladroes[qualMochileiro].mochila[ j ].push_back(k);
                    
                        double atualfObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
						
						custoBeneficio.push(make_pair(atualfObj,make_pair(j,k))); 
                        
                        ladroes[qualMochileiro].caminho.pop_back();
                        ladroes[qualMochileiro].mochila[ j ].pop_back();
                    }
                }
            }
        }
        else{
            iteracao++;
            
            for(int j=1;j<cidade.size();j++){ //Ignoramos a origem pois nao possui itens
                
                for(int k=0;k<cidade[j].itemCasa.size();k++){ //Para todos os itens daquela cidade
                    int pesoItem = cidade[j].itemCasa[k].peso;  

                    if(!cidade[j].visited[k] && pesoItem <= cap-ladroes[qualMochileiro].pesoMochila){
                        
                        ladroes[qualMochileiro].caminho.push_back(j);
                        ladroes[qualMochileiro].mochila[ j ].push_back(k);

                        bool apagueiCidade = consertaRota(ladroes,qualMochileiro); // Conserta Rota 
                
                        double atualfObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);
						
						if(atualfObj >= melhorFObj*0.9 )
                        	custoBeneficio.push(make_pair(atualfObj,make_pair(j,k))); 

                        if(!apagueiCidade){
                        	ladroes[qualMochileiro].caminho.pop_back();
                        }

                        ladroes[qualMochileiro].mochila[ j ].pop_back();
                    }
                }
            }
        }

        int qualVouPegar = rand()%randow;

	    while(qualVouPegar-- && custoBeneficio.size()>1)
            custoBeneficio.pop();
        
        
        if(!custoBeneficio.empty()){
            auto escolhido = custoBeneficio.top();

            if(iteracao < 10 || escolhido.first >0){
                coloqueiAlguem = true;
               
                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[ city ].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                cidade[city].visited[ escolhido.second.second ] = true;

                melhorFObj = escolhido.first;

                consertaRota(ladroes,qualMochileiro); // Conserta Rota 
            }
        }
    }
}

double greedy3(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int randow){

    // Defina a capacidade da mochila de cada ladrão 
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
        greedy3(cidade,itens,ladroes,distCasas,i,cap,randow);
    }

    return fObj(ladroes, itens, cidade, distCasas);
}

void Greedy3(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;

    cidadeOtima = cidade;
    ladroesOtima = ladroes;

    long double melhorFObj = numeric_limits<double>::lowest();

    for(int i=0;i<nIteracoes;i++){

        if(!temTempo()){
	    	cidade = cidadeOtima;
    		ladroes = ladroesOtima;
    		return;
	    }

    	vector<Casa> cidadeClone;
        vector<Mochileiro> ladroesClone;
        
        cidadeClone = cidade;
        ladroesClone = ladroes;

        greedy3(cidadeClone,itens,ladroesClone,distCasas, i+1);
        
        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);
        
        if( atualFObj > melhorFObj){
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
            
            melhorFObj = atualFObj;

        }
    }
    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}

void melhorGreedy(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
	vector<vector<int>> &distCasas){
    
    double melhorFObj = numeric_limits<double>::lowest();
    double atualFObj;

    vector<Casa> cidadeZero;
    vector<Mochileiro> ladroesZero;

    vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;
    
    //Grasp 1
    cidadeZero = cidade;
    ladroesZero = ladroes;

    Greedy1(cidadeZero,itens,ladroesZero,distCasas);
    
    atualFObj = fObj(ladroesZero, itens, cidadeZero, distCasas);
    
    if(atualFObj > melhorFObj){
        
        melhorFObj = atualFObj;
        cidadeOtima = cidadeZero;
        ladroesOtima = ladroesZero;
    
    }

    if(!temTempo()){
    	return;
    }

    //Grasp 2
    cidadeZero = cidade;
    ladroesZero = ladroes;

    Greedy2(cidadeZero,itens,ladroesZero,distCasas);
    
    atualFObj = fObj(ladroesZero, itens, cidadeZero, distCasas);
    
    if(atualFObj > melhorFObj){
        melhorFObj = atualFObj;
        cidadeOtima = cidadeZero;
        ladroesOtima = ladroesZero;
    
    }

    if(!temTempo())
    	return;

    //Grasp 3
    cidadeZero = cidade;
    ladroesZero = ladroes;

    Greedy3(cidadeZero,itens,ladroesZero,distCasas);
    
    atualFObj = fObj(ladroesZero, itens, cidadeZero, distCasas);
  
    if(atualFObj > melhorFObj){
        
    	melhorFObj = atualFObj;
    	cidadeOtima = cidadeZero;
    	ladroesOtima = ladroesZero; 
    }

    if(!temTempo())
    	return;

    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}

void VND(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    long double valorFOBJ;
    int cont=0;
    valorFOBJ= fObj( ladroes, itens, cidade, distCasas);
    
    int cheguei = 0;
    while(true){
    	
    	if(!temTempo()){
    		return;
    	}

        while(trocaDuasCidades(cidade,itens,ladroes,distCasas));

        if(moveUmaCidade(cidade,itens,ladroes,distCasas)) continue;
        
        if(removeItem(cidade,itens,ladroes,distCasas)) continue;

        if(removeCidade(cidade,itens,ladroes,distCasas)) continue;

        if(addItemCidadeRota(cidade,itens,ladroes,distCasas)) continue;

        if(addItemCidadeNaoRota(cidade,itens,ladroes,distCasas)) continue;
        
        if(trocaCidadeEntreLadroes(cidade,itens,ladroes,distCasas)) continue;
        
        if(trocaCidadeRepetidaEntreLadroes(cidade,itens,ladroes,distCasas)) continue;

        break;
    }
    
    valorFOBJ = fObj( ladroes, itens, cidade, distCasas);    
}

void ILS(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas, 
	bool reaproveitaSolucao){

    double bestRouboValor;

    if( !reaproveitaSolucao){

        //Entao uso grasp!!!
        for(int i=0;i<cidade.size();i++){
            for(bool j: cidade[i].visited){
                j=false;
            }
        }
        melhorGreedy(cidade,itens,ladroes,distCasas);

        VND(cidade,itens,ladroes,distCasas);
    }

    bestRouboValor = fObj(ladroes, itens, cidade, distCasas);

    imprimeInstancia3(ladroes,cidade,0,bestRouboValor);
		
    vector<Mochileiro> bestRoubo = ladroes;
    vector<Casa> bestCidade = cidade;   
    
    int nbiter=10;

    if(dimensao>3000){
        nbiter= 10;
    }else if(dimensao>2000){
        nbiter= 20;
    }else if( dimensao>1000){
        nbiter= 50;
    }else{
        nbiter=100;
    }

    while(nbiter--){

        if(!temTempo()){
            ladroes = bestRoubo;
            cidade = bestCidade;   
        }
        for(int i=0;i<ladroes.size();i++){

            //CASO ESPECIAL: Nao mexe com ladrao que passa so em uma cidade
            if( ladroes[i].caminho.size() == 1)
                continue;

            priority_queue<pair<double,int>> goodshit;

            int sizeCaminho = ladroes[i].caminho.size();

            for(int j=0;j < sizeCaminho ; j++){

                double lucroCidade = 0;
                int qualCidade = ladroes[i].caminho[j];
                
                for(int k=0;k < ladroes[i].mochila[ qualCidade].size(); k++){
                    int cima = itens[ladroes[i].mochila[qualCidade][k]].lucro;
                    int baixo = itens[ladroes[i].mochila[qualCidade][k]].peso;
                    lucroCidade += cima/baixo;
                }

                int pesoDistancia = 0;
                if( j==0 || j==(sizeCaminho-1) ){
                    pesoDistancia+= distCasas[ 0][ qualCidade+1];
                }else{
                    pesoDistancia+= distCasas[ ladroes[i].caminho[j-1]+1][ qualCidade+1];
                }

                pesoDistancia/=10;

                int fatorDecisivo = lucroCidade - pesoDistancia;
                goodshit.push( { (-1)*fatorDecisivo, qualCidade }); 
            }   

            int qtoBaguncar = ceil(sizeCaminho * 0.5);

            for(int j=0; j<qtoBaguncar; j++){

                int qualCidade = goodshit.top().second;
                int posiCidade = -1;
                for(int i1=0; i1< ladroes[i].caminho.size();i1++){
                    if( ladroes[i].caminho[i1] == qualCidade){
                        posiCidade = i1;
                        break;
                    }
                }
                if( posiCidade == -1 ) continue; //Evitar erros

                int randomTiraCidade = rand()%100;

                if( randomTiraCidade <40){ //40% de chance de tirar a cidade

                    for(int u=posiCidade; u<ladroes[i].caminho.size()-1;u++ ){
                        ladroes[i].caminho[u]=ladroes[i].caminho[u+1];
                    }
                    ladroes[i].caminho.pop_back();

                    unordered_set<int> itemDaCidade;
                    for(int u=0; u<ladroes[i].mochila[qualCidade].size();u++){
                        int item = ladroes[i].mochila[qualCidade][u];
                        itemDaCidade.insert( item );
                    }
                    for(int u=0; u< cidade[ qualCidade ].visited.size();u++){
                        if( itemDaCidade.find(cidade[ qualCidade].itemCasa[u].index)!=itemDaCidade.end()){
                            cidade[ qualCidade ].visited[u] = false; //Tira da mochila
                            ladroes[i].pesoMochila-= itens[cidade[ qualCidade].itemCasa[u].index ].peso;
                        }
                    }


                    ladroes[i].mochila[qualCidade].clear();

                }else{
                    //Caso especial: Somente um item na cidade!!
                    // Nao vou tratar por enquanto

                    unordered_set<int> removidos;

                    vector<int> novaMochila;
                    novaMochila.reserve( ladroes[i].mochila[qualCidade].size());

                    for(int u=0; u<ladroes[i].mochila[qualCidade].size(); u++ ){

                        int randomTiraItem = rand()%100;

                        if(randomTiraItem<60){
                            //Vou remover
                            removidos.insert( ladroes[i].mochila[qualCidade][u]);
                        }else{
                            //Nao vou remover
                            novaMochila.push_back( ladroes[i].mochila[qualCidade][u]);
                        }

                    }
                    // cout<<"Escolhi quais serao removidos.\n";

                    if( novaMochila.size() == 0){ //Nao remove a cidade que tem so um item
                        goodshit.pop();
                        continue;
                    }

                    if( removidos.size()!=0){
                        ladroes[ i ].mochila[qualCidade] = novaMochila;

                        for(int z=0; z<cidade[qualCidade].visited.size(); z++){
                            int item = cidade[qualCidade].itemCasa[z].index;

                            if( removidos.find( item )!= removidos.end()){
                                
                                ladroes[i].pesoMochila-= itens[item ].peso;
                                cidade[qualCidade].visited[z] = false;
                            }
                        }
                        // cout<<"Removi os itens a serem removidos\n.";
                    }
                    
                }
                goodshit.pop();
            }
        }

        // cout<<"Antes de chamar a VND: "<<temp<<endl;

        VND(cidade,itens,ladroes,distCasas);
        // cout<<"depois de chamar a VND: "<<temp2<<endl;

        double novaObj = fObj(ladroes, itens, cidade, distCasas);
        
        // cout<<novaObj<<endl;
        if( novaObj > bestRouboValor){

            // cout<<"ILS melhorou "<<novaObj-bestRouboValor<<endl;
            bestRouboValor = novaObj;
            bestRoubo = ladroes;
            bestCidade = cidade;
        }
    }

    ladroes = bestRoubo;
    cidade = bestCidade;

    return;
}

void GRASP(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
	int qualGRASP){

	vector<Casa> cidadeOtima;
    vector<Mochileiro> ladroesOtima;

    cidadeOtima = cidade;
    ladroesOtima = ladroes;

    long double melhorFObj = numeric_limits<double>::lowest();

    int cont =1;

    while(true){	
	    if(!temTempo()){
	    	cidade = cidadeOtima;
    		ladroes = ladroesOtima;
    		return;
	    }

    	vector<Casa> cidadeClone;
        vector<Mochileiro> ladroesClone;
        
        cidadeClone = cidade;
        ladroesClone = ladroes;
        
        cont++;

		if(qualGRASP == 0)
			greedy1(cidadeClone,itens,ladroesClone,distCasas, cont);
		else if(qualGRASP == 1)
			greedy2(cidadeClone,itens,ladroesClone,distCasas, cont);
		else if(qualGRASP ==2)
			greedy3(cidadeClone,itens,ladroesClone,distCasas, cont);	

		VND(cidadeClone,itens,ladroesClone,distCasas);
  
        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);
       
        if( atualFObj > melhorFObj){
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
            melhorFObj = atualFObj;
        }
    }
}

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> &distCasas, int i, ofstream& saida, int esc,
	int qualGRASP){
        
    nMochileiros =i;

    vector<Mochileiro> ladroes(nMochileiros);

	//-----------GRASP----------------
    if(esc == 0){
    	GRASP(cidade,itens,ladroes,distCasas,qualGRASP);
	    double grr = fObj(ladroes, itens, cidade, distCasas);  
	    imprimeInstancia3(ladroes,cidade,qualGRASP,grr);
	    imprimeInstancia(ladroes,cidade,saida);
	   	//imprimir(ladroes);
		//cout<<"\nNumeroLadroes (" << i << ")  temos GRASP : "<< grr << "\n";
	}
	//-----------melhorGreedy----------------
    if(esc == 1){
    	melhorGreedy(cidade,itens,ladroes,distCasas);
	    double grr = fObj(ladroes, itens, cidade, distCasas);  
	    
	    imprimeInstancia3(ladroes,cidade,0,grr);
	    
	    //imprimeInstancia(ladroes,cidade,saida);
	   	//imprimir(ladroes);
		//cout<<"\nNumeroLadroes (" << i << ")  temos GRASP : "<< grr << "\n";
	}
	
	//-----------melhorGreedy + VND----------------
    if(esc ==2){
    	
    	melhorGreedy(cidade,itens,ladroes,distCasas);
	    VND(cidade,itens,ladroes,distCasas);
  
		double atual = fObj( ladroes, itens, cidade, distCasas);
		
		imprimeInstancia(ladroes,cidade,saida);
    	imprimir(ladroes);
    	
    	cout<<"\nNumeroLadroes (" << i << ")  temos VND: "<< atual << "\n";
    }

    //-----------(melhorGreedy + VND) + ILS ----------------
    if(esc == 3){
		vector<Mochileiro> bestVndIls;

		melhorGreedy(cidade,itens,ladroes,distCasas);

	    VND(cidade,itens,ladroes,distCasas);
    	
    	bestVndIls = ladroes;
        vector<Casa> bestCidade = cidade;

        double res_mix= fObj(ladroes, itens, cidade, distCasas);

        while( temTempo()){
	    	ILS(cidade,itens,ladroes,distCasas, true);
			double newobj = fObj(ladroes, itens, cidade, distCasas);

		    if( newobj > res_mix){
                bestCidade = cidade;
		    	bestVndIls = ladroes;
		    	res_mix = newobj;
		    }
	    }

        cidade = bestCidade;
	    ladroes = bestVndIls;

	    imprimeInstancia3(ladroes,cidade,qualGRASP,res_mix);
	    imprimeInstancia(ladroes,cidade,saida);
	   	    
	    /*imprimir(bestVndIls);
	    imprimeInstancia(bestVndIls,cidade,saida);
	    imprimeInstancia2(itens,bestVndIls,cidade);
	    */
	    //cout<<"\nNumeroLadroes (" << i << ")  temos GRASP->VND->ILS : "<< res_mix << "\n";
    }

    //-----------melhorGreedy + ILS ----------------
    if(esc == 4){
    	
    	melhorGreedy(cidade,itens,ladroes,distCasas);
	    
	    double res_ils=-1;
	    vector<Mochileiro> bestGrasp;
	    
	    for(int i=0;i<30;i++){
		    vector<Mochileiro> ladroes(nMochileiros);
		    
		    ILS(cidade,itens,ladroes,distCasas, false);
		    
		    double newobj = fObj(ladroes, itens, cidade, distCasas); 
		    
		    if(newobj > res_ils){
		    	res_ils = newobj;
		    	bestGrasp = ladroes;
		    }	
	    }     
	    imprimir(bestGrasp);
	    imprimeInstancia(bestGrasp,cidade,saida);
	    cout<<"\nNumeroLadroes (" << i << ")  temos GRASP->ILS : "<< res_ils << "\n";
   }	
}

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> &distCasas, ofstream& saida, int esc){

	if(esc ==0){ //GRASP
		for(int j=0;j<=2;j++){ // Para cada GRASP
		    for(int i=1;i<=5;i++){ // Para cada Mochileiro
	    	    start = time(NULL);
				mttp(cidade,itens,distCasas,i,saida,esc,j);
				limpeza(cidade);
	    		saida << "\n";
	    	}
	    }   
	}else{ //ILS
		for(int i=1;i<=5;i++){ // Para cada Mochileiro
			start = time(NULL);
			mttp(cidade,itens,distCasas,i,saida,esc,0);
			limpeza(cidade);
    		saida << "\n";
    	}
	}
}

void contIter(){

    if(nItem < 1000){
        nIteracoes = 100;
    }else if(nItem < 5000) {
        nIteracoes =  60;
    }else if(nItem < 10000){
        nIteracoes = 40;
    }else{
        nIteracoes = 40;
    }
}

int main( int argc, char** argv ){

    srand(time(NULL));

    leitura();
    
    vector<Casa> cidade(dimensao);
    leitura2(cidade);
  
    vector<Item> itens(nItem);
    leitura3(itens);

    prenche(cidade, itens); 

    vector<vector<int> > distCasas(dimensao,vector<int>(dimensao,0));
    
    calculaDistCasas(cidade,distCasas);
    
    ofstream saida("saida.txt");

    v = (vMax-vMin) / capacidade*(1.0); //Constante da função Objetivo
    
    //Primeiro argumento: Número de Mochileiros
    //Segundo argumento: Heuristica escolhida
    //Terceiro argumento: GRASP escolhida

    start = time(NULL);

    contIter();
				
    if(atoi(argv[1]) == 6 )
        mttp(cidade, itens, distCasas,saida,atoi(argv[2]));
    else
        mttp(cidade, itens, distCasas, atoi(argv[1]),saida,atoi(argv[2]),atoi(argv[3]));
    
    saida.close();
}  