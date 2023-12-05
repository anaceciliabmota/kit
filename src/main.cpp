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
    sequencia.insert(sequencia.begin(), (rand() % (vertices) + 1));
    
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
    //ExibirSolucao(& s);
    vector<int> CL = nosRestantes(&s, vertices);
    /*for(int i = 0; i < CL.size(); i++){
        cout << CL[i] << " ";
    }*/
    while(!CL.empty()){
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, data);
        OrdenarEmOrdemCrescente(custoInsercao);
        /*for(int i = 0; i < custoInsercao.size();i++){
            cout << custoInsercao[i].custo << "->" << custoInsercao[i].noInserido << " entre " << custoInsercao[i].arestaRemovida << "  " ;
        }*/
        //cout << endl;
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil((alpha + 0.000001) * custoInsercao.size()));
        //cout << selecionado << endl;
        inserirNaSolucao(s, custoInsercao, CL, selecionado);
       // ExibirSolucao(& s);
    }
    return s;
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
}