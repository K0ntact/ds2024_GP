#include <mpi.h>
#include <cstring>
#include <iostream>
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
    MPI_Lookup_name(servicename, MPI_INFO_NULL, portname);
    MPI_Comm_connect(portname, MPI_INFO_NULL, 0, MPI_COMM_SELF, &newcomm);

    int fd;
    char inbuffer[MAX_MESSAGE_LENGTH];
    char outbuffer[MAX_MESSAGE_LENGTH];
    fd = open("./tmp/myfifo2", O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    string message;
    MPI_Request request;
    int flag;
    MPI_Irecv(&inbuffer[0], MAX_MESSAGE_LENGTH, MPI_CHAR, 0, 0, newcomm, &request);
    while(1){
        if(read(fd, outbuffer, sizeof(outbuffer)) > 0) {
            message = string(&outbuffer[0]);
            cout << "User2: " << message.c_str();
            memset(&outbuffer[0], 0, sizeof(outbuffer));
        }

        // Mon-Blocking Recv
        MPI_Test(&request, &flag, MPI_STATUS_IGNORE); // Test if message is received
        if (flag) {
            cout << "User1: " << inbuffer;
            memset(&inbuffer[0], 0, sizeof(inbuffer));
            MPI_Irecv(&inbuffer[0], MAX_MESSAGE_LENGTH, MPI_CHAR, 0, 0, newcomm, &request);
        }
    }
}