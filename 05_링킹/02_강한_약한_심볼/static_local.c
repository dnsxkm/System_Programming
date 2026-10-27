#include <stdio.h>

int f(void) { static int x = 0; return x; }   // .bss, 지역 심볼 x.1
int g(void) { static int x = 1; return x; }   // .data, 지역 심볼 x.0

int main(void)
{
    printf("f() = %d, g() = %d\n", f(), g());
    return 0;
}
