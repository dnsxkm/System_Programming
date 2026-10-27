#include <stdio.h>
#include <stdlib.h>

// 이 프로세스의 가상 메모리 영역(vm_area) 목록을 출력
int main(void)
{
    char line[512];
    FILE *fp = fopen("/proc/self/maps", "r");
    void *heap = malloc(1);             // 힙 영역이 생기도록

    while (fgets(line, sizeof(line), fp))
        fputs(line, stdout);
    fclose(fp);
    free(heap);
    return 0;
}
