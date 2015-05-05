function main_1 (  )
 {
     frame(main_1_s,main_1_shu,main_1_i,main_1_j,main_1_1_s0) and ( 
     float main_1_s[4]<==[3,3,3,3],main_1_shu[4]<==[0] and skip;
     output ("请输入四个数(0-9):\n") and skip;
     int main_1_i<==0 and skip;
     
     while(main_1_i<4)
     {
         input(main_1_s[main_1_i]) and skip;
         int main_1_j<==main_1_i and skip;
         
         while(main_1_j>0)
         {
             if(main_1_s[main_1_j]<=main_1_s[main_1_j-1]) then 
             {
                 float main_1_1_s0<==main_1_s[main_1_j-1] and skip;
                 main_1_s[main_1_j-1]:=main_1_s[main_1_j];
                 main_1_s[main_1_j]:=main_1_1_s0
                 
             }
             else 
             {
                  skip 
             };
             main_1_j:=main_1_j-1
             
         };
         main_1_i:=main_1_i+1
         
     }
    )
};
main_1()
