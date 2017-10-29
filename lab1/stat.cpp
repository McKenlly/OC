#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    struct stat sb;

    if (argc != 2) {
        fprintf(stderr, "Использование: %s <путь>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("Тип файла: ");

    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:
            printf("блочное устройство\n");
            break;
        case S_IFCHR:
            printf("символьное устройство\n");
            break;
        case S_IFDIR:
            printf("каталог\n");
            break;
        case S_IFIFO:
            printf("FIFO/канал\n");
            break;
        case S_IFLNK:
            printf("символьная ссылка\n");
            break;
        case S_IFREG:
            printf("обычный файл\n");
            break;
        case S_IFSOCK:
            printf("сокет\n");
            break;
        default:
            printf("неизвестно?\n");
            break;
    }

    printf("номер inode: %ld\n", (long) sb.st_ino);

    printf("Режим доступа:            %lo (octal)\n",
           (unsigned long) sb.st_mode);
    printf("Режим доступа:            %lo (octal)\n",
           (unsigned long) sb.st_mode);

    printf("Кол-во ссылок:            %ld\n", (long) sb.st_nlink);
    printf("Владелец:                 UID=%ld   GID=%ld\n",
           (long) sb.st_uid, (long) sb.st_gid);

    printf("Предпоч. размер бл. в/в:  %ld байт\n",
           (long) sb.st_blksize);
    printf("Размер файла:             %lld байт\n",
           (long long) sb.st_size);
    printf("Выделено блоков:          %lld\n",
           (long long) sb.st_blocks);

    printf("Посл. изм. состояния:     %s", ctime(&sb.st_ctime));
    printf("Посл. доступ к файлу:     %s", ctime(&sb.st_atime));
    printf("Посл. изм. файла:         %s", ctime(&sb.st_mtime));

    exit(EXIT_SUCCESS);
}
 
