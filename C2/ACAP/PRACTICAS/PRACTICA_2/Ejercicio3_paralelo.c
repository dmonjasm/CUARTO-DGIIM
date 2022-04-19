//Daniel Monjas Miguélez
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define FIN 666
#define NORMAL 100
#define MASTER 0

void funcionCero(int rank, int nProcs);
void funcionUno(int rank);
void funcionDos(int rank);
void funcionTres(int rank);
void consumirNuevaLinea();

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank;
    int nProcs;

    MPI_Comm_size( MPI_COMM_WORLD, &nProcs);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    if(nProcs < 4){
        printf("Hebra %i-->abortando ejecución\n", rank);
    }

    else{
        switch (rank)
        {
        case 0:
            funcionCero(rank, nProcs);
            break;
        
        case 1:
            funcionUno(rank);
            break;

        case 2:
            funcionDos(rank);
            break;

        case 3:
            funcionTres(rank);
            break;
        
        default:
            break;
        }

    }
    
    MPI_Finalize();
    return 0;
}

void funcionCero(int rank, int nProcs){
    MPI_Status status;
    int seguir = 1;
    int num_ant = 0;
    int num = 0;

    while(seguir){
        if(num_ant < 4){
            num = 0;
            printf("\nIntroduzca un número (rango [0,4]):\n");fflush(stdout);
            //El problema de combinar scanf y fgets en la toma de datos es que scaf no consume los saltos de línea, mientras que fgets si lo hace
            //Debido a esto, si no se consume el salto de línea después del scanf, fgets no se ejecuta correctamente pues consume solo el salto de línea.
            //Esto se puede arreglar con un único getchar tras el scanf que consuma el salto de línea, pero he preferido utilizar la función consumirNuevaLínea
            //, la cual consume todo lo que haya en el buffer tras el scanf, dejándolo vacío para la correcta ejecución de fgets (es más seguro que un simple getchar)
            scanf("%d", &num);
            consumirNuevaLinea();
        }

        else{
            num = (num + 1)%4;

            if(num == 0)
                num++;
        }

        if(num < 0 || num > 4){
                printf("Número introducido inválido\n");
            }

        else{
            switch (num)
            {
            case 0:
                seguir = 0;
                for(int i=1; i < nProcs; i++){
                    MPI_Send(NULL, 0, MPI_INT, i, FIN, MPI_COMM_WORLD);
                }

                printf("Hebra %d abortando\n", rank);

                break;

            case 1:
                printf("Escriba el texto a procesar:\n");fflush(stdout);
                char texto[256];
                
                fgets(texto, 256, stdin);
                
                MPI_Send(texto, sizeof(texto), MPI_CHAR, num, NORMAL, MPI_COMM_WORLD);
                MPI_Recv(texto, sizeof(texto), MPI_CHAR, num, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                printf("El texto en mayúsculas es:\n%s\n", texto);fflush(stdout);

                break;

            case 2: ;
                int size = 10;
                float reales[10];

                for(int i=0; i < size-1; i++){
                    reales[i] = 1.1*(i+1);
                }

                reales[size-1] = 10.10;

                MPI_Send(reales, size, MPI_FLOAT, num, NORMAL, MPI_COMM_WORLD);

                float resultado[2];

                MPI_Recv(resultado, 2, MPI_FLOAT, num, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                printf("La suma de los resultados es: %.3lf\nLa raíz cuadrada de los resultados es: %.3lf\n", resultado[0], resultado[1]);fflush(stdout);

                break;

            case 3:
                MPI_Send(&num, 1, MPI_INT, num, NORMAL, MPI_COMM_WORLD);

                int suma = 0;
                MPI_Recv(&suma, 1, MPI_INT, num, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                printf("La suma de los caracteres es: %i\n", suma);fflush(stdout);

                if(num_ant == 4){
                    num_ant = 0;
                }
                break;
            
            case 4:
                num_ant = 4;
                break;
            }

        }       
    }
}

void funcionUno(int rank){
    int seguir = 1;
    MPI_Status status;

    while(seguir){
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == FIN){
            seguir = 0;
            printf("Hebra %d abortando\n", rank);
        }

        else{
            int size = 0;
            MPI_Get_count(&status, MPI_CHAR, &size);

            char texto[size];
            MPI_Recv(texto, size, MPI_CHAR, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            for(int i=0; i < sizeof(texto);i++){
                texto[i] = toupper(texto[i]);
            }

            MPI_Send(texto, size, MPI_CHAR, MASTER, NORMAL, MPI_COMM_WORLD);
        }
    }
}

void funcionDos(int rank){
    int seguir = 1;
    MPI_Status status;

    while(seguir){
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == FIN){
            seguir = 0;
            printf("Hebra %d abortando\n", rank);
        }

        else{
            int size = 0;
            MPI_Get_count(&status, MPI_FLOAT, &size);

            float reales[size];

            MPI_Recv(reales, size, MPI_FLOAT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            float result[2] = {0.0,0.0};

            for(int i = 0; i < size; i++){
                result[0]+=reales[i];
            }

            result[1] = sqrt(result[0]);

            MPI_Send(result, 2, MPI_FLOAT, MASTER, NORMAL, MPI_COMM_WORLD);
        }

    }
}

void funcionTres(int rank){
    int seguir = 1;
    MPI_Status status;

    while(seguir){
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if(status.MPI_TAG == FIN){
            seguir = 0;
            printf("Hebra %d abortando\n", rank);
        }

        else{
            int suma = 0;
            MPI_Recv(&suma, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            char mensaje[] = "\nEntrando en funcionalidad 3"; 
            fflush(stdout);printf("%s\n", mensaje);

            suma = 0;
            for(int i =0; i < sizeof(mensaje); i++){
                suma += mensaje[i];
            }

            MPI_Send(&suma, 1, MPI_INT, MASTER, NORMAL, MPI_COMM_WORLD);
        }
        
    }
}

void consumirNuevaLinea()
{
    int c;
    do
    {
        c = getchar();
    } while (c != EOF && c != '\n');
}
