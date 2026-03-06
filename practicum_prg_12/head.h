#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define N 20

struct flight {
    int flight_number;
    char destination[50];
    int travel_time;
    int ticket_price;
};

void fill_array(int size, struct flight arr[]);
void bubble_sort(int size, struct flight arr[]);
void print_array(int size, struct flight arr[]);

#endif