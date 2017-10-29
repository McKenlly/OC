#include <iostream>
#include "zconf.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // open
#include <sstream>
#include <sys/stat.h>

const char *PATH = "/home/serega/Desktop/";
const char *NEW_DIR = "/home/serega/Desktop/test";
const char *NEW_FILE = "/home/serega/Desktop/test/run";
const char *OLD_FILE = "./undef";
int main() {
//    link - позволяет одному и тому же файлу появляться под двумя и более файлами в разных каталогах
//    Параллельное использование файла с разных каталогов сразу приводит к изменениям в общем
    if (mkdir(NEW_DIR, 0777) == -1)
    {
        perror("Fail. Not created directory");
        exit(-2);
    }
    int fd;
    link("/home/serega/CLionProjects/OC/lab1/undef", "/home/serega/Desktop/test/run");
    if (fork() == 0) {
        fd = open(NEW_FILE, O_RDWR);
        if (fd == -1) {
            rmdir(NEW_DIR);
            perror("File not open");
            exit(-1);
        }
        else
            write(fd, "It's me new file: run\n", 100);

    }
    else {
        rmdir(NEW_DIR);
        write(fd, "It's me new file: run\n", 100);
        unlink("/home/serega/Desktop/run");
        close(fd);
    }

    return 0;
} 
