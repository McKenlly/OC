//
// Created by bokoch on 11.11.17.
//
#include <sys/mman.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <fcntl.h>
#include <assert.h>
#include <zconf.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "TextProcessor.h"

/*void ParseFlags(const int &, char (&line)[MAX_LENGTH_FILE][MAX_LENGTH_FILE], Options &);
void Display(void);
void PrintCommandsHelp(void);
void CheckFile(Options &);
void EnterLine(char (&line)[MAX_LENGTH_FILE][MAX_LENGTH_FILE]);*/
void ReplaceText(Options &);
void DeleteText(Options &, int);
//void PrintPartText(Options &, int, int);
int main(int argc, char *argv[]) {
    Options opt;
    char make[MAX_LENGTH_FILE][MAX_LENGTH_FILE] = {"./a.out", "-f", "/home/bokoch/CLionProjects/OC/lab4/test", "-smin", "100", "-smax", "100000"};
    int k = 7;
    ParseFlags(k, make, opt);
    CheckFile(opt);
    char commandLine[MAX_LENGTH_FILE][MAX_LENGTH_FILE];
    do {
        EnterLine(commandLine);
        if (strcmp(commandLine[0], "print") == 0) {
            PrintPartText(opt, atoi(commandLine[1]), atoi(commandLine[2]));
            continue;
        }
        if (strcmp(commandLine[0], "replace") == 0) {
                ReplaceText(opt);
                printf("OK\n");
                continue;
        }
        if (strcmp(commandLine[0], "find") == 0) {
            FindSubString(opt);
            continue;
        }
        if (strcmp(commandLine[0], "clear") == 0) {
            system("clear");
            continue;
        }
        if (strcmp(commandLine[0], "help") == 0) {
            PrintCommandsHelp();
            continue;
        }


    } while(strcmp(commandLine[0], "quit") != 0);
    return 0;
}

void ReplaceText(Options& opt) {
    CheckFile(opt);

    printf("Enter Text, which neeed find\n");
    printf("> ");
    char findString[MAX_LENGTH_FILE];
    GetText(findString);

    printf("Enter Text, which neeed insert\n");
    printf("> ");
    char insertString[MAX_LENGTH_FILE];
    GetText(insertString);

    size_t size;
    char tempFileName[] = "/home/bokoch/CLionProjects/OC/lab4/testy";
    int td = creat(tempFileName, 0666);
    off_t offset = 0;
    char *addr;
    bool not_find = false;
    bool replaced = false;
    size_t lengthf = strlen(findString);
    while (offset < opt.fileSize) {
        size = (SIZE_PAGE > opt.fileSize - offset) ? opt.fileSize - offset : SIZE_PAGE;
        addr = (char *) mmap(nullptr, (size_t) size, PROT_READ, MAP_PRIVATE, opt.fd, offset);
        if (addr == MAP_FAILED) {
            handle_error("mmap");
        }
        int posBegin = -1;
        size_t length = strlen(addr);
        for (size_t ind = 0; ind < length - lengthf && !replaced; ind++) {
            size_t j;
            not_find = false;
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
                offset -= MAX_LENGTH_FILE/2;
                break;
            }
        }
        if (posBegin <= offset + size && posBegin != -1) {
            write(td, addr, posBegin - offset);
            write(td, insertString, strlen(insertString));
            replaced = true;
            write(td, addr+posBegin-offset+lengthf, size-(posBegin-offset+lengthf));
        } else {
            if (!not_find) {
              write(td, addr, size-MAX_LENGTH_FILE/2);
            } else {
                write(td, addr, size);
            }
        }
        assert(munmap((void *) addr, size * sizeof(char)) != -1);
        offset += size;
    }
    close(td);
    close(opt.fd);
    rename(tempFileName, opt.fileName);
    opt.opened = false;
    CheckFile(opt);
}
