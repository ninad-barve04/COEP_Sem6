int main(int argc, char const *argv[])
{
    int a[4][2];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            a[i][j] = i*j;
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    return 0;
}
