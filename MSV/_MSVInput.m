function int f(int f_x)
{frame(f_x) and
(
 f_x:=f_x+1;
 f_x:=f_x+1;
 return f_x
 )
};
frame(x) and
(
int x<==0 and skip;
x:= f(x) and skip

)
