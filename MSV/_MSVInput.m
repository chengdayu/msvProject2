function f(int x,int RValue)
{frame(x) and
(
 RValue:=x+1
 )
};
function g(int x,(*tst)(int x,int RValue))
{frame(x) and
(
 x:=x+1
 )
};
frame(y) and
(
	float y and skip;
	g(1,f)

)