#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// 읽기 전용 페이지(.rodata)에 쓰면 MMU 권한 검사에 걸려 SIGSEGV
int main(void)
{
    int status;

    if (fork() == 0) {
        char *s = "software";           // 문자열 리터럴 → .rodata (PTE: WRITE=No)
        s[0] = 'h';                     // 보호 폴트
        printf("never printed\n");
        exit(0);
    }
    wait(&status);
    if (WIFSIGNALED(status))
        printf("child killed by signal %d (%s)\n", WTERMSIG(status), strsignal(WTERMSIG(status)));
    return 0;
}
