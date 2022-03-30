#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <mpi.h>

void funcionCero();
void funcionUno(){};
void funcionDos(){};
void funcionTres(){};
void funcionCuatro();

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    int nProcs;
    MPI_Comm_size( MPI_COMM_WORLD, &nProcs);

    if(nProcs < 4){
        printf("Hebra %i-->abortando ejecución\n", rank);
    }

    else{
        switch (rank)
        {
        case 0:
            funcionCero();
            break;
        
        case 1:
            funcionUno();
            break;

        case 2:
            funcionDos();
            break;

        case 3:
            funcionTres();
            break;
        
        default:
            break;
        }

    }
    
    MPI_Finalize();
    return 0;
}

void funcionCero(){
    int seguir = 1;

    while(seguir){
        int num = 0;
        printf("Introduzca un número (rango [0,4]):\n");
        scanf("%d", &num);

        if(num < 0 || num > 4){
            printf("Número introducido inválido\n");
        } 

        else{
            switch (numero)
            {
            case 0:
                break;

            case 01:
                break;

            case 2:
                break;

            case 3:
                break;
            
            case 4:
                break;
            
            default:
                break;
            }

        }       
    }
}