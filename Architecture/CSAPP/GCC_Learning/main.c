int sum(int *a, int n);

int arrayA[2] = {1, 2};
int arrayB[3] = {2, 3, 4};
int g_intA = 0;
extern int sum_cut;
int g_intB;
int *gp_int = &sum_cut;
extern int g_intC;

int main()
{
    g_intB = 100;
    int val = sum(arrayA, 2);
    int val2 = sum(arrayB, 3);
    return val + val2 + g_intA + g_intB + g_intC;
}