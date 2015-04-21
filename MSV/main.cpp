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
		return 0;
	}
	CMSV *Cmplr = new CMSV();

	fout << "#include<stdio.h>" << endl;
	fout << "#include<stdlib.h>" << endl;
	fout << "int $$state_num=0;" << endl;

	//add by yubin 2015-4-13,函数语法树
	if (function_tree != NULL)
	{			
		g_InFunc = 1;
		Cmplr->SetProgram(function_tree);//转正式的msvl程序之前，先转函数定义的树，转好之后的IR结构，仍然保存在function_tree上
		Cmplr->Pre();
		//fout << "IRFunctionTree: ";
		Cmplr->SetIRTree(function_tree);
		Cmplr->PrintIRTree();
		Cmplr->SetProgram(g_syntaxTree);//将正式的msvl程序转成IR树
		g_InFunc = 0;
	}

	Cmplr->Simulation();
	system("pause");

	//fout << "IRTree: ";
	//Cmplr->PrintIRTree();
	//fout << endl;

	return 0;
}
