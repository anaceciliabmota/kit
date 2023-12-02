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

vector<int> NosRestantes(Solucao *s, size_t vertices){
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

    Solucao s1 = {{}, 0.0};
    s1.sequencia = escolher3NosAleatorios(n); 
    ExibirSolucao(& s1); 
    vector<int> CL = NosRestantes(&s1, n);
    for(int i = 0; i < CL.size(); i++){
        cout << CL[i] << " ";
    }
    cout << endl;
    
    vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s1, CL, data);
    for(int i = 0; i < custoInsercao.size(); i++){
        cout << custoInsercao[i].custo << " ";
    }
    cout << endl;
    OrdenarEmOrdemCrescente(custoInsercao);
    for(int i = 0; i < custoInsercao.size(); i++){
        cout << custoInsercao[i].custo << " ";
    }
    
    
    return 0;
    
}