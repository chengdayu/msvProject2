struct S {
 int a
 };

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
frame(y,s) and
(
	int y and skip;
	S s and skip;
	s.a:=f(1,RValue);
	s.a:=g(1,RValue)
)