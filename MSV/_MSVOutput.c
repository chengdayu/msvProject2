#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
int main() {
int y;
y=3;
printf("state %d:", $state_num);
printf("y=%d ",y);
printf("\n");
$state_num++;
y=5;
printf("state %d:", $state_num);
printf("\n");
$state_num++;
return 0;
}
