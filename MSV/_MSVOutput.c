#include<stdio.h>
#include<stdlib.h>
<<<<<<< HEAD
int $state_num=0;
int  f(int $Ext,int f_x){
return f_x;
}
int main() {
int x;
x=f(1,x);
+4;
printf("state %d:", $state_num);
printf("x=%d ",x);
=======
int $$state_num=0;
int main() {
int x;
x=1;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("\n");
$$state_num++;
int $y_1;
$y_1=3;
printf("state %d:", $$state_num);
printf("$y_1=%d ",$y_1);
printf("x=%d ",x);
printf("\n");
$$state_num++;
x=$y_1;
printf("state %d:", $$state_num);
printf("$y_1=%d ",$y_1);
printf("x=%d ",x);
printf("\n");
$$state_num++;
int z;
z=4;
printf("state %d:", $$state_num);
printf("$y_1=%d ",$y_1);
printf("x=%d ",x);
printf("z=%d ",z);
printf("\n");
$$state_num++;
z=5;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("z=%d ",z);
printf("\n");
$$state_num++;
printf("state %d:", $$state_num);
printf("x=%d ",x);
printf("z=%d ",z);
>>>>>>> c2d8e9faa831b931f2a158e360bae6652ee5fde5
printf("\n");
$$state_num++;
return 0;
}
