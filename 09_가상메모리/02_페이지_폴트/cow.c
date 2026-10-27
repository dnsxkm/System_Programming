#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define SIZE (64L * 1024 * 1024)        // 64MB

int value = 100;

static long minor_faults(void)
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_minflt;
}

// fork 후 같은 가상주소, 다른 값 + 쓰기할 때 페이지 복사(Copy-on-Write)
int main(void)
{
    char *big = malloc(SIZE);
    long before;
    memset(big, 1, SIZE);               // 부모에서 미리 전부 할당

    if (fork() == 0) {
        value = 200;                    // 쓰기 → 보호 폴트 → 이 페이지만 복사
        printf("child : &value = %p, value = %d\n", (void *)&value, value);

        before = minor_faults();
        for (long i = 0; i < SIZE; i += 4096)
            big[i] = 2;                             // 쓰기 → 페이지마다 복사
        printf("child : write 64MB -> %ld faults (64MB / 4KB = %ld pages)\n",
               minor_faults() - before, SIZE / 4096);
        exit(0);
    }
    wait(NULL);
    printf("parent: &value = %p, value = %d\n", (void *)&value, value);
    printf("parent: big[0] = %d (자식의 쓰기가 보이지 않음)\n", big[0]);
    free(big);
    return 0;
}
