#include"IR.h"
#include<iostream>
<<<<<<< HEAD
#include<map>

using namespace llvm;
using namespace std;

extern CSyntaxNode *struct_tree;

IRSymbol::IRSymbol()
{
	
=======
using namespace llvm;
using namespace std;

IRSymbol::IRSymbol()
{
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
	m_IRStruct = map<string, IRStruct*>();
	m_SVtable = map<string, string>();
=======
	
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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

<<<<<<< HEAD
	m_StNum = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, "$state_num");
=======
	m_StNum = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, "$$state_num");
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
	m_StNum->setAlignment(4);

	StoreInst *store = m_builder->CreateStore(m_builder->getInt32(0), m_StNum, false);
	store->setAlignment(4);

<<<<<<< HEAD
	//�ṹ�嶨��
	CSyntaxNode *visit = struct_tree;
	vector<CSyntaxNode*> tree;
	vector<string> name;
	//�����﷨��
	while (visit != NULL){
		name.push_back(visit->GetNName());
		tree.push_back(visit->GetChild0());
		visit = visit->GetChild1();
	}
	//���ɶ���
	for (int i = tree.size() - 1; i >= 0; i--)
	{
		IRStruct* s = new IRStruct(m_module, name.at(i), tree.at(i));
		if (!s->IsDefined() || m_IRStruct.insert(make_pair(s->GetStructName(), s)).second == false)
		{
			cout << "failed to define struct " << endl;
			break;
		}
		//m_builder->CreateAlloca(s->GetStructType());
	}
=======
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
			__Declr2IR(pTree, false);//����signed���͵��������
=======
			__Declr2IR(pTree);//����signed���͵��������
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
	     	break;
	    }
		case UNSIGN_DECLARATION_STA:
		{
<<<<<<< HEAD
			__Declr2IR(pTree, true);//����unsigned���͵��������
=======
			__UnDeclr2IR(pTree);//����unsigned���͵��������
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
		/*case DISPLAY_STA:
		{
			__Out2IR(pTree);
			break;
	    }*/
=======
		case DISPLAY_STA:
		{
			__Out2IR(pTree);
			break;
	    }
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
		case IF_ELSE_STA:
		{
			__If2IR(pTree);
			break;
<<<<<<< HEAD
		}
		//�����������
		case FUNCTION_STA:
		{
			__Call2IR(pTree);
			break;
		}
		//�ⲿ�����������
		case EXT_FUNCTION_STA:
		{
			__Call2IR(pTree);
			break;
		}
		
	}

}


/**add by yubin 2015-4-7
* ������ת�ɶ�Ӧ��IR����
* @param ������������﷨��
* @return void
*/             
///changed by shiyifang 2015-04-17
void IR::__Declr2IR(CSyntaxNode *pTree, bool sign)
{
	//��ô�����������
	Type* type = IR::GetType(m_module, pTree);
	if (type == NULL)
	{
		cout << "__Declr : struct define failed";
		return;
	}

	int alignment = type->getPrimitiveSizeInBits() / 8;

	CSyntaxNode* visit = pTree->GetChild0();
	while (visit != NULL)
	{

		switch (visit->GetChild0()->GetNType())
		{
		case IDENT_EXP:{
			__DeclrIdent(type, visit->GetChild0()->GetNName(), alignment, sign);
			break;
		}

		case ADDRESS_DECLARE_STA:{
			__DeclrPtr(type, visit->GetChild0(), alignment, sign);
			break;
		}

		case ARRAY_DECLARE_STA:{
			__DeclrArray(type, visit->GetChild0(), alignment, sign);
			break;
		}

		default: {
			cout << "__Declr2IR : Type " << visit->GetChild0()->GetNType() << "has not considered " << endl;
			break;
		}

		}

		visit = visit->GetChild1();
	}

}

/*
*����һ�������ı���
*@param  type(��������)  name(������)  alignment(���뷽ʽ)
*/
///add by shiyifang 2015-04-17
void IR::__DeclrIdent(Type *type, string name, int alignment, bool sign)
{
	AllocaInst *allocDeclr = m_builder->CreateAlloca(type);
	allocDeclr->setName(name);
	if (alignment > 0)
		allocDeclr->setAlignment(alignment);

	IRSymbol *symbol = new IRSymbol(allocDeclr, sign);

	if (m_IRSTable.insert(map<string, IRSymbol *>::value_type(name, symbol)).second == false)
	{
		cout << "__DeclrIdent : variable " << name << "has already defined" << endl;
	}
}

