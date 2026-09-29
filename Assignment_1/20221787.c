#include "20221787.h"

unsigned char *read_bits(const char *filename, size_t *out_len) {
    FILE *fp = fopen(filename, "r");            // 1) 파일 열고 결과를 fp에 저장
    if (fp == NULL) {                           // 파일이 없으면 종료
        perror(filename);
        exit(1);
    }


    size_t cap = 16;                            // 배열 용량 (처음엔 16바이트)
    size_t len = 0;                             // 실제로 채운 바이트 수
    unsigned char *bytes = malloc(cap);
    if (bytes == NULL) { perror("malloc"); exit(1); }

    unsigned char cur = 0;                      // 조립 중인 바이트
    int nbits = 0;                              // cur에 몇 비트 넣었는지
    int ch;                                     // fgetc는 EOF(-1) 때문에 int로 받음

    while ((ch = fgetc(fp)) != EOF) {          // 2) 한 글자씩 읽기
        if (ch != '0' && ch != '1') continue;   // 줄바꿈 등은 무시

        cur = (unsigned char)((cur << 1) | (ch -'0'));  // 3) 왼쪽으로 밀고 새 비트 추가
        nbits++;

        if (nbits == 8) {                       // 8비트가 모이면 1바이트 완성
            if (len == cap) {                   // 배열이 꽉 차면 2배로 늘림
                cap *= 2;
                unsigned char *tmp = realloc(bytes, cap);
                if (tmp == NULL) { perror("realloc"); exit(1); }
                bytes = tmp;
            }
            bytes[len++] = cur;                 // 4) 저장
            cur = 0;
            nbits = 0;
        }
    }
    fclose(fp);
    *out_len = len;
    return bytes;
}

int main(int argc, char *argv[]) {
    /* 인자가 있으면 그 파일, 없으면 "input" */
    const char *filename = (argc > 1) ? argv[1] : "input";
    size_t len;
    unsigned char *bytes = read_bits(filename, &len);

    /* 확인용: 16진수 덤프 (제출 전에 지우기) */
    for (size_t i = 0; i < len; i++)
        printf("%02x ", bytes[i]);
    printf("\n");

    /* 1. signed char: 같은 바이트를 부호 있는 수로 해석 */
    for (size_t i = 0; i < len; i++)
        printf("<%d> ", (signed char)bytes[i]);
    printf("\n");

    /* 2. ASCII: 출력 가능한 문자(32-126)만 그대로, 나머지는 '.' */
    for (size_t i = 0; i < len; i++) {
        if (bytes[i] >= 32 && bytes[i] <= 126)
            printf("<%c> ", bytes[i]);
        else
            printf("<.> ");
    }
    
    printf("\n");


    /* 3. unsigned char: 부호 없이 0-255로 해석 */
    for (size_t i = 0; i < len; i++)
        printf("<%u> ", bytes[i]);
    printf("\n");
   

    /* 4. signed int: 4바이트씩 리틀 엔디언으로 조립한 뒤 부호 있는 수로 해석 */
    for (size_t i = 0; i+4 <= len; i+= 4) {
        uint32_t u = (uint32_t)bytes[i]
                | (uint32_t)bytes[i+1] << 8
                | (uint32_t)bytes[i+2] << 16
                | (uint32_t)bytes[i+3] << 24;
        printf("<%d> ", (int32_t)u);        // signed로 캐스트 후 %d
    }
    printf("\n");

    /* 5. unsigned int: 같은 조립, 부호 없이 해석 */
    for (size_t i = 0; i+4 <= len; i+=4) {
        uint32_t u = (uint32_t)bytes[i]
                | (uint32_t)bytes[i+1] << 8
                | (uint32_t)bytes[i+2] << 16
                | (uint32_t)bytes[i+3] << 24;
        printf("<%u> ", u);                 // 그대로 %u
    }

    printf("\n");


    /* 6. float: 4바이트를 리틀 엔디언으로 조립한 뒤, 그 비트를 그대로 float로 해석 */
    for (size_t i = 0; i+4 <= len; i += 4) {
        uint32_t u = (uint32_t)bytes[i]
                | (uint32_t)bytes[i+1] << 8
                | (uint32_t)bytes[i+2] << 16
                | (uint32_t)bytes[i+3] << 24;
        float f;
        memcpy(&f, &u, sizeof f);           // 비트 패턴을 그대로 복사
        printf("<%.4f> ", f);               // 소수점 아래 4자리
    }
    printf("\n");

    /* 7. double: 8바이트를 리틀 엔디언으로 조립한 뒤 double로 해석 */
    for (size_t i=0; i+8 <= len; i+=8) {
        uint64_t u = (uint64_t)bytes[i]
                | (uint64_t)bytes[i + 1] << 8
                | (uint64_t)bytes[i + 2] << 16
                | (uint64_t)bytes[i + 3] << 24
                | (uint64_t)bytes[i + 4] << 32
                | (uint64_t)bytes[i + 5] << 40
                | (uint64_t)bytes[i + 6] << 48
                | (uint64_t)bytes[i + 7] << 56;
        double d;
        memcpy(&d, &u, sizeof d);
        printf("<%.4f> ", d);
    }
    printf("\n");

    free(bytes);
    return 0;
}



