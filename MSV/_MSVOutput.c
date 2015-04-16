#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
int f($Ext,x,y){
return x;}
int main() {
int x;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf();
printf("\n");
$state_num++;
int y;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$state_num++;
x=f(1,x,y);
;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$state_num++;
return 0;
}
