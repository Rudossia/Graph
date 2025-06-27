#ifndef GRAPH_H
#define GRAPH_H


#include <string.h>
#include <limits.h>

typedef enum Gender{
    MALE,
    FEMALE
} Gender;

typedef struct Vertex{
    char* name;
    Gender gender;
    int birth_year;
    int death_year;
} Vertex;

typedef struct Edge {
    int vertex1;
    int vertex2;
    struct Edge* next;
} Edge;

typedef struct Graph {
    Vertex* vertices;
    int vertex_count;
    int vertex_capacity;
    Edge** adjacency_list;
} Graph;

Graph* create_graph(int initial_capacity);
void add_vertex(Graph* graph, const char* name, Gender gender, int birth_year, int death_year);
int find_vertex_index(Graph* graph, const char* name);
void remove_vertex(Graph* graph, const char* name);
void add_edge(Graph* graph, const char* name1, const char* name2);
void remove_edge(Graph* graph, const char* name1, const char* name2);
void print_graph(Graph* graph);
void graphviz_output(Graph* graph, const char* filename);
void bfs(Graph* graph, const char* start_name);
void shortest_path(Graph* graph, const char* name1, const char* name2);
void floyd_warshall(Graph* graph, int** dist);
double* distribute_inheritance(Graph* graph, const char* ancestor_name, double amount, _Bool silent, _Bool no_return);
void free_graph(Graph* graph);
Graph* read_graph_from_file(const char* filename);
void add_inheritance_to_name(Graph* graph, double* inheritance);

#endif
