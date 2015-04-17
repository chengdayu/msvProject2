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
