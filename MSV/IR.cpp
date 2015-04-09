#include"IR.h"
#include<iostream>
using namespace llvm;
using namespace std;

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
	    	__Declr2IR(pTree);//处理声明语句
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
	}
	}
	
		

}

//add by yubin 2015/4/7，处理声明语句
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
	m_IRSTable.insert(map<string, AllocaInst *>::value_type(pTree->GetNName(), allocDeclrInt));
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

	Value *RightValue=__Expr2IR(pTree->GetChild1());
	Value* LeftValue = m_IRSTable[pTree->GetChild0()->GetNName()];

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
			return m_builder->CreateLoad(m_IRSTable[pTree->GetNName()]);
		}

		///加 例：x+y
		case ADD_EXP:
		{
			return __Add2IR(pTree);
			
		}

		///减 例：x-y
		case SUB_EXP:
		{
			if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
			{
				cout << "__Expr2IR sub error!" << endl;
				return NULL;
			}
			Value *Left = __Expr2IR(pTree->GetChild0());
			Value* Right = __Expr2IR(pTree->GetChild1());
			if (Left->getType() == IntegerType::get(m_module->getContext(), 32) &&
				Right->getType() == IntegerType::get(m_module->getContext(), 32))
			{
				return  m_builder->CreateSub(Left, Right, "sub", false, false);
			}
		}
		///乘 例：x*y
		case MUL_EXP:
		{
			if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
			{
				cout << "__Expr2IR mul error!" << endl;
				return NULL;
			}
			Value *Left = __Expr2IR(pTree->GetChild0());
			Value* Right = __Expr2IR(pTree->GetChild1());
			if (Left->getType() == IntegerType::get(m_module->getContext(), 32) &&
				Right->getType() == IntegerType::get(m_module->getContext(), 32))
			{
				return  m_builder->CreateMul(Left, Right, "mul", false, false);
			}
		}
		///除 例：x/y
		case DIV_EXP:
		{
			if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
			{
				cout << "__Expr2IR add error!" << endl;
				return NULL;
			}
			Value *Left = __Expr2IR(pTree->GetChild0());
			Value* Right = __Expr2IR(pTree->GetChild1());		
			if (Left->getType() == IntegerType::get(m_module->getContext(), 32) &&
				Right->getType() == IntegerType::get(m_module->getContext(), 32))
			{
				//return  m_builder->CreateMul(Left, Right, "mul", false, false);
			}
		}

		///取余 例：x%y
		case MOD_EXP:
		{
			if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
			{
				cout << "__Expr2IR add error!" << endl;
				return NULL;
			}
			Value *Left = __Expr2IR(pTree->GetChild0());
			Value* Right = __Expr2IR(pTree->GetChild1());
			/*if (Left->getType() == IntegerType::get(m_module->getContext(), 32) &&
				Right->getType() == IntegerType::get(m_module->getContext(), 32))
			{
				//return  m_builder->CreateDiv(Left, Right, "sub", false, false);
			}*/
		}
	}
	
	}
	

void IR::__Out2IR(CSyntaxNode *pTree)
{



	__AddOne2IR(m_StNum);
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
	Value* Right = __Expr2IR(pTree->GetChild1());

	Type* LType = Left->getType();
	Type* RType = Right->getType();

	///整数相加
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