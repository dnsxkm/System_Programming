#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// fork 후 부모와 자식은 같은 열린 파일 항목을 공유 → 파일 위치도 공유
int main(void)
{
    char c;
    int fd = open("abc.txt", O_RDONLY);

    if (fork() == 0) {
        read(fd, &c, 1);
        printf("child  read '%c'\n", c);
        return 0;
    }
    wait(NULL);                         // 자식이 먼저 읽도록 기다림
    read(fd, &c, 1);
    printf("parent read '%c'\n", c);    // 'a'가 아니라 'b' (위치가 공유됨)
    return 0;
}
