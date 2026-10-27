#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// 셸의 "ls -1 /usr | wc -l" 을 pipe + fork + dup2 + exec 로 구현
int main(void)
{
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(1);
    }

    if ((pid1 = fork()) == 0) {         // 왼쪽 명령: stdout → 파이프 쓰기 끝
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("ls", "ls", "-1", "/usr", NULL);
        perror("execlp ls");            // exec가 성공하면 여기로 오지 않음
        exit(1);
    }

    if ((pid2 = fork()) == 0) {         // 오른쪽 명령: stdin ← 파이프 읽기 끝
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        exit(1);
    }

    // 부모가 쓰기 끝을 닫지 않으면 wc가 EOF를 못 받아 끝나지 않음
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
