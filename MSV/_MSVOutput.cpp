#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
<<<<<<< HEAD
int y;

int main() {
printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
$$current=0;
$$changeState();
if ($$flag[$$current-1]==0){printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
y=0;
} else {printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
y=1;
}
;
printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
=======
float zuhe[4]={0};
int sum;
int fuhe;
float *  fsz(int $$Ext,int n);
void  main_1(int $$Ext);
int  zuheshu(int $$Ext,float s[],int n);
int  zuhefu(int $$Ext);
int  yunsuan(int $$Ext,float s[],char p[],int k0[],int k1[]);
int  hefa(int $$Ext,float suanshi[],int r);
float  jianjie(int $$Ext,float suanshi[],int r);
float  qiujie(int $$Ext,float suanshi[],int n);
float *  fsz(int $$Ext,int n){
float *  RValue;
int return_1;
return_1=0;
return_1=1;
RValue=(float *)(malloc(n*sizeof(float)));
return RValue;
}
void  main_1(int $$Ext){
float main_s[4]={3,3,3,3};
float main_shu[4]={0};
int main_i;
int main_j;
float main_1_s0;
printf("请输入四个数(0-9):\n");
main_i=0;
while(main_i<4){scanf("%f",&main_s[main_i]);
main_j=main_i;
while(main_j>0){if (main_s[main_j]<=main_s[(main_j)-(1)]){main_1_s0=main_s[(main_j)-(1)];
main_s[(main_j)-(1)]=main_s[main_j];
main_s[main_j]=main_1_s0;
}
;
main_j=(main_j)-(1);
}
;
main_i=(main_i)+(1);
}
;
printf("四个数是:");
main_i=0;
while(main_i<4){printf("%f",main_s[main_i]);
printf(" ");
printf(" ");
printf(" ");
main_i=(main_i)+(1);
}
;
printf("\n");
zuheshu(0,main_s,4);
printf("有");
printf("%d",fuhe);
printf("个算式符合\n");
printf("个算式符合\n");
}
int  zuheshu(int $$Ext,float s[],int n){
int  RValue;
int return_1;
int zuheshu_i;
float *zuheshu_2_p;
int zuheshu_2_o;
int zuheshu_2_k;
float *zuheshu_4_5_p;
int zuheshu_4_5_o;
int zuheshu_4_5_k;
return_1=0;
if (n==1){zuhe[(4)-(n)]=s[0];
zuhefu(0);
return_1=1;
RValue=1;
}
;
if (return_1==0){zuheshu_i=0;
while(zuheshu_i<n){if (zuheshu_i==0){zuhe[(4)-(n)]=s[zuheshu_i];
zuheshu_2_o=0;
zuheshu_2_p=fsz(0,(n)-(1));
zuheshu_2_k=0;
while(zuheshu_2_k<n){if (zuheshu_2_k!=zuheshu_i){zuheshu_2_p[zuheshu_2_o]=s[zuheshu_2_k];
zuheshu_2_o=(zuheshu_2_o)+(1);
}
;
zuheshu_2_k=(zuheshu_2_k)+(1);
}
;
zuheshu(0,zuheshu_2_p,(n)-(1));
free(zuheshu_2_p);
} else {if (s[zuheshu_i]!=s[(zuheshu_i)-(1)]){zuhe[(4)-(n)]=s[zuheshu_i];
zuheshu_4_5_o=0;
zuheshu_4_5_p=fsz(0,(n)-(1));
zuheshu_4_5_k=0;
while(zuheshu_4_5_k<n){if (zuheshu_4_5_k!=zuheshu_i){zuheshu_4_5_p[zuheshu_4_5_o]=s[zuheshu_4_5_k];
zuheshu_4_5_o=(zuheshu_4_5_o)+(1);
}
;
zuheshu_4_5_k=(zuheshu_4_5_k)+(1);
}
;
zuheshu(0,zuheshu_4_5_p,(n)-(1));
free(zuheshu_4_5_p);
}
;
}
;
zuheshu_i=(zuheshu_i)+(1);
}
;
}
;
return RValue;
}
int  zuhefu(int $$Ext){
int  RValue;
int continue_1;
int return_1;
char zuhefu_opter[4]={'+','-','*','/'};
int zuhefu_h;
int zuhefu_i;
int zuhefu_j;
char zuhefu_op[3];
int zuhefu_khzh[8][3]={{0,0,0},{0,0,1},{0,1,0},{1,0,0},{0,1,1},{1,0,1},{1,1,0},{1,1,1}};
int zuhefu_k;
int switch_1;
int break_1$;
int nm_1$;
int zuhefu_1_m;
int zuhefu_1_n;
continue_1=0;
return_1=0;
zuhefu_h=0;
while(zuhefu_h<4){zuhefu_i=0;
while(zuhefu_i<4){zuhefu_j=0;
while(zuhefu_j<4){zuhefu_op[0]=zuhefu_opter[zuhefu_h];
zuhefu_op[1]=zuhefu_opter[zuhefu_i];
zuhefu_op[2]=zuhefu_opter[zuhefu_j];
zuhefu_k=0;
while(zuhefu_k<=3){break_1$=0;
switch_1=0;
nm_1$=zuhefu_k;
if ((nm_1$==0) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan(0,zuhe,zuhefu_op,zuhefu_khzh[0],zuhefu_khzh[0]);
break_1$=1;
}
;
if ((nm_1$==1) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
zuhefu_1_m=1;
while(zuhefu_1_m<=3){continue_1=0;
zuhefu_1_n=1;
while(zuhefu_1_n<=3){continue_1=0;
if (((((zuhefu_1_m==2) && (zuhefu_1_n==3)) || ((zuhefu_1_m==1) && (zuhefu_1_n==2))) || ((zuhefu_1_m==3) && (zuhefu_1_n==1))) || ((zuhefu_1_m==1) && (zuhefu_1_n==3))){continue_1=1;
zuhefu_1_n=(zuhefu_1_n)+(1);
}
;
if (continue_1==0){yunsuan(0,zuhe,zuhefu_op,zuhefu_khzh[zuhefu_1_m],zuhefu_khzh[zuhefu_1_n]);
zuhefu_1_n=(zuhefu_1_n)+(1);
}
;
}
;
continue_1=0;
zuhefu_1_m=(zuhefu_1_m)+(1);
}
;
break_1$=1;
}
;
if ((nm_1$==2) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan(0,zuhe,zuhefu_op,zuhefu_khzh[5],zuhefu_khzh[5]);
break_1$=1;
}
;
if ((nm_1$==3) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
break_1$=1;
}
;
zuhefu_k=(zuhefu_k)+(1);
}
;
zuhefu_j=(zuhefu_j)+(1);
}
;
zuhefu_i=(zuhefu_i)+(1);
}
;
zuhefu_h=(zuhefu_h)+(1);
}
;
return_1=1;
RValue=1;
return RValue;
}
int  yunsuan(int $$Ext,float s[],char p[],int k0[],int k1[]){
int  RValue;
int return_1;
float yunsuan_suanshi0[13]={'0'};
int yunsuan_r;
int yunsuan_q;
int switch_1;
int break_1$;
int nm_2$;
float *yunsuan_suanshi;
int yunsuan_i;
float yunsuan_f;
float yunsuan_sum0;
int yunsuan_10_t;
return_1=0;
yunsuan_r=0;
yunsuan_q=0;
while(yunsuan_q<13){break_1$=0;
switch_1=0;
nm_2$=yunsuan_q;
if ((nm_2$==0) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (k0[0]==1){yunsuan_suanshi0[yunsuan_r]='(';
yunsuan_r=(yunsuan_r)+(1);
}
;
break_1$=1;
}
;
if ((nm_2$==1) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=s[0];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==2) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=p[0];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==3) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (k0[1]==1){yunsuan_suanshi0[yunsuan_r]='(';
yunsuan_r=(yunsuan_r)+(1);
}
;
break_1$=1;
}
;
if ((nm_2$==4) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=s[1];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==5) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (k1[0]==1){yunsuan_suanshi0[yunsuan_r]=')';
yunsuan_r=(yunsuan_r)+(1);
}
;
break_1$=1;
}
;
if ((nm_2$==6) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=p[1];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==7) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (k0[2]==1){yunsuan_suanshi0[yunsuan_r]='(';
yunsuan_r=(yunsuan_r)+(1);
}
;
break_1$=1;
}
;
if ((nm_2$==8) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=s[2];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==9) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (k1[1]==1){yunsuan_suanshi0[yunsuan_r]=')';
yunsuan_r=(yunsuan_r)+(1);
}
;
break_1$=1;
}
;
if ((nm_2$==10) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=p[2];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==11) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
yunsuan_suanshi0[yunsuan_r]=s[3];
yunsuan_r=(yunsuan_r)+(1);
break_1$=1;
}
;
if ((nm_2$==12) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (k1[2]==1){yunsuan_suanshi0[yunsuan_r]=')';
yunsuan_r=(yunsuan_r)+(1);
}
;
break_1$=1;
}
;
yunsuan_q=(yunsuan_q)+(1);
}
;
yunsuan_i=0;
yunsuan_suanshi=fsz(0,yunsuan_r);
while(yunsuan_i<yunsuan_r){yunsuan_suanshi[yunsuan_i]=yunsuan_suanshi0[yunsuan_i];
yunsuan_i=(yunsuan_i)+(1);
}
;
yunsuan_f=hefa(0,yunsuan_suanshi,yunsuan_r);
if (yunsuan_f==0){return_1=1;
RValue=0;
}
;
if (return_1==0){yunsuan_f=jianjie(0,yunsuan_suanshi,yunsuan_r);
if (yunsuan_f==0){return_1=1;
RValue=0;
}
;
if (return_1==0){yunsuan_sum0=qiujie(0,yunsuan_suanshi,yunsuan_r);
if (yunsuan_sum0==24){fuhe=(fuhe)+(1);
yunsuan_10_t=0;
while(yunsuan_10_t<yunsuan_r){if (yunsuan_suanshi[yunsuan_10_t]>10){printf("%c",(char)(yunsuan_suanshi[yunsuan_10_t]));
} else {printf("%p",yunsuan_suanshi[yunsuan_10_t]);
printf("");
}
;
yunsuan_10_t=(yunsuan_10_t)+(1);
}
;
printf("=");
printf("%f",yunsuan_sum0);
printf("\n");
}
;
free(yunsuan_suanshi);
return_1=1;
RValue=1;
}
;
}
;
return RValue;
}
int  hefa(int $$Ext,float suanshi[],int r){
int  RValue;
int break_1;
int return_1;
float *hefa_p;
float *hefa_q;
int hefa_flag;
break_1=0;
return_1=0;
hefa_p=&suanshi[0];
hefa_q=&suanshi[1];
hefa_flag=1;
break_1=0;
while((break_1==0) && (1)){if (*(hefa_p)==40){if ((*(hefa_q)>=0) && (*(hefa_q)<=9)){hefa_p=(hefa_p)+(1);
hefa_q=(hefa_q)+(1);
} else {hefa_flag=0;
break_1=1;
}
;
}
;
if (break_1==0){if ((*(hefa_p)>=0) && (*(hefa_p)<=9)){if ((hefa_p)-(suanshi)>=(r)-(1)){break_1=1;
}
;
if (break_1==0){if ((*(hefa_q)==')') || ((*(hefa_q)>41) && (*(hefa_q)<48))){hefa_p=(hefa_p)+(1);
hefa_q=(hefa_q)+(1);
} else {hefa_flag=0;
break_1=1;
}
;
}
;
}
;
if (break_1==0){if (*(hefa_p)==41){if ((hefa_p)-(suanshi)>=(r)-(1)){break_1=1;
}
;
if (break_1==0){if ((*(hefa_q)>41) && (*(hefa_q)<48)){hefa_p=(hefa_p)+(1);
hefa_q=(hefa_q)+(1);
} else {hefa_flag=0;
break_1=1;
}
;
}
;
}
;
if (break_1==0){if ((*(hefa_p)>41) && (*(hefa_p)<48)){if ((*(hefa_q)==40) || ((*(hefa_q)>=0) && (*(hefa_q)<=9))){hefa_p=(hefa_p)+(1);
hefa_q=(hefa_q)+(1);
} else {hefa_flag=0;
break_1=1;
}
;
}
;
}
;
}
;
}
;
}
;
break_1=0;
return_1=1;
RValue=hefa_flag;
return RValue;
}
float  jianjie(int $$Ext,float suanshi[],int r){
float  RValue;
int continue_1;
int return_1;
float jianjie_re;
float *jianjie_p,*jianjie_q;
int jianjie_k;
int jianjie_k1;
float jianjie_r0;
float jianjie_r2;
float jianjie_r1;
int jianjie_r3;
int jianjie_i;
continue_1=0;
return_1=0;
jianjie_re=1;
jianjie_q=&jianjie_re;
jianjie_p=&jianjie_re;
jianjie_k=0;
jianjie_k1=0;
jianjie_r0=0;
jianjie_r2=1;
jianjie_r1=0;
jianjie_r3=0;
continue_1=0;
jianjie_i=0;
while((return_1==0) && (jianjie_i<r)){continue_1=0;
if (suanshi[jianjie_i]=='('){jianjie_k=(jianjie_k)+(1);
if ((*(jianjie_p)!='(') || (jianjie_k==1)){jianjie_p=&suanshi[jianjie_i];
if (jianjie_i!=0){if ((*((jianjie_p)-(1))=='+') || (*((jianjie_p)-(1))=='-')){jianjie_r0=1;
} else {if ((*((jianjie_p)-(1))=='*') || (*((jianjie_p)-(1))=='/')){jianjie_r0=2;
}
;
}
;
if (*((jianjie_p)-(1))=='-'){jianjie_r0=(jianjie_r0)+(100);
} else {if (*((jianjie_p)-(1))=='/'){jianjie_r0=(jianjie_r0)+(1000);
}
;
}
;
}
;
}
;
} else {if (suanshi[jianjie_i]==')'){if (jianjie_k==1){jianjie_k=(jianjie_k)-(1);
jianjie_q=&suanshi[jianjie_i];
if (jianjie_i!=(r)-(1)){if ((*((jianjie_q)+(1))=='+') || (*((jianjie_q)+(1))=='-')){jianjie_r1=1;
} else {if ((*((jianjie_q)+(1))=='*') || (*((jianjie_q)+(1))=='/')){jianjie_r1=2;
}
;
}
;
}
;
jianjie_re=jianjie(0,(jianjie_p)+(1),((jianjie_q)-(jianjie_p))-(1));
if ((int)(jianjie_r0/ 100)>=1){if (((int)(jianjie_r0/ 100)==1) && ((int)(jianjie_re/ 100)==1)){continue_1=1;
jianjie_i=(jianjie_i)+(1);
} else {if (((int)(jianjie_r0/ 1000)==1) && ((int)(jianjie_re/ 1000)==1)){continue_1=1;
jianjie_i=(jianjie_i)+(1);
}
;
}
;
}
;
if (continue_1==0){if ((int)(jianjie_re/ 100)==1){jianjie_re=(jianjie_re)-(100);
}
;
if ((int)(jianjie_re/ 1000)==1){jianjie_re=(jianjie_re)-(1000);
}
;
if ((int)(jianjie_r0/ 100)==1){jianjie_r0=(jianjie_r0)-(100);
} else {if ((int)(jianjie_r0/ 1000)==1){jianjie_r0=(jianjie_r0)-(1000);
}
;
}
;
if (jianjie_re==0){return_1=1;
RValue=0;
}
;
if (return_1==0){if ((jianjie_re>=jianjie_r0) && (jianjie_re>=jianjie_r1)){return_1=1;
RValue=0;
}
;
}
;
}
;
} else {jianjie_k=(jianjie_k)-(1);
}
;
} else {if (jianjie_k==0){if ((suanshi[jianjie_i]=='+') || (suanshi[jianjie_i]=='-')){jianjie_r2=((jianjie_r2*jianjie_k1)+(1))/ ((jianjie_k1)+(1));
jianjie_k1=(jianjie_k1)+(1);
jianjie_r3=(jianjie_r3/ 10)+(1);
} else {if ((suanshi[jianjie_i]=='*') || (suanshi[jianjie_i]=='/')){jianjie_r2=((jianjie_r2*jianjie_k1)+(2))/ ((jianjie_k1)+(1));
jianjie_k1=(jianjie_k1)+(1);
jianjie_r3=(jianjie_r3%10)+(10);
}
;
}
;
}
;
}
;
}
;
if (continue_1==0){if (return_1==0){jianjie_i=(jianjie_i)+(1);
}
;
}
;
}
;
continue_1=0;
if (return_1==0){if (jianjie_r3%10==1){jianjie_r2=(jianjie_r2)+(100);
}
;
if (jianjie_r3/ 10==1){jianjie_r2=(jianjie_r2)+(1000);
}
;
return_1=1;
RValue=jianjie_r2;
}
;
return RValue;
}
float  qiujie(int $$Ext,float suanshi[],int n){
float  RValue;
int continue_1;
int return_1;
float qiujie_1_a;
int switch_1;
int break_1$;
int nm_3$;
float qiujie_pq;
float *qiujie_p;
float *qiujie_q;
int qiujie_k;
float qiujie_suanshi1[8]={'0'};
int qiujie_s;
float qiujie_sum;
int qiujie_m;
int nm_4$;
int qiujie_9_10_11_ws;
int qiujie_9_10_11_w;
int qiujie_9_10_17_ws;
int qiujie_9_10_17_w;
continue_1=0;
return_1=0;
if (n==3){qiujie_1_a=0;
break_1$=0;
switch_1=0;
nm_3$=(char)(suanshi[1]);
if ((nm_3$=='+') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
return_1=1;
RValue=(suanshi[0])+(suanshi[2]);
}
;
if ((nm_3$=='-') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
return_1=1;
RValue=(suanshi[0])-(suanshi[2]);
}
;
if ((nm_3$=='*') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
return_1=1;
RValue=suanshi[0]*suanshi[2];
}
;
if ((nm_3$=='/') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
return_1=1;
RValue=suanshi[0]/ suanshi[2];
}
;
}
;
if (return_1==0){qiujie_pq='0';
qiujie_p=&qiujie_pq;
qiujie_q=&qiujie_pq;
qiujie_k=0;
qiujie_s=0;
qiujie_sum=0;
continue_1=0;
qiujie_m=0;
while(qiujie_m<n){continue_1=0;
if (suanshi[qiujie_m]=='('){qiujie_k=(qiujie_k)+(1);
if (*(qiujie_p)!='('){qiujie_p=&suanshi[qiujie_m];
}
;
continue_1=1;
qiujie_m=(qiujie_m)+(1);
}
;
if (continue_1==0){if (suanshi[qiujie_m]==')'){if (qiujie_k==1){qiujie_k=(qiujie_k)-(1);
qiujie_q=&suanshi[qiujie_m];
qiujie_suanshi1[qiujie_s]=qiujie(0,(qiujie_p)+(1),((qiujie_q)-(qiujie_p))-(1));
qiujie_s=(qiujie_s)+(1);
qiujie_p=&qiujie_pq;
qiujie_q=&qiujie_pq;
} else {qiujie_k=(qiujie_k)-(1);
}
;
continue_1=1;
qiujie_m=(qiujie_m)+(1);
}
;
if (continue_1==0){if (qiujie_k==0){qiujie_suanshi1[qiujie_s]=suanshi[qiujie_m];
qiujie_s=(qiujie_s)+(1);
continue_1=1;
qiujie_m=(qiujie_m)+(1);
}
;
if (continue_1==0){qiujie_m=(qiujie_m)+(1);
}
;
}
;
}
;
}
;
continue_1=0;
if (qiujie_s==3){qiujie_sum=qiujie(0,qiujie_suanshi1,qiujie_s);
} else {qiujie_p=&qiujie_suanshi1[0];
qiujie_q=&qiujie_suanshi1[2];
qiujie_m=0;
while(qiujie_m<((qiujie_s)-(1))/ 2){break_1$=0;
switch_1=0;
nm_4$=(char)(qiujie_suanshi1[(2*qiujie_m)+(1)]);
if ((nm_4$=='+') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if ((((qiujie_s)-(1))/ 2!=1) && ((qiujie_suanshi1[(2*((qiujie_m)+(1)))+(1)]=='*') || (qiujie_suanshi1[(2*((qiujie_m)+(1)))+(1)]=='/'))){qiujie_9_10_11_ws=(&qiujie_suanshi1[(qiujie_s)-(1)])-(&qiujie_suanshi1[2*((qiujie_m)+(1))]);
*(qiujie_q)=qiujie(0,(qiujie_p)+(2),3);
qiujie_9_10_11_w=1;
while(qiujie_9_10_11_w<=qiujie_9_10_11_ws){if ((((qiujie_q)+(qiujie_9_10_11_w))+(2))-((qiujie_q)+(qiujie_9_10_11_ws))>0){*((qiujie_q)+(qiujie_9_10_11_w))=-1;
} else {*((qiujie_q)+(qiujie_9_10_11_w))=*(((qiujie_q)+(qiujie_9_10_11_w))+(2));
}
;
qiujie_9_10_11_w=(qiujie_9_10_11_w)+(1);
}
;
qiujie_s=(qiujie_s)-(2);
qiujie_m=(qiujie_m)-(1);
} else {if (qiujie_m==0){qiujie_sum=((qiujie_sum)+(*(qiujie_p)))+(*(qiujie_q));
} else {qiujie_sum=(qiujie_sum)+(*(qiujie_q));
}
;
qiujie_p=(qiujie_p)+(2);
qiujie_q=(qiujie_q)+(2);
}
;
break_1$=1;
}
;
if ((nm_4$=='-') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if ((((qiujie_s)-(1))/ 2!=1) && ((qiujie_suanshi1[(2*((qiujie_m)+(1)))+(1)]=='*') || (qiujie_suanshi1[(2*((qiujie_m)+(1)))+(1)]=='/'))){qiujie_9_10_17_ws=(&qiujie_suanshi1[(qiujie_s)-(1)])-(&qiujie_suanshi1[2*((qiujie_m)+(1))]);
*(qiujie_q)=qiujie(0,(qiujie_p)+(2),3);
qiujie_9_10_17_w=1;
while(qiujie_9_10_17_w<=qiujie_9_10_17_ws){if ((((qiujie_q)+(qiujie_9_10_17_w))+(2))-((qiujie_q)+(qiujie_9_10_17_ws))>0){*((qiujie_q)+(qiujie_9_10_17_w))=-1;
} else {*((qiujie_q)+(qiujie_9_10_17_w))=*(((qiujie_q)+(qiujie_9_10_17_w))+(2));
}
;
qiujie_9_10_17_w=(qiujie_9_10_17_w)+(1);
}
;
qiujie_s=(qiujie_s)-(2);
qiujie_m=-1;
} else {if (qiujie_m==0){qiujie_sum=((qiujie_sum)+(*(qiujie_p)))-(*(qiujie_q));
} else {qiujie_sum=(qiujie_sum)-(*(qiujie_q));
}
;
qiujie_p=(qiujie_p)+(2);
qiujie_q=(qiujie_q)+(2);
}
;
break_1$=1;
}
;
if ((nm_4$=='*') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (qiujie_m==0){qiujie_sum=(qiujie_sum)+(*(qiujie_p)**(qiujie_q));
} else {qiujie_sum=qiujie_sum**(qiujie_q);
}
;
qiujie_p=(qiujie_p)+(2);
qiujie_q=(qiujie_q)+(2);
break_1$=1;
}
;
if ((nm_4$=='/') || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
if (qiujie_m==0){qiujie_sum=(qiujie_sum)+(*(qiujie_p)/ *(qiujie_q));
} else {qiujie_sum=qiujie_sum/ *(qiujie_q);
}
;
qiujie_p=(qiujie_p)+(2);
qiujie_q=(qiujie_q)+(2);
break_1$=1;
}
;
qiujie_m=(qiujie_m)+(1);
}
;
}
;
return_1=1;
RValue=qiujie_sum;
}
;
return RValue;
}

int main() {
sum=0;
fuhe=0;
main_1(0);
>>>>>>> aa49ebebcbec50958e54d39fd795b0fd2b217d13
return 0;
}
