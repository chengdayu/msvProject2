#include"IR.h"
#include<iostream>

#include<map>

using namespace llvm;
using namespace std;

extern CSyntaxNode *struct_tree;

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

	m_IRStruct = map<string, IRStruct*>();
	m_SVtable = map<string, string>();
}
IR::~IR()
{
	delete m_module;
	delete m_builder;
}
void IR::Trslt2IR(CSyntaxTree* m_GlbVarTree,CSyntaxNode *function_tree, CSyntaxTree *IRTree)
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

	m_StNum = m_builder->CreateAlloca(IntegerType::get(m_module->getContext(), 32), NULL, "$$state_num");
	m_StNum->setAlignment(4);

	StoreInst *store = m_builder->CreateStore(m_builder->getInt32(0), m_StNum, false);
	store->setAlignment(4);

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
		IRStruct* s = new IRStruct(m_module,this,name.at(i), tree.at(i));
		if (!s->IsDefined() || m_IRStruct.insert(make_pair(s->GetStructName(), s)).second == false)
		{
			cout << "failed to define struct " << endl;
			break;
		}
		//m_builder->CreateAlloca(s->GetStructType());
	}

	frame2IR(m_GlbVarTree->GetRoot());

	func2IR(function_tree);

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

/**
* ����ȫ�ֱ���
* @param pTrlee(ȫ�ֱ�����) 
*/
///add by daichunchun 2015-5-5
void IR::frame2IR(CSyntaxNode *pTrlee)
{
	if (pTrlee == NULL)
	{
		return;
	}
	switch (pTrlee->GetNType())
	{
		case DECLARE_STA:
		{
			__Declr2IR(pTrlee, true);//����signed���͵��������
			break;
		}
		case UNSIGN_DECLARATION_STA:
		{
			__Declr2IR(pTrlee->GetChild0(), false);//����unsigned���͵��������
			break;
		}
		case CHOP_STA:
		{
			__Chop2IR(pTrlee);
			break;
		}
		default:
		{
			cout << "In function frame2IR,pTrlee have not-declare node" << endl;
			return;
		}
	}
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

			__Declr2IR(pTree, true);//����signed���͵��������
			break;
	    }
		case UNSIGN_DECLARATION_STA:
		{
			__Declr2IR(pTree->GetChild0(), false);//����unsigned���͵��������
			break;
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
		//2015/4/22 �޸� daichunchun.ԭ�򣺴ʷ�������IR�������ʶ�޸�
		case STATE_OUTPUT_STA:
		{
			__Out2IR(pTree);
			break;
	    }
		case IF_ELSE_STA:
		{
			__If2IR(pTree);
			break;
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
		cout << "__Declr2IR() error: Basic Type is not considered !" << pTree->GetRType() << endl;
		return;
	}

	//��ȡ���뷽ʽ
	int alignment = type->getPrimitiveSizeInBits() / 8;
	
	CSyntaxNode* visit = pTree->GetChild0();
	while (visit != NULL)
	{
		switch (visit->GetChild0()->GetNType())
		{
			case IDENT_EXP:
			{
				__DeclrIdent(type, visit->GetChild0(), alignment, sign);
				break;
			}

			case ADDRESS_DECLARE_STA:
			{
				alignment = 4;
				__DeclrPtr(type, visit->GetChild0(), alignment, sign);
				break;
			}

			case ARRAY_DECLARE_STA:
			{
				__DeclrArray(type, visit->GetChild0(), alignment, sign);
				break;
			}

			default: 
			{
				cout << "__Declr2IR() error: Combinational Type is not considered !" << visit->GetChild0()->GetNType() << endl;
				break;
			}
		}
		visit = visit->GetChild1();
	}

}

