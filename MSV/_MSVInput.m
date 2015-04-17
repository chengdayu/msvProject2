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
