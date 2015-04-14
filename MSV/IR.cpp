#include"IR.h"
#include<iostream>
using namespace llvm;
using namespace std;

IRSymbol::IRSymbol()
{
}
IRSymbol::IRSymbol(AllocaInst* InstVar, bool sign)
{
	AllocaInstVar = InstVar;
	IsSigned = sign;
}

IRSymbol::~IRSymbol()
{
}

bool IR::InstIRSymbol(string name, AllocaInst* InstVar, bool sign)
{
	return m_IRSTable.insert(make_pair(name, new IRSymbol(InstVar, sign))).second;
}

IR::IR()
{
	
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

	m_builder = new llvm::IRBuilder<>(m_module->getContext());

	FunctionType *FuncTypeOfMain = FunctionType::get(IntegerType::get(m_module->getContext(), 32), false);
	Function *MainFunc = Function::Create(FuncTypeOfMain, Function::ExternalLinkage, "main", m_module);

	BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", MainFunc, 0);

	m_builder->SetInsertPoint(entrymain);

	m_StNum = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, "$state_num");
	m_StNum->setAlignment(4);

	StoreInst *store = m_builder->CreateStore(m_builder->getInt32(0), m_StNum, false);
	store->setAlignment(4);

	Stmt2IR(IRTree->GetRoot());

	m_builder->CreateRetVoid();

	ExecutionEngine* EE = EngineBuilder(std::move(Owner)).create();

	outs() << *m_module;
	outs() << "\n\nResult: \n";
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
			__Declr2IR(pTree);//����signed���͵��������
	     	break;
	    }
		case UNSIGN_DECLARATION_STA:
		{
			__UnDeclr2IR(pTree);//����unsigned���͵��������
		}
	    case CHOP_STA:
    	{
		    __Chop2IR(pTree);
		    break;
	    }
	    case ASS_EQU_EXP:
		case EX_ASS_EXP:
	    {
		    __Ass2IR(pTree);
		    break;
	    }
		case DISPLAY_STA:
		{
			__Out2IR(pTree);
			break;
	    }
		case IF_ELSE_STA:
		{
			__If2IR(pTree);
			break;
	}
	}
	
		

}

//add by yubin 2015/4/7�������з������ͱ������������
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

	//m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrInt));
	InstIRSymbol(pTree->GetNName(), allocDeclrInt, true);
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
	allocDeclrFloat->setAlignment(4);
	//m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrFloat));
	InstIRSymbol(pTree->GetNName(), allocDeclrFloat, true);
}

//add by yubin 2015/4/10�������޷��ű������������
void IR::__UnDeclr2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __Declr2IR, IRTree is NULL" << endl;
		return;
	}
	CSyntaxNode* visit = pTree->GetChild0();
	switch (visit->GetRType())
	{

	case INTTYPE://�����int����
	{
		visit = visit->GetChild0();//������PARAMETER_EXP
		do//���Ӳ�Ϊ�ձ�ʾ�б�������
		{
			__DeclrUnInt2IR(visit->GetChild0());//�Ա�����������ת��
			visit = visit->GetChild1();//����Һ���

		} while (visit != NULL);
		break;

	}

	}
}


///add by yubin 2015/4/7,����int���ͱ���������
void IR::__DeclrUnInt2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		cout << "In function __DeclrInt2IR, IRTree is NULL" << endl;
		return;
	}
	AllocaInst *allocDeclrInt = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, pTree->GetNName());
	allocDeclrInt->setAlignment(4);
	//m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrInt));
	InstIRSymbol(pTree->GetNName(), allocDeclrInt, false);
}


///add by yubin 2015/4/7,����chop���͵Ľ��
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

	AllocaInst *LeftValue = m_IRSTable[pTree->GetChild0()->GetNName()]->GetAllocaInstVar();
	Type*LType = LeftValue->getAllocatedType();

	Value *tmpRtValue = __Expr2IR(pTree->GetChild1());
	if (tmpRtValue == NULL)
	{
		cout << "__Ass2IR() handle right error1!" << endl;
		return;
	}

	Value *RightValue = __Cast2IR(tmpRtValue, LType);

	if (RightValue == NULL)
	{
		cout << "__Ass2IR() handle right error2!" << endl;
		return;
	}

	StoreInst *store = m_builder->CreateStore(RightValue, LeftValue, false);
	store->setAlignment(4);
}

