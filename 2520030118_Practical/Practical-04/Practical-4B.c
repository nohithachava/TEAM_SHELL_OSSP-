#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
int pid; //variable to collect the status of fork
pid = fork(); //creating a process
if(pid==0)//checking whether it is child or not
{
printf("Child process %dn",getpid()); //printing the child process along with its pid
exit(0);//existing the standard input
}
else //parent process
{
printf("Parent process %d \n",getpid()); //printing the child process
//sleep(30);
wait(NULL);
printf("Parent process completed");
}
}