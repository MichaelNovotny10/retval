#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(void) {
    char* cmd_buf = NULL;
    size_t len = 0;
    
    // getline() returns -1 on error
    if (getline(&cmd_buf, &len, stdin) == -1) {
        perror("getline");
        free(cmd_buf);
        return 1;
    }

    // fork() returns 0 to the child process, for the parent, it returns the child's PID
    pid_t pid = fork();

    // fork returns -1 on error
    if (pid == -1) {
        perror("fork");
        free(cmd_buf);
        return 1;
    } else if (pid == 0) {
        int dev_null = open("/dev/null", O_RDWR);
        dup2(dev_null, STDIN_FILENO);
        dup2(dev_null, STDOUT_FILENO);
        dup2(dev_null, STDERR_FILENO);
        close(dev_null);

        execl("/bin/sh", "sh", "-c", cmd_buf, NULL);
        // if execl doesn't exit, we exit manually        
        exit(1);
    }

    int status;
    // waitpid() returns -1 on error
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }


    if (WIFEXITED(status)) {
        int retval = WEXITSTATUS(status);
        printf("%d\n", retval);
    } else if (WIFSIGNALED(status)) {
        printf("Process was killed by signal. Signal number:\n");
        printf("%d\n", WTERMSIG(status));
    }
    
    free(cmd_buf);
    return 0;
}