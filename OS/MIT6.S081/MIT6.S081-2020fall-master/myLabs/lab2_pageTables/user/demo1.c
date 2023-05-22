void swap(long long int v[], int k)
{
    long long int temp;
    temp = v[k];
    v[k] = v[k+1];
    v[k+1] = temp;
}

void sort(long long int v[], int n)
{
    int i, j;
    for (i = 0; i < n; i +=1) {
        for (j = i - 1; j >= 0 && v[j] > v[j+1]; j -= 1) {
            swap(v, j);
        } 
    }
}



void sort_asm(long long int v[], int n)
{
    int s7 = n-1;
    int s4 = -1;
    long long int* s3 = v;
    long long int* s6 = v;
    long long int* s2;
    int s5_i, s1_j;
    for (s5_i = 0; s5_i != s7; s5_i++, s6++) {
        s1_j = s5_i;
        s2 = s6;
        if (s1_j >= 0) {
            do {
                if (*(s2 + 1) < *s2) {
                    swap(s3, s1_j);
                    s2 -= 1;
                    s1_j--;
                }
            } while (s1_j != -1)
        }
    }


}

int
main(int argc, char *argv[])
{
    return 0;
}