#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int main() {
float i;
float *p;
p=&i;
printf("state %d:", $$state_num);
printf("i=%f ",i);
printf("p=%p ",p);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("\n");
$$state_num++;
return 0;
}
