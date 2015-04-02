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
	}

	CMSV *Cmplr = new CMSV();

	Cmplr->Simulation();

	//fout << "IRTree: ";
	//Cmplr->PrintIRTree();
	fout << endl;

	return 0;
}
