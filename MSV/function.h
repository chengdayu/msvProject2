#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <iostream>
#include <fstream>
#include <stdarg.h>//add by yubin 2015-4-1,为了识别va_start
using namespace std;
#define InPath "_MSVInput.m"//定义输入文件的路径
#define OutPath "_MSVOutput.txt"//定义输出文件的路径

#define GraphFilePath "_MSVGraph.dot"//定义建模图程序文件路径
#define GraphPath "_MSVGraph.bmp"//定义建模图的文件路径
//#define InPath2 _T("E:\\equal.txt")//定义临时输入文件的路径
//#define CopyPath _T("E:\\ObjectTransfer.txt")//定义备份文件的路径


extern ofstream fout;

void errorSummary();
void error(const char *cFormat, ...);
//char* itoa(int num, char*str, int radix);


#endif // FUNCTION_H_INCLUDED