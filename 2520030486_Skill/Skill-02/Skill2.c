#include <stdio.h>
#include <unistd.h>
int main()
{
char buf[100];
int n;
while (1) {
printf("Enter a Command\n ");
n = read(0, buf, 100);
if (n <= 0)
break;
if (buf[0] == 'e' &&
    buf[1] == 'x' &&
    buf[2] == 'i' &&
    buf[3] == 't') {
printf("Exiting shell...\n");
break;
}
printf("Command received\n");
write(1, buf, n);
}
}