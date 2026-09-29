#ifndef STUDENT_20221787_H
#define STUDENT_20221787_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>     // uint32_t, int32_t ( 크기가 정확히 4바이트인 정수 타입 )
#include <string.h>     // memcpy

unsigned char *read_bits(const char *filename, size_t *out_len);

#endif