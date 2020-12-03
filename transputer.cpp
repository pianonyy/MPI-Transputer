

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "math.h"

#include "mpi.h"

#define TOP_RAND 50

using namespace std;

void randomizeIntArray(int* array, int size) {
    for(int i = 0; i < size; i++) {
        array[i] = rand() % TOP_RAND;
    }

    return;
}

void printArray(int *array, int size){
  for(int i = 0; i < size ; i++) {
      cout << array[i] << " ";
  }
  cout << endl;
  return;
}

void splitArray(int *array_left, int *array_right,int *array, int len){
  for (int i = 0; i < (len / 2) ; i++){
    array_left[i] = array[i];
  }

  int j = 0;
  for (int i = (len / 2); i < len;i++){
    array_right[j] = array[i];
    j += 1;
  }
  return;
}

void shiftMsg(int rank, int size, int arraySize) {
    if (rank == 0){
      int *arr = (int*)malloc(sizeof(int) * arraySize);
      randomizeIntArray(arr, arraySize);

      int *arr_12 = (int*)malloc(sizeof(int) * (arraySize / 2));
      int *arr_22 = (int*)malloc(sizeof(int) * (arraySize / 2));
      splitArray(arr_12,arr_22,arr,arraySize);


      printArray(arr,arraySize);
      printArray(arr_12, (arraySize / 2));
      printArray(arr_22, (arraySize / 2));
      cout.flush();

      MPI_Request request;
      MPI_Isend(arr_12, (arraySize / 2), MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request);

      cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
      printArray(arr_12, (arraySize/2));
      cout.flush();

      MPI_Isend(arr_22, (arraySize / 2), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

      cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
      printArray(arr_22, (arraySize/2));
      cout.flush();

      free(arr_12);
      free(arr_22);

    }
    if (rank == 1 || rank == 2 || rank == 3){
      int recv_size = arraySize / pow(2,rank);
      int *recv = (int*)malloc(sizeof(int) * recv_size);

      MPI_Request request;
      MPI_Irecv(recv, recv_size, MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
      MPI_Status status;

      //Проверяем, получил ли процесс сообщение
      if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
          cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
          printArray(recv, recv_size);
          cout.flush();

      }
      if (rank != 3){
        int *recv_34 = (int*)malloc(sizeof(int) * (recv_size / 2));
        int *recv_44 = (int*)malloc(sizeof(int) * (recv_size / 2));

        splitArray(recv_34, recv_44, recv,recv_size);

        MPI_Request request;
        MPI_Isend(recv_34 , (recv_size / 2), MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request);

        cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
        printArray(recv_34 , (recv_size / 2));
        cout.flush();

        MPI_Isend(recv_44, (recv_size / 2), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

        cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
        printArray(recv_44, (recv_size / 2));
        cout.flush();

        free(recv_34);
        free(recv_44);
        free(recv);

      }
      else{
        MPI_Request request;
        MPI_Isend(recv , recv_size, MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request);

        cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
        printArray(recv , recv_size);
        cout.flush();

      }
    }

    if( (rank >= 4) && (rank <=15) ){
      if(rank == 4 || rank == 8 || rank == 12){
        int recv_size = arraySize / pow(2,(rank/4));
        int *recv = (int*)malloc(sizeof(int) * recv_size);

        MPI_Request request;
        MPI_Irecv(recv, recv_size, MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request);
        MPI_Status status;

        //Проверяем, получил ли процесс сообщение
        if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
            cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
            printArray(recv, recv_size);
            cout.flush();

        }
        if (rank != 12){
          int *recv_14 = (int*)malloc(sizeof(int) * (recv_size / 2));
          int *recv_24 = (int*)malloc(sizeof(int) * (recv_size / 2));
          splitArray(recv_14, recv_24, recv, recv_size);

          MPI_Request request;
          MPI_Isend(recv_14 , (recv_size / 2), MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
          printArray(recv_14 , (recv_size / 2));
          cout.flush();

          MPI_Isend(recv_24 , (recv_size / 2), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
          printArray(recv_24 , (recv_size / 2));
          cout.flush();

          free(recv_14);
          free(recv_24);

        }
        else{
          MPI_Request request;
          MPI_Isend(recv , (recv_size), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
          printArray(recv , recv_size);
          cout.flush();

          free(recv);
        }
      }
      if (rank == 5 || rank == 6 || rank == 9 || rank == 10){


        if (rank == 9){
          int *recv_left = (int*)malloc(sizeof(int) * (arraySize/8));
          int *recv_right = (int*)malloc(sizeof(int) * (arraySize/4));
          MPI_Request request;
          MPI_Irecv(recv_left, (arraySize/8), MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
          MPI_Status status;

          //Проверяем, получил ли процесс сообщение
          if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
              cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
              printArray(recv_left, (arraySize/8));
              cout.flush();

          }

          MPI_Request request_tmp;
          MPI_Irecv(recv_right, (arraySize / 4), MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request_tmp);
          MPI_Status status_tmp;

          //Проверяем, получил ли процесс сообщение
          if(MPI_Wait(&request_tmp, &status_tmp) == MPI_SUCCESS) {
              cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
              printArray(recv_right, (arraySize/4));
              cout.flush();
          }

          MPI_Request request_tmp2;
          MPI_Isend(recv_left , (arraySize / 8), MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request_tmp2);

          cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
          printArray(recv_left , (arraySize / 8));
          cout.flush();

          MPI_Request request_tmp3;
          MPI_Isend(recv_right , (arraySize / 4), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request_tmp3);

          cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
          printArray(recv_right , (arraySize / 4));
          cout.flush();

          free(recv_left);
          free(recv_right);
        }
        else if( rank == 6){
          int *recv_left = (int*)malloc(sizeof(int) * (arraySize/4));
          int *recv_right = (int*)malloc(sizeof(int) * (arraySize/8));
          MPI_Request request;
          MPI_Irecv(recv_left, (arraySize/4), MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
          MPI_Status status;

          //Проверяем, получил ли процесс сообщение
          if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
              cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
              printArray(recv_left, (arraySize/4));
              cout.flush();

          }

          // MPI_Request request;
          MPI_Irecv(recv_right, (arraySize / 8), MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request);
          // MPI_Status status;

          //Проверяем, получил ли процесс сообщение
          if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
              cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
              printArray(recv_right, (arraySize/8));
              cout.flush();
          }

          // MPI_Request request;
          MPI_Isend(recv_left , (arraySize / 4), MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
          printArray(recv_left , (arraySize / 4));
          cout.flush();

          MPI_Isend(recv_right , (arraySize / 8), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
          printArray(recv_right , (arraySize / 8));
          cout.flush();

          free(recv_left);
          free(recv_right);

        }
        else{ //5,10
          int recv_size = arraySize / 4;
          int *recv_left = (int*)malloc(sizeof(int) * recv_size);
          int *recv_right = (int*)malloc(sizeof(int) * recv_size);
          MPI_Request request;
          MPI_Irecv(recv_left, recv_size, MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
          MPI_Status status;

          //Проверяем, получил ли процесс сообщение
          if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
              cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
              printArray(recv_left, recv_size);
              cout.flush();

          }

          // MPI_Request request;
          MPI_Irecv(recv_right, recv_size, MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request);
          // MPI_Status status;

          //Проверяем, получил ли процесс сообщение
          if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
              cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
              printArray(recv_right, recv_size);
              cout.flush();
          }

          // MPI_Request request;
          MPI_Isend(recv_left , recv_size, MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
          printArray(recv_left , recv_size);
          cout.flush();

          MPI_Isend(recv_right , recv_size, MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

          cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
          printArray(recv_right , recv_size);
          cout.flush();

          free(recv_left);
          free(recv_right);


        }

       }
       if (rank == 13 || rank == 14){


         int recv_size = arraySize / (8 / ( pow(2,rank-13) ) );
         int *recv_left = (int*)malloc(sizeof(int) * recv_size);
         int *recv_right = (int*)malloc(sizeof(int) * recv_size);
         MPI_Request request;
         MPI_Irecv(recv_left, recv_size, MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
         MPI_Status status;

         //Проверяем, получил ли процесс сообщение
         if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
             cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
             printArray(recv_left, recv_size);
             cout.flush();

         }

         // MPI_Request request;
         MPI_Irecv(recv_right, recv_size, MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request);
         // MPI_Status status;

         //Проверяем, получил ли процесс сообщение
         if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
             cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
             printArray(recv_right, recv_size);
             cout.flush();
         }


         int * result = new int[recv_size + recv_size];
         std::copy(recv_left, recv_left + recv_size, result);
         std::copy(recv_right, recv_right + recv_size, result + recv_size);

         // MPI_Request request;
         MPI_Isend(result , (2 * recv_size), MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD, &request);

         cout << "process: " << rank << " sent to process " << (rank+1) % size << " array: ";
         printArray(result , (2 * recv_size));
         cout.flush();


         free(recv_left);
         free(recv_right);


       }
       if(rank == 7 || rank == 11){
         int recv_size = 1;
         recv_size += (rank==11) * 1;

         // int recv_size = arraySize / (8 / ( pow(2,rank-7) ) );
         int *recv_left = (int*)malloc(sizeof(int) * recv_size);
         int *recv_right = (int*)malloc(sizeof(int) * recv_size);
         MPI_Request request;
         MPI_Irecv(recv_left, recv_size, MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
         MPI_Status status;

         //Проверяем, получил ли процесс сообщение
         if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
             cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
             printArray(recv_left, recv_size);
             cout.flush();

         }

         MPI_Request request_4;
         MPI_Irecv(recv_right, recv_size, MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request_4);
         MPI_Status status_4;

         //Проверяем, получил ли процесс сообщение
         if(MPI_Wait(&request_4, &status_4) == MPI_SUCCESS) {
             cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
             printArray(recv_right, recv_size);
             cout.flush();
         }

         int * result = new int[recv_size + recv_size];
         std::copy(recv_left, recv_left + recv_size, result);
         std::copy(recv_right, recv_right + recv_size, result + recv_size);

         MPI_Request request_5;
         MPI_Isend(result , (2 * recv_size), MPI_INT, (rank+4) % size, 0, MPI_COMM_WORLD, &request_5);

         cout << "process: " << rank << " sent to process " << (rank+4) % size << " array: ";
         printArray(result , (2 * recv_size));
         cout.flush();



         free(recv_left);
         free(recv_right);



       }

       if (rank == 15){
         int recv_size = arraySize / 2;



         int *recv_left = (int*)malloc(sizeof(int) * recv_size);
         int *recv_right = (int*)malloc(sizeof(int) * recv_size);
         MPI_Request request;
         MPI_Irecv(recv_left, recv_size, MPI_INT, (rank-1 + size) % size, 0, MPI_COMM_WORLD, &request);
         MPI_Status status;

         //Проверяем, получил ли процесс сообщение
         if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
             cout << "process: " << rank << " recv from process "<< (rank-1 + size) % size << " array: ";
             cout.flush();
             printArray(recv_left, recv_size);

         }

         // MPI_Request request;
         MPI_Irecv(recv_right, recv_size, MPI_INT, (rank-4 + size) % size, 0, MPI_COMM_WORLD, &request);
         // MPI_Status status;

         //Проверяем, получил ли процесс сообщение
         if(MPI_Wait(&request, &status) == MPI_SUCCESS) {
             cout << "process: " << rank << " recv from process "<< (rank-4 + size) % size << " array: ";
             cout.flush();
             printArray(recv_right, recv_size);
         }

         int * result = new int[recv_size + recv_size];
         std::copy(recv_left, recv_left + recv_size, result);
         std::copy(recv_right, recv_right + recv_size, result + recv_size);
         printArray(result, arraySize);

       }
      }





    return;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Needs arg: array size\n");
        return 0;
    }
    int rank;
    int size;

    //define L - array size
    int arraySize;
    arraySize = atoi(argv[1]);

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Самый простой способ задания уникального сида для каждого процесса
    // srand(time(NULL) + rank);

    if(rank == 0) {

        cout << "Shift func: " << endl;
        cout.flush();


    }


    shiftMsg(rank, size, arraySize);

    MPI_Finalize();
    return 0;
}
