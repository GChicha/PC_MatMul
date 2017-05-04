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
    int **matriz1, **matriz2, **result;
    int inicio;
    int fim;
    int n;
};

void *multi(void *x) {
    struct thread_start *vars = (struct thread_start*)x;

    for (int i = vars->inicio; i < vars->fim; i++) {
        for (int j = 0; j < vars->n; j++) {
            for (int k = 0; k < vars->n; k++) {
                vars->result[i][j] += vars->matriz1[i][k] * vars->matriz2[k][j];
            }
        }
    }

    free(vars);
    return NULL;
}


int main(int argc, char const *argv[]) {
    srand(2);
    int n = atoi(argv[1]);

    int **matriz1 = (int **)calloc(n, sizeof(int*));
    int **matriz2 = (int **)calloc(n, sizeof(int*));

    int **result = (int **)calloc(n, sizeof(int*));

    #ifdef DEBUG
    printf("Matriz1:\n");
    #endif
    for (int i = 0; i < n; i++) {
        matriz1[i] = (int *)calloc(n, sizeof(int));
        matriz2[i] = (int *)calloc(n, sizeof(int));
        result[i] = (int *)calloc(n, sizeof(int));
        for (int j = 0; j < n; j++) {
            matriz1[i][j] = (rand() % (MAX + 1 - MIN)) + MIN;
            matriz2[i][j] = (rand() % (MAX + 1 - MIN)) + MIN;
            #ifdef DEBUG
            printf("%4d", matriz1[i][j]);
            #endif
        }
        #ifdef DEBUG
        printf("\n");
        #endif
    }
    #ifdef DEBUG
    printf("\n");

    printf("Matriz2:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", matriz2[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    #endif

    #ifndef SEQ
    pthread_t *vet_thread = (pthread_t *)calloc(N_THREAD, sizeof(pthread_t));
    #endif

    struct thread_start *vars = NULL;

    for (int i = 0; i < N_THREAD; i++) {
        vars = (struct thread_start*)calloc(1, sizeof(struct thread_start));
        vars->matriz1 = matriz1;
        vars->matriz2 = matriz2;
        vars->result = result;
        vars->inicio = (i*n)/N_THREAD;
        vars->fim = ((i+1)*n)/N_THREAD;
        vars->n = n;
        #ifdef SEQ
        multi(vars);
        #else
        pthread_create(&vet_thread[i], NULL, multi, vars);
        #endif
    }

    #ifndef SEQ
    for (int i = 0; i < N_THREAD; i++) {
        pthread_join(vet_thread[i], NULL);
    }
    free(vet_thread);
    #endif

    #ifdef DEBUG
    printf("Resultado:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d  ", result[i][j]);
        }
        printf("\n");
    }
    #endif
    return 0;
}
