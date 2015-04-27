#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int $$len_i=0;
int $$flag[10] = { 0 };
int $$length = 0, $$current = -1,$$firstTime=0;
void $$changeState()
{if ($$current == $$length){
$$length++; $$flag[$$length - 1] = 0;}
$$current++; }
int y;
int  f(int $$Ext,int x);
int  f(int $$Ext,int x){
int  RValue=0;
if(!$$Ext){printf("state %d:", $$state_num);
printf("f: " );
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
};
RValue=(x)+(1);
if(!$$Ext){printf("state %d:", $$state_num);
printf("f: " );
printf("RValue=%d ",RValue);
printf("x=%d ",x);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
};
return RValue;
}

int main() {
while (true){
while ($$length > 0){
if ($$flag[$$length - 1] == 1){
$$length--;}
else {
$$flag[$$length - 1] = 1; break;}}
if ($$length == 0 && $$flag[0] == 1 || $$current == -1 && $$firstTime==1){break;}
y=0;
printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
if (f(1,y)){printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
y=4;
} else {printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
y=5;
}
;
printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
$$firstTime=1;
$$state_num=0;
printf("\n");
}
return 0;
}
