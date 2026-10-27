#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// 쓰기 끝을 하나라도 열어 두면 read가 EOF(0)를 받지 못한다
int main(int argc, char *argv[])
{
    int pipefd[2], n;
    char buf[64];
    int close_write = (argc > 1);       // 인자가 있으면 부모도 쓰기 끝을 닫음

    (void)argv;                         // 인자 내용은 보지 않고 개수만 씀

    pipe(pipefd);
    if (fork() == 0) {
        close(pipefd[0]);
        write(pipefd[1], "data", 4);
        close(pipefd[1]);               // 자식은 쓰기 끝을 닫고 종료
        return 0;
    }
    if (close_write)
        close(pipefd[1]);
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0)
        printf("read %d bytes\n", n);
    printf("read returned %d -> EOF\n", n);   // close_write가 0이면 여기 도달 못함
    wait(NULL);
    return 0;
}
