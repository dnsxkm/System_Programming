#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// O_NONBLOCK으로 파이프 버퍼 크기 측정 + 빈 파이프 read
int main(void)
{
    int pipefd[2];
    long total = 0;
    char c = 'x';

    pipe(pipefd);
    fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

    // 빈 파이프에서 read → block 대신 EAGAIN
    if (read(pipefd[0], &c, 1) < 0)
        printf("read on empty pipe : errno = %d (%s)\n", errno, strerror(errno));

    // 가득 찰 때까지 1바이트씩 write
    while (write(pipefd[1], &c, 1) == 1)
        total++;
    printf("write on full pipe : errno = %d (%s)\n", errno, strerror(errno));
    printf("pipe capacity      : %ld bytes\n", total);
    return 0;
}
