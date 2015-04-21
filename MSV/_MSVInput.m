<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
frame(zuhe,sum,fuhe) and (
int sum<==0 and skip;

int m<==sum and skip;
frame(x) and
(int x<==sum+2 and skip)
  
  )
=======
<<<<<<< HEAD
>>>>>>> a0625e99a138a65c2d87758d4358ef63fda5f42e
>>>>>>> 7d137d80c839948354438f4c4578de1322237138
function  f(int x, int RValue)
{
   frame(x) and
   (
   x:=0
   )
};
frame(x) and
(
  int x and skip;
  //x:=f(x, y)
  x:=f(extern f(extern f(x)))
<<<<<<< HEAD
)
=======
)
<<<<<<< HEAD
=======
=======
<<<<<<< HEAD
function zuheshu ( int n,int* RValue)
 {
 frame(n) and
 (
    n:=2
)
     
 }; 
 frame(x) and
 (
    int x and skip
 )
=======
float i and float *p<==&i and skip
>>>>>>> dc6b74fe64cd819648d014634a1d906f7c9e575f
>>>>>>> 7d0b14f1f77a37b0417cd6cea642278d3860085d
>>>>>>> a95d82705015794196b5d30c9b1aa4aa444495db
>>>>>>> a0625e99a138a65c2d87758d4358ef63fda5f42e
>>>>>>> 7d137d80c839948354438f4c4578de1322237138
