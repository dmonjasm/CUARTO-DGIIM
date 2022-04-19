#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void vectorInit(double * vec, int len){
    double f=0.0;
    for(int i=0; i < len; i++){
        f=(double) rand()/ (double)RAND_MAX;
        vec[i]=f;
    }
}

double Min(double * vec, int len){
    double auxiliar=vec[0];

    for(int i=1; i < len; i++){
        if(vec[i] < auxiliar){
            auxiliar = vec[i];
        }
    }

    return auxiliar;
}

int main(int argc, char* argv[]){
    if(argc < 2)
        printf("Modo de ejecución: ./Ejercicio2 <tam_vec>\n");

    else{
        int len = atoi(argv[1]);
        double * vec =  (double *)malloc(sizeof(double)*len);
        vectorInit(vec, len);

        /*
        for(int i=0; i < len; i++){
            printf("%.3lf\n", vec[i]);
        }
        */

        printf("Minimo: %.3lf\n", Min(vec, len));
        free(vec);
    }
    return 0;
}