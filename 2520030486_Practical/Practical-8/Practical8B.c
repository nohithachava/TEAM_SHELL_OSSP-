#include<stdio.h>
#include<stdlib.h>
int main()
{
int *p;  // //4byted of dynamic memory pointer variable to store the dynamic address
p=(int *)malloc(sizeof(int));   //creating the dynamic memory addreess of size of int
*p= 25; //storing the vaue in the dynamic memory created
printf("Value of p is %d\n" ,*p); //if only p it does not print it is in the address printing the value
printf("Memory Address of p is %p\n",p);  //printing the memory address
free(p);   //if directly free(p) it doesnot change anything from previous malloc
p = NULL;  //it should allocate something so that output changes 0 or NULL to remove the address of p
printf("Memory Address of p is %p\n",p);//after removing the address
}