#include "Data.h"
#include <iostream>
#include <vector>
#include <time.h> 
#include <cstdlib>

using namespace std;

typedef struct Solucao {
        vector<int> sequencia;
        double valorObj = 0.0;
    } Solucao;

void CalculaValorObj (Solucao *s, vector<vector<double>> Matriz){
    s->valorObj = 0;
    for(int i = 0; i < s->sequencia.size() -1;i++){
        s->valorObj += Matriz[s->sequencia[i]-1][s->sequencia[i+1]-1];
    }
}
struct InsertionInfo{
        int noInserido;
        int arestaRemovida;
        double custo;   
};

vector<int> escolher3NosAleatorios(size_t vertices){
    vector<int> sequencia;
    
    sequencia.push_back(1);
    sequencia.push_back(1);

    sequencia.insert(sequencia.begin() + 1, (rand() % (vertices) + 1));
    while(sequencia[0] == sequencia[1]){
        sequencia.erase(sequencia.begin() + 1);
        sequencia.insert(sequencia.begin() + 1, (rand() % (vertices) + 1));
    }

    sequencia.insert(sequencia.begin() + 2, (rand() % (vertices) + 1));
    while(sequencia[1] == sequencia[2] || sequencia[2] == sequencia[0]){
        sequencia.erase(sequencia.begin() + 2);
        sequencia.insert(sequencia.begin() + 2, (rand() % (vertices) + 1));
    }
    
    sequencia.insert(sequencia.begin() + 3, (rand() % (vertices) + 1));
    while(sequencia[1] == sequencia[3] || sequencia[2] == sequencia[3] || sequencia[0] == sequencia[3]){
        sequencia.erase(sequencia.begin() + 3);
        sequencia.insert(sequencia.begin() + 3, (rand() % (vertices) + 1));
    }
    
    

    return sequencia;
}

void ExibirSolucao(Solucao *s){
    for(int i = 0; i < s->sequencia.size() - 1;i++){
        cout << s->sequencia[i] << "->";    
    }
    cout << s->sequencia.back() <<  endl;
}

vector<int> nosRestantes(Solucao *s, size_t vertices){
    vector<int> CL;
    bool diferente = true;
    for(size_t j = 1; j <= vertices; j++){
        for (size_t i = 0; i < s->sequencia.size(); i++)
        {
            if(j == s->sequencia[i]){
                diferente = false;
                break;
            }
        }
        if(diferente)
            CL.push_back(j);
        diferente = true;    
    }
    return CL;
}

vector<InsertionInfo> calcularCustoInsercao(Solucao& s, vector<int>& CL, vector<vector<double>> Matriz){
    vector<InsertionInfo> custoInsercao((s.sequencia.size() - 1) * CL.size());
    int l = 0;

    for(int a = 0; a < s.sequencia.size() - 1;a++){
        int i = s.sequencia[a];
        int j = s.sequencia[a + 1];
        for (auto k : CL){
            //custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            custoInsercao[l].custo = Matriz[i-1][k-1] + Matriz[k-1][j-1] - Matriz[i-1][j-1];
            custoInsercao[l].arestaRemovida = a;
            custoInsercao[l].noInserido = k;
            l++;
        }
    }
    return custoInsercao;
}
void TrocarElementos(InsertionInfo& a, InsertionInfo& b) {
    InsertionInfo aux = a;
    a = b;
    b = aux;
}

int Particionar(vector<InsertionInfo>& custoInsercao, int baixo, int alto) {
    double pivo = custoInsercao[alto].custo;
    int i = baixo - 1;

    for (int j = baixo; j < alto; j++) {
        if (custoInsercao[j].custo <= pivo) {
            i++;
            TrocarElementos(custoInsercao[i], custoInsercao[j]);
        }
    }
    //coloca o pivô na posicao em que ele fica com os menores a esquerda e os elementos maiores à direita
    TrocarElementos(custoInsercao[i + 1], custoInsercao[alto]);
    return i + 1;
}

