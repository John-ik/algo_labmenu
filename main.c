#include <stdlib.h>
// #include <locale.h>
// #include <ncurses.h> /* ncurses.h включает stdio.h */

#include "graph.h"

int  lexing(char *filename);
int iteractive_list();
int iteractive_sort();
int iteractive_tree();

int main(int argc, char** argv){
    if ( ! (argc > 1 && argc < 4) ){
        return 1;
    }

    int choice = atoi(argv[1]);

    switch(choice){
        case 1:
            lexing(argv[2]);
            break;
        case 2:
            iteractive_list();
            break;
        case 3:
            iteractive_sort();
            break;
        case 4:
            iteractive_tree();
            break;
        case 5:{
            struct Bellman_Ford_Result r = Bellman_Ford(graph, 16, 1);
            struct Floyd_Warshall_Result rf = Floyd_Warshall(graph, 16);
            
            char* mask_path = Bellman_mask_path_to(r, 7);
            // char* mask_path = Floyd_Warshall_path(rf, 1, 7);

            printf("Кратчайший путь от %d до %d = %d по Флойда-Уоршелла\n", 1, 7, rf.Dist[1*rf.V + 7]);
            printf("Кратчайший путь от %d до %d = %d по Белмана-Форда\n", 1, 7, r.dists[7]);

            if (argc > 2){
                FILE* f = fopen(argv[2], "w");
                if (f)
                    dumpEdges2dot(16, graph, mask_path, r.n, f);
            }
            break;
        }
    }

    return 0;
}
