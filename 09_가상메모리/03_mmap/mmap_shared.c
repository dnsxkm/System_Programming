#define _GNU_SOURCE                     // memmem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

// MAP_PRIVATE: 쓰기가 내 복사본에만 (COW)   MAP_SHARED: 쓰기가 파일에 반영
static void patch(const char *path, int flags, const char *name)
{
    struct stat st;
    int fd = open(path, O_RDWR);
    char *p, *hit;

    fstat(fd, &st);
    p = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, flags, fd, 0);
    if ((hit = memmem(p, st.st_size, "software", 8)) != NULL)
        memcpy(hit, "hackers!", 8);     // 포인터로 바로 수정 (read/lseek/write 없음)
    msync(p, st.st_size, MS_SYNC);
    munmap(p, st.st_size);
    close(fd);
    printf("after %-11s: ", name);
    fflush(stdout);
    system("cat msg.txt");
}

int main(void)
{
    system("echo 'Hello the school of software' > msg.txt");
    patch("msg.txt", MAP_PRIVATE, "MAP_PRIVATE");
    patch("msg.txt", MAP_SHARED, "MAP_SHARED");
    unlink("msg.txt");
    return 0;
}
