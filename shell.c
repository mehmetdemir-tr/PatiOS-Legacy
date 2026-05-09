#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    while (1) {
        printf("pati@tr-2.1> ");
        fflush(stdout);
        char girdi[256];
        fgets(girdi, 256, stdin);
        girdi[strcspn(girdi, "\n")] = 0;
        char *argv[64];
        int argc = 0;
        char *token = strtok(girdi, " ");
        while (token && argc < 63) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;
        if (argc == 0) {
            continue;
        }
        else {
            pid_t pid = fork();
            if (pid == 0) {
                execvp(argv[0], argv);
                printf("%s komutu bulunamadi, bi' yardim komutuna bak istersen..\n", argv[0]);
                exit(127);
            } else if (pid > 0) {
                wait(NULL);
            }
        }
    }
}
