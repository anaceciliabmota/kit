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
    sequencia.push_back(rand() % (vertices) + 1);
    do{
        sequencia.push_back(rand() % (vertices) + 1);
    }while(sequencia[1] == sequencia[2]);
    do{
        sequencia.push_back(rand() % (vertices) + 1);
    }while(sequencia[1] == sequencia[3] || sequencia[2] == sequencia[3]);
    sequencia.push_back(1);
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

// vector<int> Calcular

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
    //para testar a funcao NosRestantes
    for(int i = 0; i < CL.size();i++){
        cout << CL[i] << " ";
    }
    return 0;
}


    
   