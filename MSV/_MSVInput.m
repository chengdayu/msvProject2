function float* fsz ( int n)
 {
     frame(return_1, fp) and ( 
     int return_1<==0 and skip;
	// float* fp<==(float *)malloc(n*sizeof(float)) and skip;
     return fp
     )
 };
 frame(x) and
 (
   int x and skip
 )