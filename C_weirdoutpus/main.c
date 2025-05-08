#include <stdio.h>

int main() {
    int i = -1;
    unsigned int j = 1;
    
    if (i > j)
        printf("i > j\n");
    else
        printf("i <= j\n");
    
    int a = 0;
    int b = a++ + a++ + a++;
    printf("b = %d\n", b);
    printf("a = %d\n", a);
    
    char c = 255;
    if (c == -1)
        printf("c is -1\n");
    else
        printf("c is not -1\n");
    
    return 0;
}
