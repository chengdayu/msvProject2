#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
<<<<<<< HEAD
int  f(int $Ext,int x,int y){
printf("state %d: ", $state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$state_num++;
x=x+1;
return x;
}
int main() {
int x;
printf("state %d: ", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
int y;
printf("state %d: ", $state_num);
=======
int  f(int $Ext,int x){
if(!$Ext){printf("state %d:", $state_num);
printf("x=%d ",x);
printf("\n");
$state_num++;
}x=0;
return x;
}
int main() {
int x;
printf("state %d:", $state_num);
>>>>>>> 4d3f1b5329191bf09419fbf4d0e27dcdcdc0eae4
printf("x=%d ",x);
printf("\n");
$state_num++;
<<<<<<< HEAD
x=f(1,x,y);
;
printf("state %d: ", $state_num);
=======
printf("state %d:", $state_num);
>>>>>>> 4d3f1b5329191bf09419fbf4d0e27dcdcdc0eae4
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$state_num++;
<<<<<<< HEAD
printf("state %d: ", $state_num);
=======
x=f(0,x);
printf("state %d:", $state_num);
>>>>>>> 4d3f1b5329191bf09419fbf4d0e27dcdcdc0eae4
printf("x=%d ",x);
printf("\n");
$state_num++;
return 0;
}
