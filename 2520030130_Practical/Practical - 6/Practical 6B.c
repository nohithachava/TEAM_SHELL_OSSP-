#include<stdio.h>
#include<stdlib.h>
int main()
{
int *p;
p=(int *)malloc(sizeof(int));   //4byted of dynamic memory
*p= 25;
printf("Value of p is %d\n" ,*p); //if only p it does not print it is in the address
printf("Memory Address of p is %p\n",p);
}