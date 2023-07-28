
int sum_cut = 0;
int g_intC = 100;

int sum (int *a, int n)
{
    int i, s = 0;
    for (i = 0; i < n; i++) {
        s += a[i];
    }
    return s;
}