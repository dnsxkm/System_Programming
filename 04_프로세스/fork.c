#include <stdio.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();
    if (pid == 0) {
        puts("In child");
    }   else {
        printf("In parent, child PID = %d\n", pid);
    }
}