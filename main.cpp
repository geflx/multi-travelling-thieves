#include "Core.h"
#include "LocalSearch.h"
#include "Heuristics.h"

void mttp(vector<Casa> &cidade, vector<Item> &itens, vector<vector<int>> &distCasas, int i, ofstream& saida, int esc,
    int qualGRASP){
        
    nMochileiros =i;

    vector<Mochileiro> ladroes(nMochileiros);

    //-----------GRASP----------------
    if(esc == 0){
        GRASP(cidade,itens,ladroes,distCasas,qualGRASP);
        double grr = fObj(ladroes, itens, cidade, distCasas);  
        imprimeInstancia3(ladroes,cidade,qualGRASP,grr);
    }
    //-----------melhorGreedy----------------
    if(esc == 1){
        melhorGreedy(cidade,itens,ladroes,distCasas);
        
        double atual = fObj( ladroes, itens, cidade, distCasas);
        
        imprimeInstancia3(ladroes,cidade,0,atual);
    }
    
    //-----------melhorGreedy + VND----------------
    if(esc ==2){
        
        melhorGreedy(cidade,itens,ladroes,distCasas);
        
        VND(cidade,itens,ladroes,distCasas);
  
        double atual = fObj( ladroes, itens, cidade, distCasas);
        
        imprimeInstancia3(ladroes,cidade,0,atual);
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
        //for(int j=0;j<=2;j++){ // Para cada GRASP
            for(int i=1;i<=5;i++){ // Para cada Mochileiro

                int qual;

                if(cidade[1].itemCasa.size() == 10)
                    qual = 7;
                else
                    qual = 3;

                string s2(tipo.begin(),tipo.end()-1);

                cerr << "\nInstancia: " << instancia << "_n" << nItem << '_' << s2 << "_0" << qual;
                cerr << "\nNúmero de Mochileiros: " << i << endl;

                start = time(NULL);
                mttp(cidade,itens,distCasas,i,saida,esc,3);
                limpeza(cidade);
                saida << "\n";
        //  }
        }   
    }else{ //ILS
        for(int i=1;i<=5;i++){ // Para cada Mochileiro

            int qual;

            if(cidade[1].itemCasa.size() == 10)
                qual = 7;
            else
                qual = 3;

            string s2(tipo.begin(),tipo.end()-1);

            cerr << "\nInstancia: " << instancia << "_n" << nItem << '_' << s2 << "_0" << qual;
            cerr << "\nNúmero de Mochileiros: " << i << endl;

            start = time(NULL);
            mttp(cidade,itens,distCasas,i,saida,esc,0);
            limpeza(cidade);
            saida << "\n";
        }
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
	
	// Primeira ILS 40 / 60
	// Segunda  ILS 30 / 40			
    
    if(atoi(argv[1]) == 6 )
        mttp(cidade, itens, distCasas,saida,atoi(argv[2]));
    else
        mttp(cidade, itens, distCasas, atoi(argv[1]),saida,atoi(argv[2]),atoi(argv[3]));
    
    saida.close();
}  
