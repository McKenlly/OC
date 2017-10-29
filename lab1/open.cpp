#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "zconf.h"
#include <iostream>
#include <sys/stat.h>
main (){
    int fh1, fh2;
    fh1 = open("data1", O_RDONLY);
    if (fh1 == -1) {
        perror("open failed on input file");
        //exit(1);
    }
    fh2 = open("data2", O_WRONLY|O_TRUNC|O_CREAT,
    S_IREAD|S_IWRITE);
    if (fh2 == -1) {
        perror("open failed on output file");
        exit(1);
    }
}
