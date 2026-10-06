#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

char *get_cmd(int argc, char **argv);
char *escape(char* arg);
bool  needs_escape(char c);

int main(int argc, char **argv) {
    char* cmd_buf = get_cmd(argc, argv);

    if (cmd_buf == NULL) {
        fprintf(stderr, "retval: error: failed to get command\n");
        return 1;
    }

    // fork() returns 0 to the child process, for the parent, it returns the child's PID
    pid_t pid = fork();

    // fork returns -1 on error
    if (pid == -1) {
        perror("retval: fork");
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
        perror("retval: waitpid");
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

char *get_cmd(int argc, char **argv) {
    char* buf;
    size_t len = 0;
    
    if (argc > 1) {
        int total_cmd_len = 0;
        
        if (argc == 2) {
            total_cmd_len += strlen(argv[1]);
        }
        else {
            for (int i = 1; argv[i] != NULL; i++) {
                total_cmd_len += strlen(argv[i])*2+3;   // 2 bytes per char (esc) + 2 quotes + 1 space
            }
        }
        
        buf = malloc(total_cmd_len+1);

        if (buf == NULL) {
            return NULL;
        }

        if (argc == 2) {
            strcpy(buf, argv[1]);
        } else {
            int pos = 0;
            for (int i = 1; argv[i] != NULL; i++) {
                char *esc = escape(argv[i]);
                if (esc == NULL) { free(buf); return NULL; }
                strcpy(buf + pos, "\"");
                pos += 1;
                strcpy(buf + pos, esc);
                pos += strlen(esc);
                strcpy(buf + pos, "\" ");
                pos += 2;
                free(esc);
            }
        }

        // debugging
        // printf("cmd = \"%s\"\n", buf);
    } else {
        // getline() returns -1 on error
        if (getline(&buf, &len, stdin) == -1) {
            fprintf(stderr, "retval: getline: error: could not get input from stdin");
            free(buf);
            return NULL;
        }
    }
    return buf;
}


char *escape(char *arg) {
    int len = strlen(arg);
    char *out = malloc(2 * len + 1);
    if (out == NULL) return NULL;

    int j = 0;
    for (int i = 0; i < len; i++) {
        if (needs_escape(arg[i])) out[j++] = '\\';
        out[j++] = arg[i];
    }
    out[j] = '\0';
    return out;
}

bool needs_escape(char c) {
    char escapables[] = {'`', '"', '\\', '$'};
    int count = sizeof(escapables);

    for (int i = 0; i < count; i++) {
        if (c == escapables[i]) return true;
    }
    return false;
}