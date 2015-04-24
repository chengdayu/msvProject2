#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int x;
int y;

int main() {
x=3;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
y=4;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
int $$$x;
$$$x=x+y;
int $$$y;
$$$y=x-y;
x=$$$x;
y=$$$y;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
return 0;
}
