#include <stdio.h>
#include <stdlib.h>

int g1 = 10;                // .data (초기값 있음)
int g2;                     //.bss (초기값 없음. -> 0으로 시작.)
const char *s = "hi";       // "hi" 문자열 자체는 .rodata, 포인터 s는 .data

int main(void){             // main의 기계어는 .text
    int local = 5;          // 스택 (섹션 아님)
    int *h = malloc(4);     // h가 가리키는 곳은 힙 
}