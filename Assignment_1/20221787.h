#ifndef STUDENT_20221787_H          // 1. 인클루드 가드: 두 번 포함돼도 한 번만 처리
#define STUDENT_20221787_H

#include <stdio.h>                  // 2. 표준 헤더를 한곳에 모음 -> .c는 이 헤더 하나만 include
#include <stdlib.h>
#include <stdint.h>     // uint32_t, int32_t . uint64_t (크기가 고정된 정수 타입 )
#include <string.h>     // memcpy

/* '0'/'1' 문자 파일을 읽어 바이트 배열로 반환 (바이트 개수는 out_len에 저장) */
unsigned char *read_bits(const char *filename, size_t *out_len);    // 3. 함수 선언 = 이런 이름으로 된 타입의 함수가 있다는 "약속"

/* p가 가리키는 4바이트 / 8바이트를 리틀 엔디언으로 조립 */
uint32_t le32(const unsigned char *p);
uint64_t le64(const unsigned char *p);

#endif

// 선언과 정의의 차이 : 헤더에는 선언(이름과 타입)만 있고, 실제 몸체인 정의는 .c에 있다. 
// 컴파일러는 함수를 호출하는 코드를 만나기 전에 그 선언을 알고 있어야한다. 4장 fork() 연습할때 unistd.h가 없어 에러가 났던 것과 같은 이유다.