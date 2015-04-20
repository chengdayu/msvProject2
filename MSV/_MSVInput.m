frame(main_s,main_shu,main_i,main_j,main_1_s0) and ( 
     float main_s[4]<==[3,3,3,3] and skip;
     float main_shu[4]<==[0] and skip;
     output ("\n") and skip;
     int main_i<==0 and skip;
     
     while(main_i<4)
     {
 //        input(main_s[main_i]) and skip;
         int main_j<==main_i and skip;
         
         while(main_j>0)
         {
             if(main_s[main_j]<=main_s[main_j-1]) then 
             {
                 float main_1_s0<==main_s[main_j-1] and skip;
                 main_s[main_j-1]:=main_s[main_j];
                 main_s[main_j]:=main_1_s0
                 
             }
             else 
             {
                  skip 
             };
             main_j:=main_j-1
             
         };
         main_i:=main_i+1
         
     };
     output ("\n") and skip
	)