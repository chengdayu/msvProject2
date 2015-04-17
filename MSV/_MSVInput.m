function int f(int f_x)
{
   frame(f_x) and 
   (
	  return f_x
   )
};
frame(x) and
(
  int x<==extern f(x)+4 and empty
)