#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

// Variaveis globais recebidas do arquivos de leitura 
// São utilizadas em varias partes do codigo por isto estão aqui

string instancia,tipo, tipoDistancia;
	
int dimensao, nItem, capacidade, nMochileiros;
double vMin, vMax, aluguel;

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
    getline(cin,tipo);
    cin >> lixo >> dimensao;
    cin >> lixo >> lixo >> lixo >> nItem;
    cin >> lixo >> lixo >> lixo >> capacidade;
    cin >> lixo >> lixo >> vMin;	
    cin >> lixo >> lixo >> vMax; 
    cin >> lixo >> lixo >> aluguel;
    cin >> lixo >> tipoDistancia;

    cin.ignore();
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
  
	cin.ignore();
    string lixo;
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
        
    //Calculo da primeira parte da funcao objetivo para o ladrao em questao: Lucro!
    
    double somatorioParte2=0;

    //Da origem ate o primeiro...
    double parteSuperiorFracaoInicial = distCasas[0][ladroes[i].caminho[0]];
    double parteInferiorFracaoInicial = vMax- (( ( vMax-vMin ) / W ) * pesoDaMochila );
    
    somatorioParte2+= parteSuperiorFracaoInicial/parteInferiorFracaoInicial; //Da ultima casa ate a origem

    int tamanhoCaminhoLadrao= ladroes[i].caminho.size();

    for(int j=0;j<tamanhoCaminhoLadrao;j++){

        if(j<tamanhoCaminhoLadrao-1){
           
            double parteSuperiorFracao = distCasas[ladroes[i].caminho[j]][ladroes[i].caminho[j+1]];
            double parteInferiorFracao= vMax- ( ( ( vMax-vMin ) / W*(1.0) ) * pesoDaMochila );
           
            somatorioParte2+= parteSuperiorFracao/parteInferiorFracao ;
        }
        
        for(int k=0;k<ladroes[i].mochila[j].size();k++){
            
            int qualItem = ladroes[i].mochila[j][k];
            
            resultado+= itens[qualItem].lucro;
            pesoDaMochila+= itens[qualItem].peso;
        }

    }
    //Do ultimo ate a origem...
    double parteSuperiorFracaoFinal= distCasas[ladroes[i].caminho[tamanhoCaminhoLadrao-1]][0];

    double parteInferiorFracaoFinal= vMax-( ((vMax-vMin )/ W) * pesoDaMochila) ;
    somatorioParte2+= parteSuperiorFracaoFinal/parteInferiorFracaoFinal; //Da ultima casa ate a origem
    
    resultado-= aluguel*somatorioParte2;
    return resultado;
}

