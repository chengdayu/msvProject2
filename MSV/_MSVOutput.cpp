#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int $$state_num=0;
int $$len_i=0;
int $$flag[10] = { 0 };
int $$length = 0, $$current = -1,$$firstTime=0;
void $$changeState()
{if ($$current == $$length){
$$length++; $$flag[$$length - 1] = 0;}
$$current++; }

int x,y;
void * f(int $$Ext,int z);
void * f(int $$Ext,int z){
void * RValue=0;
z=1;
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
empty tree string;
$$firstTime=1;
$$state_num=0;
printf("\n");
}
return 0;
}
