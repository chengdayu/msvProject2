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

)