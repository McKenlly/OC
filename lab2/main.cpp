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
#define READ 0
#define WRITE 1
const int FAILURE_CREATE_PID = -1;

const int FILE_EXIST = 0;

const int FAIL_OPEN_FILE = -1;

const int DEFAULT_COUNT_FILES = 2;

const int MAX_SIZE_COMMAND_LINE = 1000;

const char EXIT_FROM_PROGRAM[] = "quit\n";

void Performing(char *cmd, char *filename);

int fd[2];

int main(int argc, char *argv[]) {
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
        if (strcmp(cmd, EXIT_FROM_PROGRAM) == 0) {
            printf("STATUS: COMPLETE\n");
            break;
        }
        Performing(cmd, outFile);
    }
    return 0;
}

void Performing(char *command, char *fileName) {
    int outFileID;
    pid_t pidChild;
    //Создаем канал
    pipe(fd);
    int status;
    pidChild = fork();
    if (pidChild == FAILURE_CREATE_PID) {
        perror("Child process not created");
        return;
    }
    //------fd[0] - read
    //------fd[1] - write
    int sizeStr;
    if (pidChild == 0) {
        close(STDOUT_FILENO);
        //Замена дескриптора вывода на дескриптор записи в канал
        dup2(fd[WRITE], STDOUT_FILENO);

        close(fd[WRITE]);
        close(fd[READ]);

        execl("/bin/bash", "/bin/bash", "-c", command, (char *) 0);
        perror("Command not perfom\n\n");
    } else {
        wait(&status);
        //Открытие файла
        if (access(fileName, F_OK) == FILE_EXIST) {
            outFileID = open(fileName, O_APPEND | O_RDWR | S_IRUSR | S_IWUSR);
        } else {
            outFileID = open(fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        }
        if (outFileID == FAIL_OPEN_FILE) {
            perror("File not open");
            exit(EXIT_FAILURE);
        }
        //Признак конца вывода команды
        write(fd[WRITE], "\0", sizeof(char));
        close(fd[WRITE]);
        char answer[MAX_SIZE_COMMAND_LINE];
        //Выводим команду!!!
        sizeStr = read(fd[READ], answer, MAX_SIZE_COMMAND_LINE*sizeof(char));
        write(outFileID, answer, sizeStr);
        close(outFileID);
    }
    return;
}
