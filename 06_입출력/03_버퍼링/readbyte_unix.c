#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

// 1바이트씩 read() → 바이트마다 시스템 콜
int main(void)
{
    int fd = open("megabyte.dat", O_RDONLY);
    int total = 0;
    unsigned char c;

    while (read(fd, &c, 1) == 1) {
        total += c;
    }
    printf("total = %d\n", total);
    close(fd);
    return 0;
}
