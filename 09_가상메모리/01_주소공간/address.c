#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int g_data = 1;                         // .data
int g_bss;                              // .bss

// 가상주소를 VPN(가상 페이지 번호)과 VPO(페이지 안 오프셋)로 나눠 출력
static void show(const char *name, void *p, long page)
{
    unsigned long va = (unsigned long)p;
    printf("%-8s VA = 0x%012lx  VPN = 0x%09lx  VPO = 0x%03lx\n",
           name, va, va / page, va % page);
}

int main(void)
{
    long page = sysconf(_SC_PAGESIZE);
    int local = 0;
    int *heap = malloc(16);

    printf("page size = %ld bytes (p = 12 bits)\n", page);
    show("code", (void *)main, page);
    show("rodata", (void *)"hello", page);
    show(".data", &g_data, page);
    show(".bss", &g_bss, page);
    show("heap", heap, page);
    show("stack", &local, page);
    free(heap);
    return 0;
}
