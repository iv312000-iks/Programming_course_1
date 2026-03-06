#include "head.h"

void fill_array(int size, struct flight arr[]) {
    const char* destination_list[] = {"Los-Angeles", "London", "Hong-Kong", "Spain", "Italy"};
    int destination_count = sizeof(destination_list) / sizeof(destination_list[0]);

    srand(time(0));

    for(int i = 0; i < size; i++) {
        arr[i].flight_number = 100 + rand() % 900;

        int dest_index = rand() % destination_count;
        strcpy(arr[i].destination, destination_list[dest_index]);

        arr[i].travel_time = 1 + rand() % 22;

        arr[i].ticket_price = 100 + rand() % 1501;
    }

}


void bubble_sort(int size, struct flight arr[]) {
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(arr[j].flight_number > arr[j+1].flight_number) {
                struct flight t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
}


void print_array(int size, struct flight arr[]) {
    printf("Flight Schedule\n");
    printf("--------------------------------------------------------------------------\n");
    printf("|#  | Flight Number  |   Destination   |  Travel Time  |  Ticket Price   |\n");
    printf("--------------------------------------------------------------------------\n");

    for(int i = 0; i < size; i++) {
        printf("| %d | %14d | %-15s | %5d  hours  | $%14d |\n",
            i+1, 
            arr[i].flight_number,
            arr[i].destination,
            arr[i].travel_time,
            arr[i].ticket_price);

    }
    printf("--------------------------------------------------------------------------\n");

}