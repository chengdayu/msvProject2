function f(int x,int RValue)
{frame(x) and
(
 RValue:=x+1
 )
};
frame(y) and 
(
	int y<==0 and skip;
	while(!(extern f(y,RValue)<3))
	{
		y:=y+1
	}
)