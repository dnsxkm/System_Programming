int sum(int *a, int n)          // 전역 심볼: main.c에서 참조
{
    int i, s = 0;               // 지역 변수 → 링커 심볼 아님
    for (i = 0; i < n; i++) {
        s += a[i];
    }
    return s;
}
