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
			__Declr2IR(pTree);//处理signed类型的声明语句
	     	break;
	    }
		case UNSIGN_DECLARATION_STA:
		{
			__UnDeclr2IR(pTree);//处理unsigned类型的声明语句
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

//add by yubin 2015/4/7，处理有符号类型变量的声明语句
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

	case INTTYPE://如果是int类型
	{
		visit = visit->GetChild0();//类型是PARAMETER_EXP
		do//左孩子不为空表示有变量声明
		{
			__DeclrInt2IR(visit->GetChild0());//对变量声明进行转换
			visit = visit->GetChild1();//获得右孩子
			
		} while (visit != NULL);
		break;

	}

	case FLOATTYPE://如果是int类型
	{
		visit = visit->GetChild0();//类型是PARAMETER_EXP
		do//左孩子不为空表示有变量声明
		{
			__DeclrFloat2IR(visit->GetChild0());//对变量声明进行转换
		   visit = visit->GetChild1();//获得右孩子
			
		} while (visit != NULL);
		break;
	}
	}


}

///add by yubin 2015/4/7,处理int类型变量的声明
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

///add by yubin 2015/4/7,处理float类型变量的声明
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

//add by yubin 2015/4/10，处理无符号变量的声明语句
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

	case INTTYPE://如果是int类型
	{
		visit = visit->GetChild0();//类型是PARAMETER_EXP
		do//左孩子不为空表示有变量声明
		{
			__DeclrUnInt2IR(visit->GetChild0());//对变量声明进行转换
			visit = visit->GetChild1();//获得右孩子

		} while (visit != NULL);
		break;

	}

	}
}


///add by yubin 2015/4/7,处理int类型变量的声明
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


///add by yubin 2015/4/7,处理chop类型的结点
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
* 表达式转成对应的IR代码
* @param 传入待分析的语法树
* @return 转换之后的结果
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
		///整数 例：3
	    case INTEGER_EXP: 
	    {
		    return ConstantInt::get(m_module->getContext(), APInt(32, pTree->GetiValue()));
			break;
	    }
		///浮点数 例：3.1
		case FLOATLITERAL_EXP:
		{
			return ConstantFP::get(getGlobalContext(), APFloat(pTree->GetfValue()));
			break;
	    }

		///字符串 例："good"
		case STR_EXP:
		{
			return m_builder->CreateGlobalStringPtr(pTree->GetsValue());
		}

		///变量 例：x
		case IDENT_EXP:
		{
			return m_builder->CreateLoad(m_IRSTable[pTree->GetNName()]->GetAllocaInstVar());
		}

		///加 例：x+y
		case ADD_EXP:
		{
			return __Add2IR(pTree);
		}

		///减 例：x-y
		case SUB_EXP:
			{
			return __Sub2IR(pTree);
		}
		///乘 例：x*y
		case MUL_EXP:
		{
			return __Mul2IR(pTree);
			}
		///除 例：x/y
		case DIV_EXP:
		{
			return __Div2IR(pTree);
			}

		///取余 例：x%y
		case MOD_EXP:
		{
			return __Mod2IR(pTree);
			}


	}
}

//add by yubin 2015/4/9,调用printf输出变量的值
void IR::__Out2IR(CSyntaxNode *pTree)
{
	//先声明printf函数，然后根据不同的变量类型，进行调用
	std::vector<llvm::Type *> putsArgs;
	putsArgs.push_back(m_builder->getInt8PtrTy());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);
	llvm::FunctionType *putsType = llvm::FunctionType::get(m_builder->getInt32Ty(), argsRef, true);
	llvm::Constant *putsFunc = m_module->getOrInsertFunction("printf", putsType);

	//每次输出变量的值之前，先输出是第几个状态,如state 0：
	m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("State "));
	LoadInst *m_StNumVal = m_builder->CreateLoad(m_StNum);
	m_builder->CreateCall2(putsFunc, m_builder->CreateGlobalStringPtr("%d"), m_StNumVal);
	m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(":\n"));

	if (pTree->GetST().size() != 0)//如果有变量的话，进行如下操作，否则什么也不做
	{
		vector<string> outPutSymTbl;//需要输出的变量，用vector存储
		outPutSymTbl = pTree->GetST();

		vector<string>::iterator iter;
		for (iter = outPutSymTbl.begin(); iter != outPutSymTbl.end(); iter++)
		{
			//对于每一个变量，输出形式如下x=1
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr(*iter));
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("="));

			AllocaInst *outPutVar = m_IRSTable[*iter]->GetAllocaInstVar();//通过变量的名字在m_IRSTable中找到对应的AllocaInst类型指针
			LoadInst *a = m_builder->CreateLoad(outPutVar);
			a->setAlignment(4);

			if (outPutVar->getAllocatedType() == IntegerType::get(m_module->getContext(), 32))//如果是int类型的话
			{
				Value *intFormat = m_builder->CreateGlobalStringPtr("%d");
				m_builder->CreateCall2(putsFunc, intFormat, a);
			}
			else if (outPutVar->getAllocatedType() == Type::getFloatTy(m_module->getContext()))//如果是float类型的话
			{
				//强制类型转换，将float类型转换成double类型，否则输出时会崩溃
				Value* floatTyToDoubleTy = m_builder->CreateFPExt(a, Type::getDoubleTy(m_module->getContext()));
				Value *floatFormat = m_builder->CreateGlobalStringPtr("%f");
				m_builder->CreateCall2(putsFunc, floatFormat, floatTyToDoubleTy);
			}
			m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("  "));//每个变量输出之后，输出两个空格，以便和下一个变量的输出隔开
		}
		m_builder->CreateCall(putsFunc, m_builder->CreateGlobalStringPtr("\n"));//每个状态输出之后，换行

	}
	__AddOne2IR(m_StNum);//将状态数加1
}

