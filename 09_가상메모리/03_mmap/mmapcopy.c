#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

// 파일을 메모리에 매핑해서 그대로 stdout에 씀 (read 없이)
void mmapcopy(int fd, int size)
{
    char *bufp;                         // 매핑된 영역을 가리키는 포인터

    bufp = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (bufp == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    write(1, bufp, size);
    munmap(bufp, size);
}

int main(int argc, char **argv)
{
    struct stat stat;
    int fd;

    if (argc != 2) {
        printf("usage: %s <filename>\n", argv[0]);
        exit(0);
    }
    fd = open(argv[1], O_RDONLY, 0);
    fstat(fd, &stat);                   // 파일 크기 얻기
    mmapcopy(fd, stat.st_size);
    exit(0);
}
