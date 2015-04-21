function  f(int x, int RValue)
{
   frame(x) and
   (
   x:=0
   )
};
frame(x) and
(
  int x and skip;
  //x:=f(x, y)
  x:=f(extern f(extern f(x)))
)
