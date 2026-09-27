#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    
    if (pid == 0) {                     // 자식
        puts("In child");
        exit(42);
    } else {                            // 부모
        int status;
        waitpid(pid, &status, 0);       // 자식이 끝날 때까지 대기
        printf("Child exited with status %d\n",
        WEXITSTATUS(status));           // 종료 코드(42) 꺼내기
    }
}

// 순서가 결정적이다.
// waitpid() 호출은 자식이 종료될 때까지 반환하지 않는다.