#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void menu() {
    printf("1. Добавить вершину\n");
    printf("2. Удалить вершину\n");
    printf("3. Добавить ребро\n");
    printf("4. Удалить ребро\n");
    printf("5. Печать графа\n");
    printf("6. Обход графа (поиск в ширину)\n");
    printf("7. Кратчайшее расстояние\n");
    printf("8. Распределить наследство\n");
    printf("9. Сохранить как файл graphviz\n");
    printf("10. Прочитать граф из файла, распределить наследство и сохранить как файл graphviz\n");
    printf("0. Выйти\n");
}

int main() {
    

    Graph* graph = create_graph(10);
    int choice;
    char name1[50], name2[50];
    Gender gender;
    int birth_year, death_year;
    double amount;
    char filename[100];
   

    do {
        menu();
        printf("Введите число: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите имя, пол (0 для мужского, 1 для женского), год рождения, год смерти: ");
                scanf("%s %d %d %d", name1, (int*)&gender, &birth_year, &death_year);
                add_vertex(graph, name1, gender, birth_year, death_year);
                break;
            case 2:
                printf("Введите имя: ");
                scanf("%s", name1);
                remove_vertex(graph, name1);
                break;
            case 3:
                printf("Введите два имени, чтобы добавить ребро: ");
                scanf("%s %s", name1, name2);
                add_edge(graph, name1, name2);
                break;
            case 4:
                printf("Введите два имени, чтобы удалить ребро: ");
                scanf("%s %s", name1, name2);
                remove_edge(graph, name1, name2);
                break;
            case 5:
                print_graph(graph);
                break;
            case 6:
                printf("Введите имя вершины, с которой начинать обход: ");
                scanf("%s", name1);
                bfs(graph, name1);
                break;
            case 7:
                printf("Введите два имени для поиска кратчайшего расстояния: ");
                scanf("%s %s", name1, name2);
                shortest_path(graph, name1, name2);
                break;
            case 8:
                printf("Введите имя предка и размер завещания: ");
                scanf("%s %lf", name1, &amount);
                distribute_inheritance(graph, name1, amount, 0, 1);
                break;
            case 9:
                printf("Сохранение файла graphviz: ");
                graphviz_output(graph, "graph.dot");
                system("dot -Tpng graph.dot -o graph.png");
                break;
			case 10:
                printf("Введите имя файла c родословной: ");
                scanf("%s", filename);
                Graph* graph_from_file = read_graph_from_file(filename);
                if (graph_from_file == NULL) {
                    printf("Ошибка чтения графа из файла.\n");
                    break;
                }
                
                printf("Введите имя предка и размер завещания: \n ");
                printf("<Предок по умолчанию>: Новиков Кирилл Кириллович \n");
                strcpy(name2, "Новиков Кирилл Кириллович");

                /* Код для чтения имени предка из консоли
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
                fgets(name2, sizeof(name1), stdin);
                */

                scanf("%lf", &amount);
                
                double* inheritance = distribute_inheritance(graph_from_file, name2, amount, 1, 0);
                if (inheritance != NULL) {
                    add_inheritance_to_name(graph_from_file, inheritance);
                    free(inheritance);
                }

                printf("Введите имя файла вывода: ");
                scanf("%s", filename);
                graphviz_output(graph_from_file, filename);

                free_graph(graph_from_file);
                break;                   
            case 0:
                printf("Выход...\n");
                break;
            default:
                printf("Некорректный ввод.\n");
                break;
        }
    } while (choice != 0);

    free_graph(graph);

    return 0;
}
