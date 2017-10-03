#ifndef SEQ
#include <pthread.h>
#endif

#include <stdlib.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define MAX 5
#define MIN 1

#ifdef SEQ
#define N_THREAD 1
#else
#ifndef N_THREAD
#define N_THREAD 8
#endif
#endif

struct thread_start {
    int **matrix1, **matrix2, **result;
    int start;
    int end;
    int n;
};

void *multi(void *x) {
    struct thread_start *input = (struct thread_start*)x;

    for (int i = input->start; i < input->end; i++) {
        for (int j = 0; j < input->n; j++) {
            for (int k = 0; k < input->n; k++) {
                input->result[i][j] += input->matrix1[i][k] * input->matrix2[k][j];
            }
        }
    }

    free(input);
    return NULL;
}


int main(int argc, char const *argv[]) {
    srand(2);
    int n = atoi(argv[1]);

    int **matrix1 = (int **)calloc(n, sizeof(int*));
    int **matrix2 = (int **)calloc(n, sizeof(int*));

    int **result = (int **)calloc(n, sizeof(int*));

    #ifdef DEBUG
    printf("Matrix1:\n");
    #endif
    for (int i = 0; i < n; i++) {
        matrix1[i] = (int *)calloc(n, sizeof(int));
        matrix2[i] = (int *)calloc(n, sizeof(int));
        result[i] = (int *)calloc(n, sizeof(int));
        for (int j = 0; j < n; j++) {
            matrix1[i][j] = (rand() % (MAX + 1 - MIN)) + MIN;
            matrix2[i][j] = (rand() % (MAX + 1 - MIN)) + MIN;
            #ifdef DEBUG
            printf("%4d", matrix1[i][j]);
            #endif
        }
        #ifdef DEBUG
        printf("\n");
        #endif
    }
    #ifdef DEBUG
    printf("\n");

    printf("Matrix2:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", matrix2[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    #endif

    #ifndef SEQ
    pthread_t *vet_thread = (pthread_t *)calloc(N_THREAD, sizeof(pthread_t));
    #endif

    struct thread_start *input = NULL;

    for (int i = 0; i < N_THREAD; i++) {
        input = (struct thread_start*)calloc(1, sizeof(struct thread_start));
        input->matrix1 = matrix1;
        input->matrix2 = matrix2;
        input->result = result;
        input->start = (i*n)/N_THREAD;
        input->end = ((i+1)*n)/N_THREAD;
        input->n = n;
        #ifdef SEQ
        multi(vars);
        #else
        pthread_create(&vet_thread[i], NULL, multi, input);
        #endif
    }

    #ifndef SEQ
    for (int i = 0; i < N_THREAD; i++) {
        pthread_join(vet_thread[i], NULL);
    }
    free(vet_thread);
    #endif

    #ifdef DEBUG
    printf("Result:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d  ", result[i][j]);
        }
        printf("\n");
    }
    #endif
    return 0;
}
