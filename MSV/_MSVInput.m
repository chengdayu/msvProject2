frame(fp,stone,step) and (
FILE *fp and skip;
int stone[5]<==[1,1,0,2,2] and skip;
int step[524] and skip;
 function main_1 ( int RValue )
 {
     frame(main_1_i,return_1) and ( 
     int return_1<==0 and skip;
     int main_1_i and skip;
     main_1_i:=0;
     
     while(main_1_i<524)
     {
         step[main_1_i]:=-1;
         main_1_i:=main_1_i+1
         
     };
     recursive_backtrack(0);
     return_1<==1 and RValue:=0;
     skip
     )
     }; 
  function recursive_backtrack ( int n )
 {
     frame(recursive_backtrack_i,recursive_backtrack_original_empty_pos,recursive_backtrack_2_i) and ( 
     int recursive_backtrack_i and skip;
     int recursive_backtrack_original_empty_pos and skip;
     if(is_success(RValue)) then 
     {
         print_result()
     }
     else
     {
         int recursive_backtrack_2_i<==0 and skip;
         
         while(recursive_backtrack_2_i<5)
         {
             if(can_shift(recursive_backtrack_2_i,RValue)) then 
             {
                 recursive_backtrack_original_empty_pos:=where_empty(RValue);
                 step[n]:=recursive_backtrack_2_i;
                 frog_shift(recursive_backtrack_2_i);
                 recursive_backtrack(n+1);
                 frog_shift(recursive_backtrack_original_empty_pos)
                 
             }
             else 
             {
                  skip 
             };
             recursive_backtrack_2_i:=recursive_backtrack_2_i+1
             
         }
     }
     )
     }; 
  function can_shift ( int i,int RValue )
 {
     frame(can_shift_empty_pos,nm_1$,break_1$,switch_1,return_1) and ( 
     int return_1<==0 and skip;
     int can_shift_empty_pos and skip;
     can_shift_empty_pos:=where_empty(RValue);
     int switch_1 and skip;
     int break_1$ and skip;
     break_1$<==0 and skip;
      switch_1<==0 and skip;
      int nm_1$ and skip;
     nm_1$ := stone[i];
     if (nm_1$=1 or (switch_1=1 and break_1$=0 and return_1=0) ) then 
     {
         switch_1<==1 and skip;
         if(can_shift_empty_pos>i and can_shift_empty_pos<=i+2) then 
         {
             return_1<==1 and RValue:=1;
             skip
         }
         else 
         {
              skip 
         };
         if(return_1=0)   then 
         {
             break_1$<==1 and skip
          }
         else
         {
             skip
         }
         
     }
     else
     {
         skip
     };
     if (nm_1$=2 or (switch_1=1 and break_1$=0 and return_1=0) ) then 
     {
         switch_1<==1 and skip;
         if(can_shift_empty_pos<i and can_shift_empty_pos>=i-2) then 
         {
             return_1<==1 and RValue:=1;
             skip
         }
         else 
         {
              skip 
         };
         if(return_1=0)   then 
         {
             break_1$<==1 and skip
          }
         else
         {
             skip
         }
         
     }
     else
     {
         skip
     };
     if (nm_1$=0 or (switch_1=1 and break_1$=0 and return_1=0) ) then 
     {
         switch_1<==1 and skip
         
     }
     else
     {
         skip
     };
     if(break_1$=0 and return_1=0) then 
     {
         return_1<==1 and RValue:=0;
         skip;
         break_1$<==1 and skip
          
     }
     else
     {
          skip
     };
     if(return_1=0)   then 
     {
         return_1<==1 and RValue:=0;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function frog_shift ( int i )
 {
     frame(frog_shift_empty_pos) and ( 
     int frog_shift_empty_pos and skip;
     frog_shift_empty_pos:=where_empty(RValue);
     stone[frog_shift_empty_pos]:=stone[i];
     stone[i]:=0
     )
     }; 
  function where_empty ( int RValue )
 {
     frame(where_empty_i,return_1,break_1) and ( 
     int break_1<==0 and skip;
     int return_1<==0 and skip;
     int where_empty_i and skip;
     break_1<==0 and skip;
     where_empty_i:=0;
     
     while( break_1=0 and  where_empty_i<5)
     {
         if(stone[where_empty_i]=0) then 
         {
             break_1<==1 and skip
          }
         else 
         {
              skip 
         };
         if(break_1=0)   then
         {
             where_empty_i:=where_empty_i+1
         }
         else
         {
             skip
         }
         
     };
     break_1<==0 and skip;
     return_1<==1 and RValue:=where_empty_i;
     skip
     )
     }; 
  function is_success ( int RValue )
 {
     frame(return_1) and ( 
     int return_1<==0 and skip;
     if(stone[0]=2 and stone[1]=2 and stone[2]=0 and stone[3]=1 and stone[4]=1) then 
     {
         return_1<==1 and RValue:=1;
         skip
     }
     else 
     {
          skip 
     };
     if(return_1=0)   then 
     {
         return_1<==1 and RValue:=0;
         skip
     }
     else
     {
         skip
     }
     )
     }; 
  function print_result (  )
 {
     frame(print_result_i,break_1) and (
     int break_1<==0 and skip;
     int print_result_i and skip;
     break_1<==0 and skip;
     print_result_i:=0;
     
     while( break_1=0 and  print_result_i<524)
     {
         if(step[print_result_i]=-1) then 
         {
             output ("\n") and skip;
             break_1<==1 and skip
              
         }
         else 
         {
              skip 
         };
         if(break_1=0)   then
         {
             output (step[print_result_i]," "," ") and skip;
             print_result_i:=print_result_i+1
         }
         else
         {
             skip
         }
     };
     break_1<==0 and skip
     )
 };
<<<<<<< HEAD
  main_1(RValue)
 )
=======
<<<<<<< HEAD
TestCmdE2DataMerge (  )

=======
 DecodeEntry()
>>>>>>> 15554ea7db1f6f0b26b9a362b3577dc7e08a654a
  )
>>>>>>> 6c11aef323b300f2dedc16e6da8a432dcf066b2d
