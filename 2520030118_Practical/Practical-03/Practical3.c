#include <unistd.h>
#include <stdio.h>

int main()
{
int pid;
pid = fork();
if (pid == 0)
{
printf("Child Process\n");
printf("Child PID: %d\n", getpid());
printf("Parent PID: %d\n", getppid());
printf("Child process is running\n");
}
else if (pid > 0)
{
printf("Parent Process\n");
printf("Parent PID: %d\n", getpid());
printf("Parent process is running\n");
}
else
{
printf("Fork did not execute successfully\n");
}
return 0;
}