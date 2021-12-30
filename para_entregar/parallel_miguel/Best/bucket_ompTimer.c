//Compilado em Linux.Sujeito a mudanças caso outro sistema seja utilizado.
// versão ligeiramente melhorada do código de:
//    https://pt.wikipedia.org/wiki/Bucket_sort

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define tam_bucket 10000   // Tamanho de cada balde
#define num_bucket 10       // Número de buckets
#define max 10              // Máximo valor a ordenar = max * num_bucket

// Definição de 'bucket'
typedef struct {
    int topo;       // Diz quantos elementos estão armazenados no balde
    int *balde;     // Array de inteiros (o balde em sí)
}bucket;

// Cabeçalho das funções
void bucket_sort(int v[], int tam);                
void quickSort(int v[], int low, int high);

// Funções
void bucket_sort(int v[], int tam){
    // v é o array a ordenar, tam é o seu tamanho

    // bucket é o "vetor" de baldes
    bucket b[num_bucket];
    int i,j,k;
    
    // Inicializa todos os "topo" e aloca a memoria para cada balde
    //#pragma omp parallel for schedule(guided) num_threads(64)
    for(i=0;i<num_bucket;i++)  {          
        b[i].topo=0;
        b[i].balde = malloc(sizeof(int) * tam_bucket);
    }
    

    // Colocação dos elementos nos baldes correspondentes
    //#pragma omp parallel for num_threads(64)
    for(i=0;i<tam;i++){ 
        int elem = v[i];    // Elemento atual

        // Ao fazer a divisão inteira do elemento atual
        // com o máximo de v, conseguimos obter o indice do balde
        // ao qual esse elemento corresponde
        int x = elem/max;   
        //#pragma omp critical
        b[x].balde[b[x].topo++] = elem; // Coloca o elemento no balde correspondente e incrementa o topo desse balde
    }

    // Ordena os baldes

  // Using shedule
    #pragma omp parallel for schedule(guided) num_threads(64)
    for(i=0;i<num_bucket;i++) {
        // sse houver elementos no balde é que vamos ordenar (topo >= 1 - TODO: VERIRICAR)
        if  (b[i].topo) {
            quickSort(b[i].balde,0,b[i].topo-1);
        }
    }  

    // Põe os elementos dos baldes de volta no vetor
    i=0;
    // Para cada balde
    //#pragma omp parallel for ordered num_threads(64)
    for(j=0;j<num_bucket;j++){   
        // Para cada elemento do balde
        //#pragma omp ordered
        //#pragma omp parallel for schedule(guided) num_threads(64)
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

// Quick Sort

// function to swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(int array[], int low, int high) {
  
  // select the rightmost element as pivot
  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
  }

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}


void ompQuickSort(int array[], int low, int high) {
    if (low < high) {
    
        // find the pivot element such that
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on right of pivot
        int pi = partition(array, low, high);
    
        // recursive call on the left of pivot
        #pragma omp task
        ompQuickSort(array, low, pi - 1);
        
        // recursive call on the right of pivot
        #pragma omp task
        ompQuickSort(array, pi + 1, high);

        #pragma omp taskwait
  }
}


void quickSort(int array[], int low, int high) {
  #pragma omp parallel num_threads(64)
  #pragma omp single nowait
  ompQuickSort(array, low, high);
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

int main(int argc, char const *argv[]) {

    double start;
    double end;
    
    int N = tam_bucket;
    int *v;
    
    v=malloc(sizeof(int) * N);
    random_vector(v, N);
    
    start= omp_get_wtime();
    bucket_sort(v, N);
    end = omp_get_wtime();
    
    printf("Is sorted? %s\n", is_sorted(v, N));
    printf("The time is -%f-\n", end-start);
    return 0;
}

