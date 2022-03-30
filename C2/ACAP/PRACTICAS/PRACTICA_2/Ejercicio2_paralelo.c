#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

void vectorInit(double * vec, int len){
    double f=0.0;
    for(int i=0; i < len; i++){
        f=(double) rand()/ (double)RAND_MAX + rand();
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
    MPI_Init(&argc, &argv);
    MPI_Status status;
    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD , &rank);
    if(argc < 2)
        printf("Modo de ejecución: ./Ejercicio2 <tam_vec>");

    else{
        double * vec = NULL;

        if(rank == 0){
            int nProcs = 0;
            MPI_Comm_size(MPI_COMM_WORLD, &nProcs);


            if(nProcs < 2){
                printf("Error, número de procesos insuficiente. Abortando\n");
                return(-1);
            }

            int len = atoi(argv[1]);
            vec =  (double *)malloc(sizeof(double)*len);
            
            vectorInit(vec, len);

            int workSize = len/nProcs;
            int offset = len%nProcs;
            int start, quantity;

            for(int i=1; i < nProcs; i++){
                start = i*workSize + (i < offset?i:offset);
                quantity = workSize + (i < offset);

                MPI_Send(&(vec[start]), quantity, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }

            quantity = workSize + (0 < offset);

            double min =  Min(vec, quantity);

            MPI_Reduce(&min, 0, 1, MPI_DOUBLE, MPI_MIN, 1, MPI_COMM_WORLD);

            printf("Minimo local: %.3lf\n", min);
        }

        else{
            MPI_Probe( 0,  MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            int quantity = 0;
            MPI_Get_count(&status, MPI_DOUBLE, &quantity);
            vec =  (double *)malloc(sizeof(double)*quantity); 
            MPI_Recv(vec, quantity, MPI_DOUBLE, 0,MPI_ANY_TAG, MPI_COMM_WORLD,&status);

            double min = Min(vec, quantity);

            printf("Minimo local: %.3lf\n",min);
            if(rank == 1){
                double minDef = 0.0;
                MPI_Reduce(&min, &minDef, 1, MPI_DOUBLE, MPI_MIN, rank, MPI_COMM_WORLD); 
                printf("Minimo total: %.3lf\n", minDef);  
            }      

            else{
                MPI_Reduce(&min, 0, 1, MPI_DOUBLE, MPI_MIN, 1, MPI_COMM_WORLD); 
            }  
        }

        free(vec);
    }

    MPI_Finalize();
    return 0;
}