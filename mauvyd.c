#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

int main() {
    struct dirent *entry;
    DIR* dirFile = opendir("/dev/pcgconfigs");
    // Buraya listeleme fonksiyonu gelecek.
printf("----------------------------------------\n");
printf("----- MAUVYD Configuration Startup -----\n");
printf("-------------Listing Files..------------\n");
while ((entry = readdir(dirFile)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0) {
        continue;
    };
    if (strcmp(entry->d_name, "..") == 0) {
        continue;
    }
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
    while (fgets(dosya, 256, pcgfile) != NULL) {
            printf("%s", dosya);
            char *okuyucu = strtok(dosya, "=");
            char *dosyayolu = strtok(NULL, " =");
            dosyayolu[strcspn(dosyayolu, "\n")] = 0;
            char *args[] = {dosyayolu, NULL};
            printf("%s\n", dosyayolu);
            execv(dosyayolu, args);
      };
   }
// Invoking the programs
// Example: hello.ppg
// system("")
//.. böyle devam ediyor
}
