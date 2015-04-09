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
		__Declr2IR(pTree);//�����������
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
		case DISPLAY_STA:
		{
			__Out2IR(pTree);
			break;
	    }
		case IF_STA:
		{
			__If2IR(pTree);
			break;
		}
	}
	
		

}

//add by yubin 2015/4/7�������������
void IR::__Declr2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __Declr2IR, IRTree is NULL" << endl;
		return;
	}
	CSyntaxNode* visit = pTree;
	switch (visit->GetRType())
	{

	case INTTYPE://�����int����
	{
		visit = visit->GetChild0();//������PARAMETER_EXP
		do//���Ӳ�Ϊ�ձ�ʾ�б�������
		{
			__DeclrInt2IR(visit->GetChild0());//�Ա�����������ת��
			visit = visit->GetChild1();//����Һ���
			
		} while (visit != NULL);
		break;

	}

	case FLOATTYPE://�����int����
	{
		visit = visit->GetChild0();//������PARAMETER_EXP
		do//���Ӳ�Ϊ�ձ�ʾ�б�������
		{
			__DeclrFloat2IR(visit->GetChild0());//�Ա�����������ת��
		   visit = visit->GetChild1();//����Һ���
			
		} while (visit != NULL);
		break;
	}
	}


}

///add by yubin 2015/4/7,����int���ͱ���������
void IR::__DeclrInt2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __DeclrInt2IR, IRTree is NULL" << endl;
		return;
	}
	AllocaInst *allocDeclrInt = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, pTree->GetNName());
	allocDeclrInt->setAlignment(4);
	m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrInt));
}

///add by yubin 2015/4/7,����float���ͱ���������
void IR::__DeclrFloat2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __DeclrInt2IR, IRTree is NULL" << endl;
		return;
	}
	AllocaInst *allocDeclrFloat = m_builder->CreateAlloca(Type::getFloatTy(m_module->getContext()), NULL, pTree->GetNName());
	m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrFloat));
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
* ��ֵ���ת�ɶ�Ӧ��IR����
* @param ������������﷨��
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

	Value *RightValue=__Expr2IR(pTree->GetChild1());
	StoreInst *store = m_builder->CreateStore(RightValue, m_IRSTable[pTree->GetChild0()->GetNName()], false);
	store->setAlignment(4);

}

/**
* ���ʽת�ɶ�Ӧ��IR����
* @param ������������﷨��
* @return void
*/
///2015-4-7 add by wangmeng
Value * IR::__Expr2IR(CSyntaxNode* pTree)
{
	if (pTree == NULL)
	{
		cout << "__Expr2IR error!" << endl;
		return NULL;
	}
	switch (pTree->GetNType())
	{
	    case INTEGER_EXP: 
	    {
		    return ConstantInt::get(m_module->getContext(), APInt(32, pTree->GetiValue()));
			break;
	    }
		case FLOATLITERAL_EXP:
		{
			return ConstantFP::get(getGlobalContext(), APFloat(pTree->GetfValue()));
			break;
	    }
		case STR_EXP:
		{
			return m_builder->CreateGlobalStringPtr(pTree->GetsValue());
		}
		case ADD_EXP:
		{
		/*	AllocaInst* type_a = m_IRSTable[pTree->GetChild0()->GetNName()];
			AllocaInst* type_b = m_IRSTable[pTree->GetChild1()->GetNName()];
			LoadInst *a = m_builder->CreateLoad(type_a);
			LoadInst *b = m_builder->CreateLoad(type_b);

			if (type_a->getAllocatedType() == IntegerType::get(m_module->getContext(), 32) &&
				type_b->getAllocatedType() == IntegerType::get(m_module->getContext(), 32))
			{
				return  m_builder->CreateAdd(a, b, "add", false, false);
			}*/			
		}
	}
	
	}
	

void IR::__Out2IR(CSyntaxNode *pTree)
{

}

void IR::__If2IR(CSyntaxNode *pTree)
{
	//Function *TheFunction = m_builder->GetInsertBlock()->getParent();
	//BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry");
	BasicBlock *ThenBB= BasicBlock::Create(m_module->getContext(), "then");
	BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else");
	//m_builder->SetInsertPoint(entry);

	//������ת
	Value *v = m_builder->CreateCondBr(__Cond2IR(pTree->GetChild0()), ThenBB, ElseBB, 0);

	m_builder->SetInsertPoint(ThenBB);
	Stmt2IR(pTree->GetChild1());

	m_builder->SetInsertPoint(ElseBB);
	Stmt2IR(pTree->GetChild2());

}

Value* IR::__Cond2IR(CSyntaxNode* pTree)
{
	//�жϹ�ϵ���������
	switch (pTree->GetNType())
	{
	case EQU_EXP:
	{
		




		break;
	}

	default:
		break;
	}
	
	
	Value* LHS = __Expr2IR(pTree->GetChild0());
	Value* RHS = __Expr2IR(pTree->GetChild1());

	AllocaInst *int32_i = m_builder->CreateAlloca(Type::getInt32Ty(m_module->getContext()), 0, "i");
	AllocaInst *int32_j = m_builder->CreateAlloca(Type::getInt32Ty(m_module->getContext()), 0, "j");
	StoreInst *store_i = m_builder->CreateStore(LHS, int32_i, false);
	StoreInst *store_j = m_builder->CreateStore(RHS, int32_j, false);
	LoadInst *i = m_builder->CreateLoad(int32_i);
	LoadInst *j = m_builder->CreateLoad(int32_j);

	//������ת
	return m_builder->CreateICmpEQ(i, j, "cond");
}