/**
* 变量自加操作转成对应的IR代码
* @param 待处理的变量
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

	//条件跳转
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
		//判断关系运算符类型
		switch (pTree->GetNType())
		{
		case EQU_EXP:
		{
			//条件跳转
			return m_builder->CreateICmpEQ(i, j, "cond");
		}

		default:
			break;
		}
	}
}

/**
* 加法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
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

	///整数相加 3+4
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateAdd(Left, Right, "add", false, false);
	}

	///浮点数相加
	else if (LType->isFloatTy())
	{
		///浮点数与浮点数相加 3.1+4.2
		if (RType->isFloatTy()) 
		{
			return m_builder->CreateFAdd(Left, Right, "fadd", 0);
		}
		///浮点数与整数相加 3.6+5
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFAdd(Left, fRight, "fadd", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///整数与浮点数相加 4+3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFAdd(fLeft, Right, "fadd", 0);
		}

	}
}

/**
* 减法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
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

	///整数相减 4-2
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateSub(Left, Right, "sub", false, false);
	}

	///浮点数相减
	else if (LType->isFloatTy())
	{
		///浮点数与浮点数相减 4.3-3.5
		if (RType->isFloatTy())
		{
			return m_builder->CreateFSub(Left, Right, "fsub", 0);
		}
		///浮点数与整数相减 5-3.6
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFSub(Left, fRight, "fsub", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///整数与浮点数相减 4-3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFSub(fLeft, Right, "fsub", 0);
		}

	}
}

/**
* 乘法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
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

	///整数相乘 5*6
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateMul(Left, Right, "mul", false, false);
	}

	///浮点数相乘
	else if (LType->isFloatTy())
	{
		///浮点数与浮点数相乘 4.3*3.5
		if (RType->isFloatTy())
		{
			return m_builder->CreateFMul(Left, Right, "fmul", 0);
		}
		///浮点数与整数相乘 5*3.6
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFMul(Left, fRight, "fmul", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///整数与浮点数相乘 4*3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFMul(fLeft, Right, "fmul", 0);
		}
	}
}


/**
* 除法操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
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

	///整数相除 5/6
	if (LType->isIntegerTy() &&
		RType->isIntegerTy())
	{
		return  m_builder->CreateSDiv(Left, Right, "div", false);
	}

	///浮点数相除
	else if (LType->isFloatTy())
	{
		///浮点数与浮点数相除 4.3/3.5
		if (RType->isFloatTy())
		{
			return m_builder->CreateFDiv(Left, Right, "fdiv", 0);
		}
		///浮点数与整数相除 5/3.6
		else if (RType->isIntegerTy())
		{
			Value* fRight = m_builder->CreateSIToFP(Right,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFDiv(Left, fRight, "fdiv", 0);
		}
	}
	else if (RType->isFloatTy())
	{
		///整数与浮点数相除 4/3.6
		if (LType->isIntegerTy())
		{
			Value*fLeft = m_builder->CreateSIToFP(Left,
				Type::getFloatTy(m_module->getContext()));
			return m_builder->CreateFDiv(fLeft, Right, "fdiv", 0);
		}
	}
}

/**
* 取余操作转成对应的IR代码
* @param 待处理的语法树
* @return 转之后的结果
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

	///整数取余
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
* 强制转换语句转为IR代码
* @param 传入待分析的语法树
* @return 转换后的IR代码
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