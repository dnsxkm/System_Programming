#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

// open 플래그와 권한(mode), lseek 동작 확인
int main(void)
{
    int fd;
    struct stat st;

    // ① O_CREAT|O_EXCL: 없을 때만 생성 (원자적)
    fd = open("lock.file", O_CREAT | O_EXCL | O_WRONLY, 0664);
    printf("1st O_CREAT|O_EXCL -> fd = %d\n", fd);
    close(fd);
    fd = open("lock.file", O_CREAT | O_EXCL | O_WRONLY, 0664);
    printf("2nd O_CREAT|O_EXCL -> fd = %d, errno = %d (%s)\n", fd, errno, strerror(errno));

    // ② 권한 비트: umask(022)가 적용되어 0666 -> 0644
    fd = open("data.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);
    write(fd, "hello\n", 6);
    fstat(fd, &st);
    printf("mode = %o (asked 0666, umask 022)\n", st.st_mode & 0777);
    close(fd);

    // ③ O_APPEND: 항상 파일 끝에 씀
    fd = open("data.txt", O_WRONLY | O_APPEND);
    lseek(fd, 0, SEEK_SET);                     // 처음으로 옮겨도
    write(fd, "world\n", 6);                    // 끝에 붙음
    close(fd);

    // ④ O_TRUNC: 열자마자 0바이트로 (fopen "w"와 같음)
    fd = open("data.txt", O_RDONLY);
    printf("size before O_TRUNC = %ld\n", (long)lseek(fd, 0, SEEK_END));
    close(fd);
    fd = open("data.txt", O_WRONLY | O_TRUNC);
    printf("size after  O_TRUNC = %ld\n", (long)lseek(fd, 0, SEEK_END));
    close(fd);

    unlink("lock.file");
    unlink("data.txt");
    return 0;
}
