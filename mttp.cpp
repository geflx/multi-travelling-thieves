#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

string instancia,tipo, tipoDistancia;
	
int dimensao, nItem, capacidade, nMochileiros;
double vMin, vMax, aluguel;
  
struct Mochileiro{
    vector<int> caminho;
    vector< vector<int> > mochila; 
    int pesoMochila=0;
};

struct Item{
    int index;
    int lucro;
    int peso;
    int ondeTo; 
};

struct Casa{
    int idCasa;
    long double x;
    long double y;
    vector<Item> itemCasa;
    vector<bool> visited;
};

int dist(Casa a, Casa b){
    return ceil( ((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y)) );
}
 
void imprimir( vector<Mochileiro>&ladroes, string &instancia){

    cout << "Instancia : " << instancia << endl; 
    for(int i=0;i<ladroes.size();i++){
        
        cout << "O Mochileiro " << i+1 << " passou nas cidade:\n";
        
        for(int j=0; j<ladroes[i].caminho.size()-1 ;j++)
            cout << '[' << ladroes[i].caminho[j] << "] ";
        
        cout << '[' << ladroes[i].caminho[ ladroes[i].caminho.size()-1 ] << "] ";
        
        cout << " E pegou os itens:\n";
        
        for(int j=0;j<ladroes[i].mochila.size();j++){
            
            for(int k=0; k< ladroes[i].mochila[j].size()-1 ;k++)
                cout << '[' << ladroes[i].mochila[j][k] << "] ";
            
            cout << '[' << ladroes[i].caminho[ ladroes[i].mochila[j].size()-1 ] << "]   ";
        }
        
        cout << "\n";
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
             const vector<vector<int> > &distCasas, int W){ 
    
    /*W_xi = Peso ao sair da cidade W_xi
    Pode ser calculado percorrendo o percurso de cada ladrao no vetor ladroes.*/
    double resultado=0;
    
    for(int i=0;i<ladroes.size();i++){ // Para cada mochileiro...
    
        int pesoDaMochila=0;
        
        //Calculo da primeira parte da funcao objetivo para o ladrao em questao: Lucro!
        
        double somatorioParte2=0;
        for(int j=0;j<ladroes[i].caminho.size();j++){
            
            if(j<=ladroes[i].caminho.size()-1){
                double parteSuperiorFracao = distCasas[ladroes[i].caminho[j]][ladroes[i].caminho[j+1]];
                double parteInferiorFracao= vMax- ( ( ( vMax-vMin ) / W ) * pesoDaMochila );
                somatorioParte2+= parteSuperiorFracao/parteInferiorFracao ;
            }
            
            for(int k=0;k<ladroes[i].mochila[j].size();k++){
                
                int qualItem = ladroes[i].mochila[j][k];
                
                resultado+= itens[qualItem].lucro;
                pesoDaMochila+= itens[qualItem].peso;
                
            }
            
        }
        double parteSuperiorFracaoFinal= distCasas[ladroes[i].caminho[ladroes[i].caminho.size()-1]][0];
        double parteInferiorFracaoFinal= vMax- (( ( vMax-vMin ) / W ) * pesoDaMochila );
        somatorioParte2+= parteSuperiorFracaoFinal/parteInferiorFracaoFinal; //Da ultima casa ate a origem
        
        resultado+= (-1)*aluguel*somatorioParte2;
    }
    return resultado;
}


void roubo(int qualMochileiro, vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, 
				int cap, vector<pair<double,pair<int,int > > > &custoBeneficio, vector<vector<int>> &distCasas){
    
    int contIteracao=0;
    bool coloqueiAlguem= true;
   
    while(coloqueiAlguem){
        
        coloqueiAlguem = false;
    
        if(contIteracao==0){ 
            priority_queue< pair< double, pair<int,int> > > cB;
            
            for(int j=1;j<cidade.size();j++){
                double maiorCb=-1;
                for(int k=0;k<cidade[j].itemCasa.size();k++){
                
                    if(!cidade[j].visited[k] && cidade[j].itemCasa[k].peso <= cap-ladroes[qualMochileiro].pesoMochila ){
                        cB.push(make_pair(custoBeneficio[k].first/( distCasas[0][custoBeneficio[k].second.first]+1 ), 
                        		make_pair(custoBeneficio[k].second.first, custoBeneficio[k].second.second)));
                    }
                }
            }
            
            if(!cB.empty()){
                
                pair< double,pair<int,int> > escolhido = cB.top();
                
                ladroes[qualMochileiro].caminho.push_back(escolhido.second.first);
                ladroes[qualMochileiro].mochila[escolhido.second.first].push_back( escolhido.second.second );
                
                cidade[ escolhido.second.first ].visited[ escolhido.second.second ] = true;
                coloqueiAlguem = true;
            }            
            contIteracao++;
        
        }
        else{
            // Continuar para quando não estamos na origem
        	priority_queue< pair< double, pair<int,int> > > cB;
            
            for(int j=1;j<cidade.size();j++){
                double maiorCb=-1;
                for(int k=0;k<cidade[j].itemCasa.size();k++){
                
                    if(!cidade[j].visited[k] && cidade[j].itemCasa[k].peso <= cap-ladroes[qualMochileiro].pesoMochila ){
                    	
                    	int aux = ladroes[qualMochileiro].caminho[ladroes[qualMochileiro].caminho.size()-1];
                        
                        cB.push(make_pair( custoBeneficio[k].first/( distCasas[aux][custoBeneficio[k].second.first]+1 ),
                        		make_pair(custoBeneficio[k].second.first, custoBeneficio[k].second.second)));
                    }
                }
            }
            
            if(!cB.empty()){
                
                pair< double,pair<int,int> > escolhido = cB.top();
                
                ladroes[qualMochileiro].caminho.push_back(escolhido.second.first);
                ladroes[qualMochileiro].mochila[escolhido.second.first].push_back( escolhido.second.second );
                cidade[ escolhido.second.first ].visited[ escolhido.second.second ] = true;
                coloqueiAlguem = true;
            }            
            contIteracao++;
        }
    }
}

double greedyOne(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas ){
    
    int cap = capacidade/(ladroes.size());
    
    for(int i=0;i<ladroes.size();i++){ //Para cada ladrao...
    
        vector<pair<double,pair<int,int>>> custoBeneficio;
    
        for(int j=0;j<cidade.size();j++){
            
            double maiorCb=-1;
            
            for(int k=0;k<cidade[j].itemCasa.size();k++){
                
                if(!cidade[j].visited[k]){
                    
                    double calculoCb= (cidade[j].itemCasa[k].lucro) / (cidade[j].itemCasa[k].peso) ;
                    custoBeneficio.push_back(make_pair( -1* maiorCb,make_pair( j,k ))); //Gambiarra: CB.second.first == cidade , .second= item
                     
                }
            }
        }
        
        roubo(i,cidade,itens,ladroes, cap, custoBeneficio, distCasas);
        return fObj(ladroes, itens, cidade, distCasas, capacidade);
    }
}

double greedyTwo(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes){
}

double greedyThree(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes){
}


double greedy( vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, string &tipo, double aluguel,
                int capacidade, vector<vector<int>> &distCasas){
    
    if(tipo == "bounded strongly corr"){
        return greedyOne(cidade, itens, ladroes, distCasas);
    }
    else if(tipo == "uncorrelated"){ //Depois colocar Two
        return greedyOne(cidade, itens, ladroes, distCasas);
    }
    else if(tipo == "uncorrelated, similar weights"){ // Depois colocar Three
        return greedyOne(cidade, itens, ladroes, distCasas);
    }
}



int main( int argc, char** argv ){

    nMochileiros = atoi(argv[1]);
	
	vector<Mochileiro> ladroes(nMochileiros);
	
	for(int i=0;i<nMochileiros;i++){
		ladroes[i].mochila.resize(dimensao); //Lista de adj pras cidade
	}

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

  	//imprimir(ladroes, instancia);
  	
}

