#include <mpi.h>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define MAX_MESSAGE_LENGTH 2000


int main(int argc, char *argv[]){
    int rank;
    int number_of_processes;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char portname[MAX_MESSAGE_LENGTH] = "myport";
    char servicename[MAX_MESSAGE_LENGTH] = "my_service";
    MPI_Comm newcomm;

    MPI_Open_port(MPI_INFO_NULL, portname);
    MPI_Publish_name(servicename ,MPI_INFO_NULL, portname);
    MPI_Comm_accept(portname, MPI_INFO_NULL, 0, MPI_COMM_SELF, &newcomm);

    int fd;
    char buffer[MAX_MESSAGE_LENGTH];

    fd = open("./tmp/myfifo1", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }
    while(1) {
        if (read(fd, buffer, sizeof(buffer)) > 0) {

            /*
             * Client outputs are automatically redirected to the server's console,
             * so we don't need to print them here.
             * Let client print the server's outputs and its own outputs,
             * then be redirected to the server's console.
             */

            MPI_Send(buffer, MAX_MESSAGE_LENGTH, MPI_CHAR, 0, 0, newcomm);
            memset(&buffer[0], 0, sizeof(buffer));
        }
    }
}
