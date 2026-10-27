# System Programming

시스템 프로그래밍 수업의 **실습 코드**와 **복습 노트**를 강의별로 모아 둔 저장소입니다.

## 실습 환경

| 구분 | 02~04강 실습 | 05강~ 실습 | 교재 |
|---|---|---|---|
| OS | macOS | **Linux (Docker, macOS에서 실행)** | Linux |
| 컴파일러 | clang (`gcc` 명령으로 호출) | gcc 13 (`gcc:13` 이미지) | gcc |
| 실행 파일 형식 | Mach-O | ELF | ELF |
| 라이브러리 확인 | `otool -L` | `ldd` | `ldd` |

> 링킹(`readelf`, `--wrap`, `LD_PRELOAD`)부터는 macOS와 차이가 커서 05강부터 Linux 컨테이너에서 실습했습니다.
> 기본은 `docker run --platform linux/amd64 gcc:13`이고, `strace`·페이지 폴트처럼 커널 동작을 보는 실험은 에뮬레이션 영향을 피하려고 네이티브 `linux/arm64`에서 돌렸습니다. 각 노트 머리말에 표시했습니다.

---

## 강의 목록

| 강 | 주제 | 정리 노트 | 실습 |
|:---:|---|---|---|
| 02 | C와 POSIX | [📘 정리 노트](02_C와_POSIX/02_C와_POSIX_정리.md) | [컴파일 4단계](02_C와_POSIX/01_컴파일_4단계) · [최적화 -O2](02_C와_POSIX/02_최적화_O2) |
| 03 | 메모리 표현 | [📘 정리 노트](03_메모리_표현/03_메모리_표현_정리.md) | 노트 12장 `mem.c` |
| 04 | 프로세스 | [📘 정리 노트](04_프로세스/04_프로세스_정리.md) | [`elf.c` · `fork.c` · `wait.c` · `현재_작업_디렉터리.c`](04_프로세스) |
| 05 | 링킹 · 라이브러리 인터포지셔닝 | [📘 정리 노트](05_링킹/05_링킹_정리.md) | [정적 링킹](05_링킹/01_정적_링킹) · [강한/약한 심볼](05_링킹/02_강한_약한_심볼) · [정적 라이브러리](05_링킹/03_정적_라이브러리) · [공유 라이브러리](05_링킹/04_공유_라이브러리) · [인터포지셔닝](05_링킹/05_인터포지셔닝) |
| 06 | 입출력 (+ ELF는 과제 2) | [📘 정리 노트](06_입출력/06_입출력_정리.md) | [Unix I/O](06_입출력/01_unix_io) · [표준 I/O](06_입출력/02_표준_io) · [버퍼링](06_입출력/03_버퍼링) |
| 07 | 파이프와 리디렉션 | [📘 정리 노트](07_파이프와_리디렉션/07_파이프와_리디렉션_정리.md) | [파이프](07_파이프와_리디렉션/01_파이프) · [fd 공유](07_파이프와_리디렉션/02_fd_공유) · [리디렉션·파이프라인](07_파이프와_리디렉션/03_리디렉션) |
| 08 | UNIX 셸 | [📘 정리 노트](08_셸/08_셸_정리.md) | [셸 스크립트 · `env_demo.c` · `child_cd.c`](08_셸) |
| 09 | 가상메모리 · 메모리 매핑 | [📘 정리 노트](09_가상메모리/09_가상메모리_정리.md) | [주소 공간](09_가상메모리/01_주소공간) · [페이지 폴트·COW](09_가상메모리/02_페이지_폴트) · [mmap](09_가상메모리/03_mmap) |

## 과제 목록

| 과제 | 주제 | 폴더 |
|:---:|---|---|
| 1 | 비트 문자열을 7가지 C 타입으로 해석 | [`Assignment_1/`](Assignment_1) |
| 2 | ELF 파일의 `.rodata` 문자열 수정 (`software` → `hackers!`) | [`Assignment_2/`](Assignment_2) |

---

## 02. C와 POSIX

### 실습 1 — 컴파일 4단계 · [`01_컴파일_4단계/`](02_C와_POSIX/01_컴파일_4단계)

`hello.c` 하나를 단계별로 나누어 컴파일하면서 중간 결과물을 확인했습니다.

