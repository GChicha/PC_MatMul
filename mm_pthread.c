#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX 5
#define MIN 1

#ifndef N_THREAD
#ifdef SEQ
#define N_THREAD 1
#else
#define N_THREAD 8
#endif
#endif

struct thread_start {
    int **matriz1, **matriz2;
    int i,j;
    int n;
    int *result;
};

void *multi(void *x) {
    struct thread_start *vars = (struct thread_start*)x;

    int v[vars->n];
    for (int i = 0; i < vars->n; i++) {
        v[i] = vars->matriz2[i][vars->j];
    }

    int soma = 0;
    for (int i = 0; i < vars->n; i++) {
        soma += vars->matriz1[vars->i][i] * v[i];
    }

    *vars->result = soma;

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

    pthread_t *vet_thread = (pthread_t *)calloc(n*n, sizeof(pthread_t));

    struct thread_start *vars = NULL;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vars = (struct thread_start*)calloc(1, sizeof(struct thread_start));
            vars->matriz1 = matriz1;
            vars->matriz2 = matriz2;
            vars->i = i;
            vars->j = j;
            vars->n = n;
            vars->result = &result[i][j];
            #ifdef SEQ
            multi(vars);
            #else
            pthread_create(&vet_thread[j + i*n], NULL, multi, vars);
            #endif
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pthread_join(vet_thread[j + i*n], NULL);
        }
    }
    free(vet_thread);
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