/**
* ����һ���������ͱ���
* @param type(��������)  pTree(������������﷨��)  alignment(���뷽ʽ) sign(�Ƿ��з���)
* @return ת��֮��Ľ��
*/
///add by shiyifang 2015-04-17
void IR::__DeclrIdent(Type *type, CSyntaxNode *pTree, int alignment, bool sign)
{
	string name = pTree->GetNName();

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

/**
* ����һ��ָ�����ͱ���
* @param type(��������)  pTree(������������﷨��)  alignment(���뷽ʽ) sign(�Ƿ��з���)
* @return ת��֮��Ľ��
*/
///add by shiyifang 2015-04-17
void IR::__DeclrPtr(Type *type, CSyntaxNode *pTree, int alignment, bool sign)
{     
	while (pTree != NULL)
	{
		if (pTree->GetNType() == ADDRESS_DECLARE_STA)
		{
			if (type->isVoidTy())
				type = Type::getInt8Ty(m_module->getContext());
			else
				type = PointerType::get(type, 0);//ADDRESS_SAPCE_GENERIC ??
		}
			
		else if (pTree->GetNType() == ARRAY_DECLARE_STA)
		{
			__DeclrArray(type, pTree, alignment, sign);
			return;
		}

		else if (pTree->GetNType() == IDENT_EXP)
		{
			__DeclrIdent(type, pTree, alignment, sign);
			return;
		}

		pTree = pTree->GetChild0();
	}
}

/**
* ����һ���������ͱ���
* @param type(��������)  pTree(������������﷨��)  alignment(���뷽ʽ) sign(�Ƿ��з���)
* @return ת��֮��Ľ��
*/
///add by shiyifang 2015-04-17
///changed by daichunchun 2015-04-27 �����޸ģ���������±���ʽ����
void IR::__DeclrArray(Type *type, CSyntaxNode *pTree, int alignment, bool sign)
{
	vector<int> subscript;
	
	CSyntaxNode *TempNode=pTree;

	while (TempNode != NULL)
	{ 
		if (TempNode->GetChild0()->GetNType() == INTEGER_EXP)
			subscript.push_back(TempNode->GetChild0()->GetiValue());
		//��Ϊ���ʽ
		else if (TempNode->GetChild0() != NULL)
		{
			//������ʽ��ֵ
			Value* integer_exp = __Expr2IR(TempNode->GetChild0());
			//�жϱ��ʽ��������ֵ�Ƿ�Ϊi32����
			if (integer_exp->getType() == IntegerType::get(m_module->getContext(), 32))
			{
				//�������������±�����
				ConstantInt* tempInt = (ConstantInt*)integer_exp;
				subscript.push_back(tempInt->getSExtValue());
			}
			else
			{
				//�±�Ƿ�����
				cout << "__DeclrArray error: The array subscript is not int type" << endl;
				return;
			}
		}
		else
		{
			//���±걨��
			cout << "__DeclrArray error: Array Type has no subscript " << endl;
			return;
		}
		TempNode = TempNode->GetChild1();
	}

	for (int i = subscript.size() - 1; i >= 0; i--)
		type = ArrayType::get(type, subscript.at(i));


	__DeclrIdent(type, pTree, alignment, sign);

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
		cout << "__Ass2IR() handle right error!" << endl;
		return;
	}

	Value *RightValue = __Cast2IR(tmpRtValue, LType, true);

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
		//add by chengyu	2015-5-6
		//���� 
		case MINUS_EXP:
		{
			//int���͵ĸ�ֵ	����-3
			if (pTree->GetChild0()->GetNType() == INTEGER_EXP)
			{
				return ConstantInt::get(m_module->getContext(), APInt(32, -(pTree->GetChild0()->GetiValue())));
				break;
			}
			//float���͵ĸ�ֵ	����-3.1
			else if (pTree->GetChild0()->GetNType() == FLOATLITERAL_EXP)
			{
				return ConstantFP::get(m_module->getContext(), APFloat(-(pTree->GetChild0()->GetfValue())));
			}
			//����ȡ��ֵ ��:-x
			/*
			else if (pTree->GetChild0()->GetNType() == IDENT_EXP)
			{
			Value *res = m_builder->CreateLoad(m_IRSTable[pTree->GetChild0()->GetNName()]->GetAllocaInstVar());
			}*/
		}
		///������ ����3.1
		case FLOATLITERAL_EXP:
		{
			return ConstantFP::get(getGlobalContext(), APFloat(pTree->GetfValue()));
			break;
			}
		//�ַ� ����'c',add by daichunchun 2015/4/21
		case CHARLITERAL_EXP:
		{
			return ConstantInt::get(m_module->getContext(), APInt(8, pTree->GetcValue()));
			break;
			}
		//��ַ ����&a ,add by daichunchun 2015/4/22
		case ADDRESS_EXP:
		{
			return m_IRSTable[pTree->GetChild0()->GetNName()]->GetAllocaInstVar();
			break;
			}
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
			string a = *iter;
			int position = a.find("$$");
			if (position!=0)
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
				//�ַ����, add by daichunchun 2015/4/21
				else if (outPutVar->getAllocatedType() == Type::getInt8Ty(m_module->getContext()))//�����char���͵Ļ�
				{
					Value *charFormat = m_builder->CreateGlobalStringPtr("%c");
					m_builder->CreateCall2(putsFunc, charFormat, a);
				}
				//ָ�����, add by daichunchun 2015/4/22
				else if (outPutVar->getAllocatedType()->isPointerTy())
				{
					Value *intFormat = m_builder->CreateGlobalStringPtr("%d");
					//���һ��ǿ������ת����ֱ�����ָ������Ϊ0��Ϊ�����ֵ�ַ�����ǿ������ת����LLVM 3.7��
					m_builder->CreateCall2(putsFunc, intFormat, m_builder->CreatePtrToInt(a, IntegerType::get(m_module->getContext(),
						32)));
				}
				m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("  "));//ÿ���������֮����������ո��Ա����һ���������������
			}
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

