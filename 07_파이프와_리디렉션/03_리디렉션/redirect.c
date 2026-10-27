#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// 셸의 "> output.txt"를 직접 구현
int main(void)
{
    int fd;

    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    dup2(fd, 1);                        // fd 1(stdout)이 output.txt를 가리키게 함
    close(fd);                          // 원래 fd는 더 필요 없음

    puts("Redirected output!");         // 화면이 아니라 파일로 감
    return 0;
}
