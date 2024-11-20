#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>

using namespace std;

// Función para calcular el MST (Árbol de Expansión Mínimo)
vector<pair<int, int>> calculateMST(int N, const vector<vector<int>>& distances);

// Función para calcular el flujo máximo entre dos nodos
int maxFlow(int N, const vector<vector<int>>& capacities, int source, int sink);

#endif
