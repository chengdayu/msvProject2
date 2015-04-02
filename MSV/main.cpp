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

	yyin = fopen(InPath, "r");//�Զ��ķ�ʽ��InPathָ��������ļ�

	if (yyin == NULL)
	{
		yyin = stdin;//���û�п����ļ�������ʷ��﷨����������ָ�����
	}


	//ÿ���﷨����ʱ��Ҫ��classarray��գ�����Խ��Խ��
	//	classarray.RemoveAll();

	yyparse(); // Call the parser  ���ôʷ����﷨���������ֱ����ɳ����﷨������g_syntaxTreeָ�������������﷨������g_propositionTreeָ������

	if (g_nErrors) //����д���,����������Ϊ0
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
