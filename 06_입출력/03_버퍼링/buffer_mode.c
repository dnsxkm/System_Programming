#include <stdio.h>
#include <unistd.h>

// stdout 버퍼링 방식은 출력 대상에 따라 다르다
//   터미널 → 줄 단위(line buffered),  파일/파이프 → 블록 단위(fully buffered)
int main(void)
{
    printf("1. printf\n");              // 터미널이면 \n에서 바로 출력, 파이프면 버퍼에 남음
    write(1, "2. write\n", 9);          // 시스템 콜은 버퍼를 거치지 않고 바로 출력
    printf("3. printf\n");
    return 0;                           // main 리턴 시 남은 버퍼가 비워짐
}
