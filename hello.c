#include<stdio.h>

int add(int a, int b);
int subtract(int a, int b);

int main() {
    printf("Hello Yeonoo!\n");
    printf("1+2=%d 2-3=%d\n", add(1,2), subtract(2, 3));
}

int add(int a, int b) {
    return a+b;
}
int subtract(int a, int b) {
    return a-b;
}
