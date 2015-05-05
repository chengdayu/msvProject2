function f(int x,int RValue)
{frame(x) and
(
 RValue:=x+1
 )
};
frame(y) and 
(
	int y and skip;
	if(extern f(1,RValue)<0)
	then
	{
		y:=3
	}
	else
	{
		y:=5
	}
)