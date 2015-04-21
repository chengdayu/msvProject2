#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int RValuef(int $$Ext,int x){
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
x=0;
}
int main() {
int x;
printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
x=f(0,f(1,f(1,x)));
printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
return 0;
}
