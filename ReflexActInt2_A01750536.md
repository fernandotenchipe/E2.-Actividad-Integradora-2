# Evidencia 2

## Equipo

- Fernando Tenchipe Morales:
    - Algoritmo para calcular el cableado de fibra óptica entre cuales quiera dos colonias de la manera más óptima (MST con Kruskal).
    - Algoritmo para conocer el flujo máximo de información del nodo inicial al nodo final considerando. (Ford-Fulkerson con BFS).

- Fernando Daniel Monroy Sánchez:
    - Algoritmo para calcular el viaje más corto desde una colonia origen hacia todas las colonias, regresando al origen al final. (Traveling Salesman con Algoritmo Genético)
    
## Reflexión

En este proyecto, se implementaron diversos algoritmos para abordar la situación problema planteada, cada uno con su propia complejidad computacional.

### Algoritmo de Kruskal para el Árbol de Expansión Mínima (MST)

El Algoritmo de Kruskal se utilizó para encontrar el Árbol de Expansión Mínima del grafo. Este algoritmo ordena todas las aristas por peso y las añade al árbol, asegurando que no se formen ciclos utilizando la estructura Union-Find. Su complejidad es de O(E log E), donde E es el número de aristas.

### Algoritmo de Ford-Fulkerson para Flujo Máximo

El Algoritmo de Ford-Fulkerson se implementó para calcular el flujo máximo entre dos nodos en el grafo. Utiliza búsquedas en amplitud (BFS) para encontrar caminos de aumento en el grafo residual. La complejidad en el peor caso es O(E * maxFlow), donde E es el número de aristas y maxFlow es el flujo máximo.

### Algoritmo Genético para el Problema del Viajante (TSP)

Se utilizó un Algoritmo Genético para resolver el Problema del Viajante, optimizando la ruta de reparto de correspondencia. Este enfoque evolutivo incluye operaciones de selección, cruce y mutación para generar soluciones cada vez mejores. La complejidad depende de los parámetros como el tamaño de la población y el número de generaciones, típicamente de O(P * G * N^2), donde P es el tamaño de la población, G el número de generaciones y N el número de nodos.