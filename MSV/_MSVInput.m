function int f(int x)
{
 frame(x) and
 (
  x:=0;
  return x
  )
};
frame(x) and 
(
  int x and skip;
  x:=f(x)
)