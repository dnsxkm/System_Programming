// 컴파일 타임 인터포지셔닝: -I. 로 이 파일이 시스템 malloc.h보다 먼저 포함됨
#define malloc(size) mymalloc(size)
#define free(ptr) myfree(ptr)

void *mymalloc(size_t size);
void myfree(void *ptr);
