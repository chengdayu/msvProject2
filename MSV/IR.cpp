#include"IR.h"
using namespace llvm;

IR::IR()
{
}

void IR::Trslt2IR(CSyntaxTree *IRTree)
{
	module = new Module("global", getGlobalContext());

	builder = new llvm::IRBuilder<>(module->getContext());

	FunctionType *FuncTypeOfMain = FunctionType::get(IntegerType::get(module->getContext(), 32), false);
	Function *MainFunc = Function::Create(FuncTypeOfMain, Function::ExternalLinkage, "main", module);

	BasicBlock *entrymain = BasicBlock::Create(module->getContext(), "entry", MainFunc, 0);

	builder->SetInsertPoint(entrymain);

	Stmt2IR(IRTree->GetRoot());



}

void IR::Stmt2IR(CSyntaxNode *pTree)
{
	switch (pTree->GetNType())
	{
		case DECLARE_STA: __Declr2IR(pTree);
	}
		

}

void IR::__Declr2IR(CSyntaxNode *pTree)
{
	switch (pTree->GetRType())
	{
		case INTTYPE: __DeclrInt2IR(pTree);
	}


}

void IR::__DeclrInt2IR(CSyntaxNode *pTree)
{

}

/**
* 赋值语句转成对应的IR代码
* @param 传入待分析的语法树
* @return void
*/
///2015-4-7 add by wangmeng 
void IR::__Ass2IR(CSyntaxNode* pTree)
{

}