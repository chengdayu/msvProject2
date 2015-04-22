 function qiujie ( float suanshi[],int n,float RValue )
 {
     frame(qiujie_1_a,nm_3$,break_1$,switch_1,qiujie_pq,qiujie_p,qiujie_q,qiujie_k,qiujie_suanshi1,qiujie_s,qiujie_sum,qiujie_m,nm_4$,qiujie_9_10_11_ws,qiujie_9_10_11_w,qiujie_9_10_17_ws,qiujie_9_10_17_w,return_1,continue_1) and (
     int continue_1<==0 and skip;
     int return_1<==0 and skip;
     if(n=3) then 
     {
         float qiujie_1_a<==0 and skip;
         int switch_1 and skip;
         int break_1$ and skip;
         break_1$<==0 and skip;
          switch_1<==0 and skip;
          int nm_3$ and skip;
         nm_3$ := (char)(suanshi[1]);
         if (nm_3$='+' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
         {
             switch_1<==1 and skip;
             return_1<==1 and RValue:=(suanshi[0]+suanshi[2]);
             skip
             
         }
         else
         {
             skip
         };
         if (nm_3$='-' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
         {
             switch_1<==1 and skip;
             return_1<==1 and RValue:=(suanshi[0]-suanshi[2]);
             skip
             
         }
         else
         {
             skip
         };
         if (nm_3$='*' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
         {
             switch_1<==1 and skip;
             return_1<==1 and RValue:=(suanshi[0]*suanshi[2]);
             skip
             
         }
         else
         {
             skip
         };
         if (nm_3$='/' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
         {
             switch_1<==1 and skip;
             return_1<==1 and RValue:=(suanshi[0]/ suanshi[2]);
             skip
             
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
     if(return_1=0)   then 
     {
         float qiujie_pq<=='0' and skip;
         float *qiujie_p<==&qiujie_pq and skip;
         float *qiujie_q<==&qiujie_pq and skip;
         int qiujie_k<==0 and skip;
         float qiujie_suanshi1[8]<==['0'] and skip;
         int qiujie_s<==0 and skip;
         float qiujie_sum<==0 and skip;
         continue_1<==0 and skip;
         int qiujie_m<==0 and skip;
         
         while(qiujie_m<n)
         {
              continue_1<==0 and skip;
             if(suanshi[qiujie_m]='(') then 
             {
                 qiujie_k:=qiujie_k+1;
                 if((* qiujie_p)!='(') then 
                 {
                     qiujie_p:=&suanshi[qiujie_m]
                     
                 }
                 else 
                 {
                      skip 
                 };
                 continue_1<==1 and skip;
                  qiujie_m:=qiujie_m+1
             }
             else 
             {
                  skip 
             };
             if(continue_1=0)   then 
             {
                 if(suanshi[qiujie_m]=')') then 
                 {
                     if(qiujie_k=1) then 
                     {
                         qiujie_k:=qiujie_k-1;
                         qiujie_q:=&suanshi[qiujie_m];
                         qiujie_suanshi1[qiujie_s]:=qiujie(qiujie_p+1,qiujie_q-qiujie_p-1,RValue);
                         qiujie_s:=qiujie_s+1;
                         qiujie_p:=&qiujie_pq;
                         qiujie_q:=&qiujie_pq
                         
                     }
                     else 
                     {
                         qiujie_k:=qiujie_k-1
                         
                     };
                     continue_1<==1 and skip;
                      qiujie_m:=qiujie_m+1
                 }
                 else 
                 {
                      skip 
                 };
                 if(continue_1=0)   then 
                 {
                     if(qiujie_k=0) then 
                     {
                         qiujie_suanshi1[qiujie_s]:=suanshi[qiujie_m];
                         qiujie_s:=qiujie_s+1;
                         continue_1<==1 and skip;
                          qiujie_m:=qiujie_m+1
                     }
                     else 
                     {
                          skip 
                     };
                     if(continue_1=0)   then 
                     {
                         qiujie_m:=qiujie_m+1
                     }
                     else
                     {
                         skip
                     }
                 }
                 else
                 {
                     skip
                 }
             }
             else
             {
                 skip
             }
             
         };
         continue_1<==0 and skip;
         if(qiujie_s=3) then 
         {
             qiujie_sum:=qiujie(qiujie_suanshi1,qiujie_s,RValue)
             
         }
         else
         {
             qiujie_p:=&qiujie_suanshi1[0];
             qiujie_q:=&qiujie_suanshi1[2];
             qiujie_m:=0;
             
             while(qiujie_m<(qiujie_s-1)/ 2)
             {
                 break_1$<==0 and skip;
                  switch_1<==0 and skip;
                  int nm_4$ and skip;
                 nm_4$ := (char)(qiujie_suanshi1[2*qiujie_m+1]);
                 if (nm_4$='+' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
                 {
                     switch_1<==1 and skip;
                     if((qiujie_s-1)/ 2!=1 and (qiujie_suanshi1[2*(qiujie_m+1)+1]='*' or qiujie_suanshi1[2*(qiujie_m+1)+1]='/')) then 
                     {
                         * qiujie_q:=qiujie(qiujie_p+2,3,RValue);
                         int qiujie_9_10_11_ws<==&qiujie_suanshi1[qiujie_s-1]-&qiujie_suanshi1[2*(qiujie_m+1)] and skip;
                         int qiujie_9_10_11_w<==1 and skip;
                         
                         while(qiujie_9_10_11_w<=qiujie_9_10_11_ws)
                         {
                             if(((qiujie_q+qiujie_9_10_11_w+2)-(qiujie_q+qiujie_9_10_11_ws))>0) then 
                             {
                                 * (qiujie_q+qiujie_9_10_11_w):=-1
                                 
                             }
                             else
                             {
                                 * (qiujie_q+qiujie_9_10_11_w):=* (qiujie_q+qiujie_9_10_11_w+2)
                             };
                             qiujie_9_10_11_w:=qiujie_9_10_11_w+1
                             
                         };
                         qiujie_s:=qiujie_s-2;
                         qiujie_m:=qiujie_m-1
                         
                     }
                     else
                     {
                         if(qiujie_m=0) then 
                         {
                             qiujie_sum:=qiujie_sum+* qiujie_p+* qiujie_q
                             
                         }
                         else
                         {
                             qiujie_sum:=qiujie_sum+* qiujie_q
                         };
                         qiujie_p:=qiujie_p+2;
                         qiujie_q:=qiujie_q+2
                     };
                     break_1$<==1 and skip
                      
                 }
                 else
                 {
                     skip
                 };
                 if (nm_4$='-' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
                 {
                     switch_1<==1 and skip;
                     if((qiujie_s-1)/ 2!=1 and (qiujie_suanshi1[2*(qiujie_m+1)+1]='*' or qiujie_suanshi1[2*(qiujie_m+1)+1]='/')) then 
                     {
                         * qiujie_q:=qiujie(qiujie_p+2,3,RValue);
                         int qiujie_9_10_17_ws<==&qiujie_suanshi1[qiujie_s-1]-&qiujie_suanshi1[2*(qiujie_m+1)] and skip;
                         int qiujie_9_10_17_w<==1 and skip;
                         
                         while(qiujie_9_10_17_w<=qiujie_9_10_17_ws)
                         {
                             if(((qiujie_q+qiujie_9_10_17_w+2)-(qiujie_q+qiujie_9_10_17_ws))>0) then 
                             {
                                 * (qiujie_q+qiujie_9_10_17_w):=-1
                                 
                             }
                             else
                             {
                                 * (qiujie_q+qiujie_9_10_17_w):=* (qiujie_q+qiujie_9_10_17_w+2)
                             };
                             qiujie_9_10_17_w:=qiujie_9_10_17_w+1
                             
                         };
                         qiujie_s:=qiujie_s-2;
                         qiujie_m:=-1
                         
                     }
                     else
                     {
                         if(qiujie_m=0) then 
                         {
                             qiujie_sum:=qiujie_sum+* qiujie_p-* qiujie_q
                         }
                         else
                         {
                             qiujie_sum:=qiujie_sum-* qiujie_q
                         };
                         qiujie_p:=qiujie_p+2;
                         qiujie_q:=qiujie_q+2
                     };
                     break_1$<==1 and skip
                      
                 }
                 else
                 {
                     skip
                 };
                 if (nm_4$='*' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
                 {
                     switch_1<==1 and skip;
                     if(qiujie_m=0) then 
                     {
                         qiujie_sum:=qiujie_sum+(* qiujie_p)*(* qiujie_q)
                     }
                     else
                     {
                         qiujie_sum:=qiujie_sum** qiujie_q
                     };
                     qiujie_p:=qiujie_p+2;
                     qiujie_q:=qiujie_q+2;
                     break_1$<==1 and skip
                      
                 }
                 else
                 {
                     skip
                 };
                 if (nm_4$='/' or (switch_1=1 and break_1$=0 and return_1=0) ) then 
                 {
                     switch_1<==1 and skip;
                     if(qiujie_m=0) then 
                     {
                         qiujie_sum:=qiujie_sum+(* qiujie_p)/ (* qiujie_q)
                         
                     }
                     else
                     {
                         qiujie_sum:=qiujie_sum/ * qiujie_q
                     };
                     qiujie_p:=qiujie_p+2;
                     qiujie_q:=qiujie_q+2;
                     break_1$<==1 and skip
                      
                 }
                 else
                 {
                     skip
                 };
                 qiujie_m:=qiujie_m+1
                 
             }
         };
         return_1<==1 and RValue:=qiujie_sum;
         skip
     }
     else
     {
         skip
     }
     )
 };
frame(x) and
(
   int x<==1 and skip
)