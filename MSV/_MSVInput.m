function f(int x,int RValue)
{frame(x) and
(
 RValue:=x+1
 )
};
function g(int x,int RValue)
{frame(x) and
(
 RValue:=x+1
 )
};
frame(y,z) and
(
	int y and int z and skip;
	y<==0 and skip;
	z:=f(y,RValue)
)