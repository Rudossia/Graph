#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INF INT_MAX

Graph* create_graph(int initial_capacity) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = (Vertex*)malloc(sizeof(Vertex) * initial_capacity);
    graph->vertex_count = 0;
    graph->vertex_capacity = initial_capacity;
    graph->adjacency_list = (Edge**)malloc(sizeof(Edge*) * initial_capacity);
    for (int i = 0; i < initial_capacity; i++) {
        graph->adjacency_list[i] = NULL;
    }
    return graph;
}

void add_vertex(Graph* graph, const char* name, Gender gender, int birth_year, int death_year) {
    if (graph->vertex_count == graph->vertex_capacity) {
        graph->vertex_capacity *= 2;
        graph->vertices = (Vertex*)realloc(graph->vertices, sizeof(Vertex) * graph->vertex_capacity);
        graph->adjacency_list = (Edge**)realloc(graph->adjacency_list, sizeof(Edge*) * graph->vertex_capacity);
    }
    Vertex new_vertex;
    new_vertex.name = strdup(name);
    new_vertex.gender = gender;
    new_vertex.birth_year = birth_year;
    new_vertex.death_year = death_year;
    graph->vertices[graph->vertex_count] = new_vertex;
    graph->adjacency_list[graph->vertex_count] = NULL;
    graph->vertex_count++;
}

int find_vertex_index(Graph* graph, const char* name) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void remove_vertex(Graph* graph, const char* name) {
    int index = find_vertex_index(graph, name);
    if (index == -1) {
        printf("Вершина не найдена. \n");
        return;
    }

    for (int i = 0; i < graph->vertex_count; i++) {
        if (i == index) continue;
        remove_edge(graph, name, graph->vertices[i].name);
    }

    for (int i = index; i < graph->vertex_count - 1; i++) {
        graph->vertices[i] = graph->vertices[i + 1];
        graph->adjacency_list[i] = graph->adjacency_list[i + 1];
    }

    graph->vertex_count--;
}

void add_edge(Graph* graph, const char* name1, const char* name2) {
    int index1 = find_vertex_index(graph, name1);
    int index2 = find_vertex_index(graph, name2);
    if (index1 == -1 || index2 == -1) {
        printf("Одна или обе вершины не найдены.\n");
        return;
    }

    Edge* new_edge1 = (Edge*)malloc(sizeof(Edge));
    new_edge1->vertex1 = index1;
    new_edge1->vertex2 = index2;
    new_edge1->next = graph->adjacency_list[index1];
    graph->adjacency_list[index1] = new_edge1;

    Edge* new_edge2 = (Edge*)malloc(sizeof(Edge));
    new_edge2->vertex1 = index2;
    new_edge2->vertex2 = index1;
    new_edge2->next = graph->adjacency_list[index2];
    graph->adjacency_list[index2] = new_edge2;
}

void remove_edge(Graph* graph, const char* name1, const char* name2) {
    int index1 = find_vertex_index(graph, name1);
    int index2 = find_vertex_index(graph, name2);
    if (index1 == -1 || index2 == -1) {
        printf("Одна или обе вершины не найдены.\n");
        return;
    }

    Edge** current = &graph->adjacency_list[index1];
    while (*current != NULL) {
        if ((*current)->vertex2 == index2) {
            Edge* to_delete = *current;
            *current = (*current)->next;
            free(to_delete);
            break;
        }
        current = &((*current)->next);
    }

    current = &graph->adjacency_list[index2];
    while (*current != NULL) {
        if ((*current)->vertex2 == index1) {
            Edge* to_delete = *current;
            *current = (*current)->next;
            free(to_delete);
            break;
        }
        current = &((*current)->next);
    }
}

void print_graph(Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        printf("%s: ", graph->vertices[i].name);
        Edge* edge = graph->adjacency_list[i];
        while (edge != NULL) {
            printf("%s ", graph->vertices[edge->vertex2].name);
            edge = edge->next;
        }
        printf("\n");
    }
}

