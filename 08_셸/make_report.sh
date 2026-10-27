#!/bin/bash
# 강의 슬라이드 6의 셸 프로그램 (변수, 조건문, 내장 명령 cd)

# 변수 설정 (= 앞뒤에 공백 없음)
DIR_NAME="temp_dir"
FILE_NAME="report.txt"

# 디렉토리 생성 (존재하지 않을 경우)
if [ ! -d "$DIR_NAME" ]; then
    mkdir "$DIR_NAME"
    echo "'$DIR_NAME' 디렉토리를 생성했습니다."
else
    echo "'$DIR_NAME' 디렉토리는 이미 존재합니다."
fi

# 디렉토리 이동 (실패하면 종료)
cd "$DIR_NAME" || {
    echo "디렉토리 이동에 실패했습니다."
    exit 1
}

# 파일 생성 (존재하지 않을 경우)
if [ ! -f "$FILE_NAME" ]; then
    touch "$FILE_NAME"
    echo "'$FILE_NAME' 파일을 생성했습니다."
else
    echo "'$FILE_NAME' 파일은 이미 존재합니다."
fi

# 현재 디렉토리 내용 출력
echo "현재 디렉토리 내용:"
ls

# 상위 디렉토리로 이동
cd ..
