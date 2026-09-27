# System Programming

시스템 프로그래밍 수업 실습 & 복습 노트
실습 환경: macOS (clang) / 교재 환경: Linux (gcc)

## 목차

| 강 | 주제 | 정리 노트 | 실습 |
|---|---|---|---|
| 02 | C와 POSIX | [02_C와_POSIX_정리.md](02_C와_POSIX/02_C와_POSIX_정리.md) | [01_컴파일_4단계](02_C와_POSIX/01_컴파일_4단계) · [02_최적화_O2](02_C와_POSIX/02_최적화_O2) |

## 폴더 구조

```
02_C와_POSIX/
├── 02_C와_POSIX_정리.md     # 개념 & 실습 정리 노트
├── 01_컴파일_4단계/          # 전처리 → 컴파일 → 어셈블 → 링크
│   ├── hello.c
│   └── hello.s              # -O0 어셈블리 (printf 호출)
└── 02_최적화_O2/             # 최적화 옵션과 어셈블리 관찰
    ├── helloworld.c
    └── helloworld.s         # -O2 어셈블리 (printf → puts 치환)
```

## 빌드

```bash
# 01_컴파일_4단계
gcc -E hello.c -o hello.i && gcc -S hello.c && gcc -c hello.c && gcc -o hello hello.o && ./hello

# 02_최적화_O2
gcc -Wall -Werror -O2 -std=c99 -S helloworld.c
gcc -O2 -o helloworld helloworld.c && ./helloworld
otool -L helloworld   # Linux: ldd helloworld
```

> `.o`, `.i`, 실행 파일, `.dSYM`은 `.gitignore`로 제외 (소스에서 재생성 가능)
