#include <stdio.h>
#include <time.h>

int main() {
    printf("Saat servisi baslatiliyor...\n");
    fflush(stdout);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (t == NULL) {
        perror("zaman alinamadi");
        return -1;
    }

    printf("Bismillahirahmanirrahim: %d-%d-%d %02d:%02d:%02d\n",
        t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
        t->tm_hour + 3, t->tm_min, t->tm_sec);
    fflush(stdout);

    return 0;
}
