#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
void  f1(int $$Ext);
int  f(int $$Ext);
void  f1(int $$Ext){
int x;
int y;
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
x=2;
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
y=4;
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
};
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
};
}
int  f(int $$Ext){
int  RValue;
int x;
int y;
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
x=2;
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
y=4;
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
};
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$$state_num++;
};
RValue=4;
if(!$$Ext){printf("state %d:", $$state_num);
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
return RValue;
}

int main() {
f(0);
printf("state %d:", $$state_num);
printf("\n");
$$state_num++;
f1(0);
printf("state %d:", $$state_num);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("\n");
$$state_num++;
return 0;
}
