#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// dup도 같은 열린 파일 항목을 가리킴 → 파일 위치 공유
int main(void)
{
    char c1, c2;
    int fd1 = open("abc.txt", O_RDONLY);
    int fd2 = dup(fd1);                 // 비어 있는 가장 작은 번호를 받음

    read(fd1, &c1, 1);
    read(fd2, &c2, 1);
    printf("fd1=%d -> '%c', fd2=%d -> '%c'\n", fd1, c1, fd2, c2);
    return 0;
}
