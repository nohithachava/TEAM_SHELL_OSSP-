#include<stdio.h>
#include<unistd.h>
#include<time.h>
int main() {
int fd[2];//declaring the pipe
pipe(fd);//creating the pipe
int pid,data;//declaring the cariables for data and pid
int start,end;
double timetaken;
pid=fork();//creating the process
start=clock();
if(pid==0)//checking for child
{
printf("Consumer \n");//child is not writing its reading so close the write e>
close(fd[1]);//child is not writing its reading so close the write
for(int i=0;i<5;i++)//5items in consumer so it produces the consumer
{
read(fd[0],&data,sizeof(data));
printf("Consumed %d\n",data);
}
close(fd[0]);
}
else
{
printf("Producer \n");
close(fd[0]);
for(int i=1;i<=5;i++)
{
data = i*10;
write(fd[1],&data,sizeof(data));
printf("Produced %d\n",data);
}
close(fd[1]);
end=clock();
timetaken=(double)(end-start)/CLOCKS_PER_SEC;
printf("Time taken is %f",timetaken);
printf("Communication Time is %f",timetaken);
}
}