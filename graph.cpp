#include "graph.h"
#include <algorithm>
#include <numeric>

using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Clase para manejar Union-Find
class UnionFind {
private:
    vector<int> parent, rank;
public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        iota(parent.begin(), parent.end(), 0); // Inicializar como conjunto disjunto
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]); // Compresión de ruta
        }
        return parent[u];
    }

    bool unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            } else if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            } else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
            return true;
        }
        return false;
    }
};

// Función para calcular el MST
vector<pair<int, int>> calculateMST(int N, const vector<vector<int>>& distances) {
    vector<Edge> edges;

    // Construir una lista de aristas desde la matriz de adyacencia
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) { // Evitar duplicados (grafo no dirigido)
            if (distances[i][j] > 0) {
                edges.push_back({i, j, distances[i][j]});
            }
        }
    }

    // Ordenar las aristas por peso
    sort(edges.begin(), edges.end());

    UnionFind uf(N);
    vector<pair<int, int>> mst;

    // Construir el MST
    for (const auto& edge : edges) {
        if (uf.unionSets(edge.u, edge.v)) {
            mst.push_back({edge.u, edge.v});
        }
    }

    return mst;
}