```
hello.c ─(-E)→ hello.i ─(-S)→ hello.s ─(-c)→ hello.o ─(링크)→ hello
 소스        전처리 결과       어셈블리        목적 파일          실행 파일
```

```bash
gcc -E hello.c -o hello.i   # ① 전처리
gcc -S hello.c              # ② 컴파일   → hello.s
gcc -c hello.c              # ③ 어셈블   → hello.o
gcc -o hello hello.o        # ④ 링크     → hello
./hello
```

- `hello.s` : 최적화 없이(`-O0`) 만든 어셈블리. `printf`를 그대로 호출합니다.

### 실습 2 — 최적화와 동적 링킹 · [`02_최적화_O2/`](02_C와_POSIX/02_최적화_O2)

`-O2`로 컴파일했을 때 어셈블리가 어떻게 바뀌는지, 실행 파일이 어떤 라이브러리에 의존하는지 확인했습니다.

```bash
gcc -Wall -Werror -O2 -std=c99 -S helloworld.c   # 최적화된 어셈블리
gcc -O2 -o helloworld helloworld.c && ./helloworld
otool -L helloworld                              # Linux: ldd helloworld
```

- `helloworld.s` : `-O2` 결과. 컴파일러가 `printf("...\n")`를 **`puts("...")`로 바꿨습니다.**
- `otool -L` 결과 `libSystem.B.dylib`에 동적 링킹된 것을 확인했습니다.

### 핵심 정리

- 컴파일은 **전처리 → 컴파일 → 어셈블 → 링크**의 4단계로 진행된다.
- 컴파일러는 코드를 글자 그대로 옮기지 않고, 결과가 같은 **더 효율적인 코드**로 바꾼다.
- `printf`의 실제 코드는 `.o` 안에 없고, 실행할 때 **공유 라이브러리**에서 불러온다.

---

## 03. 메모리 표현

