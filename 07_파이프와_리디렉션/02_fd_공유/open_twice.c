#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// 같은 파일을 두 번 open → 열린 파일 테이블 항목이 2개 → 파일 위치가 따로
int main(void)
{
    char c1, c2;
    int fd1 = open("abc.txt", O_RDONLY);
    int fd2 = open("abc.txt", O_RDONLY);

    read(fd1, &c1, 1);
    read(fd2, &c2, 1);
    printf("fd1=%d -> '%c', fd2=%d -> '%c'\n", fd1, c1, fd2, c2);
    return 0;
}
