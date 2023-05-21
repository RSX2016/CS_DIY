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

int
main(int argc, char *argv[])
{
    return 0;
}