/*
Salma Amin
samin06
1281885
Oct 31 2025
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 20
#define COLS 10

// Structure to represent an object (row)
typedef struct {
    int key;           // Sum of first three integers
    int numbers[10];   // All 10 numbers from the row
} Object;

// read file 
int readfile(char* f, int d[ROWS][COLS]) {
    FILE* file = fopen(f, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File '%s' does not exist.\n", f);
        return 0;
    }
    
    int c = 0;
    char s[3];
    int i, j;
    
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (fscanf(file, "%2s", s) == 1) {
                int n = atoi(s);
                if (strlen(s) == 2) {
                    d[i][j] = n;
                    c++;
                } else {
                    fprintf(stderr, "Error: Input contains non-2-digit number: %s\n", s);
                    fclose(file);
                    return 0;
                }
            } else {
                fprintf(stderr, "Error: Input does not contain 200 integers.\n");
                fclose(file);
                return 0;
            }
        }
    }
    
    fclose(file);
    
    if (c != 200) {
        fprintf(stderr, "Error: Input does not contain exactly 200 integers.\n");
        return 0;
    }
    
    return 1;
}

// function to create objects
void makeobjects(int d[ROWS][COLS], Object o[ROWS]) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            o[i].numbers[j] = d[i][j];
        }
        // Calculate key (sum of first three integers)
        o[i].key = d[i][0] + d[i][1] + d[i][2];
    }
}

// swap function 
void swapstuff(Object o[ROWS], int a, int b) {
    Object t;
    int i;
    t.key = o[a].key;
    for (i = 0; i < 10; i++) {
        t.numbers[i] = o[a].numbers[i];
    }
    
    o[a].key = o[b].key;
    for (i = 0; i < 10; i++) {
        o[a].numbers[i] = o[b].numbers[i];
    }
    
    o[b].key = t.key;
    for (i = 0; i < 10; i++) {
        o[b].numbers[i] = t.numbers[i];
    }
}

// PercolateDown function 
void MaxHeapPercolateDown(Object objects[ROWS], int nodeIndex, int arraySize) {
    int childIndex = 2 * nodeIndex + 1;
    int value = objects[nodeIndex].key;

    while (childIndex < arraySize) {
        // Find the max among the node and all the node's children
        int maxValue = value;
        int maxIndex = -1;
        int i;
        
        for (i = 0; i < 2 && i + childIndex < arraySize; i++) {
            if (objects[i + childIndex].key > maxValue) {
                maxValue = objects[i + childIndex].key;
                maxIndex = i + childIndex;
            }
        }

        if (maxValue == value) {
            return;
        }
        else {
            // Swap the objects
            swapstuff(objects, nodeIndex, maxIndex);
            nodeIndex = maxIndex;
            childIndex = 2 * nodeIndex + 1;
            value = objects[nodeIndex].key; // Update value for next iteration
        }
    }
}

// build max-heap 
void buildheap(Object o[ROWS]) {
    int i;
    // Start from the last non-leaf node and go up to the root
    for (i = ROWS / 2 - 1; i >= 0; i--) {
        MaxHeapPercolateDown(o, i, ROWS);
    }
}

//function to reconstruct the data array 
void putbackdata(int d[ROWS][COLS], Object o[ROWS]) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            d[i][j] = o[i].numbers[j];
        }
    }
}

// display the heap 
void printheap(int d[ROWS][COLS]) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            printf("%02d ", d[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char* f = "f.dat";
    int data[ROWS][COLS];
    Object objs[ROWS];
    
    // read file
    if (!readfile(f, data)) {
        return 1;
    }
    
    // make objects
    makeobjects(data, objs);
    
    // make heap
    buildheap(objs);
    
    // put back
    putbackdata(data, objs);
    
    // print
    printheap(data);
    
    return 0;
}
