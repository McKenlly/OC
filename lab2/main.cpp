#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
const int FAILURE_CREATE_PID = -1;
const int FILE_EXIST = 0;
const int STDIN_DESCRIPTOR_DEFAULT = 1;
const int STDERR_DESCRIPTOR_DEFAULT = 2;
const int FAIL_OPEN_FILE = -1;
const int DEFAULT_COUNT_FILES = 2;
const int MAX_SIZE_COMMAND_LINE = 128;
const char *EXIT_FROM_PROGRAM = "quit\n";
void Performing(char* cmd, char* filename);

int main(int argc, char *argv[])
{
    if (argc != DEFAULT_COUNT_FILES) {
        perror("Enter output file name for write!");
        exit(EXIT_FAILURE);
    }
    printf("Enter commands for bash\nFor exit enter quit\n");
    char outFile[MAX_SIZE_COMMAND_LINE];
    strcpy(outFile, argv[DEFAULT_COUNT_FILES - 1]);
    while (true) {
        printf("> ");
        char cmd[MAX_SIZE_COMMAND_LINE];
        fgets(cmd, MAX_SIZE_COMMAND_LINE, stdin);
        if (strcmp(cmd, EXIT_FROM_PROGRAM) == 0){
            printf("STATUS: COMPLETE\n");
            break;
        }
        Performing(cmd, outFile);
    }
    return 0;
}

void Performing(char* command, char* fileName)
{
    int outFileID;
    pid_t pidChild;
    int status;
    pidChild = fork();
    if (pidChild == FAILURE_CREATE_PID) {
        perror("Child process not created");
        return;
    } else if (pidChild == 0) {
        if(access(fileName, F_OK) == FILE_EXIST) {
            outFileID = open(fileName, O_APPEND | O_RDWR | S_IRUSR | S_IWUSR);
        } else {
            outFileID = open(fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        }
    if (outFileID == FAIL_OPEN_FILE) {
        perror("File not open");
        return;
    }
    dup2(outFileID, STDIN_DESCRIPTOR_DEFAULT);
    //dup2(outFileID, STDERR_DESCRIPTOR_DEFAULT);
    write(outFileID, "==============================\n", 32);
    close(outFileID);
    execl("/bin/bash", "/bin/bash", "-c", command, (char*)0);
    perror("Command not perfom\n\n");
    } else {
        wait(&status);
    }
    return;
}