void QuickSort(vector<InsertionInfo>& custoInsercao, int baixo, int alto) {
    if (baixo < alto) {
        int indicePivo = Particionar(custoInsercao, baixo, alto);
        QuickSort(custoInsercao, baixo, indicePivo - 1);
        QuickSort(custoInsercao, indicePivo + 1, alto);
    }
}

void OrdenarEmOrdemCrescente(vector<InsertionInfo>& custoInsercao) {
    QuickSort(custoInsercao, 0, custoInsercao.size() - 1);
}
/*void OrdenarEmOrdemCrescente(vector<InsertionInfo>& custoInsercao){
    InsertionInfo aux;
        for(int i = 0; i < custoInsercao.size();i++){
            for(int j = i + 1; j < custoInsercao.size();j++){
                if(custoInsercao[i].custo > custoInsercao[j].custo){  
                    aux = custoInsercao[i];
                    custoInsercao[i] = custoInsercao[j];
                    custoInsercao[j] = aux;
                }
        }
    }
}
*/

void inserirNaSolucao(Solucao& s, vector<InsertionInfo>& custoInsercao, vector<int>& CL, int selecionado){
    for(int i = 0; i < s.sequencia.size();i++){
        if(s.sequencia[i] == s.sequencia[custoInsercao[selecionado].arestaRemovida]){
            s.sequencia.insert(s.sequencia.begin() + i + 1, custoInsercao[selecionado].noInserido);
            break;
        }
    }
    for(int i = 0; i < CL.size();i++){
        if(CL[i] == custoInsercao[selecionado].noInserido){
            CL.erase(CL.begin() + i);
        }
    }
}

Solucao Construcao(int vertices, vector<vector<double>> Matriz){
    //clock_t start,end, startt, endt;
    //startt= clock();
    double tempo_ordenacao = 0;
    Solucao s = {{}, 0.0};
    s.sequencia = escolher3NosAleatorios(vertices);
    vector<int> CL = nosRestantes(&s, vertices);
    while(!CL.empty()){
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, Matriz);
        //start = clock();
        OrdenarEmOrdemCrescente(custoInsercao);
        //end = clock();
        //tempo_ordenacao += double(end-start)/double(CLOCKS_PER_SEC);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil((alpha + 0.000001) * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao, CL, selecionado); 
    }
    /*endt = clock();
    double tempo_costrucao2 = double(endt-startt)/double(CLOCKS_PER_SEC);
    cout << "Tempo ordenacao: " << fixed << tempo_ordenacao;
    cout << endl;
    cout << "tempo execucao: " << fixed << tempo_costrucao2;
    cout<< endl;*/
    return s;
}