//add by wanglei 2015-4-29
/**
* �������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
Value * IR::__Cond2IR(CSyntaxNode* pTree)
{
	if (pTree == NULL)
	{
		cout << "__Cond2IR error!" << endl;
		return NULL;
	}

	//��ȡ��ǰ������� 
	switch (pTree->GetNType())
	{
	case IDENT_EXP:   //��ʶ������if(x)
	case INTEGER_EXP:   //���ͣ���if(3)
	case FLOATLITERAL_EXP:   //�����ͣ���if(2.1)
	case ADD_EXP:    //�ӣ���if(1+2)
	case SUB_EXP:    //������if(1-2) 
	case MUL_EXP:    //�ˣ���if(1*2)
	case DIV_EXP:    //������if(1/2)
	case MOD_EXP:    //ȡģ����if(1%2)
	{
		return __Expr2IR(pTree);
		break;
	}

	case EQU_EXP:    //���ڣ���if(x==y)
	{
		CSyntaxNode* LC = pTree->GetChild0();   //����
		CSyntaxNode* RC = pTree->GetChild1();   //�Һ���
		Value* LHS = __Cond2IR(LC);      //��������
		Value* RHS = __Cond2IR(RC);      //�����Һ���
		return __EQ2IR(LHS, RHS);
		break;
	}
	case NE_EXP:     //�����ڣ���if(x!=y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return __NE2IR(LHS, RHS);
		break;
	}
	case GT_EXP:     //���ڣ���if(x>y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return __GT2IR(LHS, RHS);
		break;
	}
	case LT_EXP:     //С�ڣ���if(x<y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return __LT2IR(LHS, RHS);
		break;
	}
	case GE_EXP:     //���ڵ��ڣ���if(x>=y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return __GE2IR(LHS, RHS);
		break;
	}
	case LE_EXP:     //С�ڵ��ڣ���if(x<=y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return __LE2IR(LHS, RHS);
		break;
	}
	case NEGATION_EXP:   //�񶨱��ʽ����if(!x)
	{
		CSyntaxNode* C = pTree->GetChild0();
		Value* value = __Cond2IR(C);   //��������
		Type* type = value->getType();    //��ȡvalue������
		if (type->isIntegerTy())
		{
			value = m_builder->CreateZExt(value, IntegerType::get(m_module->getContext(), 32));   //������ֵ��չΪint32
			Value* zero_int32 = m_builder->getInt32(0);   //����int32λ����0
			return m_builder->CreateICmpEQ(value, zero_int32);    //���غ�0�ȽϵĽ��
		}
		else if (type->isFloatTy())
		{
			Value* zero_int32 = m_builder->getInt32(0);   //����int32λ����0
			Value* zero_float = m_builder->CreateSIToFP(zero_int32, Type::getFloatTy(m_module->getContext()));   //���������͵�0
			return m_builder->CreateFCmpOEQ(value, zero_float);    //���غ�0�ȽϵĽ��
		}
		break;
	}
	case AND_EXP:   //�룬��if(x and y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return m_builder->CreateAnd(LHS, RHS, "AND");
		break;
	}
	case OR_EXP:    //����if(x or y)
	{
		CSyntaxNode* LC = pTree->GetChild0();
		CSyntaxNode* RC = pTree->GetChild1();
		Value* LHS = __Cond2IR(LC);
		Value* RHS = __Cond2IR(RC);
		return m_builder->CreateOr(LHS, RHS, "OR");
		break;
	}
	default:
		break;
	}
}

//����
Value * IR::__EQ2IR(Value* LHS, Value* RHS)
{
	//��Ԫ��ϵ����
	Type* LType = LHS->getType();
	Type* RType = RHS->getType();

	if (LType->isIntegerTy() && RType->isIntegerTy())  //����
	{
		Value* temp = m_builder->CreateICmpEQ(LHS, RHS);
		temp->print(llvm::outs());
		return m_builder->CreateICmpEQ(LHS, RHS, "ICmpEQ");
	}
	else if (LType->isFloatTy() && RType->isFloatTy())   //������
	{
		return m_builder->CreateFCmpOEQ(LHS, RHS, "FCmpOEQ");
	}
}

//������
Value * IR::__NE2IR(Value* LHS, Value* RHS)
{
	Type* LType = LHS->getType();
	Type* RType = RHS->getType();

	if (LType->isIntegerTy() && RType->isIntegerTy())
	{
		return m_builder->CreateICmpNE(LHS, RHS, "ICmpNE");
	}
	else if (LType->isFloatTy() && RType->isFloatTy())
	{
		return m_builder->CreateFCmpONE(LHS, RHS, "FCmpONE");
	}
}

//����
Value * IR::__GT2IR(Value* LHS, Value* RHS)
{
	Type* LType = LHS->getType();
	Type* RType = RHS->getType();

	if (LType->isIntegerTy() && RType->isIntegerTy())
	{
		return m_builder->CreateICmpSGT(LHS, RHS, "ICmpGT");
	}
	else if (LType->isFloatTy() && RType->isFloatTy())
	{
		return m_builder->CreateFCmpOGT(LHS, RHS, "FCmpOGT");
	}
}

//С��
Value * IR::__LT2IR(Value* LHS, Value* RHS)
{
	Type* LType = LHS->getType();
	Type* RType = RHS->getType();

	if (LType->isIntegerTy() && RType->isIntegerTy())
	{
		return m_builder->CreateICmpSLT(LHS, RHS, "ICmpLT");
	}
	else if (LType->isFloatTy() && RType->isFloatTy())
	{
		return m_builder->CreateFCmpOLT(LHS, RHS, "FCmpOLT");
	}
}

//���ڵ���
Value * IR::__GE2IR(Value* LHS, Value* RHS)
{
	Type* LType = LHS->getType();
	Type* RType = RHS->getType();

	if (LType->isIntegerTy() && RType->isIntegerTy())
	{
		return m_builder->CreateICmpSGE(LHS, RHS, "ICmpGE");
	}
	else if (LType->isFloatTy() && RType->isFloatTy())
	{
		return m_builder->CreateFCmpOGE(LHS, RHS, "FCmpGE");
	}
}

//С�ڵ���
Value * IR::__LE2IR(Value* LHS, Value* RHS)
{
	Type* LType = LHS->getType();
	Type* RType = RHS->getType();

	if (LType->isIntegerTy() && RType->isIntegerTy())
	{
		return m_builder->CreateICmpSLE(LHS, RHS, "ICmpLE");
	}
	else if (LType->isFloatTy() && RType->isFloatTy())
	{
		return m_builder->CreateFCmpOLE(LHS, RHS, "FCmpLE");
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
///2015-5-6 modified by chengyu 
//2015-5-8 modified by wanglei�������sign��������ʾ��ǰ��������ֵ�������Ƿ������
Value* IR::__Cast2IR(Value *value, Type *type, bool sign)
{
	Type *valType = value->getType();  //��ת���ݵ�����
	if (valType == type)
	{
		return value;
	}
	else if (valType->isIntegerTy() && sign == true && type->isFloatTy())  //int -> float
	{
		return m_builder->CreateSIToFP(value, type);
	}
	else if (valType->isIntegerTy() && sign == false && type->isFloatTy())   //unsigned int -> float
	{
		return m_builder->CreateUIToFP(value, type);
	} 
	else if (valType->isFloatTy() && type->isIntegerTy())   //float -> int
	{
		return m_builder->CreateFPToSI(value, type);
	}
	else if (valType->isIntegerTy() && type->isPointerTy())   //int -> int*
	{
		return m_builder->CreateIntToPtr(value, type);
	}
	else if (valType->isPointerTy() && type->isIntegerTy())   //int* -> int
	{
		return m_builder->CreatePtrToInt(value, type);
	}
	//add by chengyu
	//int����תchar����
	else if (type->isIntegerTy(8) && valType->isIntegerTy(32))
	{
		return m_builder->CreateTrunc(value, IntegerType::get(m_module->getContext(), 8));
	}
	//char����תint����
	else if (type->isIntegerTy(32) && valType->isIntegerTy(8))
	{
		return m_builder->CreateZExt(value, IntegerType::get(m_module->getContext(), 32));
	}

	else
	{
		cout << "cast error!" << endl;
		return NULL;
	}

}


/**
* �����������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
///2015-4-14 add by daichunchun
Value* IR::__Call2IR(CSyntaxNode *pTree)
{
	//��ȡ���ú�����
	std::string Callee = pTree->GetNName();
	//��ȡģ���к�������
	Function *CalleeF = m_module->getFunction(Callee);
	//�жϺ����Ƿ��ڸ�ģ���д���
	if (CalleeF == 0)
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

	if (CalleeF->getReturnType() != Type::getVoidTy(m_module->getContext()))
	{
		AllocaInst *ReturnValue = m_builder->CreateAlloca(CalleeF->getReturnType());
		ReturnValue->setName("RValue");
		int alignment = CalleeF->getReturnType()->getPrimitiveSizeInBits() / 8;
		ReturnValue->setAlignment(alignment);
		vecvalue.push_back(ReturnValue);
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

/**
*���ݽṹ�嶨���ģ���declare���õ���Ӧ��llvm type
*@param module (�����ṹ���ģ��) pTree(�������)
*@return ��������Ӧ��llvm type
*/
//add by shiyifang 2015-04-16
Type* IR::GetType(Module* m_module, CSyntaxNode *pTree)
{
	/*if (pTree->GetNType() != DECLARE_STA && pTree->GetNType() != UNSIG N_DECLARATION_STA)
	{
		cout << "In function GetType : pTree is not a DECLARE CSyntaxNode !"<<endl;
		return NULL;
	}
	if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
	{
		cout << "In function GetType : Unsigned declaration is not considered !" << endl;
		return NULL;
	}
	*/

	RETURNTYPE type = pTree->GetRType();
	switch (type)
	{
		case INTTYPE:
		{
			return IntegerType::get(m_module->getContext(), 32);
		}
		case FLOATTYPE:
		{
			return Type::getFloatTy(m_module->getContext());
		}
		case CHARTYPE:
		{
			return Type::getInt8Ty(m_module->getContext());
		}
		/*
		case BOOLTYPE:
		{
		return Type::getInt8Ty(m_module->getContext());
		}
		*/
		case STRUCTTYPE:
		{
			map<string, IRStruct*>::iterator it = m_IRStruct.find(pTree->GetNName());
			if (it == m_IRStruct.end())
				return NULL;
			return ((*it).second)->GetStructType();
		}
		case VOIDTYPE:
		{
			return Type::getVoidTy(m_module->getContext());
		}
		default:
		{
			cout << "In function GetType() : Type is not considered !" << endl;
			return NULL;
		}

	}

}

