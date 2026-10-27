#!/bin/bash
# 리디렉션 연산자 = open + dup2
echo "first"  > out.txt          # > : O_TRUNC
echo "second" >> out.txt         # >>: O_APPEND
wc -l < out.txt                  # < : 파일을 stdin(0)에 연결

ls no_such_file 2> err.txt       # 2>: stderr만 파일로
echo "err.txt: $(cat err.txt)"

echo "to stderr" 1>&2            # N>&M = dup2(M, N): stdout을 stderr로

# 순서가 중요: 왼쪽부터 처리된다
ls no_such_file > both.txt 2>&1  # stdout→파일, 그다음 stderr→(지금의 stdout=파일)
echo "both.txt: $(cat both.txt)"
ls no_such_file 2>&1 > only.txt  # stderr→(지금의 stdout=화면), 그다음 stdout→파일
echo "only.txt: '$(cat only.txt)' (비어 있음)"

rm -f out.txt err.txt both.txt only.txt
