#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
int  f(int $Ext,int x){
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
x=0;
return x;
}
int main() {
int x;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
x=f(0,x);
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
return 0;
}
