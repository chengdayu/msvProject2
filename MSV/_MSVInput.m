<<<<<<< HEAD
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
=======
<<<<<<< HEAD


 frame(x,p) and
 (
   int x and skip;
   int *p and skip;
   p<==(int *)(extern malloc(4)) and skip
 )
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> 47aaae9de71b5b4f9a7548c2871ed55dca53b6da
function float* fsz ( int n)
 {
     frame(return_1, fp) and ( 
     int return_1<==0 and skip;
<<<<<<< HEAD
	 float* fp<==(float *)malloc(n*sizeof(float)) and skip;
=======
	// float* fp<==(float *)malloc(n*sizeof(float)) and skip;
>>>>>>> 47aaae9de71b5b4f9a7548c2871ed55dca53b6da
     return fp
     )
 };
 frame(x) and
 (
<<<<<<< HEAD
    frame(main_s,main_shu,main_i,main_j,main_1_s0) and ( 
     float main_s[4]<==[3,3,3,3] and skip;
     float main_shu[4]<==[0] and skip;
 )
=======
   int x and skip
 )
=======
<<<<<<< HEAD
function int f(int f_x)
{
   frame(f_x) and 
   (
	  return f_x
   )
};
frame(x) and
(
  int x<==extern f(x)+4 and empty
=======
frame(x,y) and 
(
int x<==3 and int y<==0 and skip;
if(x=1) then
{
  x:=2
}
else
{
  x:=3
}
>>>>>>> c2d8e9faa831b931f2a158e360bae6652ee5fde5
)
>>>>>>> ebb236e6f1a0db977cbc50b088c5b08c0a9639ff
>>>>>>> 47aaae9de71b5b4f9a7548c2871ed55dca53b6da
>>>>>>> e98b7ee924eedc32394652f09bc086444d67f6b4
>>>>>>> 20061fd6b76e4c556452591ad055c9ca09edc1ad
