
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: avg num_elements\n");
    exit(1);
  }

  int num_elements = atoi(argv[1]);
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int nProcs;
  MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

  int workSize = num_elements/nProcs;
  int offset = num_elements % nProcs;
  int quantity = workSize + (rank < offset);

  float *sub_rand_nums = (float *)malloc(sizeof(float) * quantity);
  assert(sub_rand_nums != NULL);
  if (rank == 0) {
    float *rand_nums = create_rand_nums(num_elements);

    int start[nProcs], quantities[nProcs];

    for(int i=0; i < nProcs; i++){
      start[i] = i*workSize + (i<offset?i:offset);
      quantities[i] =  workSize + (i<offset);
    }

    MPI_Scatterv(rand_nums, quantities, start, MPI_FLOAT, sub_rand_nums, quantity, MPI_FLOAT, 0, MPI_COMM_WORLD);

    free(rand_nums);
  }

  else{
    MPI_Scatterv(NULL, NULL, NULL, NULL, sub_rand_nums, quantity, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }

  float sub_avg = compute_avg(sub_rand_nums, quantity);

  float *sub_avgs = (float *)malloc(sizeof(float) * nProcs);
  assert(sub_avgs != NULL);
  MPI_Allgather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, MPI_COMM_WORLD);

  float avg = compute_avg(sub_avgs, nProcs);
  printf("Avg of all elements from proc %d is %f\n", rank, avg);

  free(sub_avgs);
  free(sub_rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}