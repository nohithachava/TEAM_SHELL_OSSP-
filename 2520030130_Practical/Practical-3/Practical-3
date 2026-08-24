#include<stdio.h>
#include<unistd.h>
int main()
{
int pid;
pid=fork();
if(pid==0)
{
printf("Child process\n");
printf("PID: %d\n",getpid());
printf("PPID: %d\n",getppid());
printf("Child process completed\n");
}
if(pid>0)
{
printf("parent process\n");
printf("PID: %d\n",getpid());
printf("child PID: %d\n", pid);
printf("Parent process completed\n");
}
}
