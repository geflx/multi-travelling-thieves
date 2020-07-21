#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

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

void VND(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    long double valorFOBJ;
    int cont=0;
    
    while(true){
        
        if(!temTempo()){
            return;
        }

        while(trocaDuasCidades(cidade,itens,ladroes,distCasas));

        if(moveUmaCidade(cidade,itens,ladroes,distCasas)) continue;
        
        if(removeCidade(cidade,itens,ladroes,distCasas)) continue;

        if(addItemCidadeRota(cidade,itens,ladroes,distCasas)) continue;

        if(removeItem(cidade,itens,ladroes,distCasas)) continue;

        if(addItemCidadeNaoRota(cidade,itens,ladroes,distCasas)) continue;
        
        if(trocaCidadeEntreLadroes(cidade,itens,ladroes,distCasas)) continue;
        
        if(trocaCidadeRepetidaEntreLadroes(cidade,itens,ladroes,distCasas)) continue;

        break;
    }   
}

#endif