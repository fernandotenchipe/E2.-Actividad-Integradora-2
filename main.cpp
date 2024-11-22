#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>
#include <queue>
#include <algorithm>
#include <numeric>
#include <climits> // Para INT_MAX

using namespace std;

struct Edge {
    int u, v, distance, flow;
    bool operator<(const Edge& other) const {
        return distance < other.distance;
    }
};

struct Individual {
    vector<int> tour;
    double fitness;
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

// BFS para buscar un camino de aumento en el grafo residual
bool bfs(const vector<vector<int>>& residualGraph, int N, int source, int sink, vector<int>& parent) {
    vector<bool> visited(N, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < N; ++v) {
            if (!visited[v] && residualGraph[u][v] > 0) { // Si hay capacidad residual
                parent[v] = u;
                if (v == sink) return true; // Encontramos el camino
                q.push(v);
                visited[v] = true;
            }
        }
    }
    return false;
}

// Algoritmo de Ford-Fulkerson para flujo máximo
int fordFulkerson(int N, const vector<vector<int>>& capacityMatrix, int source, int sink) {
    vector<vector<int>> residualGraph = capacityMatrix; // Grafo residual
    vector<int> parent(N); // Para guardar el camino de aumento
    int maxFlow = 0;

    while (bfs(residualGraph, N, source, sink, parent)) {
        // Encontrar el flujo mínimo en el camino de aumento
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        // Actualizar capacidades residuales
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        // Sumar el flujo del camino al flujo total
        maxFlow += pathFlow;
    }

    return maxFlow;
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

// Function to initialize population
vector<Individual> initializePopulation(int populationSize, int N) {
    vector<Individual> population(populationSize);
    // Initialize random generator
    srand(time(0));
    for(auto &ind : population) {
        ind.tour.resize(N);
        for(int i = 0; i < N; ++i) ind.tour[i] = i;
        // Shuffle the tour
        random_shuffle(ind.tour.begin()+1, ind.tour.end());
        ind.fitness = 0.0;
    }
    return population;
}

// Function to calculate fitness
double calculateFitness(const Individual &ind, const vector<vector<int>> &distanceMatrix) {
    double totalDistance = 0.0;
    for(int i = 0; i < ind.tour.size(); ++i) {
        int from = ind.tour[i];
        int to = ind.tour[(i+1) % ind.tour.size()];
        totalDistance += distanceMatrix[from][to];
    }
    return 1.0 / totalDistance;
}

// Function to evolve population
Individual geneticAlgorithmTSP(int N, const vector<vector<int>> &distanceMatrix) {
    int populationSize = 100;
    int generations = 500;
    vector<Individual> population = initializePopulation(populationSize, N);
    
    // Evaluate initial fitness
    for(auto &ind : population) {
        ind.fitness = calculateFitness(ind, distanceMatrix);
    }
    
    // Evolution process
    for(int gen = 0; gen < generations; ++gen) {
        // Selection, crossover, mutation steps
        // ...existing code...
    }
    
    // Find the best individual
    Individual best = population[0];
    for(const auto &ind : population) {
        if(ind.fitness > best.fitness) best = ind;
    }
    
    return best;
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
    
    // Implementar el Algoritmo Genético para el TSP
    Individual bestRoute = geneticAlgorithmTSP(N, distanceMatrix);
    
    // Escribir el MST y la ruta del TSP en el archivo de salida
    ofstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Unable to open output file";
        return 1;
    }

    outputFile << "Forma de cablear las colonias con fibra usando el Algoritmo de Kruskal\n";
    for (const auto& edge : mst) {
        outputFile << "(" << edge.first << "," << edge.second << ")\n";
    }

    // Calcular el flujo máximo
    int source = 0; // Nodo fuente
    int sink = N - 1; // Nodo sumidero
    int maxFlow = fordFulkerson(N, capacityMatrix, source, sink);

    outputFile << "\nFlujo máximo entre " << source << " y " << sink << ": " << maxFlow << endl;
    
    outputFile << "\nRuta para repartir correspondencia utilizando Algoritmo Genético para el TSP:\n";
    for(auto node : bestRoute.tour) {
        outputFile << node << " -> ";
    }
    outputFile << bestRoute.tour[0] << "\n"; // Regresar al origen
    
    outputFile.close();

    return 0;
}
