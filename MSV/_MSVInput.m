function int f(int x)
{ 
  frame(x) and
  (
      x:=x+1;
      return x
  )
};
frame(x,y) and
(
   int x and skip;
   x:=f(x)
)
