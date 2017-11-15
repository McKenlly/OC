//
// Created by bokoch on 14.11.17.
//

#ifndef LAB4_TEXTPROCESSOR_H
#define LAB4_TEXTPROCESSOR_H
#include "unistd.h"
#include <assert.h>
#include <sys/stat.h>
#include <csignal>
#define handle_error(msg)                             \
do { perror(msg); exit(EXIT_FAILURE); } while (0)
const unsigned long SIZE_PAGE = 4096;
const char DEFAULT_FILE_NAME[] = "test";
const char TEMP_FILE[] ="/tmp/tmpFile.XXXXXX";
unsigned long long DEFAULT_MAX_SIZE_FILE = 10024;
unsigned long long DEFAULT_MIN_SIZE_FILE = 100;
const int MAX_LENGTH_FILE = 256;
struct Options {
    Options();
    unsigned long long maxSizeFile;
    unsigned long long minSizeFile;
    int fd;
    unsigned long long fileSize;
    bool opened;
    char fileName[MAX_LENGTH_FILE];
};
Options::Options() {
    maxSizeFile = DEFAULT_MAX_SIZE_FILE;
    minSizeFile = DEFAULT_MIN_SIZE_FILE;
    fileSize = 0;
    opened = 0;
    strcpy(fileName, DEFAULT_FILE_NAME);
    fd = -1;
}

void Display(void) {
    printf("Usage next flags for run wrapper text file:\n");
    printf("usage -f <name> for load file(Default file = ./test)\n");
    printf("usage -smin <min_size> for file's(Default 300KB)\n");
    printf("usage -smax <max_size> for file's(Default 1MB)\n");
    printf("usage --help for help while the program is running\n");
}
void PrintCommandsHelp(void) {
    printf("You can enter the following commands while the program is running:\n");
    printf("print <character range> --- print range chars from X to Y of the file\n");
    printf("find -case-sensitive --- find substring case sensitive in file\n");
    printf("replace --- write text which need replaced and new string\n");
    printf("delete -pos <number-postion> -num-symbol <num-symbols> "
                   "--- delete text in file from position number-pos to position num-symbols\n");
    printf("quit --- exit from program\n");
}

void GetText(char *string) {
    int index = 0;
    char c;
    while ((c=getchar())&& c!=EOF) {
        string[index++] = c;

    }
    string[index] = '\0';
    printf("\n");
}
void CheckFile(Options& opt) {
    struct stat tmp;
    int status = stat(opt.fileName, &tmp);
    if (status) {
        perror("Stat: no find file");
        exit(EXIT_FAILURE);
    }
    if (tmp.st_size <= opt.maxSizeFile &&
        tmp.st_size >= opt.minSizeFile) {
        if (!opt.opened) {
            opt.fd = open(opt.fileName, O_RDWR);
            assert(opt.fd != -1);
            opt.opened = true;
        }
        opt.fileSize = tmp.st_size;
    }
    else {
        printf("Enter other file name. File have uncorrect size\n");
        exit(EXIT_FAILURE);
    }
   // opt.fd = fileDeskriptor;
}
void PrintPartText(Options &opt, int start, int finish) {
    CheckFile(opt);
    if (start > finish) {
        perror("start > finish");
        return;
    }
    int32_t size;
    off_t offset = 0;
    char *addr;
    bool startRunning = false;
    bool finishRunning = false;
    while (offset < opt.fileSize && (!startRunning || !finishRunning)) {
        size = (SIZE_PAGE > opt.fileSize-offset) ? opt.fileSize-offset : SIZE_PAGE;
        addr = (char *)mmap(nullptr, (size_t)size, PROT_READ, MAP_PRIVATE, opt.fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }
        //std::cout << "sizeAddr " << std::strlen(addr) << std::endl;
        if (start <= offset+size && !startRunning) {
            startRunning = true;
            start -= offset;
        }
        if (startRunning && !finishRunning) {
            if (!finishRunning && finish <= offset+size) {
                finishRunning = true;
                finish -= offset;
            }
            if (!finishRunning) {
                for (int32_t i = start; i < size; ++i) {
                    printf("%c", addr[i]);
                }
                start = 0;
            }
            else {
                for (int32_t i = start; i <= finish; ++i) {
                    printf("%c", addr[i]);

                }
            }
        }
        assert(munmap((void*)addr, size*sizeof(char)) != -1);
        offset += size;
    }
    printf("\n");
}


void ParseFlags(const int &argc, char (&argv)[MAX_LENGTH_FILE][MAX_LENGTH_FILE], Options& opt) {
    if (argc == 1) {
        Display();
        exit(EXIT_FAILURE);
    }
    for (char i = 1; i < argc; i+=2) {
        if (strcmp(argv[i], "-f") == 0) {
            //path with using file
            strcpy(opt.fileName, argv[i + 1]);
            continue;
        }
        if (strcmp(argv[i], "-smin") == 0) {
            //max size file
            opt.minSizeFile = atoi(argv[i + 1]);
            continue;
        }
        if (strcmp(argv[i], "-smax") == 0) {
            opt.maxSizeFile = atoi(argv[i + 1]);
            continue;
        }
        if (strcmp(argv[i], "--help") == 0) {
            PrintCommandsHelp();
            continue;
        }
        Display();
        perror("Wrong commands\n");
        exit(EXIT_FAILURE);
    }
}

void EnterLine(char (&line)[MAX_LENGTH_FILE][MAX_LENGTH_FILE]) {
    char tmp;
    int i = 0, j = -1;
    while ((tmp = getchar())!='\n') {
        j++;
        if (tmp == ' ') {
            line[i++][j] = '\0';
            j = -1;
            continue;
        }
        line[i][j] = tmp;
    }
    line[i][j+1] = '\0';
    return;
}
void FindSubString(Options& opt) {
    CheckFile(opt);
    printf("Enter Text, which neeed find\n");
    printf("> ");
    char findString[MAX_LENGTH_FILE];
    GetText(findString);
    off_t  offset = 0;
    char *addr;
    int countLine;
    bool not_find = false;
    size_t lengthf = strlen(findString);
    while (offset < opt.fileSize) {
        size_t size = (SIZE_PAGE > opt.fileSize - offset) ? opt.fileSize - offset : SIZE_PAGE;
        addr = (char *) mmap(nullptr, (size_t) size, PROT_READ, MAP_PRIVATE, opt.fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }
        int posBegin = -1;
        size_t length = strlen(addr);
        for (size_t ind = 0; ind < length - lengthf; ind++) {
            size_t j;
            not_find = false;
            if (!not_find && addr[ind] == '\n') {
                countLine = ind;
            }
            for (j = 0; j < lengthf; j++) {
                if (addr[ind + j] != findString[j]) {
                    not_find = true;
                    break;
                }
            }
            if (j == lengthf) {
                posBegin = ind;
                break;
            }
            if (!not_find) {
                offset += ind-MAX_LENGTH_FILE/2;
                break;
            }
        }

        if (posBegin < offset + size && posBegin != -1) {
            int j = countLine + 1;
            while (j < size && (addr[j] != '\n' || j < lengthf)) {
                printf("%c", addr[j]);
                j++;
            }
            printf("\n");
        }
        assert(munmap((void *) addr, size * sizeof(char)) != -1);
        offset += size;
    }

}

#endif //LAB4_TEXTPROCESSOR_H
