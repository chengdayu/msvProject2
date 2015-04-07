#include"IR.h"
#include<iostream>
using namespace llvm;
using namespace std;

IR::IR()
{
	m_module = new Module("global", getGlobalContext());
	m_builder=new llvm::IRBuilder<>(m_module->getContext());
}
IR::~IR()
{
	delete m_module;
	delete m_builder;
}
void IR::Trslt2IR(CSyntaxTree *IRTree)
{
	if (IRTree == NULL)
	{
		cout << "In function Trslt2IR, IRTree is NULL" << endl;
		return;
	}

	FunctionType *FuncTypeOfMain = FunctionType::get(IntegerType::get(m_module->getContext(), 32), false);
	Function *MainFunc = Function::Create(FuncTypeOfMain, Function::ExternalLinkage, "main", m_module);

	BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", MainFunc, 0);

	m_builder->SetInsertPoint(entrymain);

	Stmt2IR(IRTree->GetRoot());

	m_module->dump();

}

void IR::Stmt2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function Stmt2IR, IRTree is NULL" << endl;
		return;
	}
	
	switch (pTree->GetNType())
	{
	case DECLARE_STA:
	{
		__Declr2IR(pTree);
		break;
	}
	case CHOP_STA:
	{
		__Chop2IR(pTree);
		break;
	}
	    case ASS_EQU_EXP:
	    {
		    __Ass2IR(pTree);
		    break;
	}
	}
	
		

}

void IR::__Declr2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __Declr2IR, IRTree is NULL" << endl;
		return;
	}
	switch (pTree->GetRType())
	{
	case INTTYPE://如果是int类型
	{
		pTree = pTree->GetChild0();//类型是PARAMETER_EXP
		while (pTree->GetChild0() != NULL)//左孩子不为空表示有变量声明
		{
			__DeclrInt2IR(pTree->GetChild0());//对变量声明进行转换
			if (pTree->GetChild1() != NULL)//右孩子不为空表示还有同类型的变量声明
			{
				pTree = pTree->GetChild1();//获得右孩子
			}
		}
		break;
		
	}
	}


}

///
void IR::__DeclrInt2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __DeclrInt2IR, IRTree is NULL" << endl;
		return;
	}
	AllocaInst *allocDeclrInt = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), ConstantInt::get(m_module->getContext(), APInt(32, 4)), pTree->GetNName());
	allocDeclrInt->setAlignment(4);
	m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrInt));
}

///
void IR::__Chop2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __Chop2IR, IRTree is NULL" << endl;
		return;
	}
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "The left tree of chop node is null" << endl;
		return;
	}
	Stmt2IR(pTree->GetChild0());
	Stmt2IR(pTree->GetChild1());
}

/**
* 赋值语句转成对应的IR代码
* @param 传入待分析的语法树
* @return void
*/
///2015-4-7 add by wangmeng 
void IR::__Ass2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__Ass2IR() syntax tree error!" << endl;
		return;
	}

	StoreInst *store = builder->CreateStore(ConstantInt::get(module->getContext(), APInt(32, 4)), m_IRSTable[pTree->GetChild0()->GetNName()], false);
}