/**
* ���ʽת�ɶ�Ӧ��IR����
* @param ������������﷨��
* @return ת��֮��Ľ��
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
		///���� ����3
	    case INTEGER_EXP: 
	    {
		    return ConstantInt::get(m_module->getContext(), APInt(32, pTree->GetiValue()));
			break;
	    }
		///������ ����3.1
		case FLOATLITERAL_EXP:
		{
			return ConstantFP::get(getGlobalContext(), APFloat(pTree->GetfValue()));
			break;
	    }

		///�ַ��� ����"good"
		case STR_EXP:
		{
			return m_builder->CreateGlobalStringPtr(pTree->GetsValue());
		}

		///���� ����x
		case IDENT_EXP:
		{
			return m_builder->CreateLoad(m_IRSTable[pTree->GetNName()]->GetAllocaInstVar());
		}

		///�� ����x+y
		case ADD_EXP:
		{
			return __Add2IR(pTree);
		}

		///�� ����x-y
		case SUB_EXP:
			{
			return __Sub2IR(pTree);
		}
		///�� ����x*y
		case MUL_EXP:
		{
			return __Mul2IR(pTree);
			}
		///�� ����x/y
		case DIV_EXP:
		{
			return __Div2IR(pTree);
			}

		///ȡ�� ����x%y
		case MOD_EXP:
		{
			return __Mod2IR(pTree);
			}


	}
}

//add by yubin 2015/4/9,����printf���������ֵ
void IR::__Out2IR(CSyntaxNode *pTree)
{
	//������printf������Ȼ����ݲ�ͬ�ı������ͣ����е���
	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8PtrTy());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
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

			AllocaInst *outPutVar = m_IRSTable[*iter]->GetAllocaInstVar();//ͨ��������������m_IRSTable���ҵ���Ӧ��AllocaInst����ָ��
			LoadInst *a = m_builder->CreateLoad(outPutVar);
			a->setAlignment(4);

			if (outPutVar->getAllocatedType() == IntegerType::get(m_module->getContext(), 32))//�����int���͵Ļ�
			{
				Value *intFormat = m_builder->CreateGlobalStringPtr("%d");
				m_builder->CreateCall2(putsFunc, intFormat, a);
			}
			else if (outPutVar->getAllocatedType() == Type::getFloatTy(m_module->getContext()))//�����float���͵Ļ�
			{
				//ǿ������ת������float����ת����double���ͣ��������ʱ�����
				Value* floatTyToDoubleTy = m_builder->CreateFPExt(a, Type::getDoubleTy(m_module->getContext()));
				Value *floatFormat = m_builder->CreateGlobalStringPtr("%f");
				m_builder->CreateCall2(putsFunc, floatFormat, floatTyToDoubleTy);
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
	Function *TheFunction = m_builder->GetInsertBlock()->getParent();

	BasicBlock *ThenBB = BasicBlock::Create(m_module->getContext(), "then", TheFunction);
	BasicBlock *ElseBB = BasicBlock::Create(m_module->getContext(), "else", TheFunction);
	BasicBlock *IfEnd = BasicBlock::Create(m_module->getContext(), "ifend", TheFunction);
	//m_builder->SetInsertPoint(entry);

	//������ת
	Value *v = m_builder->CreateCondBr(__Cond2IR(pTree->GetChild0()), ThenBB, ElseBB, 0);


	m_builder->SetInsertPoint(ThenBB);
	Stmt2IR(pTree->GetChild1());
	m_builder->CreateBr(IfEnd);


	m_builder->SetInsertPoint(ElseBB);
	Stmt2IR(pTree->GetChild2());
	m_builder->CreateBr(IfEnd);

	m_builder->SetInsertPoint(IfEnd);
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

/**
* �ӷ�����ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
*/
///2015-4-9 add by wangmeng
Value * IR::__Add2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__Expr2IR add error!" << endl;
		return NULL;
	}
	Value *Left = __Expr2IR(pTree->GetChild0());

	if (Left == NULL)
	{
		cout << "__Add2IR() handle left error!" << endl;
		return NULL;
	}
	Value* Right = __Expr2IR(pTree->GetChild1());

	if (Right == NULL)
	{
		cout << "__Add2IR() handle right error!" << endl;
		return NULL;
	}

	Type* LType = Left->getType();
	Type* RType = Right->getType();

	///������� 3+4
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateAdd(Left, Right, "add", false, false);
	}

	///���������
	else if (LType->isFloatTy())
	{
		///�������븡������� 3.1+4.2
		if (RType->isFloatTy()) 
		{
			return m_builder->CreateFAdd(Left, Right, "fadd", 0);
		}
		///��������������� 3.6+5
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFAdd(Left, fRight, "fadd", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///�����븡������� 4+3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFAdd(fLeft, Right, "fadd", 0);
		}

	}
}

