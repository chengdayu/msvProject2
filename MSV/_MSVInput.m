function int f(int x,int y)
{
x:=x+1;
 return x
};
frame(x,y) and
(
int x and skip;
int y and skip;
x<== extern f(x,y) and skip

)
