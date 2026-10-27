#!/bin/bash
# 특수 변수
echo "\$0 = $0"
echo "\$1 = $1, \$2 = $2"
echo "\$# = $#"
echo "\$@ = $@"

false
echo "\$? after false = $?"     # 직전 명령의 종료 코드
true
echo "\$? after true  = $?"

sleep 0 &
echo "\$! = $! (방금 백그라운드로 실행한 sleep의 PID)"
wait

# 명령 치환 $(command)
echo "오늘 날짜: $(date +%Y-%m-%d), 이 폴더의 .sh 파일 수: $(ls *.sh | wc -l)"
