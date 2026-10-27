#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// 자식이 쓰고 부모가 읽기 (Safe Pipe and Fork)
int main(void)
{
    int pipefd[2];
    pid_t pid;
    char buf[6];

    pipe(pipefd);
    if ((pid = fork()) == 0) {          // 자식: 쓰기만 함
        close(pipefd[0]);               // 안 쓰는 읽기 끝을 닫음
        write(pipefd[1], "Hello", 6);   // "Hello" + '\0' = 6바이트
        close(pipefd[1]);
        return 0;
    } else {                            // 부모: 읽기만 함
        close(pipefd[1]);               // 안 쓰는 쓰기 끝을 닫아야 EOF를 받을 수 있음
        read(pipefd[0], buf, 6);        // 자식이 쓸 때까지 block
        puts(buf);
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }
    return 0;
}
