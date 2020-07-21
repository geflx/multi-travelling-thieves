#ifndef HEURISTICS_H
#define HEURISTICS_H

void greedy1(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
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

void greedy2(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
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

void greedy3(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
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

bool greedy4(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas,
    int qualMochileiro, int cap, int itensPorCidade, int cont){

    int iteracao = 0;
    bool coloqueiAlguem = true;
    bool coloqueiAlguemAgora = false;
    
    double melhorFObj;
    if(cont ==1)
	    melhorFObj = numeric_limits<double>::lowest();
	else
		melhorFObj = fObj(ladroes,itens,cidade,distCasas,qualMochileiro);

    while(coloqueiAlguem || iteracao < itensPorCidade){
       
        priority_queue<pair<double,pair<int,int>>> custoBeneficio; //Custo beneficio dos itens
       	
       	coloqueiAlguem = false;
       
        if(ladroes[qualMochileiro].caminho.empty()){
  			
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
 
        if(!custoBeneficio.empty()){
            auto escolhido = custoBeneficio.top();

            if(iteracao < 10 || escolhido.first >0){
                coloqueiAlguem = true;
               
                int city = escolhido.second.first;
                int item = cidade[city].itemCasa[escolhido.second.second].index;

                ladroes[qualMochileiro].caminho.push_back(city);
                ladroes[qualMochileiro].mochila[ city ].push_back(item);
                ladroes[qualMochileiro].pesoMochila += itens[item].peso;

                coloqueiAlguemAgora = true;

                cidade[city].visited[ escolhido.second.second ] = true;

                melhorFObj = escolhido.first;

                consertaRota(ladroes,qualMochileiro); // Conserta Rota 
            }
        }
    }

    return coloqueiAlguemAgora;
}

double greedy4(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas){

    // Defina a capacidade da mochila de cada ladrão 
    int cap = capacidade/(ladroes.size());
    
    vector<bool> v(ladroes.size(),true);

    bool coloqueiAlguem = true;
    int itensPorCidade = cidade[1].itemCasa.size();
    int cont =0;

    while(coloqueiAlguem){
    	cont++;
    	coloqueiAlguem = false;
    	for(int i=0;i<ladroes.size();i++){
    		if(v[i]){
        		v[i] = greedy4(cidade,itens,ladroes,distCasas,i,cap,itensPorCidade,cont);
        		if(v[i])
        			coloqueiAlguem = true;
    		}
    	}
    }

    return fObj(ladroes, itens, cidade, distCasas);
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

    //Greedy 4
    cidadeZero = cidade;
    ladroesZero = ladroes;

    greedy4(cidadeZero,itens,ladroesZero,distCasas);
    
    atualFObj = fObj(ladroesZero, itens, cidadeZero, distCasas);
  
    if(atualFObj > melhorFObj){
        
    	melhorFObj = atualFObj;
    	cidadeOtima = cidadeZero;
    	ladroesOtima = ladroesZero; 
    }

    cidade = cidadeOtima;
    ladroes = ladroesOtima;
}


void ILS(vector<Casa> &cidade, vector<Item> &itens, vector<Mochileiro> &ladroes, vector<vector<int>> &distCasas, 
    bool reaproveitaSolucao){

    if( !reaproveitaSolucao){

        //Entao uso grasp!!!
        for(int i=0;i<cidade.size();i++){
            for(bool j: cidade[i].visited){
                j=false;
            }
        }
        melhorGreedy(cidade,itens,ladroes,distCasas);
    }
    vector<Mochileiro> bestRoubo = ladroes;
    vector<Casa> bestCidade = cidade;
    
    double bestRouboValor = fObj(bestRoubo, itens, cidade, distCasas);

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
            vector<int> caminhoLadrao( sizeCaminho );

            //Random cities will be removed
            for(int j=0;j < sizeCaminho ; j++){
                caminhoLadrao[ j ] = ladroes[i].caminho[j];

            }   

            random_shuffle( caminhoLadrao.begin(), caminhoLadrao.end());

            int qtoBaguncar = ceil(sizeCaminho * 0.5);

            for(int j=0; j<qtoBaguncar; j++){

                int qualCidade = caminhoLadrao[ j ];
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
                    }
                }
            }
        }

        VND(cidade,itens,ladroes,distCasas);

        double novaObj = fObj(ladroes, itens, cidade, distCasas);
    
        if( novaObj > bestRouboValor){

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
        else if(qualGRASP==3)
            greedy4(cidadeClone,itens,ladroesClone,distCasas);
        
        VND(cidadeClone,itens,ladroesClone,distCasas);
        
        long double atualFObj = fObj(ladroesClone, itens, cidadeClone, distCasas);
       
        if( atualFObj > melhorFObj){
            cidadeOtima = cidadeClone;
            ladroesOtima = ladroesClone; 
            melhorFObj = atualFObj;
        }

        if(qualGRASP == 3){
            cidade = cidadeOtima;
            ladroes = ladroesOtima;
            break;
        }
    }
}

#endif