/*
*����һ��ָ�����ͱ���
*@param  type(��������)  name(������)  alignment(���뷽ʽ)
*/
///add by shiyifang 2015-04-17
void IR::__DeclrPtr(Type *type, CSyntaxNode *pTree, int alignment, bool sign)
{
	while (pTree != NULL)
	{
		if (pTree->GetNType() == ADDRESS_DECLARE_STA)
			type = PointerType::get(type, 0);//ADDRESS_SAPCE_GENERIC ??

		else if (pTree->GetNType() == ARRAY_DECLARE_STA)
		{
			__DeclrArray(type, pTree, alignment, sign);
			return;
		}

		else if (pTree->GetNType() == IDENT_EXP)
		{
			__DeclrIdent(type, pTree->GetNName(), alignment, sign);
			return;
		}

		pTree = pTree->GetChild0();
	}
}

/*
*����һ���������ͱ���
*@param  type(��������)  name(������)  alignment(���뷽ʽ)
*/
///add by shiyifang 2015-04-17
void IR::__DeclrArray(Type *type, CSyntaxNode *pTree, int alignment, bool sign)
{
	string name = pTree->GetNName();

	vector<int> subscript;
	while (pTree != NULL)
	{
		if (pTree->GetChild0()->GetNType() == INTEGER_EXP)
			subscript.push_back(pTree->GetChild0()->GetiValue());
		else if (pTree->GetChild0()->GetChild0() != NULL)
		{
			//�������ʽ�������ʱ������
		}
		else
		{
			cout << "__DeclrArray: invalid array subscript " << endl;
		}

		pTree = pTree->GetChild1();
	}

	for (int i = subscript.size() - 1; i >= 0; i--)
		type = ArrayType::get(type, subscript.at(i));

	__DeclrIdent(type, name, alignment, sign);
=======
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
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
		case CHARLITERAL_EXP:
		{
			return ConstantInt::get(m_module->getContext(), APInt(8, pTree->GetcValue()));
			break;
			}
=======
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd

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

<<<<<<< HEAD
		///�������ñ��ʽ
		case FUNCTION_STA:
		{
			return __Call2IR(pTree);
		}
		//�ⲿ�������ñ��ʽ
		case EXT_FUNCTION_STA:
		{
			return __Call2IR(pTree);
			}
		
=======

>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
			else if (outPutVar->getAllocatedType() == Type::getInt8Ty(m_module->getContext()))//�����float���͵Ļ�
			{
				Value *charFormat = m_builder->CreateGlobalStringPtr("%c");
				m_builder->CreateCall2(putsFunc, charFormat, a);
			}
=======
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
=======

>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
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
<<<<<<< HEAD
}