메모리에 값이 실제로 어떤 바이트로 저장되는지 `dump_mem` 함수로 직접 찍어 보며 확인했습니다.
실습 코드(`mem.c`)는 [정리 노트 12장](03_메모리_표현/03_메모리_표현_정리.md#12-실습-코드-모음)에 있습니다.

```bash
gcc -Wall -std=c99 -o mem mem.c && ./mem
```

| 실험 | 결과 | 배운 것 |
|---|---|---|
| `int 98303` 메모리 덤프 | `ff 7f 01 00` | x86은 **리틀 엔디언** |
| `char 0x80` → `int` | `-128` (`0xffffff80`) | 작은 signed → 큰 타입은 **부호 확장** |
| `sizeof(IntList / A / B / Example)` | `16 / 12 / 8 / 12` | **정렬 때문에 패딩**이 생기고, 멤버 순서로 크기가 달라짐 |
| `(double *)0 + 1` | `0x8` | 포인터 연산은 **타입 크기만큼** 이동 |
| `name[8]` (uid = `0x41`) | `65` | 배열 범위 밖 접근은 **옆 멤버를 읽음** |
| `2.0f` / `0.2f` | `0x40000000` / `0x3e4ccccd` | 실수는 **IEEE 754**, `0.2f`는 근사값 |

### 핵심 정리

- 메모리는 타입 없는 **비트/워드**이고, 타입·구조체·부호는 그 비트를 해석하는 **약속**이다.
- 정수는 **2의 보수**, 실수는 **IEEE 754 (부호 | 지수 | 가수)** 로 저장된다.
- `malloc`으로 받은 메모리는 `free` 후 포인터를 `NULL`로 만들어 **댕글링 포인터**를 막는다.

---

## 04. 프로세스

실행 파일의 섹션 구조, `fork()`로 프로세스를 만들고 `waitpid()`로 기다리는 과정, 현재 작업 디렉터리를 실습했습니다.

```bash
gcc -Wall -o fork fork.c && ./fork
gcc -Wall -o wait wait.c && ./wait
gcc -c elf.c -o elf.o && nm -m elf.o   # Linux: readelf -S elf.o
```

| 실습 | 결과 | 배운 것 |
|---|---|---|
| `elf.c` + `nm -m` | `g1`, `s` → `__data` / `main` → `__text` / `g2` → `common` | 변수 종류에 따라 **섹션**이 다르고, 스택·힙은 섹션이 아님 |
| `fork.c` | `In parent, child PID = 27906` → `In child` | `fork()`는 **두 번 반환** (자식 `0`, 부모는 자식 PID), 순서는 **비결정적** |
| `wait.c` | `In child` → `Child exited with status 42` | `waitpid()`로 순서가 **결정적**이 되고, `WEXITSTATUS`로 종료 코드를 꺼냄 |
| `현재_작업_디렉터리.c` | — | 상대 경로는 **cwd** 기준. `getwd()` 대신 크기를 받는 `getcwd()` 사용 |

### 핵심 정리

- 프로세스는 **실행 중인 프로그램**이다. 섹션(`.text` / `.rodata` / `.data` / `.bss`)에 **스택·힙**을 더한 메모리를 갖는다.
- `fork()`는 자신을 복제하고, 부모는 `waitpid()`로 자식을 **거둬야** 좀비가 남지 않는다.
- `status`는 비트로 묶인 값이라 `WIFEXITED` / `WEXITSTATUS` 매크로로 꺼낸다.

---

## 05. 링킹 · 라이브러리 인터포지셔닝

여러 `.o`가 하나의 실행 파일이 되는 과정(심볼 해석, 재배치)과 정적·공유 라이브러리, 함수 호출을 가로채는 세 가지 방법을 실습했습니다.

```bash
gcc -Og -c main.c sum.c && gcc -o prog main.o sum.o
objdump -r -d main.o                              # 재배치 엔트리 확인
ar rs libvector.a addvec.o multvec.o              # 정적 라이브러리
gcc -shared -fpic -o libvector.so addvec.c multvec.c   # 공유 라이브러리
LD_PRELOAD=./mymalloc.so ./intr                   # 런타임 인터포지셔닝
```

| 실습 | 결과 | 배운 것 |
|---|---|---|
| `objdump -r -d main.o` | `bf 00 00 00 00` + `R_X86_64_32 array` | 링크 전에는 주소가 0이고 **재배치 엔트리**가 따로 있음 |
| 링크 후 `prog` | `mov $0x404010,%edi`, `call` 오프셋 `0x05` | 절대 주소와 **PC 상대 주소**(다음 명령어 기준)가 채워짐 |
| `int x=7; int y=5;` + `double x;` | gcc 13: `multiple definition` / `-fcommon`: `y = -2147483648` | **강한/약한 심볼** 규칙. 최신 gcc는 `-fno-common`이 기본 |
| `gcc ./libvector.a main2.o` | `undefined reference to 'addvec'` | 링커는 왼쪽부터 스캔 → **라이브러리는 맨 뒤** |
| `-static` vs 동적 | 951,680 B vs 15,904 B | 정적 링크는 libc까지 복사, 동적은 `ld-linux.so`가 실행 시 연결 |
| 컴파일·링크·런타임 인터포지셔닝 | 세 방법 모두 `malloc(32) = 0x4052a0` | `#define` / `--wrap` / `LD_PRELOAD` + `dlsym(RTLD_NEXT)` |

### 핵심 정리

- 링커는 **심볼 해석**(참조 하나에 정의 하나)과 **재배치**(섹션 병합 + 주소 고치기)를 한다.
- 강한 심볼(함수, 초기화된 전역)이 약한 심볼(초기화 안 된 전역)을 이기고, 강한 것 둘은 에러다.
- 정적 라이브러리는 **필요한 `.o`만** 복사하고, 공유 라이브러리는 로드 타임이나 `dlopen`으로 런타임에 연결된다.

---

## 06. 입출력

시스템 콜(Unix I/O)과 C 표준 I/O를 비교하고, 버퍼링이 시스템 콜 횟수를 얼마나 줄이는지 `strace`로 측정했습니다. 같은 주차의 ELF는 [과제 2 (`Assignment_2/`)](Assignment_2)에서 다룹니다.

```bash
gcc -O2 -o readbyte_unix readbyte_unix.c && strace -c -e trace=read ./readbyte_unix
gcc -O2 -o readbyte_stdio readbyte_stdio.c && strace -c -e trace=read ./readbyte_stdio
./buffer_mode | cat        # 파이프로 보내면 출력 순서가 바뀜
```

| 실습 | 결과 | 배운 것 |
|---|---|---|
| `mycat.c` | `open: No such file or directory` | 시스템 콜은 실패하면 음수 + `errno`, `perror`로 출력 |
| `open_flags.c` | 2번째 `O_CREAT\|O_EXCL` → `EEXIST`, `0666` → `644`, `O_TRUNC` → 0바이트 | 원자적 생성(락), umask, `"w"` = 자르기 |
| 1MB를 1바이트씩 읽기 | `read` **1,048,578번** 0.148s vs `fread` **258번** 0.011s | 표준 I/O는 4KB씩 읽어 두는 **버퍼링** |
| `hello.c` + `strace` | `printf` 6번 → `write(1, "hello\n", 6)` 1번 | 쓰기 버퍼는 `\n`(터미널), `fflush`, `exit`에서 비워짐 |
| `buffer_mode.c` | 터미널: 1·2·3 / 파이프: 2·1·3 | stdout은 터미널이면 줄 단위, 파이프·파일이면 블록 단위 버퍼링 |

### 핵심 정리

- 모든 것은 파일이고, 프로세스는 **fd**(0/1/2 = stdin/stdout/stderr)로 접근한다.
- `read`는 **0 = EOF, 음수 = 에러**, 요청보다 적게 읽을 수 있다. `fread`는 둘 다 0이라 `feof`/`ferror`로 구분한다.
- 시스템 콜은 비싸므로 표준 I/O가 버퍼링으로 횟수를 줄인다.

---

## 07. 파이프와 리디렉션

`pipe` / `fork` / `dup2` / `exec`로 셸의 `|`와 `>`를 직접 만들고, 커널의 3단 파일 테이블을 실험으로 확인했습니다.

```bash
gcc -o pipe_capacity pipe_capacity.c && ./pipe_capacity
gcc -o fork_share fork_share.c && ./fork_share
gcc -o pipeline pipeline.c && ./pipeline            # = ls -1 /usr | wc -l
```

| 실습 | 결과 | 배운 것 |
|---|---|---|
| `pipe_simple.c` | `pipefd[0] = 3 (read), pipefd[1] = 4 (write)` | `[0]` 읽기 / `[1]` 쓰기 (stdin/stdout 순서) |
| `pipe_eof.c` | 부모가 쓰기 끝을 안 닫으면 **영원히 멈춤** | 쓰기 끝 fd가 **모두** 닫혀야 EOF |
| `pipe_capacity.c` | `EAGAIN`(11), 용량 **65,536 B** | 꽉 차면 write가 block. `O_NONBLOCK`으로 교착 방지 |
| `open_twice` / `fork_share` / `dup_share` | `a,a` / `a,b` / `a,b` | `open` 두 번은 위치 따로, `fork`·`dup`은 **열린 파일 항목(위치) 공유** |
| `redirect.c` | 화면 출력 없음, `output.txt`에 기록 | `dup2(fd, 1)` = 셸의 `>` |
| `pipeline.c` | `10` (셸 결과와 같음) | `pipe` + `fork` 두 번 + `dup2` + `exec` = 셸의 `\|` |

### 핵심 정리

- 파이프는 fd로 접근하는 **커널 버퍼**다. 비면 read가, 차면 write가 block된다.
- fd → 열린 파일 테이블(파일 위치, refcnt) → v-node 테이블. 앞의 것만 프로세스별이다.
- **안 쓰는 파이프 끝은 반드시 닫는다.**

---

## 08. UNIX 셸

셸이 내부적으로 쓰는 시스템 콜을 기준으로 내장 명령, 변수, IFS, 환경 변수, 글로빙, 리디렉션을 스크립트로 확인했습니다.

```bash
./make_report.sh                   # 강의 슬라이드 6의 스크립트
VAR="arg1 arg2"; ./writeargs.sh $VAR
./interp.sh arg1 arg2              # $argnew vs ${arg}new
./redirection.sh                   # > 2>&1 순서
```

| 실습 | 결과 | 배운 것 |
|---|---|---|
| `child_cd.c` | 자식 `/tmp`, 부모는 그대로 | `cd`는 셸 상태를 바꾸므로 **내장 명령**이어야 함 |
| `quote_ifs.sh` | `"$VAR"` 1개 / `$VAR` 2개 / `'$VAR'` 1개 / `IFS=:` 3개 | 치환 결과는 **IFS로 쪼개짐**, 큰따옴표가 막음 |
| `interp.sh` | `file_.txt` vs `file_arg1new.txt` | 이름 경계는 `${VAR}` |
| `env_demo.c` | export 전 `(없음)` → 후 `shell_only` | **export한 변수만** 자식의 환경으로 복사 |
| 글로빙 | `*.?` → `main.c stdio.c stdio.h` | `*` 0개 이상, `?` 정확히 1개, `[a-d]` 범위 |
| `redirection.sh` | `> f 2>&1`은 파일, `2>&1 > f`는 터미널 | 리디렉션은 **왼쪽부터** `dup2` |

### 핵심 정리

- 외부 명령은 `fork` + `exec` + `wait`, 셸 상태를 바꾸는 `cd`·`export`는 내장 명령이다.
- `=` 주변에 공백 없이 대입하고, 변수는 `"$VAR"`로 감싼다.
- `<` `>` `>>` `N>&M`은 `open` + `dup2`, `|`는 `pipe` + `dup2`다.

---

## 09. 가상메모리 · 메모리 매핑

주소를 VPN/VPO로 나눠 보고, 게으른 할당, 메모리 보호, Copy-on-Write, `mmap`을 RSS와 페이지 폴트 수로 확인했습니다.

```bash
gcc -o address address.c && ./address        # VPN / VPO
gcc -o lazy_alloc lazy_alloc.c && ./lazy_alloc
gcc -o cow cow.c && ./cow
gcc -o mmap_shared mmap_shared.c && ./mmap_shared
```

| 실습 | 결과 | 배운 것 |
|---|---|---|
| `address.c` | `.data` VA `0x404030` → VPN `0x404`, VPO `0x030` | 4KB 페이지면 아래 12비트가 페이지 안 오프셋 |
| `maps.c` | `r-xp` 코드, `rw-p` 데이터, `[heap]`, `libc.so`, `[stack]` | 프로세스 주소 공간 = 영역(`vm_area_struct`)의 목록 |
| `lazy_alloc.c` | `malloc(256MB)` 후 RSS 1.2MB → 접근 후 263MB | **게으른 할당**, 접근할 때 페이지 폴트로 할당 (**요구 페이징**) |
| `protect.c` | `child killed by signal 11` | `.rodata` 페이지는 PTE에 쓰기 권한이 없음 → 보호 폴트 |
| `cow.c` | 같은 `&value`, 부모 100 / 자식 200, 64MB 쓰기 → 16,384 폴트 | `fork`는 페이지를 공유하다 **쓸 때 복사 (COW)** |
| `mmap_shared.c` | `MAP_PRIVATE` → 파일 그대로, `MAP_SHARED` → `hackers!` | 과제 2를 포인터로 직접 고치기. PRIVATE는 COW |

### 핵심 정리

- MMU가 페이지 테이블(프로세스별)로 **VPN → PPN**을 바꾸고, VPO = PPO다.
- 물리 메모리는 디스크의 캐시다. 없는 페이지는 **페이지 폴트** 후 명령어를 재실행한다.
- 가상 메모리는 캐시, 메모리 관리(공유·링킹·로딩), 메모리 보호의 도구다.

---

## 과제 1 — 비트 문자열을 7가지 타입으로 해석 · [`Assignment_1/`](Assignment_1)

`'0'`/`'1'` 문자로 된 파일을 읽어 8비트씩 묶어 바이트 배열로 만든 뒤, **같은 메모리**를 7가지 C 타입으로 해석해 한 줄씩 출력합니다.

```bash
cd Assignment_1
make                  # → 실행 파일 20221787
./20221787            # 인자가 없으면 ./input 을 읽음
./20221787 <파일>     # 다른 입력 파일 지정
make clean
```

| 줄 | 해석 | 방법 |
|:---:|---|---|
| 1 | `signed char` | 1바이트씩 `(signed char)` 캐스트 |
| 2 | ASCII | 출력 가능한 문자(32~126)만, 나머지는 `.` |
| 3 | `unsigned char` | 1바이트씩 0~255 |
| 4 | `signed int` | 4바이트를 **리틀 엔디언**으로 조립(`le32`) → `int32_t` |
| 5 | `unsigned int` | `le32` 결과를 그대로 |
| 6 | `float` | `le32` 비트를 `memcpy`로 `float`에 복사, `%.4f` |
| 7 | `double` | 8바이트 조립(`le64`) → `double`, `%.4f` |

실행 결과 (`Assignment_1/input`):

```
<98> <-95> <-42> <-36> <41> <-116> <103> <65> <-99> <17> <101> <-60> <-2> <-27> <-108> <-63>
<b> <.> <.> <.> <)> <.> <g> <A> <.> <.> <e> <.> <.> <.> <.> <.>
<98> <161> <214> <220> <41> <140> <103> <65> <157> <17> <101> <196> <254> <229> <148> <193>
<-589913758> <1097305129> <-1000009315> <-1047206402>
<3705053538> <1097305129> <3294957981> <3247760894>
<-483304698359578624.0000> <14.4717> <-916.2752> <-18.6123>
<12345678.9012> <-87654321.0987>
```

- 바이트를 **시프트 연산으로 직접 조립**해서, 실행하는 CPU의 엔디언과 상관없이 결과가 같습니다.
- `float`/`double`은 값 변환(캐스트)이 아니라 **비트 복사(`memcpy`)** 로 재해석합니다.
- 8비트가 안 되는 나머지 비트, 4·8바이트에 모자란 끝부분은 출력하지 않습니다.

---

## 과제 2 — ELF `.rodata` 문자열 수정 · [`Assignment_2/`](Assignment_2)

ELF 파일인지 검사한 뒤 `.rodata` 섹션에서 `"software"`를 찾아 같은 자리에 `"hackers!"`를 덮어씁니다. 수정한 파일을 다시 실행하면 바뀐 문자열이 출력됩니다.
`<elf.h>`가 필요해서 **Linux x86-64**에서 빌드합니다. (macOS에서는 Docker `--platform linux/amd64` 컨테이너로 테스트했습니다.)

```bash
cd Assignment_2
make                          # → 실행 파일 20221787
gcc -o test test.c && ./test  # Hello the school of software
./20221787 ./test             # ELF 정보 출력 + .rodata 수정
./test                        # Hello the school of hackers!
make clean
```

| 단계 | 함수 | 방법 |
|:---:|---|---|
| 1 | `read_file` | `open(O_RDWR)` → `lseek`로 크기 → `read`를 반복해 전부 읽음 |
| 2 | `is_elf` | 크기 ≥ 64, 매직 `7F 'E' 'L' 'F'`, `ELFCLASS64` 확인 |
| 3 | `find_section` | `e_shoff`로 섹션 헤더 테이블 → `.shstrtab + sh_name`으로 이름 비교 |
| 4 | `patch_rodata` | `.rodata` 범위 안에서 `memcmp`로 검색 → `pwrite(sh_offset + i)`로 8바이트만 기록 |

실행 결과 (`test`):

```
ELF Magic     : 7F 45 4C 46
Class         : ELF64
Data          : 2's complement, little endian
Machine       : AMD X86-64
Entry point   : 0x1060
Section count : 31

.rodata section info
   File Offset = 0x00002000
          Size = 0x00000021

   "software" -> "hackers!" at file offset 0x00002018
1 string(s) modified
```

- 두 문자열 길이가 같아서(8바이트) 다른 섹션 오프셋이나 코드의 주소 참조가 바뀌지 않습니다. `\0`까지 9바이트를 쓰면 뒤에 이어지는 문자열이 잘리므로 8바이트만 씁니다.
- `.rodata`에는 `\0`이 섞여 있어(맨 앞 `01 00 02 00`) `strstr` 대신 `memcmp`로 섹션 크기만큼 검색합니다.
- 헤더가 가리키는 모든 범위가 파일 안에 있는지 검사해서, 잘리거나 망가진 파일에서도 버퍼 밖을 읽지 않습니다.
- 제공된 `editro`는 Size를 `0x%08ld`(10진수)로 출력해 `0x00000033`이 나오지만 실제 크기는 33바이트 = `0x21`입니다. `%08lX`로 고쳤습니다.
- 개념 정리: [`ELF_스터디노트.html`](Assignment_2/ELF_스터디노트.html) — ELF 헤더 바이트 해석, 섹션 이름 찾기, 섹션 vs 세그먼트, 5주차 링킹 · 6주차 Unix I/O 요약, 확인 문제 25개 (브라우저로 열기)

---

## 저장소 구조

```
System_Programming/
├── README.md
├── .gitignore
├── 02_C와_POSIX/
│   ├── 02_C와_POSIX_정리.md       # 개념 & 실습 정리 노트
│   ├── 01_컴파일_4단계/
│   │   ├── hello.c
│   │   └── hello.s                # -O0 어셈블리
│   └── 02_최적화_O2/
│       ├── helloworld.c
│       └── helloworld.s           # -O2 어셈블리
├── 03_메모리_표현/
│   └── 03_메모리_표현_정리.md     # 개념 & 실습 정리 노트 (mem.c 포함)
├── 04_프로세스/
│   ├── 04_프로세스_정리.md        # 개념 & 실습 정리 노트
│   ├── elf.c                      # 섹션별 변수 배치
│   ├── fork.c                     # 프로세스 생성
│   ├── wait.c                     # waitpid로 자식 기다리기
│   └── 현재_작업_디렉터리.c       # getcwd / chdir
├── 05_링킹/
│   ├── 05_링킹_정리.md            # 링킹 + 라이브러리 인터포지셔닝
│   ├── 01_정적_링킹/              # main.c, sum.c (심볼 해석, 재배치)
│   ├── 02_강한_약한_심볼/         # p1.c, p2.c, static_local.c
│   ├── 03_정적_라이브러리/        # addvec.c, multvec.c, vector.h, main2.c
│   ├── 04_공유_라이브러리/        # dll.c (dlopen/dlsym)
│   └── 05_인터포지셔닝/           # int.c, malloc.h, mymalloc.c
├── 06_입출력/
│   ├── 06_입출력_정리.md
│   ├── 01_unix_io/                # mycat.c, open_flags.c
│   ├── 02_표준_io/                # stdio_cat.c
│   └── 03_버퍼링/                 # readbyte_unix.c, readbyte_stdio.c, hello.c, buffer_mode.c
├── 07_파이프와_리디렉션/
│   ├── 07_파이프와_리디렉션_정리.md
│   ├── 01_파이프/                 # pipe_simple, pipe_fork, pipe_eof, pipe_capacity
│   ├── 02_fd_공유/                # open_twice, fork_share, dup_share
│   └── 03_리디렉션/               # redirect.c, pipeline.c
├── 08_셸/
│   ├── 08_셸_정리.md
│   ├── *.sh                       # make_report, writeargs, interp, special_vars, quote_ifs, redirection
│   ├── env_demo.c                 # export와 환경 변수
│   └── child_cd.c                 # cd가 내장 명령인 이유
├── 09_가상메모리/
│   ├── 09_가상메모리_정리.md      # 가상메모리 + 메모리 매핑·COW
│   ├── 01_주소공간/               # address.c (VPN/VPO), maps.c
│   ├── 02_페이지_폴트/            # lazy_alloc.c, cow.c, protect.c
│   └── 03_mmap/                   # mmapcopy.c, mmap_shared.c
├── Assignment_1/                  # 과제 1
│   ├── 20221787.c                 # read_bits, le32/le64, main
│   ├── 20221787.h
│   ├── Makefile
│   └── input                      # 예제 입력 (비트 문자열)
└── Assignment_2/                  # 과제 2
    ├── 20221787.c                 # is_elf, find_section, patch_rodata, main
    ├── 20221787.h
    ├── Makefile
    ├── test.c                     # 수정 대상 예제 프로그램
    └── ELF_스터디노트.html        # 개념 & 시험 대비 정리
```

- 폴더 규칙: `NN_강의주제/` 아래에 정리 노트와 `NN_실습명/` 폴더를 두고, 과제는 `Assignment_N/`에 둡니다.
- 빌드 결과물(`.o`, `.i`, 실행 파일, `.dSYM`)은 소스로 다시 만들 수 있어서 `.gitignore`로 제외했습니다.
