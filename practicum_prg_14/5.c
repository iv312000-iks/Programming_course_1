#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

#define LIST_SIZE_TOP 10
#define LIST_SIZE_BUT 20

struct room {
    char name[50]; 
    int level; 
    int number; 
    int resolution;
};

struct node {
    struct node* prev;  
    struct node* up;    
    struct node* down; 
    struct node* next;  
    struct room data;   
};

typedef struct node node_t;

node_t* create_node(struct room data);
void print_menu();
void print_cur_node(node_t* cur);
node_t* build_list(int size1, int size2);
void free_list(node_t* start);
struct room generate_room(int index);

int main() {
    node_t* S = build_list(LIST_SIZE_TOP, LIST_SIZE_BUT);
    node_t* cur = S;
    
    printf("\n--- LIST NAVIGATION ---\n");
    printf("Cur pos: S (a1)\n\n");
    print_cur_node(cur);
    print_menu();

    char choice;
    while (1) {
        printf("\n> ");
        choice = _getch();
        printf("%c\n", choice);
        
        switch (choice) {
            case 'w':  // вверх
                if (cur->up != NULL) {
                    cur = cur->up;
                    printf("\nMoved up\n");
                    print_cur_node(cur);
                } else {
                    printf("\nCannot move up\n");
                    printf("Go back to the beginning (S)? (y/n): ");
                    char ans = _getch();
                    printf("%c\n", ans);
                    if (ans == 'y') {
                        cur = S;
                        printf("\nBack to S (a1)\n");
                        print_cur_node(cur);
                    } else {
                        printf("Ok, dude\n");
                        print_cur_node(cur);
                    }
                }
                break;
                
            case 's':  // вниз
                if (cur->down != NULL) {
                    cur = cur->down;
                    printf("\nMoved down\n");
                    print_cur_node(cur);
                } else {
                    printf("\nCannot move down\n");
                    printf("Go back to the beginning (S)? (y/n): ");
                    char ans = _getch();
                    printf("%c\n", ans);
                    if (ans == 'y') {
                        cur = S;
                        printf("\nBack to S (a1)\n");
                        print_cur_node(cur);
                    } else {
                        printf("Ok, dude\n");
                        print_cur_node(cur);
                    }
                }
                break;
                
            case 'a':  // влево
                if (cur->prev != NULL) {
                    cur = cur->prev;
                    printf("\nMoved left\n");
                    print_cur_node(cur);
                } else {
                    printf("\nCannot move left\n");
                    printf("Go back to the beginning (S)? (y/n): ");
                    char ans = _getch();
                    printf("%c\n", ans);
                    if (ans == 'y') {
                        cur = S;
                        printf("\nBack to S (a1)\n");
                        print_cur_node(cur);
                    } else {
                        printf("Ok, dude\n");
                        print_cur_node(cur);
                    }
                }
                break;
                
            case 'd':  // вправо
                if (cur->next != NULL) {
                    cur = cur->next;
                    printf("\nMoved right\n");
                    print_cur_node(cur);
                } else {
                    printf("\nCannot move right\n");
                    printf("Go back to the beginning (S)? (y/n): ");
                    char ans = _getch();
                    printf("%c\n", ans);
                    if (ans == 'y') {
                        cur = S;
                        printf("\nBack to S (a1)\n");
                        print_cur_node(cur);
                    } else {
                        printf("Ok, dude\n");
                        print_cur_node(cur);
                    }
                }
                break;
                
            case 'q':
                printf("\nExiting the program.\n");
                free_list(S);
                return 0;
        }
    }
    
    return 0;
}

node_t* build_list(int size1, int size2) {
    int size = size1 + size2;
    int big_s = size1 > size2 ? size1 : size2;
    
    // массив указателей на узлы
    node_t* nodes[size + 1];
    
    // создаём все узлы
    for (int i = 1; i <= size; i++) {
        struct room r = generate_room(i);
        nodes[i] = create_node(r);
        snprintf(nodes[i]->data.name, sizeof(nodes[i]->data.name), "a%d", i);
    }
    
    // a2 вверх на a1
    if (size >= 2) {
        nodes[2]->up = nodes[1];
    }

    if(big_s == size1) {
        // связи для верхнего ряда нечёт
        for (int i = 1; i <= size1 - 2; i += 2) {
            if (i + 2 <= size1) {
                nodes[i]->next = nodes[i + 2];
            }
        }
        // связи для нижнего ряда чёт
        for (int i = 2; i <= size2; i += 2) {
            if (i - 2 >= 2) {
                nodes[i]->prev = nodes[i - 2];
            }
        }
        // вертикальные связи
        for (int i = 3; i <= size2 - 1; i += 2) {
            if (i + 1 <= size2) {
                nodes[i]->down = nodes[i + 1];
            }
        }
    } else {
        // связи для верхнего ряда нечёт
        for (int i = 1; i <= size1 - 2; i += 2) {
            if (i + 2 <= size1) {
                nodes[i]->next = nodes[i + 2];
            }
        }
        // связи для нижнего ряда чёт
        for (int i = 2; i <= size2; i += 2) {
            if (i - 2 >= 2) {
                nodes[i]->prev = nodes[i - 2];
            }
        }
        // вертикальные связи
        for (int i = 3; i <= size1 - 1; i += 2) {
            if (i + 1 <= size1) {
                nodes[i]->down = nodes[i + 1];
            }
        }
    }   
    return nodes[1];
}

node_t* create_node(struct room data) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->up = NULL;
    new_node->down = NULL;
    return new_node;
}

struct room generate_room(int index) {
    struct room r;
    snprintf(r.name, sizeof(r.name), "Room_%d", index);
    r.level = rand() % 10 + 1;
    r.number = rand() % 100;
    r.resolution = rand() % 100 + 10;
    return r;
}

void print_cur_node(node_t* cur) {
    printf("\n--- cur node ---\n");
    printf("  name: %s\n", cur->data.name);
    printf("  level: %d\n", cur->data.level);
    printf("  number: %d\n", cur->data.number);
    printf("  resolution: %d\n", cur->data.resolution);
    printf("  next -> %s\n", cur->next ? cur->next->data.name : "NULL");
    printf("  prev -> %s\n", cur->prev ? cur->prev->data.name : "NULL");
    printf("  up -> %s\n", cur->up ? cur->up->data.name : "NULL");
    printf("  down -> %s\n", cur->down ? cur->down->data.name : "NULL");
    printf("--------------------\n");
}

void print_menu() {
    printf("\n--- controlling ---\n");
    printf("w - up\n");
    printf("s - down\n");
    printf("a - left\n");
    printf("d - right\n");
    printf("q - exit\n");
    printf("---------------------\n");
}

void free_list(node_t* start) {
    node_t* cur = start;
    node_t* next;
    node_t* visited[100] = {NULL};
    int visited_count = 0;
    
    while (cur != NULL) {
        int already_freed = 0;
        for (int i = 0; i < visited_count; i++) {
            if (visited[i] == cur) {
                already_freed = 1;
                break;
            }
        }
        
        if (!already_freed) {
            visited[visited_count++] = cur;
            next = cur->next;
            free(cur);
            cur = next;
        } else {
            break;
        }
    }
}

