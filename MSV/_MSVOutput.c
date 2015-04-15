#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
float *  f(int $Ext,float* x){
x=0;
printf("state %d:", $state_num);
printf();
printf("\n");
$state_num++;
return x;
}
int main() {
int x;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
return 0;
}
