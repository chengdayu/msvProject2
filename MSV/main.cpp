#include <iostream>
#include"function.h"
#include"SyntaxNode.h"
#include"SyntaxTree.h"
#include"MSVCompiler.h"
using namespace std;

extern FILE *yyin;
extern FILE *yyout;
extern int yyparse();

extern int g_nErrors;

extern CSyntaxTree* g_tree;

extern CSyntaxNode *g_syntaxTree;

//add by yubin 2015-4-13,函数语法树
extern CSyntaxNode * function_tree;

bool g_InFunc=0;

int main()
{
	yyin = fopen(InPath, "r");//以读的方式打开InPath指向的输入文件

	if (yyin == NULL)
	{
		yyin = stdin;//如果没有可用文件，则令词法语法分析的输入指向键盘
	}


	//每次语法分析时都要将classarray清空，否则越来越大
	//	classarray.RemoveAll();

	yyparse(); // Call the parser  调用词法，语法分析器，分别生成程序语法树（由g_syntaxTree指向它）和性质语法树（由g_propositionTree指向它）

	if (g_nErrors) //如果有错误,即错误数不为0
	{
		errorSummary();
		//add by yubin 2015/4/17,如果有语法错误，直接输出结果文件，并且返回
		system("TYPE _MSVOutput.cpp");
		system("pause");
		return 0;
	}
	CMSV *Cmplr = new CMSV();


	fout << "#include<stdio.h>" << endl;
	fout << "#include<stdlib.h>" << endl;
	fout << "int $$state_num=0;" << endl;

	//先对frame中的程序进行预处理
	Cmplr->Simulation();
	string mainFunc = Cmplr->PrintIRTree();

	//add by yubin 2015-4-13,预处理函数语法树
	string func = "";
	if (function_tree != NULL)
	{
		g_InFunc = 1;
		Cmplr->SetProgram(function_tree);//转正式的msvl程序之前，先转函数定义的树，转好之后的IR结构，仍然保存在function_tree上
		Cmplr->Pre();
		Cmplr->SetIRTree(function_tree);
		func = Cmplr->PrintIRTree();
		g_InFunc = 0;
	}

	//add by yubin 2015-4-13,函数语法树
	/*if (function_tree != NULL)
	{			
		g_InFunc = 1;
		Cmplr->SetProgram(function_tree);//转正式的msvl程序之前，先转函数定义的树，转好之后的IR结构，仍然保存在function_tree上
		Cmplr->Pre();
		//fout << "IRFunctionTree: ";
		Cmplr->SetIRTree(function_tree);
		Cmplr->PrintIRTree();
		Cmplr->SetProgram(g_syntaxTree);//将正式的msvl程序转成IR树
		g_InFunc = 0;
	}*/

	//输出全局符号表
	Cmplr->PrintGlbVar();

	//输出函数定义
	fout << func << endl;

	//输出main函数
	fout << "int main() {" << endl;
	fout << mainFunc << ";" << endl;
	fout << "return 0;" << endl << "}" << endl;

	///add by yubin 2015-4-3
	//IR *m_IR=new IR();
	//m_IR->Trslt2IR(m_IRTree);

	system("clang++.exe _MSVOutput.cpp -o result.exe");
	system("clang++.exe -emit-llvm _MSVOutput.cpp -S -o result.ll");
	system("TYPE result.ll");

	cout << endl;
	cout << "result:" << endl;
	system("result.exe");
    cout << endl;

	system("pause");

	//fout << "IRTree: ";
	//Cmplr->PrintIRTree();
	//fout << endl;

	return 0;
}
