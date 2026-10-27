#include <stdio.h>

int x = 7;                      // 강한 심볼 (초기화된 전역)
int y = 5;                      // 강한 심볼

void p2(void);

int main(void)
{
    printf("before: x = %d, y = %d\n", x, y);
    p2();                       // p2.c의 double x에 값을 씀
    printf("after : x = %d, y = %d\n", x, y);
    return 0;
}
