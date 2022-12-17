// worked with sarim

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

int MAX;

int main() {
  srand(time(0));  // Do Not Remove. Just ignore and continue below.

  // 0. Get the matrix size from the command line and assign it to MAX
  char input[3];
  printf("Enter the size of matrix you want to initialize: ");
  fgets(input, 3, stdin);
  MAX = atoi(input);
  
  // Moving inside main to fix variably modified ‘res’ at file scope
  int matA[MAX][MAX]; 
  int matB[MAX][MAX]; 

  int matSumResult[MAX][MAX];
  int matDiffResult[MAX][MAX]; 
  int matProductResult[MAX][MAX]; 

  // struct for args
  struct args_t {
    int (*matA)[MAX];
    int (*matB)[MAX];
    int (*res)[MAX];
    int row;
  };

  void fillMatrix(int matrix[MAX][MAX]) {
      for(int i = 0; i<MAX; i++) {
          for(int j = 0; j<MAX; j++) {
              matrix[i][j] = rand()%10+1;
          }
      }
  }

  void printMatrix(int matrix[MAX][MAX]) {
      for(int i = 0; i<MAX; i++) {
          for(int j = 0; j<MAX; j++) {
              printf("%5d", matrix[i][j]);
          }
          printf("\n");
      }
      printf("\n");
  }

  // Fetches the appropriate coordinates from the argument, and sets
  // the cell of matSumResult at the coordinates to the sum of the
  // values at the coordinates of matA and matB.
  void* computeSum(void* args) { // pass in the number of the ith thread
    struct args_t* arg = (struct args_t*) args;
    int offset = arg->row * MAX;
    for (int i=0; i<MAX; i++) {
      int added = arg->matA[0][i+offset] + arg->matB[0][i+offset];
      arg->res[0][i+offset] = added;
    }
  }

  // Fetches the appropriate coordinates from the argument, and sets
  // the cell of matSumResult at the coordinates to the difference of the
  // values at the coordinates of matA and matB.
  void* computeDiff(void* args) { // pass in the number of the ith thread
    struct args_t* arg = (struct args_t*) args;
    int offset = arg->row * MAX;
    for (int i=0; i<MAX; i++) {
      int added = arg->matA[0][i+offset] - arg->matB[0][i+offset];
      arg->res[0][i+offset] = added;
    }
  }

  // Fetches the appropriate coordinates from the argument, and sets
  // the cell of matSumResult at the coordinates to the inner product
  // of matA and matB.
  void* computeProduct(void* args) { // pass in the number of the ith thread
    struct args_t* arg = (struct args_t*) args;
    int offset = arg->row * MAX;
    for (int i=0; i<MAX; i++) {
      int added = arg->matA[0][i+offset] * arg->matB[0][i+offset];
      arg->res[0][i+offset] = added;
    }
  }

  // 1. Fill the matrices (matA and matB) with random values.
  fillMatrix(matA);
  fillMatrix(matB);

  // 2. Print the initial matrices.
  printf("Matrix A:\n");
  printMatrix(matA);
  printf("Matrix B:\n");
  printMatrix(matB);

  // 3. Create pthread_t objects for our threads.
  pthread_t threads[MAX];
  struct args_t args[MAX];
  
  // Sum
  for (int i=0; i<MAX; i++) {
    args[i].matA = matA;
    args[i].matB = matB;
    args[i].res = matSumResult;
    args[i].row = i;
    int error = pthread_create(&threads[i], NULL, &computeSum, (void *) &args[i]);
    if (error) {
      printf("Creation of threads failed!");
      exit(1);
    }
  }
  
  // waiting for the threads
  for (int i=0; i<MAX; i++) {
    pthread_join(threads[i], NULL);
  }
  
  // Difference
  for (int i=0; i<MAX; i++) {
    args[i].matA = matA;
    args[i].matB = matB;
    args[i].res = matDiffResult;
    args[i].row = i;
    int error = pthread_create(&threads[i], NULL, &computeDiff, (void *) &args[i]);
    if (error) {
      printf("Creation of threads failed!");
      exit(1);
    }
  }
  
  // 5 waiting for the threads
  for (int i=0; i<MAX; i++) {
    pthread_join(threads[i], NULL);
  }
  
  // Product
  for (int i=0; i<MAX; i++) {
    args[i].matA = matA;
    args[i].matB = matB;
    args[i].res = matProductResult;
    args[i].row = i;
    int error = pthread_create(&threads[i], NULL, &computeProduct, (void *) &args[i]);
    if (error) {
      printf("Creation of threads failed!");
      exit(1);
    }
  }
  
  // waiting for the threads
  for (int i=0; i<MAX; i++) {
    pthread_join(threads[i], NULL);
  }

  // 6. Print the results.
  printf("Results:\n");
  printf("Sum:\n");
  printMatrix(matSumResult);
  printf("Difference:\n");
  printMatrix(matDiffResult);
  printf("Product:\n");
  printMatrix(matProductResult);
  return 0;
}