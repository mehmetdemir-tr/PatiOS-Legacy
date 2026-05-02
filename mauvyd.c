#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main() {
    struct dirent *entry;
    pid_t pid;
    int status;
    struct dirent **namelist;
    int n = scandir("/dev/pcgconfigs", &namelist, NULL, alphasort);
    if (n < 0) {
        perror("[UYARI]: pcgconfigs acilamadi, sistem devam ediyor");
        while(wait(NULL) > 0);
        return 0;
    }
    printf("----------------------------------------\n");
    printf("----- MAUVYD Configuration Startup -----\n");
    printf("-------------Mounting FS..--------------\n");
    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    mkdir("/data", 0755);
    int ret = mount("/dev/vda", "/data", "ext4", 0, NULL);
    if (ret == 0) {
        printf("[TAMAM]: Kalici depolama aktif.\n");
    } else if (errno == EBUSY) {
        printf("[BILGI]: Depolama zaten bagli.\n");
    } else {
        perror("[HATA]: Baglanamadi");
    }
    printf("ip yolu: ");
    system("which ip");
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
    ifr.ifr_flags = IFF_UP | IFF_RUNNING;
    ioctl(fd, SIOCSIFFLAGS, &ifr);

    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
    addr->sin_family = AF_INET;
    inet_pton(AF_INET, "10.0.2.15", &addr->sin_addr);
    ioctl(fd, SIOCSIFADDR, &ifr);

    inet_pton(AF_INET, "255.255.255.0", &addr->sin_addr);
    ioctl(fd, SIOCSIFNETMASK, &ifr);

    struct rtentry route;
    memset(&route, 0, sizeof(route));

    struct sockaddr_in *gw = (struct sockaddr_in *)&route.rt_gateway;
    gw->sin_family = AF_INET;
    inet_pton(AF_INET, "10.0.2.2", &gw->sin_addr);

    struct sockaddr_in *dst = (struct sockaddr_in *)&route.rt_dst;
    dst->sin_family = AF_INET;
    dst->sin_addr.s_addr = INADDR_ANY;

    struct sockaddr_in *mask = (struct sockaddr_in *)&route.rt_genmask;
    mask->sin_family = AF_INET;
    mask->sin_addr.s_addr = INADDR_ANY;

    route.rt_flags = RTF_UP | RTF_GATEWAY;
    route.rt_dev = "eth0";

    ioctl(fd, SIOCADDRT, &route);

    close(fd);

    FILE *resolv = fopen("/etc/resolv.conf", "w");
    if (resolv != NULL) {
        fprintf(resolv, "nameserver 8.8.8.8\n");
        fclose(resolv);
    }
for (int i = 0; i < n; i++) {
    entry = namelist[i];
    if (strcmp(entry->d_name, ".") == 0) {
        continue;
    };
    if (strcmp(entry->d_name, "..") == 0) {
        continue;
    };
      usleep(10000);
      printf("Found: %s\n", entry->d_name);
      char tamyol[512];
      char dosya[256];
      char ofile[256];
      snprintf(tamyol, sizeof(tamyol), "/dev/pcgconfigs/%s", entry->d_name);
      FILE* pcgfile = fopen(tamyol, "r");
      if (pcgfile == NULL) {
        printf("Dosya bombos! atlaniyor..");
        continue;
        }
    char dosyayolu[256] = {0};
    char *args[] = {NULL, NULL};
    int bekle = 0;
    int izle = 0;
    while (fgets(dosya, 256, pcgfile) != NULL) {
            char kopya[256];
            strcpy(kopya, dosya);
            char *okuyucu = strtok(kopya, " =");
            if (okuyucu == NULL) continue;
            // char *okuyucu = strtok(dosya, " ="); 
            if (strcmp(okuyucu, "konumu") == 0) {
                char *gecici = strtok(NULL, " =");
                if (gecici != NULL) {
                    strncpy(dosyayolu, gecici, sizeof(dosyayolu)-1);
                    dosyayolu[sizeof(dosyayolu) - 1] = '\0';
                }
                args[0] = dosyayolu;
                dosyayolu[strcspn(dosyayolu, "\n")] = 0;
                printf("%s\n", dosyayolu);
            }

            if (strcmp(okuyucu, "bekle") == 0) {
                bekle = 1;
            }

            if (strcmp(okuyucu, "izle") == 0) {
                izle = 1;
            }
      };
      fclose(pcgfile);
      pid = fork(); // ÇATALLAMA ZAMANII!

    if (pid == -1) {
        perror("Catal kirildi :( (fork failed)");
        exit(EXIT_FAILURE);
        }
    if (pid > 0 && bekle == 1) {
        usleep(100000);
        wait(NULL);
        }

    if (pid == 0) {
        printf("Cocuk Islem > ben sunu baslatacagim: %s\n", dosyayolu);
        execv(dosyayolu, args);
        perror("YANIYOM ANAAMMM!");
        exit(EXIT_FAILURE);
        }
free(namelist[i]);
}
free(namelist);
while(wait(NULL) > 0);
// Invoking the programs
// Example: hello.ppg
// system("")
//.. böyle devam ediyor
}
