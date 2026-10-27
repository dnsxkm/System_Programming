#include <stdio.h>
#include <stdlib.h>

// 자식 프로세스는 export된(환경) 변수만 볼 수 있다
int main(void)
{
    char *v = getenv("MYVAR");
    printf("MYVAR = %s\n", v ? v : "(없음)");
    return 0;
}
