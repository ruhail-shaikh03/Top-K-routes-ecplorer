#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include<mpi.h>



#define INF INT_MAX

#define const int INF = INT_MAX;

// Function to reconstruct and print the path
void printPath(int node, int kth, int **pred, int numNodes) {
    int *path = (int *)malloc(numNodes * sizeof(int));
    int current = node;
    int index = 0;

    // Trace the path back to the source
    while (current != -1) {
        path[index++] = current;
        current = pred[current][kth];
    }

    // Print the path from the source to the destination
    for (int i = index - 1; i >= 0; i--) {
        printf("%d ", path[i]);
    }

}



struct Edge {
    int dest;
    int cost;
    struct Edge *next;
};

struct PriorityQueue {
    int dist;
    int node;
};

// Function to find K shortest path lengths using an adjacency list
void findKShortest(struct Edge **g, int n, int k, int source, int destination) {
    int **dis = (int **)malloc((n + 1) * sizeof(int *));
    int **pred = (int **)malloc((n + 1) * sizeof(int *));
    #pragma omp parallel for
    for (int i = 0; i <= n; i++) {
        dis[i] = (int *)malloc(k * sizeof(int));
        pred[i] = (int *)malloc(k * sizeof(int));
        for (int j = 0; j < k; j++) {
            dis[i][j] = INF;
            pred[i][j] = -1;
        }
    }
    
    struct PriorityQueue *pq = (struct PriorityQueue *)malloc((n + 1) * sizeof(struct PriorityQueue));
    int pqSize = 0;
    
    pq[pqSize].dist = 0;
    pq[pqSize++].node = source;
    dis[source][0] = 0;
    
    for (int i = 0; i < pqSize; i++) {
        int u = pq[i].node;
        int d = pq[i].dist;

        if (dis[u][k - 1] < d) continue;
        #pragma omp parallel for
        for (struct Edge *current = g[u]; current != NULL; current = current->next) {
            int dest = current->dest;
            int cost = current->cost;
            int new_dist = d + cost;

            if (new_dist < dis[dest][k - 1]) {
                dis[dest][k - 1] = new_dist;
                pred[dest][k - 1] = u;
                #pragma omp parallel for
                for (int j = k - 1; j > 0 && dis[dest][j] < dis[dest][j - 1]; j--) {
                    int temp = dis[dest][j];
                    dis[dest][j] = dis[dest][j - 1];
                    dis[dest][j - 1] = temp;

                    temp = pred[dest][j];
                    pred[dest][j] = pred[dest][j - 1];
                    pred[dest][j - 1] = temp;
                }

                if (pqSize < n) {
                    pq[pqSize].dist = new_dist;
                    pq[pqSize++].node = dest;
                }
            }
        }
    }

    printf("K shortest paths from %d to node %d:\n", source, destination);
    #pragma omp parallel for

    for (int i = 0; i < k; i++) {
        if (dis[destination][i] == INF) {
            printf("Path %d: Unreachable\n", i + 1);
        } else {
            printf("Path %d (Cost: %d): ", i + 1, dis[destination][i]);
            // printPath(destination, i, pred);  // Uncomment to print path
            printf("\n");
        }
    }


}

void printGraph(struct Edge **graph, int size);

// Function to print the graph representation (Adjacency List)
void printGraph( struct Edge **graph, int size) {
    printf("Graph representation (Adjacency List):\n");
    #pragma omp parallel for

    for (int i = 1; i < size; i++) {
        printf("Node %d:", i);
        struct Edge *current = graph[i];
        while (current != NULL) {
            printf(" (to: %d, weight: %d)", current->dest, current->cost);
            current = current->next;
        }
        printf("\n");
    }
}

void fillArrayWithRandomPairs(int arr[][2], int numRows, int numNodes, int var) {
    for (int i = 0; i < numRows; i++) {
        arr[i][0] = rand() % numNodes/(var);
        arr[i][1] = rand() % numNodes/(var);
    }
}


