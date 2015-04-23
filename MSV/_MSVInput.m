function f(int RValue)
{
  frame(x) and
  (
    int x and skip;
	x<==2 and skip;
	frame(y) and 
	(
	 int y<==4 and skip
	);
	RValue:=4
  )
};

function f1()
{
  frame(x) and
  (
    int x and skip;
	x<==2 and skip;
	frame(y) and 
	(
	 int y<==4 and skip
	)
  )
};


frame(name) and
(
///char *name[4]<==["空手","带狼","带羊","带菜"] and skip
f();
f1()
)