/**
* �����������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
///2015-4-14 add by daichunchun
Value* IR::__Call2IR(CSyntaxNode *pTree)
{
	//���ں������������δ��ɣ����ﹹ�캯�����ڲ��� dai

	//�޲κ���add1
	/*
	SmallVector<Type*, 0> Params;

	//��������
	FunctionType *funcType = FunctionType::get(IntegerType::get(m_module->getContext(), 32), Params, false);
	Function *ASSFUNCTION = Function::Create(funcType, Function::ExternalLinkage, "ASSFUNCTION", m_module);
	
	//�����幹��
	llvm::IRBuilder<> builder(m_module->getContext());
	BasicBlock *entryASSIGNFUNCTION = BasicBlock::Create(m_module->getContext(), "entry", ASSFUNCTION, 0);
	builder.SetInsertPoint(entryASSIGNFUNCTION);
	AllocaInst *allo_addr = builder.CreateAlloca(IntegerType::get(m_module->getContext(), 32), 0, "a.addr");
	builder.CreateStore(ConstantInt::get(m_module->getContext(), APInt(32, 6666)), allo_addr);
	LoadInst * load_addr = builder.CreateLoad(allo_addr);
	builder.CreateRet(load_addr);
	*/
	
	//����������add1
	/*
	//��������
	SmallVector<Type*, 1> Params;
	Params.push_back(IntegerType::get(m_module->getContext(), 32));

	//��������
	FunctionType *funcType = FunctionType::get(IntegerType::get(m_module->getContext(), 32), Params, false);
	Function *add1Func = Function::Create(funcType, Function::ExternalLinkage, "add1", m_module);
	Function::arg_iterator args = add1Func->arg_begin();

	//�β�����
	Value *int32_a = args++;
	int32_a->setName("a");
	
	//�����幹��
	llvm::IRBuilder<> builder(m_module->getContext());
	BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", add1Func, 0);
	builder.SetInsertPoint(entrymain);
	AllocaInst *allo_addr = builder.CreateAlloca(IntegerType::get(m_module->getContext(), 32), 0, "a.addr");
	builder.CreateStore(int32_a,allo_addr);
	LoadInst* load_addr=builder.CreateLoad(allo_addr);
	Value* addresult = builder.CreateAdd(load_addr,ConstantInt::get(m_module->getContext(), APInt(32, 1)));
	builder.CreateRet(addresult);
	*/
	
	//�Ĳ�������add1
	/*
	//��������
	SmallVector<Type*, 4> Params;
	Params.push_back(IntegerType::get(m_module->getContext(), 32));
	Params.push_back(IntegerType::get(m_module->getContext(), 32));
	Params.push_back(IntegerType::get(m_module->getContext(), 32));
	Params.push_back(IntegerType::get(m_module->getContext(), 32));

	//��������
	FunctionType *funcType = FunctionType::get(Type::getVoidTy(m_module->getContext()), Params, false);
	Function *add4Func = Function::Create(funcType, Function::ExternalLinkage, "add4", m_module);
	Function::arg_iterator args = add4Func->arg_begin();

	//�β�����
	Value *int32_a = args++;
	int32_a->setName("a");
	Value *int32_b = args++;
	int32_b->setName("b");
	Value *int32_c = args++;
	int32_c->setName("c");
	Value *int32_d = args++;
	int32_d->setName("d");

	//�����幹��
	llvm::IRBuilder<> builder(m_module->getContext());
	BasicBlock *entrymain = BasicBlock::Create(m_module->getContext(), "entry", add4Func, 0);
	builder.SetInsertPoint(entrymain);
	AllocaInst *allo_addr = builder.CreateAlloca(IntegerType::get(m_module->getContext(), 32), 0, "a.addr");
	builder.CreateStore(int32_a,allo_addr);
	LoadInst * load_addr = builder.CreateLoad(allo_addr);
	AllocaInst *allo_bddr = builder.CreateAlloca(IntegerType::get(m_module->getContext(), 32), 0, "b.addr");
	builder.CreateStore(int32_b, allo_bddr);
	LoadInst * load_bddr = builder.CreateLoad(allo_bddr);
	Value *addresult1=builder.CreateNSWAdd(load_addr, load_bddr,"add");
	AllocaInst *allo_cddr = builder.CreateAlloca(IntegerType::get(m_module->getContext(), 32), 0, "c.addr");
	builder.CreateStore(int32_c, allo_cddr);
	LoadInst * load_cddr = builder.CreateLoad(allo_cddr);
	Value *addresult2 = builder.CreateNSmWAdd(addresult1, load_cddr, "add");
	AllocaInst *allo_dddr = builder.CreateAlloca(IntegerType::get(m_module->getContext(), 32), 0, "c.dddr");
	builder.CreateStore(int32_d, allo_dddr);
	LoadInst * load_dddr = builder.CreateLoad(allo_dddr);
	Value *addresult3 = builder.CreateNSWAdd(addresult2, load_dddr, "add");
	builder.CreateRet(addresult3);
	
		*/
	//��ȡ���ú�����
	std::string Callee = pTree->GetNName();
	//��ȡģ���к�������
	Function *CalleeF = m_module->getFunction(Callee);
	//�жϺ����Ƿ��ڸ�ģ���д���
	if (CalleeF==0)
	{
		cout << "__Call2IR Unknown function referenced" << endl;
		return NULL;
	}
	//��ȡ�����б�
	std::vector<Value*> vecvalue;                                             
	CSyntaxNode *ParameterLeader = pTree->GetChild0();
	while (ParameterLeader != NULL)
	{
	CSyntaxNode *ParameterNode = ParameterLeader->GetChild0();
	if (ParameterNode->GetNName()!="")
	{
		AllocaInst* parameter = m_IRSTable[ParameterNode->GetNName()]->GetAllocaInstVar();
		LoadInst* parameter_load = m_builder->CreateLoad(parameter);
		vecvalue.push_back(parameter_load);
		}
	ParameterLeader = ParameterLeader->GetChild1();
	}
	//�жϴ���Ĳ��������Ƿ�����ú����Ĳ������������
	if (CalleeF->arg_size() != vecvalue.size())
	{
		cout << "__Call2IR Incorrect arguments number" << endl;
		return NULL;
		}
	//���캯������
	llvm::ArrayRef<Value*>  Idx(vecvalue);
	return m_builder->CreateCall(CalleeF, Idx);
	}

	/*
	*���ݽṹ�嶨���ģ���declare���õ���Ӧ��llvm type
	*@param module (�����ṹ���ģ��) pTree(�������)
	*@return ��������Ӧ��llvm type
	*/
	//add by shiyifang 2015-04-16
	Type* IR::GetType(Module* m_module, CSyntaxNode *pTree)
	{
		if (pTree->GetNType() != DECLARE_STA && pTree->GetNType() != UNSIGN_DECLARATION_STA)
		{
			cout << "In function GetType : pTree is not a DECLARE CSyntaxNode !";
			return NULL;
		}

		if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
		{
			cout << "In function GetType : Unsigned declaration is not considered !";
			return NULL;
		}

		RETURNTYPE type = pTree->GetRType();

		switch (type)
		{
		case INTTYPE:     		{ return IntegerType::get(m_module->getContext(), 32); }     //return integer type
								//case STRTYPE:     		{ return SYMBOLSTR; }   	//return string type
								//case FILETYPE:	 		{ return SYMBOLFILE; }		//add by yubin, return file type

		case FLOATTYPE:   		{ return Type::getFloatTy(m_module->getContext()); }     //2013-4-26 add by YY[fixed]
		case CHARTYPE:    		{ return Type::getInt8Ty(m_module->getContext()); }     //2013-4-26 add by YY[fixed]
								//case ARITHMETICTYPE:  	{ return ;} 	//2013-4-26 add by YY[fixed]

								//case LISTTYPE:		 	{ return SYMBOLLIST; }		// return list value
		case BOOLTYPE:        	{ return Type::getInt8Ty(m_module->getContext()); }		//return bool type
								//case POINTERTYPE:    	{ return SYMBOLPOINTER;} 	//return pointer value
								//case SYMBOLTYPE:		{ return ;}		// return symbol value
								//case CHANNELRTYPE:		{ return ;}		//add by mdp 2010-07-09


		case INTPTYPE:         	{ return Type::getInt8PtrTy(m_module->getContext()); }		//return integer pointer type
								//case UINTPTYPE:		   	{ return SYMBOLUINT; }
		case FLOATPTYPE:		{ return Type::getFloatPtrTy(m_module->getContext()); }
								//case UCHARPTYPE:		{ return SYMBOLCHAR; }
		case STRUCTTYPE:		{
			map<string, IRStruct*>::iterator it = m_IRStruct.find(pTree->GetNName());
			if (it == m_IRStruct.end())
				return NULL;
			return ((*it).second)->GetStructType();
		}
								//case STRUCTPTYPE:		{ return SYMBOLSTRUCTP; }

								//case DOUBLEINTPTYPE:	{ return ;}
								//case DOUBLEUINTPTYPE:	{ return ;}
								//case DOUBLEFLOATPTYPE:	{ return ;}
								//case DOUBLEUCHARPTYPE:	{ return ;}
								//case DOUBLESTRUCTPTYPE:	{ return ;}
		case VOIDTYPE:			{ return Type::getVoidTy(m_module->getContext()); }
		case VOIDPTYPE:			{ return Type::getInt8PtrTy(m_module->getContext()); }
								//add by yubin 2013-12-23 15:31 ����ָ��
								//case FUNCPTYPE:			{ return SYMBOLFUNCP;}

								//add by YY 2014/01/07 10:57 ǿ��ת��
		case CHARPTYPE:			{ return Type::getInt8PtrTy(m_module->getContext()); }
								//case DOUBLECHARPTYPE:	{ return ;}
								//case UINTTYPE:			{ return SYMBOLUINTP; }
								//case UCHARTYPE:			{ return SYMBOLUCHARP; }
								//case STRUCT_NAME_TYPE:	{ return ;}
								//case FUNCPPTYPE:		{ return ;}
		default:   			{ cout << "In function GetType() : Type is not considered !"; return NULL; }
		}
	}
=======
}
>>>>>>> 13d38610e489529e7c37773832b25d05ba23aecd
