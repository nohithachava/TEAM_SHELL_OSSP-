#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
pid_t pid;
pid = fork();
if (pid == 0)
{
printf("current pid: %d\n", getpid());
printf("parent pid: %d\n", getppid());
execl("/bin/ls", "ls", NULL);
}
else if (pid > 0)
{
printf("child pid: %d\n", pid);
printf("parent pid: %d\n", getpid());
wait(NULL);
}
else
{
printf("Fork failed\n");
}
return 0;
}