#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int main() {
int sum;
sum=0;
printf("state %d:", $$state_num);
printf("sum=%d ",sum);
printf("\n");
$$state_num++;
int m;
m=sum;
printf("state %d:", $$state_num);
printf("sum=%d ",sum);
printf("m=%d ",m);
printf("\n");
$$state_num++;
int x;
x=sum+2;
printf("state %d:", $$state_num);
printf("sum=%d ",sum);
printf("x=%d ",x);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("sum=%d ",sum);
printf("x=%d ",x);
printf("\n");
$$state_num++;
return 0;
}
