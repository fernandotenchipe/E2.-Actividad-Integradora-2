#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// For algorithm
#include <algorithm>
#include <numeric>

using namespace std;

struct Edge {
    int u, v, distance, flow;
    bool operator<(const Edge& other) const {
        return distance < other.distance;
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

// Función para calcular el MST usando la lista de adyacencias
vector<pair<int, int>> calculateMSTFromAdjList(int N, const vector<vector<Edge>>& adjacencyList) {
    vector<Edge> edges;

    // Convertir la lista de adyacencias en una lista de aristas
    for (int u = 0; u < N; ++u) {
        for (const auto& edge : adjacencyList[u]) {
            if (edge.u < edge.v) { // Evitar duplicados en un grafo no dirigido
                edges.push_back(edge);
            }
        }
    }

    // Ordenar las aristas por peso (distancia)
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

void parseInput(const string& filename, int& N, vector<vector<int>>& distanceMatrix, vector<vector<int>>& capacityMatrix, vector<pair<int, int>>& coordinates) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Unable to open file";
        exit(1);
    }

    inputFile >> N;

    distanceMatrix.resize(N, vector<int>(N));
    capacityMatrix.resize(N, vector<int>(N));
    coordinates.resize(N);

    // Leer matriz de distancias
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            inputFile >> distanceMatrix[i][j];
        }
    }

    // Leer matriz de capacidades
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            inputFile >> capacityMatrix[i][j];
        }
    }

    // Leer coordenadas
    string line;
    getline(inputFile, line); // Consumir el salto de línea
    for (int i = 0; i < N; ++i) {
        getline(inputFile, line);
        stringstream ss(line);
        char ignore;
        ss >> ignore >> coordinates[i].first >> ignore >> coordinates[i].second >> ignore;
    }

    inputFile.close();
}

// Crear lista de adyacencias desde las matrices
vector<vector<Edge>> createAdjacencyList(int N, const vector<vector<int>>& distanceMatrix, const vector<vector<int>>& capacityMatrix) {
    vector<vector<Edge>> adjacencyList(N);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (distanceMatrix[i][j] > 0) {
                adjacencyList[i].push_back({i, j, distanceMatrix[i][j], capacityMatrix[i][j]});
            }
        }
    }

    return adjacencyList;
}

int main() {
    int N;
    vector<vector<int>> distanceMatrix;
    vector<vector<int>> capacityMatrix;
    vector<pair<int, int>> coordinates;

    parseInput("input.txt", N, distanceMatrix, capacityMatrix, coordinates);

    vector<vector<Edge>> adjacencyList = createAdjacencyList(N, distanceMatrix, capacityMatrix);

    // Calcular el MST usando la lista de adyacencias
    vector<pair<int, int>> mst = calculateMSTFromAdjList(N, adjacencyList);

    // Escribir el MST en el archivo de salida
    ofstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Unable to open output file";
        return 1;
    }

    outputFile << "Forma de cablear las colonias con fibra usando el Algoritmo de Kruskal\n";
    for (const auto& edge : mst) {
        outputFile << "(" << edge.first << "," << edge.second << ")\n";
    }

    outputFile.close();

    return 0;
}
