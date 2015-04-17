<<<<<<< HEAD
function int f(int x)
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
x:= extern f(x)
)
=======
<<<<<<< HEAD
frame(x,y,z) and
(
 int x<==1 and skip;
 local(y):
 {
   frame(y) and 
   (
	int y<==3 and skip;
	x<==y and skip
   )
 };
 int z<==4 and skip;
 z<==5 and skip
)
=======
frame(p) and
(
  int *p and skip;
  p<==()
)
>>>>>>> fd7d613a2f93a04c3cd4e320ada6e052ca6b4d93
>>>>>>> fc113392dcbad894ccff9f86afb696eb581ffa5c
