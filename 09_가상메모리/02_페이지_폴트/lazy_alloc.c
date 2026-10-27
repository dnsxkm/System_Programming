#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#define SIZE (256L * 1024 * 1024)       // 256MB

// 현재 실제로 물리 메모리에 올라온 크기(RSS)
static long rss_kb(void)
{
    char line[256];
    long kb = -1;
    FILE *fp = fopen("/proc/self/status", "r");
    while (fgets(line, sizeof(line), fp))
        if (sscanf(line, "VmRSS: %ld kB", &kb) == 1)
            break;
    fclose(fp);
    return kb;
}

static long minor_faults(void)
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return ru.ru_minflt;                // 디스크 I/O 없이 처리된 페이지 폴트 수
}

int main(void)
{
    char *p;

    printf("start          : RSS = %7ld kB, faults = %ld\n", rss_kb(), minor_faults());
    p = malloc(SIZE);                   // 가상 주소 공간만 예약
    printf("after malloc   : RSS = %7ld kB, faults = %ld\n", rss_kb(), minor_faults());
    memset(p, 1, SIZE / 2);             // 절반만 실제로 접근 → 그만큼 페이지 폴트
    printf("touch half     : RSS = %7ld kB, faults = %ld\n", rss_kb(), minor_faults());
    memset(p, 1, SIZE);                 // 전부 접근
    printf("touch all      : RSS = %7ld kB, faults = %ld\n", rss_kb(), minor_faults());
    free(p);
    return 0;
}
