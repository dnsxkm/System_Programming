int sum(int *a, int n);         // 외부 심볼: sum.c에 정의됨

int array[2] = {1, 2};          // 전역 심볼, 초기값 있음 → .data

int main(void)
{
    int val = sum(array, 2);    // val은 지역 변수 → 링커는 모름
    return val;
}
