#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int main() {
int x;
x=2;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("\n");
$$state_num++;
return 0;
}
