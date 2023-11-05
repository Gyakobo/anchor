#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int* arr = (int*)malloc(50 * sizeof(int));

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int n = omp_get_num_threads();
        int i;

        for (i = id; i<50; ++i) {
            arr[i] = 2;
        }
    } 

    int i; 
    for (i = 0; i<50; ++i) {
        printf("x: %d\n", arr[i]);
    }
}


