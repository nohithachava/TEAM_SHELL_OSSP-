#include <stdio.h>
#include <unistd.h>
#include <time.h>
int main()
{
int fd[2]; // declaring the pipe
pipe(fd);  // creating the pipe
int pid, data;
int start, end;
double timetaken, efficiency;
pid = fork(); // creating process
start = clock();
if(pid == 0) // child
{
printf("Consumer\n");
close(fd[1]); // close write end
for(int i = 0; i < 5; i++)
{
read(fd[0], &data, sizeof(data));
printf("Consumed %d\n", data);
}
close(fd[0]);
}
else
{
printf("Producer\n");
close(fd[0]); // close read end
for(int i = 1; i <= 5; i++)
{
data = i * 10;
write(fd[1], &data, sizeof(data));
printf("Produced %d\n", data);
}
close(fd[1]);
end = clock();
timetaken = (double)(end - start) / CLOCKS_PER_SEC;
efficiency = 5 / timetaken;
printf("Communication Time is %f sec\n", timetaken);
printf("Communication Efficiency = %f items/sec\n", efficiency);
}
}