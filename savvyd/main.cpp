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

int openSocket() {
    struct addrinfo hints;
    struct addrinfo *results, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
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
        
            dup2(input_pipe[0], STDIN_FILENO); // wirte to STDIN of child process
            // dup2(output_pipe[1], STDOUT_FILENO); // read from STDOUT of child process

            // Execute child process
            execl("/bin/bash", "bash", NULL);
        break;

        default:
            close(input_pipe[0]); // with input stream data are provided to STDIN of child process, so close read end
            close(output_pipe[1]); // output pipe provide us with data, so close write end

            cout << "Pid of child process is: " << child_pid << endl;
            sleep(1);
            cout << "Parent awake." << endl;

            int sfd = openSocket();

            char pwd[] = "pwd\n";
            write(input_pipe[1], pwd, sizeof(pwd));
            sleep(1);

            char ls[] = "ls\n"; // New line is important here, cause it 'flushes' the command
            write(input_pipe[1], ls, sizeof(ls));
            sleep(1);

            char * c = new char;
            // read(output_pipe[0], c, 1);

            char * buf = new char[1024];
            int bytesRead = 0;
            while ( (bytesRead = read(sfd, buf, 1024)) != 0) {
                write(input_pipe[1], buf, bytesRead);
            }

            // Close resources
            sleep(3);
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