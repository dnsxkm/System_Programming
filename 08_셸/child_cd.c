#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// cd가 외부 프로그램이면 안 되는 이유: 자식의 chdir은 부모에게 영향이 없다
int main(void)
{
    char buf[256];

    if (fork() == 0) {
        chdir("/tmp");
        printf("child  cwd = %s\n", getcwd(buf, sizeof(buf)));
        return 0;
    }
    wait(NULL);
    printf("parent cwd = %s\n", getcwd(buf, sizeof(buf)));
    return 0;
}
