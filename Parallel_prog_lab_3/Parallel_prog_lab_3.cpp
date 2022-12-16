#include <iostream>
#include "mpi.h"
#include <time.h>

int main(int argc, char* argv[])
{
    srand(time(NULL));
    double timeStart, timeStop;
    int i, j, k, l;
    int size = 4000;
    int rank, numprocs, line;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    line = size / numprocs;
    double* firstMatrix = new double [size * size];
    double* secondMatrix = new double[size * size];
    double* resultMatrix = new double [size * size];
    double*  ans = new double[size * line];
    double* buff = new double[size * line];
    if (rank == 0)
    {
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                firstMatrix[i * size + j] = rand() % 1000;
        timeStart = MPI_Wtime();
        for (i = 1; i < numprocs; i++) {
            MPI_Send(secondMatrix, size * size, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
        for (l = 1; l < numprocs; l++) {
            MPI_Send(firstMatrix + (l - 1) * line * size, size * line, MPI_FLOAT, l, 1, MPI_COMM_WORLD);
        }
        for (k = 1; k < numprocs; k++) {
            MPI_Recv(ans, line * size, MPI_FLOAT, k, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (i = 0; i < line; i++) {
                for (j = 0; j < size; j++) {
                    resultMatrix[((k - 1) * line + i) * size + j] = ans[i * size + j];
                }
            }
        }
        for (i = (numprocs - 1) * line; i < size; i++) {
            for (j = 0; j < size; j++) {
                int temp = 0;
                for (k = 0; k < size; k++)
                    temp += firstMatrix[i * size + k] * secondMatrix[k * size + j];
                resultMatrix[i * size + j] = temp;
            }
        }
        timeStop = MPI_Wtime();
        std::cout << "Rank: " << rank << "\nTime: " << timeStop - timeStart << " sec" << std::endl;
        delete[] firstMatrix;
        delete[] secondMatrix;
        delete[] resultMatrix;
        delete[] buff;
        delete[] ans;
    }
    else {
        MPI_Recv(secondMatrix, size * size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Recv(buff, size * line, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = 0; i < line; i++) {
            for (j = 0; j < size; j++) {
                int temp = 0;
                for (k = 0; k < size; k++)
                    temp += buff[i * size + k] * secondMatrix[k * size + j];
                ans[i * size + j] = temp;
            }
        }
        MPI_Send(ans, line * size, MPI_FLOAT, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}