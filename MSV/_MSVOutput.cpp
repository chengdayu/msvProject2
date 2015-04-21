#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
 f(int ){
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
x=0;
}
int main() {
<<<<<<< HEAD
int x;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
x=f(0,f(f(x)));
printf("state %d:", $$state_num);
=======
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
=======
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
>>>>>>> a95d82705015794196b5d30c9b1aa4aa444495db
>>>>>>> a0625e99a138a65c2d87758d4358ef63fda5f42e
printf("x=%d ",x);
printf("\n");
$$state_num++;
return 0;
}
