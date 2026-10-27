#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// Unix I/O(시스템 콜)만으로 만든 cat
int main(int argc, char *argv[])
{
    char buf[1024];
    int fd, bytes;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");                         // errno를 사람이 읽을 수 있게 출력
        return 1;
    }
    while ((bytes = read(fd, buf, sizeof(buf))) > 0) {   // 0 = EOF, <0 = 에러
        if (write(1, buf, bytes) < 0) {         // fd 1 = 표준 출력
            perror("write");
            return 1;
        }
    }
    if (bytes < 0)
        perror("read");
    close(fd);
    return bytes < 0;
}
