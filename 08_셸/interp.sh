#!/bin/bash
# $argnew 는 "argnew"라는 (없는) 변수, ${arg}new 는 arg 뒤에 new를 붙임
for arg in $*; do
    echo "wrong: file_$argnew.txt"
    echo "right: file_${arg}new.txt"
done
