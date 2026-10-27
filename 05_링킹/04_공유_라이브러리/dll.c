#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int main(void)
{
    void *handle;
    void (*addvec)(int *, int *, int *, int);
    char *error;

    // 실행 중에 공유 라이브러리를 불러옴 (RTLD_LAZY: 함수는 처음 호출할 때 해석)
    handle = dlopen("./libvector.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    // 이름으로 함수 주소를 얻음
    addvec = dlsym(handle, "addvec");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    addvec(x, y, z, 2);         // 일반 함수처럼 호출
    printf("z = [%d %d]\n", z[0], z[1]);

    // 라이브러리 해제
    if (dlclose(handle) < 0) {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    return 0;
}
