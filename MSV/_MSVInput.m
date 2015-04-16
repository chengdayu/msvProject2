function void f(int x)
{frame(x) and
(
 x:=x+1;
 x:=x+1;
 return x
 )
};
frame(x) and
(
int x<==0 and skip;
f(x)
)