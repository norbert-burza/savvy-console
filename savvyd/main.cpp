#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;

pthread_t process_stdout_reading_thread;
pthread_t socket_reading_thread;

void * read_process_stdout(void * args) {
        int * fd = (int *) args;
    
        cout << "Reading thread alive." << endl;
        cout << "fd[0]=" << fd[0] << endl;
    
        char * buf = new char[1024];
        
        int bytesRead = 0;
        while ( (bytesRead = read(fd[0], buf, 1024)) != 0) { // Read from stdout
            for (int i = 0; i < bytesRead; i++) {
                cout << buf[i];
            }
            write(fd[1], buf, bytesRead); // Write to socket

        }
        cout << "STDOUT reading thread exited." << endl;
}

void * read_socket(void * args) {
    int * fd = (int *) args;
    
        cout << "Reading thread alive." << endl;
    
        char * buf = new char[1024];
    
        int bytesRead = 0;
        while ( (bytesRead = read(fd[0], buf, 1024)) != 0) { // Read from socket
            write(fd[1], buf, bytesRead); // Write to process stdin
        }
}

int openSocket() {
    struct addrinfo hints;
    struct addrinfo *results, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    const char * service = "8080";

    int err = getaddrinfo(NULL, service, &hints, &results);
    cout << "getaddrinfo(...) returned " << err << endl;
    if (err != 0) {
        cout << gai_strerror(err) << endl;
    }

    int sfd;
    for (rp = results; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        int optVal = 1;
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            cout << "bind() was successful" << endl;
            break;
        }

        close(sfd); // bind failed
    }

    if (listen(sfd, 5) == -1) {
        cout << "listen() failed" << endl;
    }

    return accept(sfd, NULL, NULL);
}

int main() {
    cout << "Started." << endl;

    int input_pipe[2];
    int output_pipe[2];
    pipe(input_pipe);
    pipe(output_pipe);

    pid_t child_pid = fork();

    switch(child_pid) {
        case -1:
            cout << "Could not fork." << endl;
        break;

        case 0:
            cout << "Child process here." << endl;
            close(input_pipe[1]); // close write end
            close(output_pipe[0]); // close read end
        
            cout << "output_pipe[0]=" << output_pipe[0] << endl;
            cout << "output_pipe[1]=" << output_pipe[1] << endl;
            cout << "STDOUT_FILENO=" << STDOUT_FILENO << endl;

            dup2(input_pipe[0], STDIN_FILENO); // wirte to STDIN of child process
            dup2(output_pipe[1], STDERR_FILENO); 
            dup2(output_pipe[1], STDOUT_FILENO); // read from STDOUT of child process

            // Execute child process
            execl("/bin/bash", "bash", NULL);
        break;

        default:
            close(input_pipe[0]); // with input stream data are provided to STDIN of child process, so close read end
            close(output_pipe[1]); // output pipe provide us with data, so close write end

            int sfd = openSocket();

            int * read_process_stdout_args = new int[2];
            read_process_stdout_args[0] = output_pipe[0];
            read_process_stdout_args[1] = sfd;
            pthread_create(&process_stdout_reading_thread, NULL, read_process_stdout, read_process_stdout_args);

            int * read_socket_args = new int[2];
            read_socket_args[0] = sfd;
            read_socket_args[1] = input_pipe[1];
            pthread_create(&socket_reading_thread, NULL, read_socket, read_socket_args);

            cout << "Pid of child process is: " << child_pid << endl;
            sleep(5);
            cout << "Parent will close pipes immediately." << endl;

            // Close resources
            close(input_pipe[1]);
            close(output_pipe[0]);
        break;
    }

    int status;

    cout << "Wait for child." << endl;
    int result = wait(&status);
    cout << "wait(...) exited with result " << result << endl;
    cout << "Child finished with status " << status << endl;
}