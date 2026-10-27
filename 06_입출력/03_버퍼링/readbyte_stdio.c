#include <stdio.h>

// 1바이트씩 fread() → 실제 read()는 버퍼(4KB) 단위로만
int main(void)
{
    FILE *fp = fopen("megabyte.dat", "rb");
    int total = 0;
    unsigned char c;

    while (!ferror(fp) && fread(&c, 1, 1, fp) == 1) {
        total += c;
    }
    printf("total = %d\n", total);
    fclose(fp);
    return 0;
}
