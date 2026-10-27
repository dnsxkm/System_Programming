#!/bin/bash
# 인자를 한 줄에 하나씩 출력 ($*는 IFS로 다시 쪼개짐)
for arg in $*; do
    echo $arg
done
