#include <stdio.h>
//#include <sys/types.h>
#include <unistd.h>
int main() {
    char buff[66];
    if (getcwd(buff, sizeof(buff))==NULL)
        perror("ошибка getcwd");
    else
        printf("%s", buff);
    sleep(30);
}