bool bestImprovementSwap(Solucao *s, vector<vector<double>> Matriz){
    double bestDelta = 0;
    int best_i,best_j;
    for(int i = 1; i < s->sequencia.size() - 1; i++){
        int vi = s->sequencia[i];
        int vi_next = s->sequencia[i+1];
        int vi_prev = s->sequencia[i-1];
        for(int j = i+1; j < s->sequencia.size() - 1;j++){
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[j+1];
            int vj_prev = s->sequencia[j-1];
            double delta = 0;
            if(i + 1!= j){
                /*delta = -data.getDistance(vi_prev,vi) - data.getDistance(vi, vi_next) + data.getDistance(vi_prev, vj) + data.getDistance(vj, vi_next) - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next) + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next);*/
                delta = -Matriz[vi_prev-1][vi-1] - Matriz[vi-1][vi_next-1] + Matriz[vi_prev-1][vj-1] + Matriz[vj-1][vi_next-1] - Matriz[vj_prev-1][vj-1] - Matriz[vj-1][vj_next-1] + Matriz[vj_prev-1][vi-1] + Matriz[vi-1][vj_next-1];
            } else {
                /*delta = -data.getDistance(vi_prev,vi) + data.getDistance(vi_prev, vj) - data.getDistance(vj, vj_next)  + data.getDistance(vi, vj_next);*/
                delta = -Matriz[vi_prev-1][vi-1] + Matriz[vi_prev-1][vj-1] - Matriz[vj-1][vj_next-1] + Matriz[vi-1][vj_next-1];
            }
            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    
    if(bestDelta < 0){
        swap(s->sequencia[best_i], s->sequencia[best_j]);
        s->valorObj = s->valorObj + bestDelta;
        return true;
    }
    return false;
}

bool bestImprovement2Opt(Solucao *s, vector<vector<double>> Matriz){
    double bestDelta = 0;
    int best_i;
    int best_j;
    for(int i = 1; i < s->sequencia.size() - 1;i++){
        //i = primeiro elemento da aresta a ser invertida
        int vi = s->sequencia[i];
        int vi_prev = s->sequencia[i-1];
        for(int j = i + 3; j < s->sequencia.size() - 1; j++){
            // j = ultimo elemento da aresta a ser invertida
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[j+1];
            /*double delta = - data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next) + data.getDistance(vi_prev, vj) + data.getDistance(vi, vj_next);*/
            double delta = -Matriz[vi_prev-1][vi-1] - Matriz[vj-1][vj_next-1] + Matriz[vi_prev-1][vj-1] + Matriz[vi-1][vj_next-1];
            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(bestDelta < 0){
        int cont = 0;
        while((best_i + cont) < (best_j - cont)){
            swap(s->sequencia[best_i + cont], s->sequencia[best_j - cont]);
            cont++;
        }
        s->valorObj = s->valorObj + bestDelta;
        return true;
    }
    return false;
}

bool bestImprovementOrOpt(Solucao *s, vector<vector<double>> Matriz, int tam_bloco){
    double bestDelta = 0;
    int best_i1;
    int best_i2;
    int best_j;
    for(int i = 1; i < s->sequencia.size() - 1;i++){
        int vi1 = s->sequencia[i];
        int vi1_prev = s->sequencia[i-1];
        int vi2 = s->sequencia[i + tam_bloco - 1];
        int vi2_next = s->sequencia[i + tam_bloco];
        for(int j = i + tam_bloco; j < s->sequencia.size() - 1; j++){
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[j+1];
            /*double delta = -data.getDistance(vi1_prev, vi1) - data.getDistance(vi2, vi2_next) - data.getDistance(vj, vj_next) + data.getDistance(vi1_prev, vi2_next) + data.getDistance(vj, vi2) + data.getDistance(vi1, vj_next);*/ 
            double delta = -Matriz[vi1_prev-1][vi1-1] - Matriz[vi2-1][vi2_next-1]- Matriz[vj-1][vj_next-1] + Matriz[vi1_prev-1][vi2_next-1] + Matriz[vj-1][vi2-1] + Matriz[vi1-1][vj_next-1];
            if (delta < bestDelta){
                bestDelta = delta;
                best_i1 = i;
                best_i2 = i+tam_bloco -1;
                best_j = j;
            }
        }
    }

    int cont = 0;
    if(bestDelta < 0){
        do{
        s->sequencia.insert(s->sequencia.begin() + best_j + 1, s->sequencia[best_i1]);
        s->sequencia.erase(s->sequencia.begin() + best_i1);
        best_j--;
        cont++;
        }while(cont < tam_bloco);
        s->valorObj = s->valorObj + bestDelta;
        return true;
    }
    return false;
}

void BuscaLocal(Solucao *s, vector<vector<double>> Matriz){
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;

    while (NL.empty() == false){
        int n = rand() % NL.size();
        switch (NL[n]){
            case 1:
                improved = bestImprovementSwap(s, Matriz);
                //cout << improved << endl;
                break;
            case 2:
                improved = bestImprovement2Opt(s, Matriz);
                //cout << improved<< endl;
                break;
            case 3:
                improved = bestImprovementOrOpt(s, Matriz, 1);
                //cout << "3"<< endl;
                break;
            case 4: 
                improved = bestImprovementOrOpt(s, Matriz, 2);
                //cout << "4"<< endl;
                break;
            case 5:
                improved = bestImprovementOrOpt(s, Matriz, 3);
                //  cout << "5"<< endl;
                break;
        }
        if(improved){
            NL = {1,2,3,4,5};
        }else{
            NL.erase(NL.begin() + n);
        }
    }
}

Solucao Perturbacao (Solucao best, vector<vector<double>> Matriz){

    Solucao s = best;
    int vertices = best.sequencia.size() - 1;
    int i, j, tam_i, tam_j;
    //escolhe os valores aleatorios para os dois segmentos e seus tamanhos
    do{
        i = (rand() % (vertices - 1)) + 1;
        if(vertices/10 > 2){
            tam_i = (rand() % ((vertices/10) - 2) + 2);
        } else{
            tam_i = 2;
        }

    }while((i + tam_i) > vertices);
    do{
        j = (rand() % (vertices - 1)) + 1;
        if(vertices/10 > 2){
            tam_j = (rand() % (vertices/10 - 2) + 2);
        }
        else{
            tam_j = 2;
        }
    
    }while(((i + tam_i) > j && (j + tam_j) > i) || ((j + tam_j) > vertices));
    
    double delta;
    if((i+ tam_i) == j || (j +tam_j) == i){
        if( i < j){
            delta = - Matriz[s.sequencia[i]-1][s.sequencia[i-1]-1] - Matriz[s.sequencia[i+tam_i -1]-1][s.sequencia[i+tam_i]-1] -Matriz[s.sequencia[j+tam_j-1]-1][s.sequencia[j+tam_j]-1] + Matriz[s.sequencia[i-1]-1][s.sequencia[j]-1] + Matriz[s.sequencia[j+tam_j-1]-1][s.sequencia[i]-1] + Matriz[s.sequencia[i+tam_i-1]-1][s.sequencia[j+tam_j]-1];
        }
        else{
            delta = -Matriz[s.sequencia[j]-1][s.sequencia[j-1]-1] - Matriz[s.sequencia[j+tam_j -1]-1][s.sequencia[j+tam_j]-1] -Matriz[s.sequencia[i+tam_i-1]-1][s.sequencia[i+tam_i]-1] + Matriz[s.sequencia[j-1]-1][s.sequencia[i]-1] + Matriz[s.sequencia[i+tam_i-1]-1][s.sequencia[j]-1] + Matriz[s.sequencia[j+tam_j-1]-1][s.sequencia[i+tam_i]-1];
        }
    }else{
        if(i < j){
            delta = -Matriz[s.sequencia[i]-1][s.sequencia[i-1]-1] - Matriz[s.sequencia[i+tam_i-1]-1][s.sequencia[i+tam_i]-1] - Matriz[s.sequencia[j]-1][s.sequencia[j-1]-1] - Matriz[s.sequencia[j+tam_j]-1][s.sequencia[j+tam_j-1]-1] + Matriz[s.sequencia[i-1]-1][s.sequencia[j]-1] + Matriz[s.sequencia[j+tam_j-1]-1][s.sequencia[i+tam_i]-1] + Matriz[s.sequencia[j-1]-1][s.sequencia[i]-1] + Matriz[s.sequencia[i+tam_i-1]-1][s.sequencia[j+tam_j]-1];
        }
        else{
            delta = -Matriz[s.sequencia[j]-1][s.sequencia[j-1]-1] - Matriz[s.sequencia[j+tam_j-1]-1][s.sequencia[j+tam_j]-1] - Matriz[s.sequencia[i]-1][s.sequencia[i-1]-1] - Matriz[s.sequencia[i+tam_i]-1][s.sequencia[i+tam_i-1]-1] + Matriz[s.sequencia[j-1]-1][s.sequencia[i]-1] + Matriz[s.sequencia[i+tam_i-1]-1][s.sequencia[j+tam_j]-1] + Matriz[s.sequencia[i-1]-1][s.sequencia[j]-1] + Matriz[s.sequencia[j+tam_j-1]-1][s.sequencia[i+tam_i]-1];
        }
    }
    s.valorObj = s.valorObj + delta;
    
    //faz a troca dos segmentos
    int cont = 0;
    if(i < j){
        do{
            s.sequencia.insert(s.sequencia.begin() + j, s.sequencia[i]);
            s.sequencia.erase(s.sequencia.begin() + i);
            cont++;
        }while(cont < tam_i);
        
        cont = 0;
        do{
            s.sequencia.insert(s.sequencia.begin() + i + cont, s.sequencia[j + cont]);
            s.sequencia.erase(s.sequencia.begin() + j + cont + 1);
            cont++;
        }while(cont < tam_j);
        
    }
    else{
        do{
            s.sequencia.insert(s.sequencia.begin() + i, s.sequencia[j]);
            s.sequencia.erase(s.sequencia.begin() + j);
            cont++;
        }while(cont < tam_j);
        
        cont = 0;
        do{
            s.sequencia.insert(s.sequencia.begin() + j + cont, s.sequencia[i + cont]);
            s.sequencia.erase(s.sequencia.begin() + i + cont + 1);
            cont++;
        }while(cont < tam_i);
    }
    
   return s; 
}


Solucao ILS(int maxIter, int maxIterIls, vector<vector<double>> Matriz){
   // clock_t start1, start2, start3, start4, start5;
    //double tempo_while = 0, tempo_for = 0, tempo_construcao = 0;
    Solucao bestOfAll;
    bestOfAll.valorObj = INFINITY;
    int vertices = Matriz.size();
    //start1 =clock();
    for(int i = 0; i < maxIter;i++){
       // start5 = clock();
        Solucao s = Construcao(vertices, Matriz);
        //start2= clock();
        CalculaValorObj(& s, Matriz);
        Solucao best = s;
        int iterIls = 0;
        
        //tempo_construcao += double(start2 - start5)/ double(CLOCKS_PER_SEC);
        while(iterIls <= maxIterIls){
            
            BuscaLocal(&s, Matriz);
            if(s.valorObj < best.valorObj){
                best = s;
                iterIls = 0;
            }
            
            s = Perturbacao(best, Matriz);
            iterIls++;
        }
        //start3 =clock();
        //tempo_while = tempo_while + (double(start3-start2) / double(CLOCKS_PER_SEC));
    
        if(best.valorObj < bestOfAll.valorObj)
            bestOfAll = best;
    }
    /*start4 =clock();
    tempo_for = tempo_for +  (double(start4-start1) / double(CLOCKS_PER_SEC));
    cout<< " tempo while: " << fixed << tempo_while;
    cout<< " tempo for: " << fixed << tempo_for;
    cout << " tempo construcao: " << fixed << tempo_construcao;
    cout << endl;*/
    return bestOfAll;   
}


int main(int argc, char** argv) {
    double custo = 0;
    double tempo = 0;
    srand(time(NULL));
    for(int l = 0; l < 10;l++){
        clock_t start, end;
        start = clock();
        auto data = Data(argc, argv[1]);
        data.read();
        int n = data.getDimension();
        vector<vector<double>> Matriz(n, vector<double>(n));
    
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                Matriz[i][j] = data.getDistance(i+1,j+1);
            }
        }
   
        int maxIter = 50;
        int maxIterIls;
        if(n >= 150)
            maxIterIls = n/2;
        else
            maxIterIls = n;
        Solucao best = ILS(maxIter, maxIterIls, Matriz);
        CalculaValorObj(& best, Matriz);
        custo += best.valorObj;
        end = clock();
        double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
        tempo += time_taken;
    }
    
    double custo_medio = custo/10;
    double tempo_medio = tempo/10;
    cout << "custo: " << custo_medio << endl;
    cout << "media de tempo gasto: " << fixed << tempo_medio;
    cout << " secs" << endl;
    
    
    return 0;

}
 /*Solucao s;

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();
    cout << "Exemplo de Solucao s = ";
    double cost = 0.0;
    for (size_t i = 1; i < n; i++) {
        cout << i << " -> ";
        s.sequencia.push_back(i);
        cost += data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << cost << endl;*/