/**
* ��������ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
*/
///2015-4-9 add by wangmeng
Value * IR::__Sub2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__Expr2IR add error!" << endl;
		return NULL;
	}
	Value *Left = __Expr2IR(pTree->GetChild0());

	if (Left == NULL)
	{
		cout << "__Sub2IR() handle left error!" << endl;
		return NULL;
	}

	Value* Right = __Expr2IR(pTree->GetChild1());

	if (Right == NULL)
	{
		cout << "__Sub2IR() handle right error!" << endl;
		return NULL;
	}

	Type* LType = Left->getType();
	Type* RType = Right->getType();

	///������� 4-2
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateSub(Left, Right, "sub", false, false);
	}

	///���������
	else if (LType->isFloatTy())
	{
		///�������븡������� 4.3-3.5
		if (RType->isFloatTy())
		{
			return m_builder->CreateFSub(Left, Right, "fsub", 0);
		}
		///��������������� 5-3.6
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFSub(Left, fRight, "fsub", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///�����븡������� 4-3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFSub(fLeft, Right, "fsub", 0);
		}

	}
}

/**
* �˷�����ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
*/
///2015-4-9 add by wangmeng
Value * IR::__Mul2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__Expr2IR add error!" << endl;
		return NULL;
	}
	Value *Left = __Expr2IR(pTree->GetChild0());

	if (Left == NULL)
	{
		cout << "__Mul2IR() handle left error!" << endl;
		return NULL;
	}

	Value* Right = __Expr2IR(pTree->GetChild1());

	if (Right == NULL)
	{
		cout << "__Mul2IR() handle right error!" << endl;
		return NULL;
	}

	Type* LType = Left->getType();
	Type* RType = Right->getType();

	///������� 5*6
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateMul(Left, Right, "mul", false, false);
	}

	///���������
	else if (LType->isFloatTy())
	{
		///�������븡������� 4.3*3.5
		if (RType->isFloatTy())
		{
			return m_builder->CreateFMul(Left, Right, "fmul", 0);
		}
		///��������������� 5*3.6
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFMul(Left, fRight, "fmul", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///�����븡������� 4*3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFMul(fLeft, Right, "fmul", 0);
		}
	}
}


/**
* ��������ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
*/
///2015-4-9 add by wangmeng
Value * IR::__Div2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__Expr2IR add error!" << endl;
		return NULL;
	}
	Value *Left = __Expr2IR(pTree->GetChild0());
	if (Left == NULL)
	{
		cout << "__Div2IR() handle left error!" << endl;
		return NULL;
	}
	Value* Right = __Expr2IR(pTree->GetChild1());

	if (Right == NULL)
	{
		cout << "__Div2IR() handle right error!" << endl;
		return NULL;
	}

	Type* LType = Left->getType();
	Type* RType = Right->getType();

	///������� 5/6
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateSDiv(Left, Right, "div", false);
	}

	///���������
	else if (LType->isFloatTy())
	{
		///�������븡������� 4.3/3.5
		if (RType->isFloatTy())
		{
			return m_builder->CreateFDiv(Left, Right, "fdiv", 0);
		}
		///��������������� 5/3.6
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFDiv(Left, fRight, "fdiv", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///�����븡������� 4/3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFDiv(fLeft, Right, "fdiv", 0);
		}
	}
}

/**
* ȡ�����ת�ɶ�Ӧ��IR����
* @param ��������﷨��
* @return ת֮��Ľ��
*/
///2015-4-9 add by wangmeng
Value * IR::__Mod2IR(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		cout << "__Expr2IR add error!" << endl;
		return NULL;
	}
	Value *Left = __Expr2IR(pTree->GetChild0());

	if (Left == NULL)
	{
		cout << "__Mod2IR() handle left error!" << endl;
		return NULL;
	}
	Value* Right = __Expr2IR(pTree->GetChild1());

	if (Right == NULL)
	{
		cout << "__Mod2IR() handle right error!" << endl;
		return NULL;
	}

	Type* LType = Left->getType();
	Type* RType = Right->getType();

	///����ȡ��
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateSRem(Left, Right, "rem");
	}
	else
	{
		cout << "__Mod2IR() error: cannot from float to float !" << endl;
		return NULL;
	}


}

/**
* ǿ��ת�����תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
///2015-4-9 add by wangmeng
Value* IR::__Cast2IR(Value *value, Type *type)
{
	Type *valType = value->getType();
	if (valType == type)
	{
		return value;
	}
	else if (type->isFloatTy() && valType->isFloatTy())
	{
		return value;
	}
	else if (type->isIntegerTy() && valType->isIntegerTy())
	{
		return value;
	}
	else if (type->isFloatTy() && valType->isIntegerTy())
	{
		return m_builder->CreateSIToFP(value, type);
	}
	else if (type->isIntegerTy() && valType->isFloatTy())
	{
		return m_builder->CreateFPToSI(value, type);
	}
	else
	{
		cout << "cast error!"<<endl;
		return NULL;
	}
}