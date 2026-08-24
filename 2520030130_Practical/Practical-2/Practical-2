#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
int fd1,fd2;
char buf[100];
size_t n;
fd1=open("a7",O_RDONLY);
fd2=open("z9",O_CREAT|O_WRONLY,0644);
n=read(fd1,buf,100);
write(fd2,buf,n);
close(fd1);
close(fd2);
printf("file copied succesfully\n");
}
