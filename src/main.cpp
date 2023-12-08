#include "Data.h"
#include <iostream>
#include <vector>
#include <time.h> 
#include <cstdlib>

using namespace std;

typedef struct Solucao {
        vector<int> sequencia;
        double valorObj;
    } Solucao;

struct InsertionInfo{
        int noInserido;
        int arestaRemovida;
        double custo;   
};

vector<int> escolher3NosAleatorios(size_t vertices){
    vector<int> sequencia;
    srand(time(NULL));
    
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

// ainda não usei costrutor ao declarar o vector
vector<InsertionInfo> calcularCustoInsercao(Solucao& s, vector<int>& CL, Data& data){
    vector<InsertionInfo> custoInsercao((s.sequencia.size() - 1) * CL.size());
    int l = 0;

    for(int a = 0; a < s.sequencia.size() - 1;a++){
        int i = s.sequencia[a];
        int j = s.sequencia[a + 1];
        for (auto k : CL){
            custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            custoInsercao[l].arestaRemovida = a;
            custoInsercao[l].noInserido = k;
            l++;
        }
    }
    return custoInsercao;
}

void OrdenarEmOrdemCrescente(vector<InsertionInfo>& custoInsercao){
    InsertionInfo aux;
        for(int i = 0; i < custoInsercao.size();i++){
            for(int j = i + 1; j < custoInsercao.size();j++){
                if(custoInsercao[i].custo > custoInsercao[j].custo){  
                    aux.custo = custoInsercao[i].custo;
                    aux.arestaRemovida = custoInsercao[i].arestaRemovida;
                    aux.noInserido = custoInsercao[i].noInserido;

                    custoInsercao[i].custo = custoInsercao[j].custo;
                    custoInsercao[i].arestaRemovida = custoInsercao[j].arestaRemovida;
                    custoInsercao[i].noInserido = custoInsercao[j].noInserido;

                    custoInsercao[j].custo = aux.custo;
                    custoInsercao[j].arestaRemovida = aux.arestaRemovida;
                    custoInsercao[j].noInserido = aux.noInserido;
                }
        }
    }
}


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

Solucao Construcao(size_t vertices, Data& data){
    Solucao s = {{}, 0.0};
    s.sequencia = escolher3NosAleatorios(vertices);
    vector<int> CL = nosRestantes(&s, vertices);
    while(!CL.empty()){
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, data);
        OrdenarEmOrdemCrescente(custoInsercao);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil((alpha + 0.000001) * custoInsercao.size()));
        inserirNaSolucao(s, custoInsercao, CL, selecionado);
    }
    return s;
}

bool bestImprovementSwap(Solucao *s, Data& data){
    double bestDelta = 0;
    int best_i,best_j;
    for(int i = 1; i < s->sequencia.size() - 1; i++){
        int vi = s->sequencia[i];
        int vi_next = s->sequencia[i+1];
        int vi_prev = s->sequencia[i-1];
        for(int j = i+1; j < s->sequencia.size();j++){
            int vj = s->sequencia[j];
            int vj_next = s->sequencia[j+1];
            int vj_prev = s->sequencia[j-1];
            double delta = -data.getDistance(vi_prev,vi) - data.getDistance(vi, vi_next) + data.getDistance(vi_prev, vj) + data.getDistance(vj, vi_next) - data.getDistance(vj_prev, vj) - data.getDistance(vj, vj_next) + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next);

            if(delta < bestDelta){
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    //cout << bestDelta << "troca entre: " << best_i << best_j;
    if(bestDelta < 0){
        swap(s->sequencia[best_i], s->sequencia[best_j]);
        s->valorObj = s->valorObj + bestDelta;
        return true;
    }
    return false;
}

bool bestImprovement2Opt(Solucao *s, Data& data){
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
            double delta = - data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next) + data.getDistance(vi_prev, vj) + data.getDistance(vi, vj_next);
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
int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();
    Solucao s;

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
    cout << "Custo de S: " << cost << endl;

    Solucao s1 = Construcao(n, data);

    ExibirSolucao(& s1);
    
    /*bool troca = bestImprovement2Opt(& s1, data);
    cout << endl;
    cout << troca << endl;
    ExibirSolucao(& s1);*/
    return 0;
}