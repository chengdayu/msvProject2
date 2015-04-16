#include<stdio.h>
#include<stdlib.h>
int $state_num=0;
<<<<<<< HEAD
=======
<<<<<<< HEAD
int  f(int $Ext,int x,int y){
=======
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
>>>>>>> c3d5d4b8e563a664ae2cec7044e17675c3315439
int main() {
int y;
<<<<<<< HEAD
y=3;
=======
printf("state %d: ", $state_num);
=======
int  f(int $Ext,int x){
>>>>>>> aee1613b052601d96a44fcbd9db9e3370e23b714
if(!$Ext){printf("state %d:", $state_num);
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$state_num++;
}x=x+1;
return x;
}
int main() {
int x;
printf("state %d:", $state_num);
>>>>>>> 4d3f1b5329191bf09419fbf4d0e27dcdcdc0eae4
printf("x=%d ",x);
printf();
printf("\n");
$state_num++;
<<<<<<< HEAD
int y;
=======
<<<<<<< HEAD
x=f(1,x,y);
;
printf("state %d: ", $state_num);
=======
>>>>>>> aee1613b052601d96a44fcbd9db9e3370e23b714
>>>>>>> c3d5d4b8e563a664ae2cec7044e17675c3315439
printf("state %d:", $state_num);
printf("y=%d ",y);
printf("\n");
$state_num++;
<<<<<<< HEAD
y=5;
printf("state %d:", $state_num);
=======
<<<<<<< HEAD
x=f(1,x,y);
=======
<<<<<<< HEAD
printf("state %d: ", $state_num);
=======
x=f(0,x);
>>>>>>> aee1613b052601d96a44fcbd9db9e3370e23b714
printf("state %d:", $state_num);
>>>>>>> 4d3f1b5329191bf09419fbf4d0e27dcdcdc0eae4
printf("x=%d ",x);
printf("y=%d ",y);
printf("\n");
$state_num++;
printf("state %d:", $state_num);
printf("x=%d ",x);
printf("y=%d ",y);
>>>>>>> c3d5d4b8e563a664ae2cec7044e17675c3315439
printf("\n");
$state_num++;
return 0;
}
