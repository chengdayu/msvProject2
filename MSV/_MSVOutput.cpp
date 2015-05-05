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


void  main_1(int $$Ext);
void  main_1(int $$Ext){
float main_1_s[4]={3,3,3,3};
float main_1_shu[4]={0};
int main_1_i;
int main_1_j;
float main_1_1_s0;
printf("请输入四个数(0-9):\n");
main_1_i=0;
while(main_1_i<4){scanf("%f",&main_1_s[main_1_i]);
main_1_j=main_1_i;
while(main_1_j>0){if (main_1_s[main_1_j]<=main_1_s[(main_1_j)-(1)]){main_1_1_s0=main_1_s[(main_1_j)-(1)];
main_1_s[(main_1_j)-(1)]=main_1_s[main_1_j];
main_1_s[main_1_j]=main_1_1_s0;
}
;
main_1_j=(main_1_j)-(1);
}
;
main_1_i=(main_1_i)+(1);
}
;
}

int main() {
while (true){
while ($$length > 0){
if ($$flag[$$length - 1] == 1){
$$length--;}
else {
$$flag[$$length - 1] = 1; break;}}
if ($$length == 0 && $$flag[0] == 1 || $$current == -1 && $$firstTime==1){break;}
main_1(0);
$$firstTime=1;
$$state_num=0;
printf("\n");
}
return 0;
}
