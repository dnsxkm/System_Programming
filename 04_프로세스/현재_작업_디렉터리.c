#include <stdio.h>

int main(void) {    


    // 모든 프로세스에는 현재 작업 디렉토리가 있다(current working directory)
    // 모든 상대 경로는 이 디렉토리를 기준으로 한다
    // 시스템 콜 chdir()로 설정하고, getcwd()로 얻는다
    // getwd() 라는 함수도 있지만 이는 위험해서 사용하면 안됨. --> 버퍼 크기를 받지 않음. == gets()와 같은 문제

    char *getwd(char *buf);                         // 사용금지
    char *getcwd(char *buf, size_t size);           // 이것을 사용

}