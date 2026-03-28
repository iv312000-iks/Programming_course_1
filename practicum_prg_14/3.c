#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct room {
    char name[50]; 
    int level; 
    int number; 
    int resolution;
};

struct node {
    struct room data;   // данные
    struct node* next;  // указатель на следующий узел
};

typedef struct node node_t;

void add_to_end(node_t** head, struct room new_data);
void print(node_t* head);
void free_list(node_t* head);
struct room generate();

int main() {
    srand(time(NULL));
    int num;
    printf("Enter the number of items in the list: ");
    if (scanf("%d", &num) <= 0) {
        printf("Error\n");
        return 1;
    }

    // Ссздание списка
    node_t* head = NULL;
    for (int i = 0; i < num; i++) {
        struct room new_room = generate();
        add_to_end(&head, new_room);
    }

    printf("Generated list of rooms:\n");
    print(head);
    free_list(head);
    return 0;
}

void add_to_end(node_t** head, struct room new_data) {
    // создаём новый узел
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    new_node->data = new_data;
    new_node->next = NULL;

    // если список пуст новый узел становится головой
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    // иначе в конец списка
    struct node* cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_node;
}

void print(node_t* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    node_t* cur = head;
    int i = 1;
    while (cur != NULL) {
        printf("room #%d:\n", i++);
        printf("  name: %s\n", cur->data.name);
        printf("  level: %d\n", cur->data.level);
        printf("  number: %d\n", cur->data.number);
        printf("  resolution: %d\n", cur->data.resolution);
        printf("\n");
        cur= cur->next;
    }
}

struct room generate() {
    struct room r;
    int name_num = rand() % 1000;
    snprintf(r.name, sizeof(r.name), "Room_%d", name_num);

    r.level = rand() % 10 + 1;    
    r.number = rand() % 100;           
    r.resolution = rand() % 100 + 10;     
    return r;
}

void free_list(node_t* head) {
    node_t* cur = head;
    while (cur != NULL) {
        node_t* tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}