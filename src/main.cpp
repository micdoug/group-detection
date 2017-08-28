#include <iostream>
#include <fstream>
#include <memory>
#include <array>
#include "Connection.h"
#include "Node.h"
#include "GroupDetection.h"

using namespace std;


// Parâmetros de simulação
const int nro_nodes = 100;

using namespace std;

int main() {
    // Cria os nós da simulação
    array<unique_ptr<GroupDetection>, nro_nodes> groupDetection;
    for(auto i = 0; i<nro_nodes; ++i)
    {
        groupDetection[i] = make_unique<GroupDetection>(i);
    }

    // Tenta abrir o arquivo de teste
    ifstream file("teste.txt");
    if (!file.is_open())
    {
        cout << "Error while opening the trace file. Try again." << endl;
        return 1;
    }

    while(!file.eof())
    {
        Connection con(file);
        int nodeA, nodeB;
        nodeA = std::get<0>(con.nodes());
        nodeB = std::get<1>(con.nodes());
        if (con.type() == ConnectionType::UP)
        {
            groupDetection[nodeA]->node().addNeighbor(nodeB);
            groupDetection[nodeB]->node().addNeighbor(nodeA);
        }
        else
        {
            groupDetection[nodeA]->node().removeNeighbor(nodeB);
            groupDetection[nodeB]->node().removeNeighbor(nodeA);
        }
    }

    return 0;
}