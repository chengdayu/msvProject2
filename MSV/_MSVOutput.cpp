#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int $$state_num=0;
int $$len_i=0;
int $$flag[10] = { 0 };
int $$length = 0, $$current = -1,$$firstTime=0;
void $$changeState()
{if ($$current == $$length){
$$length++; $$flag[$$length - 1] = 0;}
$$current++; }
struct S_TAG_DECODE_00_FF;
struct S_TAG_RECOG__00_FF;

struct S_TAG_DECODE_00_FF{
unsigned char *decodeAddr;
int decodeSt;
int delelteIdx[32];
int blkNum;
int blkIdx;
int blkLen[1024];
int winPos;
};
struct S_TAG_RECOG__00_FF{
int index;
int state;
int counter;
int num;
};

S_TAG_DECODE_00_FF mDecodeVar;
char *  memcpy(int $$Ext,unsigned char *des,unsigned char * src,int num);
void  DecodeRec00(int $$Ext,int deleteIdx[],S_TAG_RECOG__00_FF* recog,unsigned char * buffer);
void  DecodeRecFF(int $$Ext,int deleteIdx[],S_TAG_RECOG__00_FF* recog,unsigned char * buffer);
int  DecodeFindIndexDelete(int $$Ext,unsigned char buffer[],int len$,int deleteIdx[]);
void  DecodeRemoveBytes(int $$Ext,unsigned char buffer[],int bLen,int deleteIdx[],int dLen);
int  DecodeWinMerge(int $$Ext,unsigned char *startAddr,int blkSize,int blkNum,int blkLen[]);
void  DecodeRemoveBlk(int $$Ext,unsigned char *startAddr,int winPos,int blkNum,int winRemoveNum);
int  DecodeLenUpdate(int $$Ext,int lastIdx,int winPos,int winSize,int blkNum,int newLen[]);
int  DecodeRemoveByteBlk(int $$Ext,unsigned char buffer[],int len$,int delelteIdx[]);
int  DecodeDataMerge(int $$Ext,unsigned char *startAddr,int winPos,int * blkNum,int newLen[]);
void  DecodeInitial(int $$Ext,unsigned char *addr);
void  DecodeGetState(int $$Ext,int *st,int * blockNum,int * lastLen);
void  DecodeEntry(int $$Ext);
void  FillBuffer(int $$Ext,unsigned char buffer[],int len$);
void  TestCmdE2DataMerge(int $$Ext);
char *  memcpy(int $$Ext,unsigned char *des,unsigned char * src,int num){
char *  RValue=0;
int i;
i=0;
while(i<num){des[i]=src[i];
i=(i)+(1);
}
;
return RValue;
}
void  DecodeRec00(int $$Ext,int deleteIdx[],S_TAG_RECOG__00_FF* recog,unsigned char * buffer){
int return_1;
unsigned char DecodeRec00_ch;
unsigned char DecodeRec00_ch1;
return_1=0;
DecodeRec00_ch=buffer[recog->index];
if (DecodeRec00_ch==0){recog->counter=(recog->counter)+(1);
if (recog->counter==20){DecodeRec00_ch1=buffer[(recog->index)+(1)];
if (DecodeRec00_ch1==170){deleteIdx[recog->num]=(recog->index)+(1);
recog->num=(recog->num)+(1);
recog->index=(recog->index)+(1);
}
;
recog->counter=0;
recog->state=13107;
}
;
} else {if (DecodeRec00_ch==255){recog->counter=1;
recog->state=43690;
} else {recog->counter=0;
recog->state=13107;
}
;
}
;
return_1=1;
}
void  DecodeRecFF(int $$Ext,int deleteIdx[],S_TAG_RECOG__00_FF* recog,unsigned char * buffer){
int return_1;
unsigned char DecodeRecFF_ch;
unsigned char DecodeRecFF_ch1;
return_1=0;
DecodeRecFF_ch=buffer[recog->index];
if (DecodeRecFF_ch==0){recog->counter=1;
recog->state=21845;
} else {if (DecodeRecFF_ch==255){recog->counter=(recog->counter)+(1);
if (recog->counter==20){DecodeRecFF_ch1=buffer[(recog->index)+(1)];
if (DecodeRecFF_ch1==85){deleteIdx[recog->num]=(recog->index)+(1);
recog->num=(recog->num)+(1);
recog->index=(recog->index)+(1);
}
;
recog->counter=0;
recog->state=13107;
}
;
} else {recog->counter=0;
recog->state=13107;
}
;
}
;
return_1=1;
}
int  DecodeFindIndexDelete(int $$Ext,unsigned char buffer[],int len$,int deleteIdx[]){
int  RValue=0;
int return_1;
S_TAG_RECOG__00_FF DecodeFindIndexDelete_recog;
return_1=0;
DecodeFindIndexDelete_recog.state=13107;
DecodeFindIndexDelete_recog.counter=0;
DecodeFindIndexDelete_recog.num=0;
DecodeFindIndexDelete_recog.index=0;
while(DecodeFindIndexDelete_recog.index<(len$)-(1)){if (DecodeFindIndexDelete_recog.state==21845){DecodeRec00(0,deleteIdx,&DecodeFindIndexDelete_recog,buffer);
} else {if (DecodeFindIndexDelete_recog.state==43690){DecodeRecFF(0,deleteIdx,&DecodeFindIndexDelete_recog,buffer);
} else {if (buffer[DecodeFindIndexDelete_recog.index]==0){DecodeFindIndexDelete_recog.counter=1;
DecodeFindIndexDelete_recog.state=21845;
} else {if (buffer[DecodeFindIndexDelete_recog.index]==255){DecodeFindIndexDelete_recog.counter=1;
DecodeFindIndexDelete_recog.state=43690;
}
;
}
;
}
;
}
;
DecodeFindIndexDelete_recog.index=(DecodeFindIndexDelete_recog.index)+(1);
}
;
return_1=1;
RValue=DecodeFindIndexDelete_recog.num;
return RValue;
}
void  DecodeRemoveBytes(int $$Ext,unsigned char buffer[],int bLen,int deleteIdx[],int dLen){
int return_1;
int DecodeRemoveBytes_i;
int DecodeRemoveBytes_j;
return_1=0;
DecodeRemoveBytes_i=0;
while(DecodeRemoveBytes_i<(dLen)-(1)){DecodeRemoveBytes_j=(deleteIdx[DecodeRemoveBytes_i])+(1);
while(DecodeRemoveBytes_j<deleteIdx[(DecodeRemoveBytes_i)+(1)]){buffer[((DecodeRemoveBytes_j)-(DecodeRemoveBytes_i))-(1)]=buffer[DecodeRemoveBytes_j];
DecodeRemoveBytes_j=(DecodeRemoveBytes_j)+(1);
}
;
DecodeRemoveBytes_i=(DecodeRemoveBytes_i)+(1);
}
;
if (dLen>0){DecodeRemoveBytes_j=(deleteIdx[(dLen)-(1)])+(1);
while(DecodeRemoveBytes_j<bLen){buffer[(DecodeRemoveBytes_j)-(dLen)]=buffer[DecodeRemoveBytes_j];
DecodeRemoveBytes_j=(DecodeRemoveBytes_j)+(1);
}
;
}
;
return_1=1;
}
int  DecodeWinMerge(int $$Ext,unsigned char *startAddr,int blkSize,int blkNum,int blkLen[]){
int  RValue=0;
int return_1;
int DecodeWinMerge_i;
int DecodeWinMerge_j;
unsigned char DecodeWinMerge_tmpBuffer[40];
int DecodeWinMerge_tmpRemoveIdx[16];
int DecodeWinMerge_tmpNum;
int DecodeWinMerge_shiftLen;
int DecodeWinMerge_removeLen;
int DecodeWinMerge_idxTarget;
return_1=0;
DecodeWinMerge_tmpRemoveIdx[0]=-1;
DecodeWinMerge_i=1;
while(DecodeWinMerge_i<blkNum){DecodeWinMerge_j=0;
while(DecodeWinMerge_j<20){DecodeWinMerge_tmpBuffer[DecodeWinMerge_j]=startAddr[(((((DecodeWinMerge_i)-(1))*blkSize)+(blkLen[(DecodeWinMerge_i)-(1)]))-(20))+(DecodeWinMerge_j)];
DecodeWinMerge_tmpBuffer[(20)+(DecodeWinMerge_j)]=startAddr[(DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j)];
DecodeWinMerge_j=(DecodeWinMerge_j)+(1);
}
;
DecodeWinMerge_tmpNum=DecodeFindIndexDelete(0,DecodeWinMerge_tmpBuffer,40,&DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]);
if (DecodeWinMerge_tmpNum==0){DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]=-1;
} else {DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]=(DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i])-(20);
}
;
DecodeWinMerge_i=(DecodeWinMerge_i)+(1);
}
;
DecodeWinMerge_shiftLen=0;
DecodeWinMerge_removeLen=0;
DecodeWinMerge_idxTarget=-1;
DecodeWinMerge_i=1;
while(DecodeWinMerge_i<blkNum){DecodeWinMerge_shiftLen=(DecodeWinMerge_shiftLen)+((blkSize)-(blkLen[(DecodeWinMerge_i)-(1)]));
if (DecodeWinMerge_tmpRemoveIdx[(DecodeWinMerge_i)-(1)]>=0){DecodeWinMerge_removeLen=(DecodeWinMerge_removeLen)+(1);
}
;
if (DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]<0){DecodeWinMerge_j=0;
while(DecodeWinMerge_j<blkLen[DecodeWinMerge_i]){DecodeWinMerge_idxTarget=(((DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j))-(DecodeWinMerge_removeLen))-(DecodeWinMerge_shiftLen);
startAddr[DecodeWinMerge_idxTarget]=startAddr[(DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j)];
DecodeWinMerge_j=(DecodeWinMerge_j)+(1);
}
;
} else {DecodeWinMerge_j=0;
while(DecodeWinMerge_j<DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i]){DecodeWinMerge_idxTarget=(((DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j))-(DecodeWinMerge_removeLen))-(DecodeWinMerge_shiftLen);
startAddr[DecodeWinMerge_idxTarget]=startAddr[(DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j)];
DecodeWinMerge_j=(DecodeWinMerge_j)+(1);
}
;
DecodeWinMerge_j=(DecodeWinMerge_tmpRemoveIdx[DecodeWinMerge_i])+(1);
while(DecodeWinMerge_j<blkLen[DecodeWinMerge_i]){DecodeWinMerge_idxTarget=((((DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j))-(DecodeWinMerge_removeLen))-(DecodeWinMerge_shiftLen))-(1);
startAddr[DecodeWinMerge_idxTarget]=startAddr[(DecodeWinMerge_i*blkSize)+(DecodeWinMerge_j)];
DecodeWinMerge_j=(DecodeWinMerge_j)+(1);
}
;
}
;
DecodeWinMerge_i=(DecodeWinMerge_i)+(1);
}
;
return_1=1;
RValue=(DecodeWinMerge_idxTarget)+(1);
return RValue;
}
void  DecodeRemoveBlk(int $$Ext,unsigned char *startAddr,int winPos,int blkNum,int winRemoveNum){
int return_1;
int DecodeRemoveBlk_i;
return_1=0;
DecodeRemoveBlk_i=(winPos)+(4);
while(DecodeRemoveBlk_i<blkNum){memcpy(0,&startAddr[((DecodeRemoveBlk_i)-(winRemoveNum))*512],&startAddr[DecodeRemoveBlk_i*512],512);
DecodeRemoveBlk_i=(DecodeRemoveBlk_i)+(1);
}
;
return_1=1;
}
int  DecodeLenUpdate(int $$Ext,int lastIdx,int winPos,int winSize,int blkNum,int newLen[]){
int  RValue=0;
int break_1;
int return_1;
int DecodeLenUpdate_i;
int DecodeLenUpdate_winRemoveNum;
break_1=0;
return_1=0;
break_1=0;
DecodeLenUpdate_i=0;
while((break_1==0) && (DecodeLenUpdate_i<winSize)){if (lastIdx>512){newLen[(winPos)+(DecodeLenUpdate_i)]=512;
lastIdx=(lastIdx)-(512);
} else {newLen[(winPos)+(DecodeLenUpdate_i)]=lastIdx;
break_1=1;
}
;
if (break_1==0){DecodeLenUpdate_i=(DecodeLenUpdate_i)+(1);
}
;
}
;
break_1=0;
DecodeLenUpdate_winRemoveNum=((winSize)-(DecodeLenUpdate_i))-(1);
if (DecodeLenUpdate_winRemoveNum>0){DecodeLenUpdate_i=(winPos)+(winSize);
while(DecodeLenUpdate_i<blkNum){newLen[(DecodeLenUpdate_i)-(DecodeLenUpdate_winRemoveNum)]=newLen[DecodeLenUpdate_i];
DecodeLenUpdate_i=(DecodeLenUpdate_i)+(1);
}
;
}
;
return_1=1;
RValue=DecodeLenUpdate_winRemoveNum;
return RValue;
}
int  DecodeRemoveByteBlk(int $$Ext,unsigned char buffer[],int len$,int delelteIdx[]){
int  RValue=0;
int return_1;
int DecodeRemoveByteBlk_delCnt;
return_1=0;
DecodeRemoveByteBlk_delCnt=DecodeFindIndexDelete(0,buffer,512,delelteIdx);
DecodeRemoveBytes(0,buffer,len$,delelteIdx,DecodeRemoveByteBlk_delCnt);
return_1=1;
RValue=(len$)-(DecodeRemoveByteBlk_delCnt);
return RValue;
}
int  DecodeDataMerge(int $$Ext,unsigned char *startAddr,int winPos,int * blkNum,int newLen[]){
int  RValue=0;
int return_1;
int DecodeDataMerge_winEleCnt;
int DecodeDataMerge_tmpWinSize;
int DecodeDataMerge_winRemoveNum;
return_1=0;
if ((winPos)+(4)>*(blkNum)){DecodeDataMerge_tmpWinSize=(*(blkNum))-(winPos);
} else {DecodeDataMerge_tmpWinSize=4;
}
;
DecodeDataMerge_winRemoveNum=0;
if (DecodeDataMerge_tmpWinSize>1){DecodeDataMerge_winEleCnt=DecodeWinMerge(0,&startAddr[winPos*512],512,DecodeDataMerge_tmpWinSize,&newLen[winPos]);
DecodeDataMerge_winRemoveNum=DecodeLenUpdate(0,DecodeDataMerge_winEleCnt,winPos,DecodeDataMerge_tmpWinSize,*(blkNum),newLen);
if (DecodeDataMerge_winRemoveNum>0){DecodeRemoveBlk(0,startAddr,winPos,*(blkNum),DecodeDataMerge_winRemoveNum);
*(blkNum)=(*(blkNum))-(DecodeDataMerge_winRemoveNum);
}
;
}
;
return_1=1;
RValue=DecodeDataMerge_winRemoveNum;
return RValue;
}
void  DecodeInitial(int $$Ext,unsigned char *addr){
int return_1;
return_1=0;
mDecodeVar.decodeAddr=addr;
mDecodeVar.decodeSt=13107;
mDecodeVar.blkNum=1024;
mDecodeVar.blkIdx=0;
mDecodeVar.winPos=0;
return_1=1;
}
void  DecodeGetState(int $$Ext,int *st,int * blockNum,int * lastLen){
int return_1;
return_1=0;
*(st)=mDecodeVar.decodeSt;
*(blockNum)=mDecodeVar.blkNum;
*(lastLen)=mDecodeVar.blkLen[(mDecodeVar.blkNum)-(1)];
return_1=1;
}
void  DecodeEntry(int $$Ext){
int return_1;
int DecodeEntry_i;
int DecodeEntry_winRemoveNum;
int switch_1;
int break_1$;
int nm_1$;
return_1=0;
break_1$=0;
switch_1=0;
nm_1$=mDecodeVar.decodeSt;
if ((nm_1$==13107) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
mDecodeVar.decodeSt=21845;
break_1$=1;
}
;
if ((nm_1$==21845) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
DecodeEntry_i=0;
while((mDecodeVar.blkIdx<mDecodeVar.blkNum) && (DecodeEntry_i<4)){mDecodeVar.blkLen[(mDecodeVar.blkIdx)+(DecodeEntry_i)]=DecodeRemoveByteBlk(0,&mDecodeVar.decodeAddr[((mDecodeVar.blkIdx)+(DecodeEntry_i))*512],512,mDecodeVar.delelteIdx);
DecodeEntry_i=(DecodeEntry_i)+(1);
}
;
if (mDecodeVar.blkIdx>=mDecodeVar.blkNum){mDecodeVar.decodeSt=43690;
} else {mDecodeVar.blkIdx=(mDecodeVar.blkIdx)+(4);
}
;
break_1$=1;
}
;
if ((nm_1$==43690) || (((switch_1==1) && (break_1$==0)) && (return_1==0))){switch_1=1;
DecodeEntry_i=0;
while((mDecodeVar.winPos<mDecodeVar.blkNum) && (DecodeEntry_i<4)){DecodeEntry_winRemoveNum=DecodeDataMerge(0,mDecodeVar.decodeAddr,mDecodeVar.winPos,&mDecodeVar.blkNum,mDecodeVar.blkLen);
mDecodeVar.winPos=(((mDecodeVar.winPos)+(4))-(1))-(DecodeEntry_winRemoveNum);
DecodeEntry_i=(DecodeEntry_i)+(1);
}
;
if (mDecodeVar.winPos>=mDecodeVar.blkNum){mDecodeVar.decodeSt=52428;
}
;
break_1$=1;
}
;
if ((break_1$==0) && (return_1==0)){break_1$=1;
}
;
}
void  FillBuffer(int $$Ext,unsigned char buffer[],int len$){
int return_1;
unsigned char FillBuffer_flg;
int FillBuffer_i;
return_1=0;
FillBuffer_flg=0;
FillBuffer_i=0;
while(FillBuffer_i<len$){if (FillBuffer_flg==0){buffer[FillBuffer_i]=0;
if (((FillBuffer_i)+(1))%21==0){buffer[FillBuffer_i]=170;
FillBuffer_flg=1;
}
;
} else {buffer[FillBuffer_i]=255;
if (((FillBuffer_i)+(1))%21==0){buffer[FillBuffer_i]=85;
FillBuffer_flg=0;
}
;
}
;
FillBuffer_i=(FillBuffer_i)+(1);
}
;
return_1=1;
}
void  TestCmdE2DataMerge(int $$Ext){
int break_1;
int return_1;
int TestCmdE2DataMerge_len08;
unsigned char TestCmdE2DataMerge_buffer[1024][512];
int TestCmdE2DataMerge_blkNum;
int TestCmdE2DataMerge_i;
int TestCmdE2DataMerge_cnt;
int TestCmdE2DataMerge_lastLen;
int TestCmdE2DataMerge_st;
break_1=0;
return_1=0;
TestCmdE2DataMerge_len08=512*1024;
TestCmdE2DataMerge_blkNum=1024;
FillBuffer(0,(unsigned char *)(TestCmdE2DataMerge_buffer),TestCmdE2DataMerge_len08);
DecodeInitial(0,(unsigned char *)(TestCmdE2DataMerge_buffer));
break_1=0;
while((break_1==0) && (1)){DecodeEntry(0);
DecodeGetState(0,&TestCmdE2DataMerge_st,&TestCmdE2DataMerge_blkNum,&TestCmdE2DataMerge_lastLen);
if (TestCmdE2DataMerge_st==52428){break_1=1;
}
;
}
;
break_1=0;
TestCmdE2DataMerge_cnt=(((TestCmdE2DataMerge_blkNum)-(1))*512)+(TestCmdE2DataMerge_lastLen);
TestCmdE2DataMerge_i=0;
while(TestCmdE2DataMerge_i<TestCmdE2DataMerge_cnt){printf("%x",((unsigned char *)(TestCmdE2DataMerge_buffer))[TestCmdE2DataMerge_i]);
if (((TestCmdE2DataMerge_i)+(1))%20==0){printf("\n");
}
;
TestCmdE2DataMerge_i=(TestCmdE2DataMerge_i)+(1);
}
;
printf("%d",TestCmdE2DataMerge_cnt);
printf("\n");
printf("\n");
return_1=1;
}

int main() {
while (true){
while ($$length > 0){
if ($$flag[$$length - 1] == 1){
$$length--;}
else {
$$flag[$$length - 1] = 1; break;}}
if ($$length == 0 && $$flag[0] == 1 || $$current == -1 && $$firstTime==1){break;}
TestCmdE2DataMerge(0);
$$firstTime=1;
$$state_num=0;
printf("\n");
}
return 0;
}
