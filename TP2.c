#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define K 5
#define T 4
#define D 5

int stock[K];
int ventes_valides = 0;
int ventes_refusees = 0;
int stop = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* vendeur(void* arg) {
    while (!stop) {
        int produit = rand() % K;

        pthread_mutex_lock(&mutex);  
        if (stock[produit] > 0) {
            stock[produit]--;
            ventes_valides++;
        } else {
            ventes_refusees++;
        }
        pthread_mutex_unlock(&mutex);   

        usleep(1000);
    }
    return NULL;
}

void* reapprovisionnement(void* arg) {
    while (!stop) {
        int produit = rand() % K;

        pthread_mutex_lock(&mutex);
        stock[produit] += rand() % 3;
        pthread_mutex_unlock(&mutex);

        usleep(500000);
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < K; i++) {
        stock[i] = 5;
    }

    pthread_t vendeurs[T];
    pthread_t supplier;

    for (int i = 0; i < T; i++) {
        pthread_create(&vendeurs[i], NULL, vendeur, NULL);
    }

    pthread_create(&supplier, NULL, reapprovisionnement, NULL);

    sleep(D);
    stop = 1;

    for (int i = 0; i < T; i++) pthread_join(vendeurs[i], NULL);
    pthread_join(supplier, NULL);

    printf("\n=== Résultats (Mutex) ===\n");
    for (int i = 0; i < K; i++) {
        printf("Article %d : Stock final = %d\n", i, stock[i]);
    }
    printf("Ventes valides = %d\n", ventes_valides);
    printf("Ventes refusées = %d\n", ventes_refusees);

    pthread_mutex_destroy(&mutex);
    return 0;
}
