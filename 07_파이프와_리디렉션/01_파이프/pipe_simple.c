#include <stdio.h>
#include <unistd.h>

// 한 프로세스 안에서 파이프에 쓰고 읽기
int main(void)
{
    int pipefd[2], rval, wval = 42;

    if (pipe(pipefd) < 0) {             // pipefd[0] = 읽기 끝, pipefd[1] = 쓰기 끝
        perror("pipe");
        return 1;
    }
    printf("pipefd[0] = %d (read), pipefd[1] = %d (write)\n", pipefd[0], pipefd[1]);

    write(pipefd[1], &wval, sizeof(wval));
    read(pipefd[0], &rval, sizeof(rval));
    printf("%d\n", rval);
    return 0;
}
