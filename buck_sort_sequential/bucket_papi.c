#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "papi.h"


#define tam_bucket 100000   // Tamanho de cada balde
#define num_bucket 10       // Número de buckets
#define max 10              // Máximo valor a ordenar = max * num_bucket

// Definição de 'bucket'
typedef struct {
    int topo;       // Diz quantos elementos estão armazenados no balde
    int *balde;     // Array de inteiros (o balde em sí)
}bucket;

// Cabeçalho das funções
void bucket_sort(int v[], int tam);                
void bubble(int v[], int tam);


// Funções
void bucket_sort(int v[], int tam){
    // v é o array a ordenar, tam é o seu tamanho

    // bucket é o "vetor" de baldes
    bucket b[num_bucket];
    int i,j,k;
    
    // Inicializa todos os "topo" e aloca a memoria para cada balde
    for(i=0;i<num_bucket;i++)  {          
        b[i].topo=0;
        b[i].balde = malloc(sizeof(int) * tam_bucket);
    }
    

    // Colocação dos elementos nos baldes correspondentes
    for(i=0;i<tam;i++){ 
        int elem = v[i];    // Elemento atual

        // Ao fazer a divisão inteira do elemento atual
        // com o máximo de v, conseguimos obter o indice do balde
        // ao qual esse elemento corresponde
        int x = elem/max;   
        b[x].balde[b[x].topo++] = elem; // Coloca o elemento no balde correspondente e incrementa o topo desse balde
    }
    
    // Ordena os baldes
    for(i=0;i<num_bucket;i++) {
        // sse houver elementos no balde é que vamos ordenar (topo >= 1 - TODO: VERIRICAR)
        if  (b[i].topo) {
            bubble(b[i].balde,b[i].topo);
        }
    }             

    // Põe os elementos dos baldes de volta no vetor
    i=0;
    // Para cada balde
    for(j=0;j<num_bucket;j++){   
        // Para cada elemento do balde
        for(k=0;k<b[j].topo;k++){
            // Vamos coloca-lo no vetor inicial
            v[i]=b[j].balde[k];

            // Incrementar o indice no vetor inicial
            i++;
        }
        // Libertar espaço do bucket
        free(b[j].balde);
    }
}

void bubble(int v[],int tam){
    int i,j,temp,flag;
    if(tam)
        for(j=0;j<tam-1;j++){
            flag=0;
            for(i=0;i<tam-1;i++){
                if(v[i+1]<v[i]){
                    temp=v[i];
                    v[i]=v[i+1];
                    v[i+1]=temp;
                    flag=1;
                }
            }
            if(!flag)
                break;
        }
}

// Código para testar o algoritmo

void random_vector(int *v, int N) {
    int i;
    for (i = 0; i < N; i++) {
        v[i] = rand() % (max * num_bucket);
    }
}

char *is_sorted(int *v, int N) {
    int r = 1;
    for (int i = 0; i < N - 1; i++) {
        if (v[i] > v[i + 1])
            r = 0;
    }
    return (r) ? "Yes" : "No";
}

void print_array(int v[], int N) {
    int i;
    for (i = 0; i < N; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}


//**************************//
//******* PAPI STUFF *******//
//**************************//

void (*func)(float *, float *, float *, int);

// PAPI events to monitor
#define NUM_EVENTS 4
int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_L1_DCM, PAPI_L2_DCM };

// PAPI counters' values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];
int retval, EventSet=PAPI_NULL;

// number of times the function is executed and measured
#define NUM_RUNS 5

int main (int argc, char *argv[]) {
    long long start_usec, end_usec, elapsed_usec, min_usec=0L;
    int version, i, run;
    int num_hwcntrs = 0;
    
    fprintf (stdout, "We have %d buckets with %d elem each.\n", num_bucket, tam_bucket);
    
    fprintf (stdout, "\nSetting up PAPI...");
    // Initialize PAPI
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr,"PAPI library init error!\n");
        return 0;
    }
    
    /* create event set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        fprintf(stderr,"PAPI create event set error\n");
        return 0;
    }
    
    /* Get the number of hardware counters available */
    if ((num_hwcntrs = PAPI_num_hwctrs()) <= PAPI_OK)  {
        fprintf (stderr, "PAPI error getting number of available hardware counters!\n");
        return 0;
    }
    fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);
    
    // We will be using at most NUM_EVENTS counters
    if (num_hwcntrs >= NUM_EVENTS) {
        num_hwcntrs = NUM_EVENTS;
    } else {
        fprintf (stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
        return 0;
    }
    
    if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK)  {
        fprintf(stderr,"PAPI library add events error!\n");
        return 0;
    }
    
    
    // INICIALIZAR VETOR ALEATORIO
    fprintf (stdout, "Initializing random vector...");
    int N = tam_bucket;
    int *v;
    
    v = malloc(sizeof(int) * N);
    random_vector(v, N);
    fprintf (stdout, "done!\n");
    
    
    // Warmup caches
    fprintf (stdout, "Warming up caches...");
    bucket_sort(v, N);
    fprintf (stdout, "done!\n");
    
    for (run=0 ; run < NUM_RUNS ; run++) {
        fprintf (stdout, "Initializing random vector...");
        int N = tam_bucket;
        int *v;
        
        v = malloc(sizeof(int) * N);
        random_vector(v, N);
        fprintf (stdout, "done!\n");
        fprintf (stdout, "run=%d - Computing bucket_sort...", run);
        
        // use PAPI timer (usecs) - note that this is wall clock time
        // for process time running in user mode -> PAPI_get_virt_usec()
        // real and virtual clock cycles can also be read using the equivalent
        // PAPI_get[real|virt]_cyc()
        start_usec = PAPI_get_real_usec();
        
        /* Start counting events */
        if (PAPI_start(EventSet) != PAPI_OK) {
            fprintf (stderr, "PAPI error starting counters!\n");
            return 0;
        }
        
        bucket_sort(v, N);
        
        /* Stop counting events */
        if (PAPI_stop(EventSet,values) != PAPI_OK) {
            fprintf (stderr, "PAPI error stoping counters!\n");
            return 0;
        }
        
        end_usec = PAPI_get_real_usec();
        fprintf (stdout, "done!\n");
        
        elapsed_usec = end_usec - start_usec;
        
        if ((run==0) || (elapsed_usec < min_usec)) {
            min_usec = elapsed_usec;
            for (i=0 ; i< NUM_EVENTS ; i++) min_values[i] = values [i];
        }
        
        printf("FORA DO PAPI - Is sorted? %s\n", is_sorted(v, N));

    } // end runs
    fprintf (stdout,"\nWall clock time: %lld usecs\n", min_usec);
    
    // output PAPI counters' values
    for (i=0 ; i< NUM_EVENTS ; i++) {
        char EventCodeStr[PAPI_MAX_STR_LEN];
        
        if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
            fprintf (stdout, "%s = %lld\n", EventCodeStr, min_values[i]);
        } else {
            fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", min_values[i]);
        }
    }

    fprintf (stdout,"\nThat's all, folks\n");
    return 0;
}