int main() {
    

    // Skipping header lines
    FILE *inFile = fopen("doctorwho.txt", "r");
    char line[256];
    for (int i = 0; i < 3; ++i) {
        fgets(line, sizeof(line), inFile);
    }

    int numNodes, numEdges;

    if (sscanf(line, "# Nodes: %d Edges: %d", &numNodes, &numEdges) == 2) {
        printf("Number of Nodes: %d\n", numNodes);
        printf("Number of Edges: %d\n", numEdges);
    } else {
        fprintf(stderr, "Error parsing the line\n");
        return 1;
    }
    
    

    // Initialize the adjacency list
    struct Edge **adjacencyList = (struct Edge **)malloc((numNodes + 1) * sizeof(struct Edge *));
    for (int i = 0; i <= numNodes; i++) {
        adjacencyList[i] = NULL;
    }

    // Read the edges from the file
    while (fgets(line, sizeof(line), inFile)) {
        int fromNodeId, toNodeId;
        sscanf(line, "%d %d", &fromNodeId, &toNodeId);

        // Ensure no duplicate edges in an undirected graph
        if (fromNodeId < toNodeId) {
            struct Edge *newEdge1 = (struct Edge *)malloc(sizeof(struct Edge));
            newEdge1->dest = toNodeId;
            newEdge1->cost = 1;
            newEdge1->next = adjacencyList[fromNodeId];
            adjacencyList[fromNodeId] = newEdge1;

            struct Edge *newEdge2 = (struct Edge *)malloc(sizeof(struct Edge));
            newEdge2->dest = fromNodeId;
            newEdge2->cost = 1;
            newEdge2->next = adjacencyList[toNodeId];
            adjacencyList[toNodeId] = newEdge2;
        }
    }


    fclose(inFile);
    //printGraph( adjacencyList, numNodes);
    int K = 10; // Finding 10 shortest paths

    int pair1[3][2], pair2[3][2], pair3[3][2], pair4[3][2];

    fillArrayWithRandomPairs(pair1, 3, numNodes,1);
    fillArrayWithRandomPairs(pair2, 3, numNodes,2);
    fillArrayWithRandomPairs(pair3, 3, numNodes,3);
    fillArrayWithRandomPairs(pair4, 3, numNodes,4);

    

   

    clock_t start, end;
    double cpu_time_used;
    srand(time(NULL));  // Seed for random number generation

    MPI_Init(NULL, NULL);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Barrier(MPI_COMM_WORLD);
    // Get the rank of the process
    int world_rank;
    int process = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    if (world_rank == 0){

        start = clock(); // Start the clock
        #pragma omp parallel for

        for(int i = 0; i < 3; i++) {
            findKShortest(adjacencyList, numNodes, K, pair1[i][0], pair1[i][1]);
        }
    }
    if (world_rank == 1){
        #pragma omp parallel for

        for(int i = 0; i < 3; i++) {
            findKShortest(adjacencyList, numNodes, K, pair2[i][0], pair2[i][1]);
        }
    }
    if (world_rank == 2){
        #pragma omp parallel for

        for(int i = 0; i < 3; i++) {
            findKShortest(adjacencyList, numNodes, K, pair3[i][0], pair3[i][1]);
        }
    }
    if (world_rank == 3){
        #pragma omp parallel for

        for(int i = 0; i < 3; i++) {
            findKShortest(adjacencyList, numNodes, K, pair4[i][0], pair4[i][1]);
        }
    }

    // findKShortest(adjacencyList, numNodes, K, 1, 44);

    MPI_Barrier(MPI_COMM_WORLD);

    if (world_rank == 0) {
        end = clock(); // End the clock
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // Calculate the time taken in seconds
        printf("Time taken: %f seconds\n", cpu_time_used);
    }
    MPI_Finalize();


    


}