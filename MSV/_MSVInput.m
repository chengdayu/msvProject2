

 frame(x,p) and
 (
   int x and skip;
   int *p and skip;
   p<==(int *)(extern malloc(4)) and skip
 )
