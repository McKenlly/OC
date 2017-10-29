#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    //удаление файла
    if (fork() == 0) {
        execl("/bin/ls", "-all", (char *) 0);
        perror("exec one failed");
        exit(1);
    }
    if (fork() == 0) {
        execl("rm", "undef", (char *) 0);
        perror("exec two failed");
        exit(2);
    }
    if (fork() == 0) {
        execl("/bin/ls", "-all", (char *) 0);
        perror("exec three failed");
        exit(3);
    }
    printf("Parent program ending\n");
}