void graphviz_output(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    fprintf(file, "graph G {\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        fprintf(file, "  \"%s\";\n", graph->vertices[i].name);
    }

    for (int i = 0; i < graph->vertex_count; i++) {
        Edge* edge = graph->adjacency_list[i];
        while (edge != NULL) {
            if (i < edge->vertex2) {
                fprintf(file, "  \"%s\" -- \"%s\";\n", graph->vertices[i].name, graph->vertices[edge->vertex2].name);
            }
            edge = edge->next;
        }
    }
    fprintf(file, "}\n");
    fclose(file);
}

void bfs(Graph* graph, const char* start_name) {
    int start_index = find_vertex_index(graph, start_name);
    if (start_index == -1) {
        printf("Начальная вершина не найдена.\n");
        return;
    }

    int* visited = (int*)calloc(graph->vertex_count, sizeof(int));
    int* queue = (int*)malloc(graph->vertex_count * sizeof(int));
    int front = 0, rear = 0;

    visited[start_index] = 1;
    queue[rear++] = start_index;

    while (front < rear) {
        int current = queue[front++];
        printf("%s ", graph->vertices[current].name);

        Edge* edge = graph->adjacency_list[current];
        while (edge != NULL) {
            if (!visited[edge->vertex2]) {
                visited[edge->vertex2] = 1;
                queue[rear++] = edge->vertex2;
            }
            edge = edge->next;
        }
    }
    printf("\n");
    free(visited);
    free(queue);
}

void shortest_path(Graph* graph, const char* name1, const char* name2) {
    int start_index = find_vertex_index(graph, name1);
    int end_index = find_vertex_index(graph, name2);
    if (start_index == -1 || end_index == -1) {
        printf("Одна или обе вершины не найдены.\n");
        return;
    }

    int* distance = (int*)malloc(graph->vertex_count * sizeof(int));
    int* predecessor = (int*)malloc(graph->vertex_count * sizeof(int));
    for (int i = 0; i < graph->vertex_count; i++) {
        distance[i] = -1;
        predecessor[i] = -1;
    }

    int* queue = (int*)malloc(graph->vertex_count * sizeof(int));
    int front = 0, rear = 0;

    distance[start_index] = 0;
    queue[rear++] = start_index;

    while (front < rear) {
        int current = queue[front++];

        Edge* edge = graph->adjacency_list[current];
        while (edge != NULL) {
            if (distance[edge->vertex2] == -1) {
                distance[edge->vertex2] = distance[current] + 1;
                predecessor[edge->vertex2] = current;
                queue[rear++] = edge->vertex2;
            }
            edge = edge->next;
        }
    }

    if (distance[end_index] == -1) {
        printf("Путь не найден.\n");
    } else {
        printf("Длина кратчайшего пути %d\n", distance[end_index]);
        int path_vertex = end_index;
        while (path_vertex != -1) {
            printf("%s ", graph->vertices[path_vertex].name);
            path_vertex = predecessor[path_vertex];
        }
        printf("\n");
    }

    free(distance);
    free(predecessor);
    free(queue);
}


