#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Edge {
    int from;
    int to;
    int weight;
} Edge;

Edge graph[] = {
    {1, 2, 5},
    {1, 7, 19},
    {1, 8, 3}, // на картинке нету числа

    {2, 3, 5},
    {2, 5, 4},
    {2, 8, 3},
    
    {3, 7, 2},
    
    {4, 5, 1},
    {4, 9, 6},
    
    {5, 6, 9},
    {5, 7, 6},
    
    {6, 1, 4},
    {6, 7, 4},
    
    {7, 8, 4},
    {7, 9, 1},
    
    {8, 5, 4},

    // от 9 не исходит ребер
};

void dumpEdges2dot (int edges_len, const Edge* edges, const char* mask, int mask_len, FILE* out){
    fputs("digraph {\n", out);
    for(size_t i = 0; i < edges_len; i++)
        fprintf(out, "\t%d -> %d [label = %d]\n", edges[i].from, edges[i].to, edges[i].weight);
    for(int i = 0; i < mask_len; i++){
        if (mask[i])
            fprintf(out, "\t%d [color=red, style=filled]\n", i);
    }
    fputs("}\n", out);
}

/**
 * @brief Требуем, что нету пропускав в номерах узлов, а конкретно, что макс номер узла < edges_len * 2
 */
int edge_NodeLens(const Edge* edges, int edges_len){
    // будем хранить список bool
    char* nodes = malloc(edges_len*2); // в худшем случае у нас у каждого ребра свой узел начала и конца
    for (size_t i = 0; i < edges_len; i++){
        Edge cur = edges[i];
        nodes[cur.from] = 1;
        nodes[cur.to]   = 1;
    }
    size_t len = 0;
    for(size_t i = 0; i < edges_len*2; i++){
        len += nodes[i]; // считаем все узлы
    }
    free(nodes);
    return len;
}


typedef struct Node {
    int edges_len;
    struct {
        int to;
        int weight;
    } edges[];
} Node;

Node** edges2nodes (Edge edges[], int edges_len, int *nodes_len){
    *nodes_len = edge_NodeLens(edges, edges_len);
    Node** nodes = calloc(*nodes_len, sizeof(Node*));

    for(size_t i = 0; i < edges_len; i++){
        Edge cur = edges[i];
        Node* n = nodes[cur.from];
        // эээ не, тут много думать надо
    }
    return NULL;
}

void dijkstra (Node* nodes, int nodes_len){

}

struct Bellman_Ford_Result {
    int n;
    int* dists;
    int* prevs; // по i храниться предыдущий в пути
};
struct Bellman_Ford_Result Bellman_Ford (Edge* edges, int edges_len, int from){
    int V_len = edge_NodeLens(edges, edges_len);
    int *d = malloc(sizeof(int)*V_len);
    int *prevs = malloc(sizeof(int)*V_len);
    for(size_t i = 0; i < V_len; i++){
        prevs[i] = -1;
        d[i] = INT_MAX >> 4; // бага с переполнением
    }
    d[from] = 0;

    for(size_t i = 0; i < V_len - 1; i++){
        for (size_t e = 0; e < edges_len; e++){
            Edge cur = edges[e];
            long t = d[cur.from] + cur.weight; // тут бага была
            if (d[cur.to] > t){
                d[cur.to] = t;
                prevs[cur.to] = cur.from;
            }
        }
    }

    // В полном Беллмана-Форда, надо ещё проверить на отрицательные циклы

    return (struct Bellman_Ford_Result){V_len, d, prevs};
}
char* Bellman_mask_path_to(struct Bellman_Ford_Result r, int to){
    char *path = calloc(r.n, sizeof(char));
    while(r.prevs[to] != -1){
        path[to] = 1;
        to = r.prevs[to];
    }
    path[to] = 1;
    return path;
}

struct Floyd_Warshall_Result {
    int V; /// число узлов
    int* Dist; /// матрица
    int* Prev; /// матрица
};
struct Floyd_Warshall_Result Floyd_Warshall(Edge* edges, int edges_len){
    int V = edge_NodeLens(edges, edges_len);
    int* dist = malloc(V*V * sizeof(int));
    int* prev = malloc(V*V * sizeof(int));
    for (size_t i = 0; i < V*V; i++){
        dist[i] = INT_MAX >> 4;
        prev[i] = -1;
    }


    for (size_t i = 0; i < edges_len; i++){
        Edge cur = edges[i];
        dist[cur.from*V + cur.to] = cur.weight;
        prev[cur.from*V + cur.to] = cur.from;
    }
    for (size_t i = 0; i < V; i++){
        dist[i*V + i] = 0;
        prev[i*V + i] = i;
    }
    for(size_t k = 0; k < V; k++){
        for (size_t i = 0; i < V; i++){
            if (k ==  i) continue;
            for(size_t j = 0; j < V; j++){
                if (k == j || j == i) continue;
                int t = dist[i*V + k] + dist[k*V + j];
                if (dist[i*V + j] > t){
                    dist[i*V + j] = t;
                    prev[i*V + j] = prev[k*V + j];
                }
            }
        }
    }

    return (struct Floyd_Warshall_Result){.V = V, .Dist = dist, .Prev = prev};
}

char* Floyd_Warshall_path (struct Floyd_Warshall_Result r, int from, int to){
    if (r.Prev[from*r.V + to] == -1) return NULL;

    char* path = calloc(r.V, sizeof(char));
    path[to] = 1;
    while(from != to){
        to = r.Prev[from*r.V + to];
        path[to] = 1;
    }
    return path;
} 

// Я НЕ ЗНАЮ КАК ОНО РАБОТАЕТ, т.к. может происходить обращение в память за 9-ым узлом

// int main(int argc, char** argv){
//     struct Bellman_Ford_Result r = Bellman_Ford(graph, 16, 1);
//     struct Floyd_Warshall_Result rf = Floyd_Warshall(graph, 16);
    
//     char* mask_path = Bellman_mask_path_to(r, 7);
//     // char* mask_path = Floyd_Warshall_path(rf, 1, 7);

//     printf("Кратчайший путь от %d до %d = %d\n", 1, 7, rf.Dist[1*rf.V + 7]);

//     if (argc > 1){
//         FILE* f = fopen(argv[1], "w");
//         if (f)
//             dumpEdges2dot(16, graph, mask_path, r.n, f);
//     }
    

//     return 0;
// }
