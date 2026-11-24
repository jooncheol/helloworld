#include<stdio.h>

int add(int a, int b);

int main() {
    printf("Hello Yeonoo!\n");
    printf("1+2=%d\n", add(1,2));
}

int add(int a, int b) {
    return a+b;
}