void floyd_warshall(Graph* graph, int** dist) {
    int V = graph->vertex_count;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INF;
            }
        }
    }

    for (int i = 0; i < V; i++) {
        Edge* edge = graph->adjacency_list[i];
        while (edge != NULL) {
            dist[i][edge->vertex2] = 1;
            edge = edge->next;
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
}

double* distribute_inheritance(Graph* graph, const char* ancestor_name, double amount, _Bool silent, _Bool no_return) {
    int start_index = find_vertex_index(graph, ancestor_name);
    
    if (start_index == -1) {
        printf("Потомки не найдены.\n");
        return NULL;
    }

    int ancestor_death_year = graph->vertices[start_index].death_year;
    int V = graph->vertex_count;
    int** dist = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        dist[i] = (int*)malloc(V * sizeof(int));
    }

    floyd_warshall(graph, dist);

    double* inheritance = (double*)calloc(V, sizeof(double));
    double total_weight = 0;

    for (int i = 0; i < V; i++) {
        if (dist[start_index][i] != INF && dist[start_index][i] != 0 && graph->vertices[i].death_year > ancestor_death_year) {
           total_weight += pow(0.5, dist[start_index][i]);
        }
    }

    if (total_weight > 0) {
        for (int i = 0; i < V; i++) {
            if (dist[start_index][i] != INF && dist[start_index][i] != 0 && graph->vertices[i].death_year > ancestor_death_year) {
                inheritance[i] = amount * (pow(0.5, dist[start_index][i]) / total_weight);
            }
        }
    } else {
        printf("Потомки не найдены.\n");
    }

    if (!silent) {
        for (int i = 0; i < V; i++) {
            if (inheritance[i] > 0) {
                printf("%s получает %.2f\n", graph->vertices[i].name, inheritance[i]);
            }
        }
    }

    for (int i = 0; i < V; i++) {
        free(dist[i]);
    }
    free(dist);
    if (no_return) {
        free(inheritance);
        return NULL;
    }
    return inheritance;
}

void add_inheritance_to_name(Graph* graph, double* inheritance) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (inheritance[i] > 0) {
            char new_name[256];
            snprintf(new_name, sizeof(new_name), "%s (%.2f)", graph->vertices[i].name, inheritance[i]);
            free(graph->vertices[i].name);
            graph->vertices[i].name = strdup(new_name);
        }
    }
}

Graph* read_graph_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла.\n");
        return NULL;
    }

    int vertex_count;
    if (fscanf(file, "%d\n", &vertex_count) != 1) {
        printf("Ошибка чтения количества вершин.\n");
        fclose(file);
        return NULL;
    }

    Graph* graph = create_graph(vertex_count);

    for (int i = 0; i < vertex_count; i++) {
        char line[256];
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Ошибка чтения строки.\n");
            continue;
        }

        char lastname[100], firstname[100], patronymic[100], gender_str[10];
        int birth_year, death_year;

        char* token = strtok(line, " ");
        strcpy(lastname, token);

        token = strtok(NULL, " ");
        strcpy(firstname, token);

        token = strtok(NULL, ",");
        strcpy(patronymic, token);

        token = strtok(NULL, ",");
        strcpy(gender_str, token);

        token = strtok(NULL, ",");
        birth_year = atoi(token);

        token = strtok(NULL, ",");
        death_year = atoi(token);

        char full_name[300];
        snprintf(full_name, sizeof(full_name), "%s %s %s", lastname, firstname, patronymic);

        Gender gender;
        if (strcmp(gender_str, "M") == 0) {
            gender = MALE;
        } else if (strcmp(gender_str, "F") == 0) {
            gender = FEMALE;
        } else {
            printf("Некорректный пол для %s\n", full_name);
            continue;
        }

        add_vertex(graph, full_name, gender, birth_year, death_year);
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char name1[100], name2[100];
        char* token;

        // Разбор первой вершины
        token = strtok(line, ",");
        strcpy(name1, token);

        // Разбор второй вершины
        token = strtok(NULL, ",");
        strcpy(name2, token);

        // Удаляем символ новой строки в конце имени
        name2[strcspn(name2, "\n")] = '\0';

        add_edge(graph, name1, name2);
    }

    fclose(file);
    return graph;
}
void free_graph(Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        free(graph->vertices[i].name);
        Edge* edge = graph->adjacency_list[i];
        while (edge != NULL) {
            Edge* to_delete = edge;
            edge = edge->next;
            free(to_delete);
        }
    }
    free(graph->vertices);
    free(graph->adjacency_list);
    free(graph);
}
