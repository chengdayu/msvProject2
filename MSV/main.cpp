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

extern CSyntaxNode * struct_tree;

bool g_InFunc=0;
bool g_InStruct = 0;

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

	//add by yubin 2015/4/27,$$len_i变量与await语句有关
	fout << "int $$len_i=0;" << endl;

	//add by yubin 2015/4/27,下面添加的语句与or有关
	fout << "int $$flag[10] = { 0 };" << endl; //标识符，0代表走if，1代表走else
	fout << "int $$length = 0, $$current = -1,$$firstTime=0;" << endl;//len代表遇到分支的个数，firstTime变量为了标记程序是否执行一遍，如果执行一遍之后，$$current 还是-1，证明没有or分支，程序就可以直接结束了
	//current代表当前是第几个分支

	/*在每个分支也就是每个if...else语句前调用changeState()函数，如果current等于len说明出现新的节点，增加节点，最后将current加1，代表又遇到一个分支*/
	fout << "void $$changeState()" << endl;
	fout << "{if ($$current == $$length){" << endl;
	fout << "$$length++; $$flag[$$length - 1] = 0;}" << endl;
	fout << "$$current++; }"<<endl;


	//先对frame中的程序进行预处理
	Cmplr->Simulate();
	string mainFunc = Cmplr->PrintIRTree();
	/*
	//预处理结构体定义语法树 yangkai
	string structDef = "";
	if (struct_tree != NULL)
	{
		g_InStruct = 1;
		CSyntaxNode* p = struct_tree;
		while (p != NULL)
		{
			structDef += "struct " + p->GetNName() + "{\n";
			structDef += p->GetChild0()->TreeToString();
			structDef += ";\n};\n";
			p = p->GetChild1();
		}
		g_InStruct = 0;
	}

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

	//add by yubin 20y15-4-13,函数语法树
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

	/*
	//输出结构体定义
	fout << structDef << endl;

	//输出全局符号表
	Cmplr->PrintGlbVar();

	///输出函数定义
	Cmplr->PrintFuncDec();

	//输出函数定义
	fout << func << endl;

	//输出main函数
	fout << "int main() {" << endl;
	fout << "while (true){" << endl;
	fout << "while ($$length > 0){" << endl;
	fout << "if ($$flag[$$length - 1] == 1){" << endl; //删除已经遍历过两边的节点
	fout << "$$length--;}"<<endl;
	fout << "else {" << endl;//遍历最后一个左分支的另一个分支
	fout << "$$flag[$$length - 1] = 1; break;}}" << endl;
	fout << "if ($$length == 0 && $$flag[0] == 1 || $$current == -1 && $$firstTime==1){break;}" << endl;


	fout << mainFunc << ";" << endl;
	fout << "$$firstTime=1;" << endl;
	fout << "$$state_num=0;" << endl;
	fout << "printf(\"\\n\");" << endl;
	fout << "}" << endl;
	fout << "return 0;" << endl << "}" << endl;

	///add by yubin 2015-4-3
	//IR *m_IR=new IR();
	//m_IR->Trslt2IR(m_IRTree);
/*
	system("clang++.exe _MSVOutput.cpp -o result.exe");
	system("clang++.exe -emit-llvm _MSVOutput.cpp -S -o result.ll");
//	system("TYPE result.ll");

	cout << endl;
	cout << "result:" << endl;
	system("result.exe");
    cout << endl;
	*/
	system("pause");

	//fout << "IRTree: ";
	//Cmplr->PrintIRTree();
	//fout << endl;

	return 0;
}
