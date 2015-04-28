<<<<<<< HEAD
frame(a) and
(
  unsigned int a[4,32] and skip;
  output (((unsigned int *)a)[3]) and skip
=======
function f(int x,int RValue)
{frame(x) and
(
 RValue:=x+1
 )
};
frame(y) and
(
int y<==0 and skip;
if(extern f(y,RValue)) then
{
y:=4
}
else
{
y:=5
}

>>>>>>> 1ed553aa624d176f8e9ad29593483645dab81eeb
)