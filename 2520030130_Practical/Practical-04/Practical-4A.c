#include<stdio.h>
#include<unistd.h>  //fork() system call
#include<sys/wait.h>  //for wait() and waitpid() systemcall
#include<stdlib.h>
int main()
{
int pid[3];      //array to create 3 childs
for(int i=0;i<3;i++) //for loop to navigate throughthe childs
{
pid[i]=fork();  //creating the childs
if(pid[i]==0)  //checking for the child process
{
printf("Child %d pid=%d\n",i+1,getpid());  //printing the child and its process id
sleep(i+1);  //making every child sleep for 1sec
exit(0);  //esisting the standard input
}
}
wait(NULL);//taking the status of one child
printf("Parent : Anyone child complete with wait()\n"); //one parent printing its status
waitpid(pid[2],NULL,0); //taking the status of the 3rd child
printf("Parent:child 3 completes with waitpid()\n"); //
wait(NULL);
printf("Parent : All the childs completed");
}