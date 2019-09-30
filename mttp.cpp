#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

struct Mochileiro{
    vector<int> caminho;
    vector<vector<int> > mochila;
    int pesoMochila;
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
};

int dist(Casa a, Casa b){
    return ceil( ((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y)) );
}
  
void imprimir( vector<Mochileiro>&ladroes){
    
    for(int i=0;i<ladroes.size();i++){
        
        cout << "O Mochileiro " << i+1 << " passou nas cidades:\n";
        
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

void leitura(string &instancia, string &tipo, int &dimensao,int &nItem, int &capacidade, 
             double &vMin, double &vMax, double &precoAluguel, string &tipoDistancia){
  
    string lixo;    
    cin >> lixo >> lixo >> instancia;   
    cin >> lixo >> lixo >> lixo >> tipo;
    cin >> lixo >> dimensao;
    cin >> lixo >> lixo >> lixo >> nItem;
    cin >> lixo >> lixo >> lixo >> capacidade;
    cin >> lixo >> lixo >> vMin;	
    cin >> lixo >> lixo >> vMax; 
    cin >> lixo >> lixo >> precoAluguel;
    cin >> lixo >> tipoDistancia;

    cin.ignore();
    getline(cin,lixo);
}
  
void leitura2(vector<Casa> &cidade,int dimensao){
    
    int aux;
    
    for(int i=0;i<dimensao;i++){
        cin >> aux; 
		cidade[i].idCasa = aux-1;
        cin >> cidade[i].x;
      	cin >> cidade[i].y;
	}
}

void leitura3(vector<Item> &itens,int nItem){
  
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
    }
}

void calculaDistCasas(const vector<Casa> &cidade, vector<vector<int> > &distCasas,int dimensao){
    for(int i=0;i<dimensao;i++){
        for(int j=i;j<dimensao;j++){
            int repetirSimetria=dist(cidade[i],cidade[j]);
            distCasas[i][j]=repetirSimetria;
            distCasas[j][i]=repetirSimetria;
        }
    }    
}

double fObj(const vector<Mochileiro> &ladroes, const vector<Item> &itens, const vector<Casa> &cidade, 
            double aluguel, const vector<vector<int> > &distCasas, double vMin, double vMax, int W){ 
    
    /*W_xi = Peso ao sair da cidade W_xi:
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

int main( int argc, char** argv ){

	string instancia,tipo, tipoDistancia;
	
	int dimensao, nItem, capacidade, nMochileiros;
	double vMin, vMax, aluguel;
  
    nMochileiros = atoi(argv[1]);
	
	vector<Mochileiro> ladroes(nMochileiros);
	
  	leitura(instancia,tipo,dimensao,nItem,capacidade,vMin,vMax,aluguel,tipoDistancia);
  	
  	vector<Casa> cidade(dimensao);
 	leitura2(cidade, dimensao);
  
  	vector<Item> itens(nItem);
  	leitura3(itens, nItem);
  	
    prenche(cidade, itens);
  	
  	vector<vector<int> > distCasas(dimensao,vector<int>(dimensao,0));
  	
  	calculaDistCasas(cidade,distCasas,dimensao);
  	
}