/**
* �����������תΪIR����
* @param ������������﷨��
* @return ת�����IR����
*/
///2015-4-27 add by daichunchun
void IR::func2IR(CSyntaxNode *pTree)
{
	if (pTree == NULL)
	{
		return;
	}
	//������������
	std::vector<Type*> vectype;
	//����������
	std::vector<string> vecname;
	//����ֵ����
	Type* rettype = Type::getVoidTy(m_module->getContext());
	//���������
	CSyntaxNode *ParameterLeader = pTree->GetChild0();
	while (ParameterLeader != NULL)
	{
		Type* temptype = IR::GetType(m_module, ParameterLeader);
		vectype.push_back(temptype);
		string parametername = ParameterLeader->GetChild0()->GetNName();
		vecname.push_back(parametername);
		if (parametername == "RValue")
			rettype = temptype;
		ParameterLeader = ParameterLeader->GetChild1();
	}
	//���캯������
	FunctionType* FT = FunctionType::get(rettype, vectype, false);
	//��ȡ��������
	string functionname = pTree->GetNName();
	//��������
	Function* Func = Function::Create(FT, Function::ExternalLinkage, functionname, m_module);

	//�������ű�
	map<string, IRSymbol *> fun_STable;

	//�洢ȫ��״̬
	BasicBlock *tempinsertpoint = m_builder->GetInsertBlock();
	map<string, IRSymbol *> temp_IRStable = m_IRSTable;

	//��Ӻ�����
	if (pTree->GetChild1() != NULL)
	{
		BasicBlock *entry = BasicBlock::Create(m_module->getContext(), "entry", Func, 0);
		m_builder->SetInsertPoint(entry);
		Function::arg_iterator args = Func->arg_begin();
		vector<string>::iterator argsnames = vecname.begin();
		vector<Type*>::iterator types = vectype.begin();
		while (args != Func->arg_end())
		{
			string tempname = *(argsnames++);
			Type* temptype = *(types++);
			Value* temparg = args++;
			//�����б���ϱ�����
			temparg->setName(tempname);
			//Ϊ�����б�����ڴ�
			AllocaInst *allocDeclr = m_builder->CreateAlloca(temptype);
			allocDeclr->setName(tempname + ".addr");
			int alignment = temptype->getPrimitiveSizeInBits() / 8;
			allocDeclr->setAlignment(alignment);
			//��������ű�
			IRSymbol *symbol = new IRSymbol(allocDeclr,1);
			if (fun_STable.insert(map<string, IRSymbol *>::value_type(tempname, symbol)).second == false)
			{
				cout << "__DeclrIdent : variable " << tempname << "has already defined" << endl;
			}
			//�洢����
			StoreInst *store = m_builder->CreateStore(temparg, allocDeclr, false);
			}
		//�޸ķ��ű�������
		m_IRSTable = fun_STable;
		Stmt2IR(pTree->GetChild1());
	}

	if (FT->getReturnType() != Type::getVoidTy(m_module->getContext()))
	{
		AllocaInst *ReturnValue = m_IRSTable["RValue"]->GetAllocaInstVar();
		LoadInst *LoadValue = m_builder->CreateLoad(ReturnValue);
		m_builder->CreateRet(LoadValue);
	}

	m_IRSTable = temp_IRStable;
	m_builder->SetInsertPoint(tempinsertpoint);

	return;
	}