double fObj(const vector<Mochileiro> &ladroes, const vector<Item> &itens, const vector<Casa> &cidade, 
    const vector<vector<int> > &distCasas, int W){ 
    
    double resultado=0;
    
    for(int i=0;i<ladroes.size();i++) // Para cada mochileiro...
        resultado+= fObj(ladroes,itens,cidade,distCasas,W,i);
    
    return resultado; 
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

                    if(!cidade[j].visited[k] && cidade[j].itemCasa[k].peso <= cap-ladroes[qualMochileiro].pesoMochila ){
                        
                        cB.push(make_pair(custoBeneficio[contcB].first/( distCasas[0][custoBeneficio[contcB].second.first]+1 ) /*CB / DIST */, 
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
                	
                    if(!cidade[j].visited[k] && cidade[j].itemCasa[k].peso <= cap-ladroes[qualMochileiro].pesoMochila ){
                    	
                    	int ultimaVitima = ladroes[qualMochileiro].caminho[ladroes[qualMochileiro].caminho.size()-1];
                        
                        // cout<<"Estou na iteracao "<<contIteracao<<"e tenho candidatos a ser roubados.\n";
            	   	    cB.push(make_pair(custoBeneficio[contcB].first/( distCasas[ultimaVitima][custoBeneficio[contcB].second.first]+1 ), 
                        	make_pair(custoBeneficio[contcB].second.first, custoBeneficio[contcB].second.second)));
                   
                    }                	
                }
            }

            if(!cB.empty()){
                
                pair< double,pair<int,int> > escolhido = cB.top();
                
                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                //cout<<"CidadeEscolhida: "<<city<<". ItemIndex roubado: "<<item<<" durante a iteracao "<<contIteracao<<".\n";

                casaVisitada[ city ] = true;

                ladroes[qualMochileiro].caminho.push_back(city);
                
                /* ladroes[qualMochileiro].mochila[escolhido.second.first].push_back( escolhido.second.second ); Essa push back ta estranho, deveria
                 dar push back no index do item, como considera a funcao objetivo, nao em um k de 0 ao numItens na Casa gerados no custoBeneficio. */
                ladroes[qualMochileiro].mochila[ city ].push_back( item ); 


                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                cidade[ city ].visited[ escolhido.second.second ] = true;
                
                coloqueiAlguem = true;
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
                double calculoCb= (cidade[j].itemCasa[k].lucro) / (cidade[j].itemCasa[k].peso) ;
                // custoBeneficio.push_back(make_pair( -1* calculoCb,make_pair( j, k ))); //Gambiarra: CB.second.first == cidade , .second= item
                custoBeneficio.push_back(make_pair( -1* calculoCb,make_pair( j, k ))); //Gambiarra: CB.second.first == cidade , .second= item

            }
        }
        roubo(i,cidade,itens,ladroes, cap, custoBeneficio, distCasas);
    }
    
    return fObj(ladroes, itens, cidade, distCasas, capacidade);
}

double greedyTwo(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes){
}

double greedyThree(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes){
}

double greedy(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, string &tipo, 
	vector<vector<int>> &distCasas){
    
    if(tipo == " bounded strongly corr"){
        return greedyOne(cidade, itens, ladroes, distCasas);
    }
    else if(tipo == " uncorrelated"){ //Depois colocar Two
    	return greedyOne(cidade, itens, ladroes, distCasas);
    }
    else if(tipo == " uncorrelated, similar weights"){ // Depois colocar Three
    	return greedyOne(cidade, itens, ladroes, distCasas);
    }
}

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> distCasas, int i, ofstream& saida){
		
		int nMochileiros =i;

		vector<Mochileiro> ladroes(nMochileiros);

		double greedyResult = greedy(cidade, itens, ladroes, tipo, distCasas);	
  	
	  	cout<<"\nUtilizando " << i << " ladrao(ladroes) \n\nO resultado do greedy: "<< greedyResult << "\n";

	  	imprimir(ladroes, instancia);

	    //cout << "\n\nImprimindo a instancia: \n";
	    
	    imprimeInstancia(ladroes,cidade,saida);
	    
	    limpeza(cidade);
}

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> distCasas, ofstream& saida){

	for(int i=1;i<5;i++){
		mttp(cidade,itens,distCasas,i,saida);
		saida << "\n";
	}

	mttp(cidade,itens,distCasas,5,saida);	
}

int main( int argc, char** argv ){

  	leitura();
  	
  	vector<Casa> cidade(dimensao);
 	leitura2(cidade);
  
  	vector<Item> itens(nItem);
  	leitura3(itens);
	
    prenche(cidade, itens);	
	
  	//imprimiCasas(cidade);
  	//imprimiItens(itens);
	
	//Criando com dimensao+1 posicoes devido ao 0 ser a origem. Comecamos do 1.
    // vector<vector<int> > distCasas(dimensao+1,vector<int>(dimensao+1,0)); Acredito que o +1 nao seja necessario
  	
  	vector<vector<int> > distCasas(dimensao,vector<int>(dimensao,0));
  	
  	calculaDistCasas(cidade,distCasas);

  	ofstream saida("saida.txt");

  	if(atoi(argv[1]) == 6 )
  		mttp(cidade, itens, distCasas,saida);
    else
    	mttp(cidade, itens, distCasas, atoi(argv[1]),saida);

    saida.close();

}  