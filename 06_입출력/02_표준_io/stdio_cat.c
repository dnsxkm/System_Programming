#include <stdio.h>

// 표준 I/O(버퍼링 스트림)로 만든 cat
int main(int argc, char *argv[])
{
    char buf[1024];
    FILE *fp;
    size_t bytes;

    if (argc < 2 || (fp = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        return 1;
    }
    while ((bytes = fread(buf, 1, sizeof(buf), fp)) > 0) {
        if (fwrite(buf, 1, bytes, stdout) == 0)
            return 1;
    }
    // fread는 EOF와 에러 모두 0을 반환 → feof/ferror로 구분
    printf("[feof=%d ferror=%d]\n", feof(fp) != 0, ferror(fp) != 0);
    fclose(fp);
    return 0;
}
