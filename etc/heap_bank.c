#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// struct heap_bank heap[64];

struct heap_bank {
    uint8_t bytes[8];
    int allocated;
};


// size referring to ceiling division number of 64b banks req to store value
int heap_find_tail(struct heap_bank heap_arr[], int size) {

    int i = 0;
    int count = 0;

    while (count < 5) {

        if (i > 63)
            return -1;
    
        if (heap_arr[i].allocated == 0)
            count++;
        else 
            count = 0;
        
        i++;
    }

    return i - 5;
}

int vm_malloc(struct heap_bank heap_arr[], int size) {
    int start_index = heap_find_tail(heap_arr, size);

    for (int i = start_index; i < start_index + size; i++) {
        heap_arr[i].allocated = 1;
    }

    return start_index;

}

int vm_free(struct heap_bank heap_arr[], int len) {



    return 1;
}
