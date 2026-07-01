// Karabaş Process İzleme Servisi - 5 Nisan 2026 14:58
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    while (1) {
        struct dirent **namelist;
        struct dirent *entry;
        int n = scandir("/dev/pcgconfigs", &namelist, NULL, alphasort);
        if (n < 0) {
            perror("Klasor acilamadi");
            sleep(5);
            continue;
        }

        for (int i = 0; i < n; i++) {
            entry = namelist[i];
            if (strcmp(entry->d_name, ".") == 0) continue;
            if (strcmp(entry->d_name, "..") == 0) continue;
            if (strstr(entry->d_name, ".pcg") == NULL) continue;

            char tamyol[512];
            char dosya[256];
            snprintf(tamyol, sizeof(tamyol), "/dev/pcgconfigs/%s", entry->d_name);
            FILE *pcgfile = fopen(tamyol, "r");
            if (pcgfile == NULL) {
                printf("Dosya acilamadi, atlaniyor: %s\n", entry->d_name);
                free(namelist[i]);
                continue;
            }

            char dosyayolu[256] = {0};
            char *args[] = {NULL, NULL};
            int izle = 0;
            int bulundu = 0;

            while (fgets(dosya, sizeof(dosya), pcgfile) != NULL) {
                char kopya[256];
                strncpy(kopya, dosya, sizeof(kopya) - 1);
                kopya[sizeof(kopya) - 1] = '\0';
                char *okuyucu = strtok(kopya, " =\n");
                if (okuyucu == NULL) continue;

                if (strcmp(okuyucu, "konumu") == 0) {
                    char *gecici = strtok(NULL, " =\n");
                    if (gecici != NULL) {
                        strncpy(dosyayolu, gecici, sizeof(dosyayolu) - 1);
                        dosyayolu[sizeof(dosyayolu) - 1] = '\0';
                        args[0] = dosyayolu;
                    }
                }
                if (strcmp(okuyucu, "izle") == 0) {
                    izle = 1;
                }
            }
            fclose(pcgfile);

            if (izle == 1 && strlen(dosyayolu) > 0) {
                struct dirent **proclist;
                int np = scandir("/proc", &proclist, NULL, alphasort);
                if (np < 0) {
                    free(namelist[i]);
                    continue;
                }

                for (int j = 0; j < np; j++) {
                    if (atoi(proclist[j]->d_name) > 0) {
                        snprintf(tamyol, sizeof(tamyol), "/proc/%s/cmdline", proclist[j]->d_name);
                        FILE *cmdfile = fopen(tamyol, "r");
                        if (cmdfile != NULL) {
                            char cmdline[256] = {0};
                            fread(cmdline, 1, sizeof(cmdline) - 1, cmdfile);
                            fclose(cmdfile);
                            if (strcmp(cmdline, dosyayolu) == 0) {
                                bulundu = 1;
                            }
                        }
                    }
                    free(proclist[j]);
                }
                free(proclist);

                if (bulundu == 0) {
                    printf("Oldu process yeniden baslatiliyor: %s\n", dosyayolu);
                    pid_t pid = fork();
                    if (pid == 0) {
                        execv(dosyayolu, args);
                        perror("execv basarisiz");
                        exit(1);
                    }
                }
            }
            free(namelist[i]);
        }
        free(namelist);
        sleep(5);
    }
}
