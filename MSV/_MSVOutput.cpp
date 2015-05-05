#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int $$state_num=0;
int $$pathNum=1;
int $$len_i=0;
int $$flag[100] = { 0 };
int $$length = 0, $$current = 0,$$firstTime=0;
void $$changeState()
{if ($$current == $$length){
$$length++; $$flag[$$length - 1] = 0;}
$$current++; }

int y;
int  f(int $$Ext,int x);
int  f(int $$Ext,int x){
int  RValue=0;
RValue=(x)+(1);
return RValue;
}

int main() {
while (true){
while ($$length > 0){
if ($$flag[$$length - 1] == 1){
$$length--;}
else {
$$flag[$$length - 1] = 1; break;}}
if ( ($$length == 0 && $$flag[0] == 1) || ($$current == 0 && $$firstTime==1)){break;}
$$current = 0;
printf("Path Number: %d",$$pathNum);
printf("\n");
if (f(1,1)<0){y=3;
} else {y=5;
}
;
$$firstTime=1;
$$state_num=0;
printf("\n");
$$pathNum=$$pathNum+1;
}
return 0;
}
