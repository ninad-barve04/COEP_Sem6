int add(int a, int b) {
    int c = a + b;
    return c;
}


int main(int argc, char const *argv[])
{
    int a = 10;
    int b = 20;
    int c = add(a, b);
    int d = add(100, 300);
    // printf("%d\t%d\n", c, d);
    return c+d;
}
