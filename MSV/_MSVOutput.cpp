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
