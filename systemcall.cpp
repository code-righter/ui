#include <iostream>
#include <unistd.h>      // fork, read, write, close
#include <sys/types.h>   // pid_t
#include <sys/wait.h>    // wait
#include <fcntl.h>       // open
#include <cstring>       // strlen
using namespace std;

int main() 
{
    int fd;
    const char *file = "demo.txt";
    const char *msg = "Hello written using write() system call\n";
    char buffer[100];

    // ---- File write section ----
    fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);   // open file
    write(fd, msg, strlen(msg));                          // write to file
    close(fd);                                            // close file

    // ---- Process creation ----
    pid_t pid = fork();                                    // fork new process

    if (pid == 0)                                          // child process
    {
        fd = open(file, O_RDONLY);                         // open file for reading
        int n = read(fd, buffer, sizeof(buffer));          // read data
        buffer[n] = '\0';                                  // null terminate
        write(1, buffer, n);                               // write to STDOUT
        close(fd);                                         // close file
    }
    else                                                   // parent process
    {
        wait(nullptr);                                     // wait for child
    }

    return 0;
}