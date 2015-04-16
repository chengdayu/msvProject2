#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
 f(int $$Ext,int x){
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
}x=x+1;
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
}x=x+1;
return x;
}
int main() {
int x;
x=0;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
f(0,x);
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
return 0;
}
