//Compilado em Linux.Sujeito a mudanças caso outro sistema seja utilizado.
// versão ligeiramente melhorada do código de:
//    https://pt.wikipedia.org/wiki/Bucket_sort

#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[]) {
    
    int N = tam_bucket;
    int *v;
    
    v = malloc(sizeof(int) * N);
    random_vector(v, N);
    
    bucket_sort(v, N);
    
    printf("Is sorted? %s\n", is_sorted(v, N));
    //print_array(v, N);
    return 0;
}
