#include<stdio.h>
#include<stdlib.h>
int main() {
float *p;
p=(float *)malloc(sizeof(float));
*p=13.2;
printf("Value of p is %f\n" ,*p);
printf("Memory Address of p is %p\n" ,p);
free(p);
p=0;
printf("Memory Address of p is %p\n" ,p);
}