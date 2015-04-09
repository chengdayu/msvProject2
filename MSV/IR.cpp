#include"IR.h"
#include<iostream>
using namespace llvm;
using namespace std;

IR::IR()
{
	//m_module = new Module("global", getGlobalContext());
	
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

	LLVMContext Context;
	std::unique_ptr<Module> Owner = make_unique<Module>("test", Context);
	m_module = Owner.get();

	FunctionType *FuncTypeOfMain = FunctionType::get(IntegerType::get(m_module->getContext(), 32), false);
	Function *MainFunc = Function::Create(FuncTypeOfMain, Function::ExternalLinkage, "main", m_module);

	BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", MainFunc, 0);

	m_builder->SetInsertPoint(entrymain);

	m_StNum = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, "$state_num");

	Stmt2IR(IRTree->GetRoot());

	m_builder->CreateRetVoid();

	ExecutionEngine* EE = EngineBuilder(std::move(Owner)).create();

	outs() << "We just constructed this LLVM module:\n\n" << *m_module;
	outs() << "\n\nRunning foo: ";
	outs().flush();

	//m_module->dump();


	std::vector<GenericValue> noargs;
	//EE->runFunction(FooF, noargs);
	GenericValue gv = EE->runFunction(MainFunc, noargs);

	//outs() << "Result: " << gv.IntVal << "\n";

	outs().flush();
	delete EE;
	llvm_shutdown();

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
		case IDENT_EXP:
		{
			 
			return m_builder->CreateLoad(m_IRSTable[pTree->GetNName()]);
		}
		case ADD_EXP:
		{
			if (pTree->GetChild0()==NULL||pTree->GetChild1()==NULL)
			{
				cout << "__Expr2IR add error!" << endl;
				return NULL;
			}
			Value *Left = __Expr2IR(pTree->GetChild0());
			Value* Right = __Expr2IR(pTree->GetChild1());

		/*	Left->

			if (Left->getAllocatedType() == IntegerType::get(m_module->getContext(), 32) &&
				Right->getAllocatedType() == IntegerType::get(m_module->getContext(), 32))
			{
				return  m_builder->CreateAdd(a, b, "add", false, false);
			}		*/
		}
	}
}

//add by yubin 2015/4/9,����printf���������ֵ
void IR::__Out2IR(CSyntaxNode *pTree)
{
	//������printf������Ȼ����ݲ�ͬ�ı������ͣ����е���
	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8Ty()->getPointerTo());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, false);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	//ÿ�����������ֵ֮ǰ��������ǵڼ���״̬,��state 0��
	m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("State "));
	LoadInst *m_StNumVal = m_builder->CreateLoad(m_StNum);
	m_builder->CreateCall2(putsFunc, m_builder->CreateGlobalStringPtr("%d"), m_StNumVal);
	m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(":\n"));

	if (pTree->GetST().size() != 0)//����б����Ļ����������²���������ʲôҲ����
	{
		vector<string> outPutSymTbl;//��Ҫ����ı�������vector�洢
		outPutSymTbl = pTree->GetST();

		vector<string>::iterator iter;
		for (iter = outPutSymTbl.begin(); iter != outPutSymTbl.end(); iter++)
		{
			//����ÿһ�������������ʽ����x=1
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(*iter));
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("="));

			AllocaInst *outPutVar = m_IRSTable[*iter];//ͨ��������������m_IRSTable���ҵ���Ӧ��AllocaInst����ָ��
			LoadInst *a = m_builder->CreateLoad(outPutVar);
			if (outPutVar->getAllocatedType() == IntegerType::get(m_module->getContext(), 32))//�����int���͵Ļ�
			{
				Value *intFormat = m_builder->CreateGlobalStringPtr("%d");
				m_builder->CreateCall2(putsFunc, intFormat, a);
			}
			else if (outPutVar->getAllocatedType() == Type::getFloatTy(m_module->getContext()))//�����float���͵Ļ�
			{
				Value *intFormat = m_builder->CreateGlobalStringPtr("%f");
				m_builder->CreateCall2(putsFunc, intFormat, a);
			}
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("  "));//ÿ���������֮����������ո��Ա����һ���������������
		}
		m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("\n"));//ÿ��״̬���֮�󣬻���

	}
	__AddOne2IR(m_StNum);//��״̬����1
}

/**
* �����ԼӲ���ת�ɶ�Ӧ��IR����
* @param ������ı���
* @return void
*/
///2015-4-8 add by wangmeng
void IR::__AddOne2IR(AllocaInst * alloc)
{
	LoadInst *load = m_builder->CreateLoad(alloc);
	Value *One = m_builder->getInt32(1);
	Value *result = m_builder->CreateNSWAdd(load, One, "inc");
	StoreInst *store = m_builder->CreateStore(result, alloc, false);

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
	if (pTree->GetNType() == IDENT_EXP)
	{

	}
	else if (pTree->GetNType() == NEGATION_EXP)
	{
		
	}
	else
	{
		Value* LHS = __Expr2IR(pTree->GetChild0());
		Value* RHS = __Expr2IR(pTree->GetChild1());

        AllocaInst *int32_i = m_builder->CreateAlloca(Type::getInt32Ty(m_module->getContext()), 0, "i");
		AllocaInst *int32_j = m_builder->CreateAlloca(Type::getInt32Ty(m_module->getContext()), 0, "j");
		StoreInst *store_i = m_builder->CreateStore(LHS, int32_i, false);
		StoreInst *store_j = m_builder->CreateStore(RHS, int32_j, false);
		LoadInst *i = m_builder->CreateLoad(int32_i);
		LoadInst *j = m_builder->CreateLoad(int32_j);
		//�жϹ�ϵ���������
		switch (pTree->GetNType())
		{
		case EQU_EXP:
		{
			//������ת
			return m_builder->CreateICmpEQ(i, j, "cond");
		}

		default:
			break;
		}
	}
}