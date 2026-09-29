#ifndef STUDENT_20221787_H
#define STUDENT_20221787_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>     // uint32_t, int32_t . uint64_t (크기가 고정된 정수 타입 )
#include <string.h>     // memcpy

/* '0'/'1' 문자 파일을 읽어 바이트 배열로 반환 (바이트 개수는 out_len에 저장) */
unsigned char *read_bits(const char *filename, size_t *out_len);

/* p가 가리키는 4바이트 / 8바이트를 리틀 엔디언으로 조립 */
uint32_t le32(const unsigned char *p);
uint64_t le64(const unsigned char *p);

#endif