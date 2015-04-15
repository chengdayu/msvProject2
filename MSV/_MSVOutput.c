#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
int f(int $Ext,int x){
printf("state %d:", $state_num);
printf();
printf("\n");
$state_num++;
return x;x=x+1;
}
int main() {
int x;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
f(1,x);
return 0;
}
