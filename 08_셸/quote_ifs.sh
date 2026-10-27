#!/bin/bash
# 따옴표와 IFS가 단어 분리에 주는 영향
VAR="arg1 arg2"

echo "--- ./writeargs.sh \$VAR (따옴표 없음)"
./writeargs.sh $VAR

echo "--- ./count \"\$VAR\" (큰따옴표) vs \$VAR"
count() { echo "인자 $#개"; }
count "$VAR"
count $VAR
count '$VAR'                    # 작은따옴표 안에서는 치환 안 됨

echo "--- IFS=: 로 바꾸면"
PATHS="/bin:/usr/bin:/usr/local/bin"
IFS=:
count $PATHS
