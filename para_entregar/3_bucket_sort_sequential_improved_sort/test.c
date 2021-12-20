#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Quick sort of an array of integers
void sort(int v[], int tam) {
    int i,j,x;
    if(tam<=1) return;
    i=0;
    j=tam-1;
    x=v[tam/2];
    do{
        while(v[i]<x) i++;
        while(v[j]>x) j--;
        if(i<=j){
            int aux=v[i];
            v[i]=v[j];
            v[j]=aux;
            i++;
            j--;
        }
    }while(i<=j);
    sort(v,i);
    sort(v+i,tam-i);
}

int v[8] = {4,6,2,67,9,341,1,2};
int N = 8;

// Define print_array
void print_array(int v[], int N) {
    int i;
    for (i = 0; i < N; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}


// Sort v 
void main(int argc, char const *argv[]) {
    sort(v, N);
    print_array(v, N);
}