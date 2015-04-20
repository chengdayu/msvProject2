function float* fsz ( int n)
 {
     frame(return_1, fp) and ( 
     int return_1<==0 and skip;
	 float* fp<==(float *)malloc(n*sizeof(float)) and skip;
     return fp
     )
 };
 frame(x) and
 (
    frame(main_s,main_shu,main_i,main_j,main_1_s0) and ( 
     float main_s[4]<==[3,3,3,3] and skip;
     float main_shu[4]<==[0] and skip;
 )
