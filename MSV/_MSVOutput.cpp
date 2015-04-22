#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
__SearchAndInsert : variable x has been declarated in the frame table.
__SortPrsnt: declaration of variable fail : int x
empty tree string f(int $$Ext,int x){
if(!$$Ext){printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
};
}
int main() {
int x;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
f(1,x);
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
return 0;
}
