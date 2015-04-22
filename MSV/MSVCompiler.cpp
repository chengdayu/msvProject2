#include"function.h"
#include"MSVCompiler.h"
#include<algorithm>
#include <sstream>


extern CSyntaxNode* g_syntaxTree;
extern CSyntaxNode *struct_tree;
extern CSyntaxNode * function_tree;
extern bool g_InFunc;

///constructor
///2015-3-9 add by wangmeng
CMSV::CMSV()
{
	m_Program = new CSyntaxTree(g_syntaxTree);
	m_PrgmFuture = new CSyntaxTree();
	m_IRTree = new CSyntaxTree();
	m_GlbVarTree = new CSyntaxTree();

	m_FrmSymbl = new SymbolTable();
	m_NFrmSymbl = new SymbolTable();
	m_GlbSymbl = new SymbolTable();

	struct_map = map<string, SymbolTable*>();

	m_IfTree = NULL;

	m_More = -1;
	m_Done = -1;
	m_RdcOk = -1;
	m_Keep = 0;
	m_Fin = 0;
	m_Alw = 0;
	//	m_Frm=0;
	m_FreeFrm = 0;

	m_InAndPal = 0;//yangkai

	m_MutlWhile = "";//yangkai 处理while相关

	m_Label = 0;//yangkai 处理while相关

	///2015-4-9 add by shiyifang 
	///存储结构体定义
	CSyntaxNode *node = struct_tree;
	while (m_RdcOk != 0 && node != NULL)
	{
		__InitStructDel(node->GetChild0());
		if (m_RdcOk == 0) break;
		if (struct_map.insert(make_pair(node->GetNName(), m_NFrmSymbl)).second == false)
		{
			fout << "结构体声明失败!" << endl;
			m_RdcOk = 0;
		}

		m_NFrmSymbl = new SymbolTable();
		node = node->GetChild1();
	}

}

///destructor
///2015-3-9 add by wangmeng
CMSV::~CMSV()
{
	if (m_Program != NULL)
	{
		delete m_Program;
		m_Program = NULL;
	}
	if (m_PrgmFuture != NULL)
	{
		delete m_PrgmFuture;
		m_PrgmFuture = NULL;
	}
	if (m_IRTree != NULL)
	{
		delete m_IRTree;
		m_IRTree = NULL;
	}

	while (!m_KeepTree.empty())
	{
		delete m_KeepTree.top();
		m_KeepTree.pop();
	}

	while (!m_FinTree.empty())
	{
		delete m_KeepTree.top();
		m_KeepTree.pop();
	}

	while (!m_AlwTree.empty())
	{
		delete m_KeepTree.top();
		m_KeepTree.pop();
	}

	vector<CSyntaxNode*> ::iterator iter;
	for (iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++)
	{
		delete* iter;
	}
	m_PrgmPrsnt.clear();

}

//2015-4-13,add by yubin,将m_Program设置成pTree
void CMSV::SetProgram(CSyntaxNode* pTree)
{
	m_Program->SetRoot(pTree);
	m_More = -1;//将m_More置为1，因为pre函数一开始就会判断m_More是否为0
	m_IRTree = new CSyntaxTree();

}

///Preprocess the tree
///2015-3-9 add by wangmeng
void CMSV::Pre()
{
	int i = 0;
	m_IfTree = NULL;
	m_WhileTree = NULL;//yangkai

	while (m_More != 0 && m_Program->GetRoot() != NULL)
	{
		m_More = -1;
		m_RdcOk = -1;
		m_MutlWhile = "";//yangkai 处理while相关

		GetPresent(m_Program->GetRoot());

		///2015-3-12 add by wangmeng for keep,fin,alw
		if ((m_Keep || m_Fin || m_Alw) && m_RdcOk == 1)
		{
			__KpFnAl();
		}

		if (m_RdcOk == 0 || m_More == -1)
		{
			fout << "__Pre() error!" << endl;
			
			if (m_IRTree != NULL)
			{
			delete m_IRTree->GetRoot();
			m_IRTree->SetRoot(new CSyntaxNode(FALSE_EXP, VOIDTYPE));
		}
			exit(1);
		}
		if (m_IfTree != NULL || m_WhileTree != NULL)
		{
			__PreIfWhile();
		}

		else
		{
			//fout << "State " << i << ": " << endl;
			//fout << "Present:  ";
			//PrintPrsnt();
			//fout << endl;
			//fout << "Future:  " << m_PrgmFuture->GetRoot()->TreeToString().c_str()<< endl;
			__SortPrsnt();
			__AdPrst2IR(); ///将当前状态加入到IR树中

			///2015-3-31 add by wanmeng
			if (m_More != 0)
			{
				CSyntaxNode* output = __OutputState();
				m_IRTree->MergeTree(output, CHOP_STA, VOIDTYPE);
			}
		}

		delete m_Program->GetRoot();
		m_Program->SetRoot(m_PrgmFuture->GetRoot());
		m_PrgmFuture->SetRoot(NULL);
		i++;
	}

}

/**
* Get present tree of pTree, including the assignment statement at present state
* @param the syntax tree to be handled
* @return void
*/
///2015-3-9 add by wangmeng
void CMSV::GetPresent(CSyntaxNode*pTree)
{
	if (pTree == NULL)
	{
		return;
	}
	switch (pTree->GetNType())
	{
	case UNSIGN_DECLARATION_STA:  
	case DECLARE_STA:    {__Declr(pTree); break; }  /// handle declaration: int x, y<==x
	case ASS_EQU_EXP:  	                           /// handle x<==e
	case EX_ASS_EXP:     {__Ass(pTree);   break; }  /// handle x<==e obtained by x:=e
	case UNITASSIGN_EXP: {__UAss(pTree);  break; }  /// handle x:=e
	case EMPTY_EXP:      {__Emp(pTree);   break; }  /// handle empty
	case MORE_STA:       {__More(pTree);  break; }  /// handle more //yangkai
	case TRUE_EXP:       {__True(pTree);  break; }  /// handle true //yangkai
	case SKIP_STA:       {__Skip(pTree);  break; }  /// handle skip
	case LENGTH_STA:     {__Len(pTree);   break; }  /// handle len(n)
	case NEXT_STA:       {__Next(pTree);  break; }  /// handle next p
	case AND_EXP:        {__And(pTree);   break; }  /// handle p and q
	case PARALLEL_STA:	 {__Pal(pTree);   break; }  /// handle p||q
	case PROJECTION_STA: {__Prj(pTree);   break; }  /// handle {p1,p2,...}prj{q}
	case CHOP_STA:       {__Chop(pTree);  break; }  /// handle p;q
	case IF_ELSE_STA:    {m_IfTree = pTree; m_RdcOk = 1; m_More = 1; break; }  /// handle if(b)then{p}else{q}
	case WHILE_DO_STA:   {
		m_WhileTree = pTree;
		m_RdcOk = 1;
		m_More = 1;
		//yangkai 处理while相关
		if (m_InAndPal > 0)
		{
			m_MutlWhile += pTree->TreeToString();

			__While2If(pTree);
		}
		break;
	}  /// handle while(b)do{p}
	case FOR_STA:        {__For(pTree);   break; }  /// handle for(p1,b,p2){q}
	case SWITCH_STA:     {__Switch(pTree); break; }  /// handle switch(e){case n1: (p1)...}
	case FRAME_STA:      {__Frm(pTree);   break; }  /// frame(c)
	case UNFRAME_STA:    {__UnFrm(pTree); break; }  /// frame(c)
	case OR_EXP:         {__Or(pTree);    break; }  /// p or q
	case EXIST_STA:      {__Ext(pTree);   break; }  ///exists(x,y,...):p

	case FUNCTION_DEFINE_STA:     {__FuncDef(pTree); break; }//2015-3-17,add by yubin,函数定义
	case FUNCTION_STA:     {__FuncCall(pTree); break; }///2015-3-17,add by yubin,内部函数调用
	case EXT_FUNCTION_STA:  {__EFuncCall(pTree); break; }//2015-3-17,add by yubin,外部函数调用

	case AWAIT_STA:      {__Await(pTree); break; }  ///await(b)
	case FINAL_STA:      {__Fin(pTree);   break; }  ///fin(p)
	case ALWAYS_STA:     {__Alw(pTree);   break; }  ///alw(p)
	case KEEP_STA:       {__Keep(pTree);  break; }  ///keep(p)
	case RETURN_STA:     {__Return(pTree);  break; }  ///2015-4-13,add by yubin,增加处理return语句的函数
	case REQUEST_STA:    {__Input(pTree); break; }
	case DISPLAY_STA:    {__Output(pTree); break; }


	default: break;
	}
}

/**
* Get present tree of declaration statement: int x,y<==x
* @param the syntax tree to be handled
* @return void
*/
///将定义和初始化分开放入m_PrgmPrsnt中
///2015-3-11 add by wangmeng
//2015-4-8 changed by syf   for hanlding a[] <== {1,2,3}
void CMSV::__Declr(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild0()->GetChild0() == NULL)
	{
		fout << "__Declr() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}

	CSyntaxNode* firstvisit = pTree;
	CSyntaxNode* previsit = pTree;
	CSyntaxNode*visit = pTree->GetChild0();
	if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
	{
		firstvisit = visit;
		previsit = visit;
		visit = pTree->GetChild0()->GetChild0();
	}

	///2015-4-14 modified by wangmeng 基本整个函数都修改了。主要处理数组初始化，不在分开。
	CSyntaxNode*temp = NULL;
	while (visit != NULL)
	{
		CSyntaxNode *store = visit;
		//2015-04-17 处理int *y<==&x 类赋值
		while (visit != NULL && visit->GetChild0() != NULL && visit->GetChild0()->GetNType() == ADDRESS_DECLARE_STA)
		{
			visit = visit->GetChild0();
		}

		if (visit->GetChild0()->GetNType() == ASS_EQU_EXP) ///int x<==y分成两部分int x和x<==y分别加入链表中
		{
			///处理int a[] <== {1,2}类的数组初始化
			if (visit->GetChild0()->GetChild0()->GetNType() == ARRAY_DECLARE_NULL_STA)
			{
				string NName = visit->GetChild0()->GetChild0()->GetNName();
				CSyntaxNode *node = visit->GetChild0()->GetChild1();
				int num = 1;

				while (node->GetChild0() != NULL)
				{
					if (node->GetChild1() == NULL)
						break;

					if (node->GetChild1()->GetNType() == LIST_ASSVALUE_STA ||
						node->GetChild1()->GetNType() == STRUCT_LIST_ASSVALUE_STA)
					{
						num++;
						node = node->GetChild1();
					}
				}

				///node: a[3]<==[3,4,5]
				
				///node->child1: [3,4,5]
				node = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
				node->SetChild1(visit->GetChild0()->GetChild1());
				visit->GetChild0()->SetChild1(NULL);
				
				///temp: a[3]
				temp = new CSyntaxNode(ARRAY_DECLARE_STA, LISTTYPE);
				temp->SetNName(NName);
				temp->SetChild0(new CSyntaxNode(INTEGER_EXP, num, INTTYPE));

				node->SetChild0(temp);

				///ArrayDec  int a[3]<==[3,4,5]
				CSyntaxNode* ArrayDec = new CSyntaxNode();
				ArrayDec->CopyNode(pTree);

				if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
				{
					ArrayDec->SetChild0(new CSyntaxNode());
					ArrayDec->GetChild0()->CopyNode(pTree->GetChild0());
					ArrayDec->GetChild0()->SetChild0(node);
				}
				else
				{
					ArrayDec->SetChild0(node);
				}
				
				///2015-4-22 add by wangmeng 加入全局变量
				if (!g_InFunc&&m_GlbSymbl->HasSymbol(NName))
				{
					__InstDec2ST(ArrayDec);
					if (!m_GlbVarTree->MergeTree(ArrayDec,CHOP_STA))
					{
						fout << "__Declr merge tree error!" << endl;
						m_RdcOk = 0;
						return;
					}
				}
				else
				{
				__InsrtAss(ArrayDec);
				}
				///end by wangmeng

				if (previsit == firstvisit)
				{
					previsit->SetChild0(store->GetChild1());
					store->SetChild1(NULL);
					delete store;
					visit = previsit->GetChild0();
					continue;
				}
				else
				{
					previsit->SetChild1(store->GetChild1());
					store->SetChild1(NULL);
					delete store;
					visit = previsit->GetChild1();
					continue;
				}
								
			}
			///处理int a[2] <== {1,2}类的数组初始化
			else if (visit->GetChild0()->GetChild0()->GetNType() == ARRAY_DECLARE_STA)
			{
				string NName = visit->GetChild0()->GetChild0()->GetNName();
				///ArrayDec  int a[2]<==[1,2]
				CSyntaxNode* ArrayDec = new CSyntaxNode();
				ArrayDec->CopyNode(pTree);

				if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
				{
					ArrayDec->SetChild0(new CSyntaxNode());
					ArrayDec->GetChild0()->CopyNode(pTree->GetChild0());
					ArrayDec->GetChild0()->SetChild0(visit->GetChild0());

					////待加入对int * a[]<=={&i,&j}的处理
				}
				else
				{
					ArrayDec->SetChild0(visit->GetChild0());

					////待加入对int * a[]<=={&i,&j}的处理
				}

				visit->SetChild0(NULL);

				///2015-4-22 add by wangmeng 加入全局变量
				if (!g_InFunc&&m_GlbSymbl->HasSymbol(NName))
				{
					__InstDec2ST(ArrayDec);
					if (!m_GlbVarTree->MergeTree(ArrayDec,CHOP_STA))
					{
						fout << "__Declr merge tree error!" << endl;
						m_RdcOk = 0;
						return;
					}
				}
				else
				{
				__InsrtAss(ArrayDec);
				}
				///end by wangmeng


				if (previsit == firstvisit)
				{
					previsit->SetChild0(store->GetChild1());
					store->SetChild1(NULL);
					delete store;
					visit = previsit->GetChild0();
					continue;
			}
			else
			{
					previsit->SetChild1(store->GetChild1());
					store->SetChild1(NULL);
					delete store;
					visit = previsit->GetChild1();
					continue;
				}
			}
			else
			{
				temp = new CSyntaxNode();
				temp->CopyTree(visit->GetChild0()->GetChild0());
			
				///2015-4-17 add by wangmeng 右边包含函数调用的情况处理
				__AssRight(visit->GetChild0()->GetChild1(), true);

				if (m_RdcOk == 0)
				{
					return;
				}

				__InsrtAss(visit->GetChild0()); ///插入赋值语句 x<==y 

				visit->SetChild0(temp);
				temp = NULL;
			}
		}
		previsit = store;
		visit = store->GetChild1();
	}

	if (firstvisit->GetChild0() != NULL)
	{
	CSyntaxNode* DecTree = new CSyntaxNode();
	DecTree->CopyNode(pTree);
	DecTree->SetChild0(pTree->GetChild0());
	pTree->SetChild0(NULL);

	    if (g_InFunc)
	    {
	__InsrtAss(DecTree); ///int x,z 插入声明语句
	    }
    	else
    	{		
			firstvisit = DecTree;
			previsit = DecTree;
			visit = DecTree->GetChild0();
			CSyntaxNode *GlbVarTree = NULL;
			CSyntaxNode *vstGlb = NULL;
			while (visit != NULL)
			{				
				if (m_GlbSymbl->HasSymbol(visit->GetSymblName()))
				{
					if (GlbVarTree == NULL)
					{
						GlbVarTree = new CSyntaxNode();
						GlbVarTree->CopyNode(DecTree);
						vstGlb = GlbVarTree;
						if (GlbVarTree->GetNType() == UNSIGN_DECLARATION_STA)
						{
							GlbVarTree->SetChild0(new CSyntaxNode());
							GlbVarTree->GetChild0()->CopyNode(DecTree->GetChild0());
							vstGlb = GlbVarTree->GetChild0();
						}
						vstGlb->SetChild0(new CSyntaxNode(PARAMETER_EXP, VOIDTYPE));
						vstGlb = vstGlb->GetChild0();
					}
					else
					{
						vstGlb->SetChild1(new CSyntaxNode(PARAMETER_EXP, VOIDTYPE));
						vstGlb = vstGlb->GetChild1();
					}

					vstGlb->SetChild0(visit->GetChild0());

					visit->SetChild0(NULL);

					if (previsit == firstvisit)
					{
						previsit->SetChild0(visit->GetChild1());
						visit->SetChild1(NULL);
						delete visit;
						visit = previsit->GetChild0();
						continue;
					}
					else
					{
						previsit->SetChild1(visit->GetChild1());
						visit->SetChild1(NULL);
						delete visit;
						visit = previsit->GetChild1();
						continue;
					}

				}

				previsit = visit;
				visit = visit->GetChild1();
			}			

			if (GlbVarTree != NULL)
			{
				__InstDec2ST(GlbVarTree);
				if (!m_GlbValTree->MergeTree(GlbVarTree, CHOP_STA, VOIDTYPE))
				{
					fout << "__Declr merge tree error 2!" << endl;
					m_RdcOk = 0;
					return;
				}
			}
			
			if (firstvisit->GetChild0() != NULL)
			{
				__InsrtAss(DecTree);
			}
			else
			{
				delete DecTree;
			}
			
    	}


	DecTree = NULL;
	}
	
	m_RdcOk = 1;
	return;

}

/**
* Get present tree of assignment statement: x<==e
* @param the syntax tree to be handled
* @return void
*/
///直接将pTree加入到m_PrgmPrsnt中
///2015-3-9 add by wangmeng
void CMSV::__Ass(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		fout << "__ASS() syntax tree error1!" << endl;
		m_RdcOk = 0;
		return;
	}

	///2015-3-31 add by wangmeng
	///next x<==3
	if (pTree->GetChild0()->GetNType() == NEXT_STA)
	{
		if (pTree->GetChild0()->GetChild0() == NULL)
		{
			fout << "__ASS() syntax tree error2!" << endl;
			m_RdcOk = 0;
			return;
		}
		///temp : x<==e
		CSyntaxNode *temp = new CSyntaxNode(EX_ASS_EXP, VOIDTYPE);
		temp->SetChild0(pTree->GetChild0()->GetChild0());
		temp->SetChild1(pTree->GetChild1());
		pTree->GetChild0()->SetChild0(NULL);
		pTree->SetChild1(NULL);

		if (m_PrgmFuture->MergeTree(temp, AND_EXP, VOIDTYPE))  ///temp 加入到下一状态语法树
		{
			m_More = 1;
			m_RdcOk = 1;

		}
		else
		{
			fout << "__UASS() MergeTree() error!" << endl;
			m_RdcOk = 0;
		}
		return;
	}

	///2015-3-16 modified by wangmeng
	CSyntaxNode* AssTree = new CSyntaxNode(pTree->GetNType(), VOIDTYPE);
	AssTree->SetChild0(pTree->GetChild0());


	///2015-4-17 add by wangmeng 右边包含函数调用的情况处理
	__AssRight(pTree->GetChild1(), true);

	if (m_RdcOk == 0)
	{
		return;
	}
	//add by yubin 2015/4/15,右边是外部函数调用的情况，当前赋值，只能是外部函数调用
		AssTree->SetChild1(pTree->GetChild1());

	///end add by wangmeng 

	pTree->SetChild0(NULL);
	pTree->SetChild1(NULL);
	__InsrtAss(AssTree);
	m_RdcOk = 1;
}

/**
* Get present tree of unit assignment statement: x:=e
* @param the syntax tree to be handled
* @return void
*/
///想法：y:=x+4，这里的x取当前状态值，如果直接将y<==x+4放到下一状态，需要将其放在下一状态对x的赋值的前边.
///这里，我们将由:=得到的<==，与程序中原本存在的<==进行区分。由:=得到的<==其节点类型我们定为EX_ASS_EXP。
///2015-3-9 add by wangmeng
void CMSV::__UAss(CSyntaxNode* pTree)
{
	///x:=e
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		fout << "__UASS() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}

	if (m_More == 0)  ///如果没有下一状态了，就报错。
	{
		fout << "__UASS() empty and next error!" << endl;
		m_RdcOk = 0;
		return;
	}

	CSyntaxNode *temp = NULL;
	//add by yubin 2015/4/15,
	if (pTree->GetChild1()->GetNType() == FUNCTION_STA)//右边是内部函数调用的情况
	{
		temp = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
		__FuncCall(pTree->GetChild1(), 0);

		CSyntaxNode* prsntTree = new CSyntaxNode(EX_ASS_EXP, VOIDTYPE);
		prsntTree->SetChild0(pTree->GetChild0());
		prsntTree->SetChild1(pTree->GetChild1());
		pTree->SetChild0(NULL);
	pTree->SetChild1(NULL);
		__InsrtAss(prsntTree);
		m_More = 0;
		m_RdcOk = 1;
		m_Done = 0;
		return;
			
	}
	else
	{ 
		///temp : x<==e and empty
		temp = new CSyntaxNode(AND_EXP, VOIDTYPE);
		temp->SetChild0(new CSyntaxNode(EX_ASS_EXP, VOIDTYPE));
		temp->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
		temp->GetChild0()->SetChild0(pTree->GetChild0());
		pTree->SetChild0(NULL);


		temp->GetChild0()->SetChild1(pTree->GetChild1());
		pTree->SetChild1(NULL);
	}

		

	if (m_PrgmFuture->MergeTree(temp, AND_EXP, VOIDTYPE))  ///temp 加入到下一状态语法树
	{
		m_More = 1;
		m_RdcOk = 1;
		m_Done = 0;
	}
	else
	{
		fout << "__UASS() MergeTree() error!" << endl;
		m_RdcOk = 0;
	}
	temp = NULL;


}

/**
* Get present tree of empty statement: empty
* @param the syntax tree to be handled
* @return void
*/
///对相应的标记位进行赋值操作。
///2015-3-9 add by wangmeng
void CMSV::__Emp(CSyntaxNode* pTree)
{
	if (m_More == 1)  ///如果有下一状态了，就报错。
	{
		fout << "__Emp() empty and next error!" << endl;
		m_RdcOk = 0;
		return;
	}
	else
	{
		m_More = 0;
		m_RdcOk = 1;
		m_Done = 1;
	}

}

/**
* Get present tree of more statement: more
* @param the syntax tree to be handled
* @return void
*/
///对相应的标记位进行赋值操作。
///2015-3-9 add by yangkai
void CMSV::__More(CSyntaxNode* pTree)
{
	if (m_More == 0)  ///如果没有一状态了，就报错。
	{
		fout << "__More() empty and next error!" << endl;
		m_RdcOk = 0;
		return;
	}
	else
	{
		CSyntaxNode* temp_True = new CSyntaxNode(TRUE_EXP, VOIDTYPE);
		if (m_PrgmFuture->MergeTree(temp_True, AND_EXP, VOIDTYPE))
		{
			m_More = 1;
			m_RdcOk = 1;
		}
		else
		{
			fout << "__More() MergeTree() error!" << endl;
			m_RdcOk = 0;
		}
	}
}

/**
* Get present tree of true statement: true
* @param the syntax tree to be handled
* @return void
*/
///对相应的标记位进行赋值操作。
///2015-3-9 add by yangkai
void CMSV::__True(CSyntaxNode* pTree)
{
	m_RdcOk = 1;
	return;
}

/**
* Get present tree of skip statement: skip
* @param the syntax tree to be handled
* @return void
*/
///empty加入到下一状态语法树中
///2015-3-9 add by wangmeng
void CMSV::__Skip(CSyntaxNode* pTree)
{
	if (m_More == 0)  ///如果没有下一状态了，就报错。
	{
		fout << "__Skip() empty and next error!" << endl;
		m_RdcOk = 0;
		return;
	}
	CSyntaxNode *temp = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);
	if (m_PrgmFuture->MergeTree(temp, AND_EXP, VOIDTYPE)) ///empty加入到下一状态语法树中
	{
		m_More = 1;
		m_RdcOk = 1;
		m_Done = 0;
	}
	else
	{
		fout << "__Skip() MergeTree() error!" << endl;
		m_RdcOk = 0;
	}
	temp = NULL;
}

///add by yubin 2015-3-24,将len(n)变成$$len_i<==0 and while($$len_i<n){skip}
void CMSV::__Len(CSyntaxNode* pTree)
{
	CSyntaxNode *andStat = new CSyntaxNode(AND_EXP, VOIDTYPE);
	CSyntaxNode *assStat = new CSyntaxNode(ASS_EQU_EXP, VOIDTYPE);
	CSyntaxNode *assStatLeft = new CSyntaxNode(IDENT_EXP, "$$len_i", NULL, VOIDTYPE);
	CSyntaxNode *assStatRight = new CSyntaxNode(INTEGER_EXP, 0, INTTYPE);
	assStat->SetChild0(assStatLeft);
	assStat->SetChild1(assStatRight);
	andStat->SetChild0(assStat);

	CSyntaxNode * whileStat = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);//and右边的while语句
	CSyntaxNode * whileCondStat = new CSyntaxNode(LT_EXP, BOOLTYPE);//
	CSyntaxNode *whileCondLeft = new CSyntaxNode(IDENT_EXP, "$$len_i", NULL, VOIDTYPE);
	CSyntaxNode *whileCondRight = pTree->GetChild0();
	whileCondStat->SetChild0(whileCondLeft);
	whileCondStat->SetChild1(whileCondRight);
	whileStat->SetChild0(whileCondStat);
	CSyntaxNode * skipStat = new CSyntaxNode(SKIP_STA, VOIDTYPE);
	whileStat->SetChild1(skipStat);
	andStat->SetChild1(whileStat);//将while语句作为and的右孩子
}

/**
* Get present tree of next statement: next p
* @param the syntax tree to be handled
* @return void
*/
///p加入到下一状态树
///2015-3-9 add by wangmeng
void CMSV::__Next(CSyntaxNode* pTree)
{
	////next p
	if (pTree->GetChild0() == NULL)
	{
		fout << "__Next() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}
	if (m_More == 0) ///如果没有下一状态了，就报错。
	{
		fout << "__Next() empty and next error!" << endl;
		m_RdcOk = 0;
		return;
	}

	///将p加入到下一状态语法树中
	if (m_PrgmFuture->MergeTree(pTree->GetChild0(), AND_EXP, VOIDTYPE))
	{
		pTree->SetChild0(NULL);
		m_More = 1;
		m_RdcOk = 1;
		m_Done = 0;
		return;
	}
	else
	{
		fout << "__Next() MergeTree() error!" << endl;
		m_RdcOk = 0;
		return;
	}

}
/*
///for test add by wangmeng
void CMSV::__And(CSyntaxNode* pTree)
{
if(pTree->GetChild0()==NULL||pTree->GetChild1()==NULL)
{
fout<<"__And() syntax tree error!"<<endl;
m_RdcOk=0;
return;
}
GetPresent(pTree->GetChild0());
if(m_RdcOk==0)
{
fout<<"__And() handle child 0 error!"<<endl;
return;
}
int tmpMore=m_More;
CSyntaxNode *LFutr=m_PrgmFuture->GetRoot();

if(pTree->GetChild1()!=KEEP_STA)
{
m_More=-1;
}

m_RdcOk=-1;
m_PrgmFuture->SetRoot(NULL);

GetPresent(pTree->GetChild1());

//fout<<"__And:"<<m_PrgmFuture->GetRoot()->TreeToString()<<endl;

if(m_More==-1)
{
m_More=tmpMore;
m_PrgmFuture->SetRoot(LFutr);
}
else if(m_More==0)
{
if(tmpMore==1)
{
m_RdcOk=0;
fout<<"__And() empty and next error 1!"<<endl;
return;
}
}
else
{
if(tmpMore==1)
{
m_PrgmFuture->MergeTree(LFutr,AND_EXP,VOIDTYPE);
LFutr=NULL;
}
else if(tmpMore==0)
{
m_RdcOk=0;
fout<<"__And() empty and next error 2!"<<endl;
return;
}
else
{
return;
}
}


}
*/

///added by yangkai
void CMSV::__And(CSyntaxNode* pTree)
{
	m_InAndPal++;//yangkai

	//得到左边的当前状态，（左边含有if语句时,pTree的左子树会变成if语句）
	m_PrgmFuture->SetRoot(NULL);
	GetPresent(pTree->GetChild0());

	if (!m_RdcOk)
	{
		fout << "__And() error 791!" << endl;
		return;
	}

	int tmpKeep = m_Keep;///2015-3-12 add by wangmeng for keep
	m_Keep = 0;  ///2015-3-12 add by wangmeng for keep

	int tmpFin = m_Fin; ///2015-3-12 add by wangmeng for fin
	m_Fin = 0;  ///2015-3-12 add by wangmeng for fin

	int tmpAlw = m_Alw;///2015-3-12 add by wangmeng for alw
	m_Alw = 0; ///2015-3-12 add by wangmeng for alw

	/*可支持frame(x) and frame(y) 这种形式的版本
	int tmpFrm=m_Frm;///2015-3-13 add by wangmeng for frame
	m_Frm=0;///2015-3-13 add by wangmeng for frame
	*/

	CSyntaxNode* leftFutr = m_PrgmFuture->GetRoot();
	m_PrgmFuture->SetRoot(NULL);
	int leftMore = m_More;
	m_More = -1;

	string left_MutlWhile = m_MutlWhile;//yangkai 处理while相关
	m_MutlWhile = "";//yangkai 处理while相关

	//得到右边的当前状态，（右边含有if语句时,pTree的右子树会变成if语句）
	GetPresent(pTree->GetChild1());

	string right_MutlWhile = m_MutlWhile;

	if (left_MutlWhile != "" && right_MutlWhile != "")//yangkai 处理while相关
	{
		m_MutlWhile = left_MutlWhile + " and " + right_MutlWhile;
	}
	else
	{
		m_MutlWhile = "";
	}

	m_Keep += tmpKeep;///2015-3-12 add by wangmeng for keep
	m_Fin += tmpFin; ///2015-3-12 add by wangmeng for fin
	m_Alw += tmpAlw; ///2015-3-12 add by wangmeng for fin

	CSyntaxNode* rightFutr = m_PrgmFuture->GetRoot();
	m_PrgmFuture->SetRoot(NULL);
	int rightMore = m_More;

	//左子树或者右子树是if语句（包括左子树和右子树都是if语句）
	if (pTree->GetChild0()->GetNType() == IF_ELSE_STA || pTree->GetChild1()->GetNType() == IF_ELSE_STA)
	{
		//左边不是if语句，右边是if语句
		if (pTree->GetChild0()->GetNType() != IF_ELSE_STA)
		{
			int test = 1;
			//左边有future部分，则将左边的future部分加上next节点后调用__AndIf函数
			if (leftMore == 1)
			{
				CSyntaxNode* next_leftFutr = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				next_leftFutr->SetChild0(leftFutr);
				pTree->SetChild0(next_leftFutr);//这里会不会有内存泄露

				__AndIf(pTree);
			}
			else if (leftMore == 0)
			{
				delete pTree->GetChild0();
				pTree->SetChild0(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));//这里会不会有内存泄露

				__AndIf(pTree);
			}
			//左边没有future部分，则将整个pTree变为它的右子树
			else if (leftMore == -1)
			{
				pTree->SetNType(IF_ELSE_STA);
				delete pTree->GetChild0();
				pTree->SetChild0(pTree->GetChild1()->GetChild0());
				pTree->GetChild1()->SetChild0(NULL);

				CSyntaxNode* temp_Child_1_1 = pTree->GetChild1()->GetChild1();
				pTree->GetChild1()->SetChild1(NULL);
				CSyntaxNode* temp_Child_1_2 = pTree->GetChild1()->GetChild2();
				pTree->GetChild1()->SetChild2(NULL);

				delete pTree->GetChild1();

				pTree->SetChild1(temp_Child_1_1);
				pTree->SetChild2(temp_Child_1_2);

				m_IfTree = pTree;
			}

		}
		//右边不是if语句，左边是if语句，同上一种情况相反
		else if (pTree->GetChild1()->GetNType() != IF_ELSE_STA)
		{
			if (rightMore == 1)
			{
				CSyntaxNode* next_rightFutr = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				next_rightFutr->SetChild0(rightFutr);
				pTree->SetChild1(next_rightFutr);

				__AndIf(pTree);
			}
			else if (rightMore == 0)
			{
				delete pTree->GetChild1();
				pTree->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));//这里会不会有内存泄露

				__AndIf(pTree);
			}
			else if (rightMore == -1)
			{
				pTree->SetNType(IF_ELSE_STA);
				delete pTree->GetChild1();

				CSyntaxNode* temp_Child_0_0 = pTree->GetChild0()->GetChild0();
				pTree->GetChild0()->SetChild0(NULL);

				pTree->SetChild1(pTree->GetChild0()->GetChild1());
				pTree->GetChild0()->SetChild1(NULL);

				pTree->SetChild2(pTree->GetChild0()->GetChild2());
				pTree->GetChild0()->SetChild2(NULL);

				delete pTree->GetChild0();
				pTree->SetChild0(temp_Child_0_0);

				m_IfTree = pTree;
			}
		}
		//两边都是if语句，直接调用__AndIf相乘
		else
		{
			__AndIf(pTree);
		}

		m_More = 1;
	}
	//左右都不是if语句
	else
	{
		//左边和右边都含有future部分，则将两边的future部分用and节点连接起来作为新的future
		if (leftMore == 1 && rightMore == 1)
		{
			CSyntaxNode* temp_FutrTree = new CSyntaxNode(AND_EXP, VOIDTYPE);
			temp_FutrTree->SetChild0(leftFutr);
			temp_FutrTree->SetChild1(rightFutr);
			m_PrgmFuture->SetRoot(temp_FutrTree);

			m_More = 1;
		}
		//左右都没有future部分
		else if (leftMore == 0 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(NULL);

			m_More = 0;
		}
		//左边有future部分，右边没有规定区间长度
		else if (leftMore == 1 && rightMore == -1)
		{
			m_PrgmFuture->SetRoot(leftFutr);

			m_More = 1;
		}
		//右边有future部分，左边没有规定区间长度
		else if (leftMore == -1 && rightMore == 1)
		{
			m_PrgmFuture->SetRoot(rightFutr);

			m_More = 1;
		}
		//左边没有下一状态，右边没有规定区间长度
		else if (leftMore == 0 && rightMore == -1)
		{
			m_PrgmFuture->SetRoot(NULL);

			m_More = 0;
		}
		//左边没有规定区间长度，右边没有下一状态
		else if (leftMore == -1 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(NULL);

			m_More = 0;
		}
		//两边都没有规定区间长度
		else if (leftMore == -1 && rightMore == -1)
		{
			m_More = -1;
		}
		//左右区间长度不一致，报错，将整个IRTree置为false
		else//(1,0) (0,1)
		{
			fout << "Error interval length" << endl;

			//delete m_IRTree->GetRoot();//出错时将整个IR树设置为false
			//m_IRTree->SetRoot(new CSyntaxNode(FALSE_EXP, VOIDTYPE));

			m_RdcOk = 0;
			return;
		}
	}

	m_RdcOk = 1;
	m_InAndPal--;
}

///2015-3-17 add by yangkai
void CMSV::__Pal(CSyntaxNode* pTree)
{
	m_InAndPal++;//yangkai 2015/3/24

	m_PrgmFuture->SetRoot(NULL);
	GetPresent(pTree->GetChild0());

	if (!m_RdcOk)
	{
		fout << "__Pal() error 1011!" << endl;
		return;
	}

	int tmpKeep = m_Keep;///2015-3-12 add by wangmeng for keep
	m_Keep = 0;  ///2015-3-12 add by wangmeng for keep

	int tmpFin = m_Fin; ///2015-3-12 add by wangmeng for fin
	m_Fin = 0;  ///2015-3-12 add by wangmeng for fin

	int tmpAlw = m_Alw;///2015-3-12 add by wangmeng for alw
	m_Alw = 0; ///2015-3-12 add by wangmeng for alw

	/*可支持frame(x) and frame(y) 这种形式的版本
	int tmpFrm=m_Frm;///2015-3-13 add by wangmeng for frame
	m_Frm=0;///2015-3-13 add by wangmeng for frame
	*/

	CSyntaxNode* leftFutr = m_PrgmFuture->GetRoot();
	m_PrgmFuture->SetRoot(NULL);
	int leftMore = m_More;
	m_More = -1;

	string left_MutlWhile = m_MutlWhile;//yangkai 处理while相关
	m_MutlWhile = "";//yangkai 处理while相关

	GetPresent(pTree->GetChild1());

	string right_MutlWhile = m_MutlWhile;

	if (left_MutlWhile != "" && right_MutlWhile != "")//yangkai 处理while相关
	{
		m_MutlWhile = left_MutlWhile + " || " + right_MutlWhile;
	}
	else
	{
		m_MutlWhile = "";
	}

	m_Keep += tmpKeep;///2015-3-12 add by wangmeng for keep
	m_Fin += tmpFin; ///2015-3-12 add by wangmeng for fin
	m_Alw += tmpAlw; ///2015-3-12 add by wangmeng for fin

	//m_Frm+=tmpFrm; 可支持frame(x) and frame(y) 这种形式的版本

	CSyntaxNode* rightFutr = m_PrgmFuture->GetRoot();
	m_PrgmFuture->SetRoot(NULL);
	int rightMore = m_More;


	if (pTree->GetChild0()->GetNType() == IF_ELSE_STA || pTree->GetChild1()->GetNType() == IF_ELSE_STA)
	{
		if (pTree->GetChild0()->GetNType() != IF_ELSE_STA)
		{
			if (leftMore == 1)
			{
				CSyntaxNode* next_leftFutr = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				next_leftFutr->SetChild0(leftFutr);
				pTree->SetChild0(next_leftFutr);

				__PalIf(pTree);

				m_PrgmPrsnt.clear();
			}
			else
			{
				pTree->SetNType(IF_ELSE_STA);
				delete pTree->GetChild0();
				pTree->SetChild0(pTree->GetChild1()->GetChild0());
				pTree->GetChild1()->SetChild0(NULL);

				CSyntaxNode* temp_Child_1_1 = pTree->GetChild1()->GetChild1();
				pTree->GetChild1()->SetChild1(NULL);
				CSyntaxNode* temp_Child_1_2 = pTree->GetChild1()->GetChild2();
				pTree->GetChild1()->SetChild2(NULL);

				delete pTree->GetChild1();

				pTree->SetChild1(temp_Child_1_1);
				pTree->SetChild2(temp_Child_1_2);

				m_IfTree = pTree;
			}
		}
		else if (pTree->GetChild1()->GetNType() != IF_ELSE_STA)
		{
			if (rightMore == 1)
			{
				CSyntaxNode* next_rightFutr = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				next_rightFutr->SetChild0(rightFutr);
				pTree->SetChild1(next_rightFutr);

				__PalIf(pTree);

				m_PrgmPrsnt.clear();
			}
			else
			{
				pTree->SetNType(IF_ELSE_STA);
				delete pTree->GetChild1();

				CSyntaxNode* temp_Child_0_0 = pTree->GetChild0()->GetChild0();
				pTree->GetChild0()->SetChild0(NULL);

				pTree->SetChild1(pTree->GetChild0()->GetChild1());
				pTree->GetChild0()->SetChild1(NULL);

				pTree->SetChild2(pTree->GetChild0()->GetChild2());
				pTree->GetChild0()->SetChild2(NULL);

				delete pTree->GetChild0();
				pTree->SetChild0(temp_Child_0_0);

				m_IfTree = pTree;
			}
		}
		else
		{
			__PalIf(pTree);
		}

		m_More = 1;
	}
	else
	{
		if (leftMore == -1 || rightMore == -1)
		{
			fout << "Error interval length" << endl;
			m_RdcOk = 0;
			return;
		}
		else if (leftMore == 1 && rightMore == 1)
		{
			//yangkai 处理while相关
			if (leftFutr != NULL && rightFutr != NULL)
			{
				CSyntaxNode* temp_FutrTree = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);
				temp_FutrTree->SetChild0(leftFutr);
				temp_FutrTree->SetChild1(rightFutr);
				m_PrgmFuture->SetRoot(temp_FutrTree);
				m_More = 1;
			}
			else if (leftFutr == NULL && rightFutr == NULL && m_MutlWhile == "")
			{
				fout << "__Pal() error 1296!" << endl;
				m_RdcOk = 0;
				return;
			}
		}
		else if (leftMore == 1 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(leftFutr);
			m_More = 1;
		}
		else if (leftMore == 0 && rightMore == 1)
		{
			m_PrgmFuture->SetRoot(rightFutr);
			m_More = 1;
		}
		else if (leftMore == 0 && rightMore == 0)
		{
			m_PrgmFuture->SetRoot(NULL);
			m_More = 0;
		}
	}

	m_RdcOk = 1;
	m_InAndPal--;
	return;
}

/**
* Get present tree of projection statement: {p1,p2,...}prj q
* @param the syntax tree to be handled
* @return void
*/
///先处理投影左边，再处理投影右边，根据得到的左右两边是否有下一个状态，组成整个投影
///语句的下一个状态。
///2015-3-9 add by wangmeng
void CMSV::__Prj(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild2() == NULL)
	{
		fout << "__Prj() syntax error!" << endl;
		m_RdcOk = 0;
		return;
	}

	///and的时候指向的if树
	CSyntaxNode* tmpIf = m_IfTree;
	m_IfTree = NULL;

	CSyntaxNode* visit = __PrjLeft(pTree); ///handle the left of prj

	if (m_RdcOk == 0 || m_More == -1)
	{
		fout << "__Prj handle left error!" << endl;
		return;
	}

	///record the condition of handling the left of prj
	int LMore = m_More;
	CSyntaxNode* LFutr = m_PrgmFuture->GetRoot();

	CSyntaxNode* LIfTree = m_IfTree;
	m_IfTree = NULL;

	///reset
	m_More = -1;
	m_RdcOk = -1;
	m_PrgmFuture->SetRoot(NULL);

	///handle right of prj
	__PrjRight(pTree, LMore);



	if (m_RdcOk == 0 || m_More == -1)
	{
		fout << "__Prj handle right error!" << endl;
		return;
	}

	CSyntaxNode*RFutr = m_PrgmFuture->GetRoot();

	m_PrgmFuture->SetRoot(NULL);

	/// combine the futures
	CSyntaxNode*Future = __PrjObFutr(pTree, LFutr, RFutr, visit, LMore, m_More);

	m_PrgmFuture->SetRoot(Future);

	LFutr = NULL;
	RFutr = NULL;
	visit = NULL;
	Future = NULL;

}
/**
* Handle left of prj: {p1,p2,...}prj q
* @param the syntax tree to be handled
* @return 投影左子树处理到的结点位置，如果p1 是p and empty形式，而p2有下一状态，则返回p2节点的位置
*/
///处理投影左边，如果投影左边第一项到了最后一个状态，就继续处理后边的项，直到不是最后一个状态，或者
///是处理到投影左边的最后一项
///2015-3-9 add by wangmeng
CSyntaxNode* CMSV::__PrjLeft(CSyntaxNode*pTree)
{
	///{p1,p2,...}prj {q}

	m_InAndPal++;

	//CSyntaxNode* tmpIf=m_IfTree;
	//m_IfTree=NULL;

	///得到p1的present
	GetPresent(pTree->GetChild0());

	///2015-3-12 add by wangmeng for keep,fin,alw
	if ((m_Keep || m_Fin || m_Alw) && m_RdcOk == 1)
	{
		__KpFnAl();
	}


	/* 可支持frame(x) and frame(y) 这种形式的版本
	///2015-3-13 add by wangmeng for frame
	int tmpFrm=m_Frm;
	if(m_Frm && m_RdcOk == 1)
	{
	__AddUnFrm();
	}
	///end add by wangmeng for frame
	*/


	if (m_RdcOk == 0 || m_More == -1)
	{
		fout << "__Prj() handle child 0 error!" << endl;
		return NULL;
	}

	CSyntaxNode * visit = pTree;

	///{p1 and empty, p2 and empty,p3 and next q,...} prj{q}
	///handle p1 and empty, p2 and empty, p3 and next q
	while (m_More == 0 && visit->GetChild1() != NULL)
	{
		m_More = -1;
		m_RdcOk = -1;

		/*可支持frame(x) and frame(y) 这种形式的版本
		///2015-3-13 add by wangmeng for frame
		if(tmpFrm)
		{
		GetPresent(m_PrgmFuture->GetRoot());///当前状态即需要释放unframe
		delete m_PrgmFuture->GetRoot();
		m_PrgmFuture->SetRoot(NULL);
		}
		///end add by wangmeng for frame
		*/
		visit = visit->GetChild1();
		GetPresent(visit->GetChild0());///handle p2 and empty, p3 and next q,...
		///2015-3-12 add by wangmeng for keep,fin,alw
		if ((m_Keep || m_Fin || m_Alw) && m_RdcOk == 1)
		{
			__KpFnAl();
		}

		if (m_RdcOk == 0 || m_More == -1)
		{
			fout << "__Prj() handle child 1 error!" << endl;
			return NULL;
		}

	}

	/*可支持frame(x) and frame(y) 这种形式的版本
	///2015-3-13 add by wangmeng for frame
	if(m_More==0&&tmpFrm)
	{
	GetPresent(m_PrgmFuture->GetRoot());///当前状态即需要释放unframe
	delete m_PrgmFuture->GetRoot();
	m_PrgmFuture->SetRoot(NULL);
	}
	///end add by wangmeng for frame
	*/

	//	m_IfTree=tmpIf;

	///2015-3-31 add by wangmeng
	if (m_IfTree != NULL&&visit->GetChild0()->GetNType() != IF_ELSE_STA)
	{
		delete visit->GetChild0();
		visit->SetChild0(m_IfTree);
	}

	m_InAndPal--;

	return visit;
}

/**
* Handle right of prj: {p1,p2,...}prj q
* @param the syntax tree to be handled
* @return void
*/
void CMSV::__PrjRight(CSyntaxNode*pTree, int LMore)
{
	//CSyntaxNode* tmpIf=m_IfTree;
	//m_IfTree=NULL;

	if (LMore == 0)
	{
		GetPresent(pTree->GetChild2());
	}
	else
	{
		m_InAndPal++;
		GetPresent(pTree->GetChild2());
		m_InAndPal--;

	}

	if (m_RdcOk == 0 || m_More == -1)
	{
		fout << "__Prj handle right error!" << endl;
		return;
	}

	/*if(m_IfTree!=NULL)
	{
	pTree->SetChild2(m_IfTree);
	}*/


	//m_IfTree=tmpIf;


}



/**
* Handle the next state program of prj: {p1,p2,...}prj q
* @param1  处理的投影树{p1,p2,...}prj q
* @param2  投影左边处理后得到的下一状态语法树
* @param3   投影右边处理后得到的下一状态语法树
* @param4   投影左边处理到的节点位置
* @param5   投影左边是否有下一状态
* @param6   投影右边是否有下一状态
* @return  处理以后得到的prj的下一状态语法树
*/
///根据投影左右两边处理的结果，分四种情况，得到整个投影语句的下一状态语法树：
///1.左右两边均有下一状态 2.左边有下一状态，右边没有下一状态
///3.左边没有下一状态，右边有下一状态 4.左右两边均为最后一个状态
///2015-3-9 add by wangmeng
CSyntaxNode* CMSV::__PrjObFutr(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* RFutr, CSyntaxNode* visit, int LMore, int RMore)
{
	///{p1,p2,p3}prj{q}
	CSyntaxNode*Future = NULL;
	if (LMore == 1)
	{
		if (RMore == 1)  /// 投影左边右边均有下一状态
		{
			Future = __PrjLRRmn(pTree, LFutr, RFutr, visit);
		}
		else    ///投影左边有下一状态，右边没有下一状态
		{
			Future = __PrjLRmn(pTree, LFutr, visit);
		}
	}
	else
	{
		if (RMore == 1)///投影左边没有下一状态，右边有下一状态
		{
			Future = RFutr;
		}
		else       /// 投影左边右边均没有下一状态
		{
			Future = NULL;
		}
	}
	return Future;

}


/**
* Handle the next state program of prj when both the left and right have next state
* @param1  处理的投影树{p1,p2,...}prj q
* @param2  投影左边处理后得到的下一状态语法树
* @param3   投影右边处理后得到的下一状态语法树
* @param4   投影左边处理到的节点位置
* @return  处理以后得到的prj的下一状态语法树
*/
///投影左右两边均有下一状态，根据投影左边是不是处理到了最后一项来分两种情况讨论。
///1.如果处理到最后一项，将最后一项的下一状态语法树与右边的下一状态语法树用chop连接，即为下一状态语法树
///2.没有处理到最后一项，假设处理到第n项，则下一状态语法树为：Pnf;{Pn+1,Pn+2,...}prj{Qf}
/// 其中，Pnf为处理到的那一项得到的下一状态语法树，Qf为投影右边的下一状态语法树，Pn+1,Pn+2,...为未处理的项
///2015-3-9 add by wangmeng
CSyntaxNode * CMSV::__PrjLRRmn(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* RFutr, CSyntaxNode* visit)
{
	///2015-3-26 add by wangmeng 处理左右两边有if的情况

	///投影左边是if
	if (visit->GetChild0() != NULL&&visit->GetChild0()->GetNType() == IF_ELSE_STA)
	{
		if (visit->GetChild0()->GetChild0() == NULL || visit->GetChild0()->GetChild1() == NULL)
		{
			m_RdcOk = 0;
			fout << "__PrjLRRmn() if syntax tree error!" << endl;
			return NULL;
		}
		///投影右边是if
		///{if(b1)then{p1}else{p1'},P2,...}prj{if(b2)then{Q1}else{Q2}} ->
		///if(b1)then{if(b2)then{{p1,P2,...}prj{Q1}}else{{p1,P2,...}prj{Q2}}}
		///else{if(b2)then{{p1',P2,...}prj{Q1}}else{{p1',P2,...}prj{Q2}}}

		if (pTree->GetChild2()->GetNType() == IF_ELSE_STA)
		{
			if (pTree->GetChild2()->GetChild0() == NULL || pTree->GetChild2()->GetChild1() == NULL)
			{
				m_RdcOk = 0;
				fout << "__PrjLRRmn() if syntax tree error!" << endl;
				return NULL;
			}
			__PrjLRIf(pTree, visit);
			return NULL;
		}
		else
		{
			__PrjLIf(pTree, RFutr, visit);
			return NULL;
		}
	}
	else
	{
		if (pTree->GetChild2() != NULL&&pTree->GetChild2()->GetNType() == IF_ELSE_STA)
		{
			if (pTree->GetChild2()->GetChild0() == NULL || pTree->GetChild2()->GetChild1() == NULL)
			{
				m_RdcOk = 0;
				fout << "__PrjLRRmn() if syntax tree error!" << endl;
				return NULL;
			}
			__PrjRIf(pTree, LFutr, visit);
			return NULL;
		}
	}

	///end add by wangmeng


	CSyntaxNode* Future = new CSyntaxNode(CHOP_STA, VOIDTYPE);
	Future->SetChild0(LFutr);
	if (visit->GetChild1() == NULL) ///投影左边处理到了最后一部分p3
	{
		Future->SetChild1(RFutr); ///Future: p3f;qf
	}
	else
	{
		Future->SetChild1(visit->GetChild1());
		visit->SetChild1(NULL);
		Future->GetChild1()->SetChild2(RFutr); ///Future: p1f;{p2,p3}prj{qf}
	}
	return Future;
}


/**
* Handle the next state program of prj when both the left and right are if statement
* @param1  处理的投影树{p1,p2,...}prj q
* @param2  投影左边处理到的节点位置
* @return  处理以后得到的prj的下一状态语法树
*/
///{if(b1)then{p1}else{p1'},P2,...}prj{if(b2)then{Q1}else{Q2}} ->
///if(b1)then{if(b2)then{{p1,P2,...}prj{Q1}}else{{p1,P2,...}prj{Q2}}}
///else{if(b2)then{{p1',P2,...}prj{Q1}}else{{p1',P2,...}prj{Q2}}}
///2015-3-26 add by wangmeng
void CMSV::__PrjLRIf(CSyntaxNode* pTree, CSyntaxNode* visit)
{
	///visit->child0: if(b1)then{p1}else{p1'}
	///visit->child1: P2,....
	///pTree->child2: Q

	///thenthenPrj: {p1,P2,...}prj{Q1}
	CSyntaxNode* thenthenPrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
	thenthenPrj->SetChild0(visit->GetChild0()->GetChild1()); ///p1
	thenthenPrj->SetChild1(visit->GetChild1()); ///P2,...
	thenthenPrj->SetChild2(pTree->GetChild2()->GetChild1()); ///Q1

	///thenelsePrj: {p1,P2,P3,...}prj{Q2}
	CSyntaxNode* thenelsePrj = NULL;
	if (pTree->GetChild2()->GetChild2() != NULL)
	{
		thenelsePrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		///p1
		thenelsePrj->SetChild0(new CSyntaxNode());
		thenelsePrj->GetChild0()->CopyTree(visit->GetChild0()->GetChild1());

		///P2,...
		if (visit->GetChild1() != NULL)
		{
			thenelsePrj->SetChild1(new CSyntaxNode());
			thenelsePrj->GetChild1()->CopyTree(visit->GetChild1());
		}
		///Q2
		thenelsePrj->SetChild2(pTree->GetChild2()->GetChild2());
	}

	///thenPrj: if(b2)then{{p1,P2,...}prj{Q1}}else{{p1,P2,...}prj{Q2}}
	CSyntaxNode* thenPrj = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	thenPrj->SetChild0(pTree->GetChild2()->GetChild0());
	thenPrj->SetChild1(thenthenPrj);
	thenPrj->SetChild2(thenelsePrj);

	CSyntaxNode* elsePrj = NULL;
	if (visit->GetChild2() != NULL)
	{
		///elsethenPrj: {p1',P2,P3,...}prj{Q1}
		CSyntaxNode* elsethenPrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		///p1
		elsethenPrj->SetChild0(visit->GetChild0()->GetChild2());
		///P2,...
		if (visit->GetChild1() != NULL)
		{
			elsethenPrj->SetChild1(new CSyntaxNode());
			elsethenPrj->GetChild1()->CopyTree(visit->GetChild1());
		}

		///Q1
		elsethenPrj->SetChild2(new CSyntaxNode());
		elsethenPrj->GetChild2()->CopyTree(pTree->GetChild2()->GetChild1());

		CSyntaxNode* elseelsePrj = NULL;
		if (pTree->GetChild2()->GetChild2() != NULL)
		{
			///elseelsePrj: {p1',P2,P3,...}prj{Q2}
			elseelsePrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
			///p1
			elseelsePrj->SetChild0(new CSyntaxNode());
			elseelsePrj->GetChild0()->CopyTree(visit->GetChild0()->GetChild2());
			///P2,...
			if (visit->GetChild1() != NULL)
			{
				thenelsePrj->SetChild1(new CSyntaxNode());
				thenelsePrj->GetChild1()->CopyTree(visit->GetChild1());
			}
			///Q2
			elseelsePrj->SetChild2(new CSyntaxNode());
			elseelsePrj->GetChild2()->CopyTree(pTree->GetChild2()->GetChild2());
		}

		///elsePrj: if(b2)then{{p1',P2,...}prj{Q1}}else{{p1',P2,...}prj{Q2}}
		elsePrj = pTree->GetChild2();
		elsePrj->SetChild0(new CSyntaxNode());
		elsePrj->GetChild0()->CopyTree(pTree->GetChild2()->GetChild0());
		elsePrj->SetChild1(elsethenPrj);
		elsePrj->SetChild2(elseelsePrj);
		pTree->SetChild2(NULL);
	}

	///
	CSyntaxNode*Ifbool = visit->GetChild0()->GetChild0();
	visit->GetChild0()->SetChild0(NULL);
	visit->GetChild0()->SetChild1(NULL);
	visit->GetChild0()->SetChild2(NULL);
	visit->SetChild1(NULL);


	///pTree:if(b1)then{if(b2)then{{p1,P2,...}prj{Q1}}else{{p1,P2,...}prj{Q2}}}
	///       else{if(b2)then{{p1',P2,...}prj{Q1}}else{{p1',P2,...}prj{Q2}}}
	pTree->SetNType(IF_ELSE_STA);
	delete pTree->GetChild0();
	delete pTree->GetChild1();
	delete pTree->GetChild2();

	pTree->SetChild0(Ifbool);
	pTree->SetChild1(thenPrj);
	pTree->SetChild2(elsePrj);

	m_IfTree = pTree;
}

/**
* Handle the next state program of prj when only the left is if statement
* @param1  处理的投影树{p1,p2,...}prj q
* @param2   投影右边处理后得到的下一状态语法树
* @param3  投影左边处理到的节点位置
* @return  处理以后得到的prj的下一状态语法树
*/
///{if(b1)then{p1}else{p1'},P2,...}prj{Q} ->
///if(b1)then{{p1,P2,...}prj{Q}}else{{p1',P2,...}prj{Q}}
///2015-3-26 add by wangmeng
void CMSV::__PrjLIf(CSyntaxNode* pTree, CSyntaxNode* RFutr, CSyntaxNode* visit)
{
	///visit->child0: if(b1)then{p1}else{p1'}
	///visit->child1: P2,....
	///pTree->child2: Q

	///thenPrj: {p1,P2,...}prj{Q}
	CSyntaxNode* thenPrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
	thenPrj->SetChild0(visit->GetChild0()->GetChild1());
	thenPrj->SetChild1(visit->GetChild1());

	CSyntaxNode* RPrj = new CSyntaxNode(NEXT_STA, VOIDTYPE);
	RPrj->SetChild0(RFutr);
	thenPrj->SetChild2(RPrj);

	CSyntaxNode* elsePrj = NULL;
	if (visit->GetChild2() != NULL)
	{
		///elsePrj: {p1',P2,...}prj{Q}
		elsePrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		elsePrj->SetChild0(visit->GetChild0()->GetChild2());
		if (visit->GetChild1() != NULL)
		{
			elsePrj->SetChild1(new CSyntaxNode());
			elsePrj->GetChild1()->CopyTree(visit->GetChild1());
		}

		elsePrj->SetChild2(new CSyntaxNode());
		elsePrj->GetChild2()->CopyTree(RPrj);
		pTree->SetChild2(NULL);
	}

	///
	CSyntaxNode*Ifbool = visit->GetChild0()->GetChild0();
	visit->GetChild0()->SetChild0(NULL);
	visit->GetChild0()->SetChild1(NULL);
	visit->GetChild0()->SetChild2(NULL);
	visit->SetChild1(NULL);

	///pTree:if(b1)then{if(b2)then{{p1,P2,...}prj{Q1}}else{{p1,P2,...}prj{Q2}}}
	///       else{if(b2)then{{p1',P2,...}prj{Q1}}else{{p1',P2,...}prj{Q2}}}
	pTree->SetNType(IF_ELSE_STA);
	delete pTree->GetChild0();
	delete pTree->GetChild1();
	delete pTree->GetChild2();

	pTree->SetChild0(Ifbool);
	pTree->SetChild1(thenPrj);
	pTree->SetChild2(elsePrj);

	m_IfTree = pTree;
}

/**
* Handle the next state program of prj when only the right is if statement
* @param1  处理的投影树{p1,p2,...}prj q
* @param2  投影左边处理后得到的下一状态语法树
* @param3  投影左边处理到的节点位置
* @return  处理以后得到的prj的下一状态语法树
*/
///{P1,P2,...}prj{if(b1)then{Q1}else{Q2}} ->
///if(b1)then{{P1,P2,...}prj{Q1}}else{{P1,P2,...}prj{Q2}}
///2015-3-26 add by wangmeng
void CMSV::__PrjRIf(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* visit)
{
	///visit->child0: P1
	///visit->child1: P2,....
	///pTree->child2: if(b1)then{Q1}else{Q2}

	///thenPrj: {P1,P2,...}prj{Q1}
	CSyntaxNode* thenPrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);

	CSyntaxNode* LPrj = new CSyntaxNode(NEXT_STA, VOIDTYPE);
	LPrj->SetChild0(LFutr);
	thenPrj->SetChild0(LPrj);
	thenPrj->SetChild1(visit->GetChild1());
	thenPrj->SetChild2(pTree->GetChild2()->GetChild1());

	CSyntaxNode* elsePrj = NULL;
	if (pTree->GetChild2()->GetChild2() != NULL)
	{
		///elsePrj: {P1,P2,...}prj{Q2}
		elsePrj = new CSyntaxNode(PROJECTION_STA, VOIDTYPE);
		elsePrj->SetChild0(new CSyntaxNode());
		elsePrj->GetChild0()->CopyTree(LPrj);
		if (visit->GetChild1() != NULL)
		{
			elsePrj->SetChild1(new CSyntaxNode());
			elsePrj->GetChild1()->CopyTree(visit->GetChild1());
		}
		elsePrj->SetChild2(pTree->GetChild2()->GetChild2());
	}

	///
	CSyntaxNode*Ifbool = pTree->GetChild2()->GetChild0();
	visit->SetChild1(NULL);
	pTree->GetChild2()->SetChild0(NULL);
	pTree->GetChild2()->SetChild1(NULL);
	pTree->GetChild2()->SetChild2(NULL);

	///pTree:if(b1)then{if(b2)then{{p1,P2,...}prj{Q1}}else{{p1,P2,...}prj{Q2}}}
	///       else{if(b2)then{{p1',P2,...}prj{Q1}}else{{p1',P2,...}prj{Q2}}}
	pTree->SetNType(IF_ELSE_STA);
	delete pTree->GetChild0();
	delete pTree->GetChild1();
	delete pTree->GetChild2();

	pTree->SetChild0(Ifbool);
	pTree->SetChild1(thenPrj);
	pTree->SetChild2(elsePrj);

	m_IfTree = pTree;

}

/**
* Handle the next state program of prj when only the left has next state
* @param1  处理的投影树{p1,p2,...}prj q
* @param2  投影左边处理后得到的下一状态语法树
* @param3   投影左边处理到的节点位置
* @return  处理以后得到的prj的下一状态语法树
*/
///投影左边有下一状态，投影右边没有下一状态
///根据投影左边是否处理到最后一项分为两种情况
///1.处理到了最后一项，左边的下一状态语法树，就是整个投影的下一状态语法树
///2.未处理到最后一项，假设处理到第n项，则整个投影的下一状态语法树为Pnf;Pn+1;Pn+2;...
/// 其中，Pnf为处理到的那一项的到的下一状态语法树，Pn+1,Pn+2,...为未处理到的项
///2015-3-9 add by wangmeng
CSyntaxNode * CMSV::__PrjLRmn(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* visit)
{
	m_More = 1;

	if (visit->GetChild1() == NULL) ///投影左边处理到了最后一部分,投影右边处理结束
	{
		///visit->child0: if(b)then{P}else{Q}
		if (visit->GetChild0() != NULL&&visit->GetChild0()->GetNType() == IF_ELSE_STA)
		{
			CSyntaxNode* boolPrt = visit->GetChild0();
			CSyntaxNode* thenPrt = visit->GetChild1();
			CSyntaxNode* elsePrt = visit->GetChild2();
			visit->SetChild0(NULL);
			visit->SetChild1(NULL);
			visit->SetChild2(NULL);
			delete pTree->GetChild0();
			delete pTree->GetChild1();
			delete pTree->GetChild2();
			pTree->SetNType(IF_ELSE_STA);
			pTree->SetChild0(boolPrt);
			pTree->SetChild1(thenPrt);
			pTree->SetChild2(elsePrt);
			m_IfTree = pTree;
			return NULL;
		}
		return LFutr;
	}
	else
	{
		/// 投影左边还有没处理的部分。如{p1,p2,p3,p4}prj{empty} 只处理了p1，后边p2，p3，p4均未处理.
		CSyntaxNode* Future = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		if (visit->GetChild0() != NULL&&visit->GetChild0()->GetNType() == IF_ELSE_STA)
		{
			Future->SetChild0(visit->GetChild0());
			visit->SetChild0(NULL);
		}
		else
		{
			Future->SetChild0(LFutr); /// Future: p1f;NULL
		}


		///previsit: p1,p2,p3,p4  visit: p2,p3,p4
		CSyntaxNode*previsit;
		CSyntaxNode*tmpvisit = visit;
		previsit = visit;
		visit = visit->GetChild1();

		///Future:  p1f;p2
		if (visit->GetChild1() == NULL)
		{
			Future->SetChild1(visit->GetChild0());
			visit->SetChild0(NULL);

		}
		else ///Future:  p1f;p2;p3;...
		{
			Future->SetChild1(visit); ///Future:  p1f;p2,p3,p4
			previsit->SetChild1(NULL);

			while (visit->GetChild1() != NULL) ///Future: p1f;p2;p3,p4
			{
				visit->SetNType(CHOP_STA);
				previsit = visit;
				visit = visit->GetChild1();
			}
			previsit->SetChild1(visit->GetChild0()); ///Future: p1f;p2;p3;p4
			visit->SetChild0(NULL);
			delete visit;
			visit = NULL;
		}

		///visit->child0: if(b)then{P}else{Q}
		if (tmpvisit->GetChild0() != NULL&&tmpvisit->GetChild0()->GetNType() == IF_ELSE_STA)
		{
			if (m_InAndPal > 0)
			{
				__ChopIf(Future);
				m_IfTree = Future;
				return NULL;
			}
		}
		return Future;
	}
}

/**
* Get present tree of chop statement: p;q
* @param the syntax tree to be handled
* @return void
*/
///p;q
///首先处理p，如果是最后一个状态，处理q，如果不是最后一个状态pf;q为下一状态语法树，其中pf为处理p得到的
///下一状态语法树
///这里加入了对keep，fin，alw的处理，在处理完p后，要看p中是否含有这些语句，如果有，则进行相应的处理，因为
///这里已经的到了p是否是到达了最后一个状态，所以可以对keep，fin，alw这些语句处理
///2015-3-10 add by wangmeng
void CMSV::__Chop(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		fout << "__Chop() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}

	///处理chop左边
	GetPresent(pTree->GetChild0());

	///2015-3-12 add by wangmeng for keep,fin,alw
	if ((m_Keep || m_Fin || m_Alw) && m_RdcOk == 1)
	{
		__KpFnAl();
	}

	/* 可支持frame(x) and frame(y) 这种形式的版本
	///2015-3-13 add by wangmeng for frame
	int tmpFrm=m_Frm;
	if(m_Frm && m_RdcOk == 1)
	{
	__AddUnFrm();
	}
	///end add by wangmeng for frame
	*/

	if (m_RdcOk == 0 || m_More == -1)///处理左边出现错误，直接返回
	{
		fout << "__Chop() handles child 0 error!" << endl;
		return;
	}

	if (m_More == 0) ///处理左边到最后一个状态，处理右边
	{
		m_More = -1;
		m_RdcOk = -1;

		/*可支持frame(x) and frame(y) 这种形式的版本
		///2015-3-13 add by wangmeng for frame
		if(tmpFrm)
		{
		GetPresent(m_PrgmFuture->GetRoot());///当前状态即需要释放unframe
		delete m_PrgmFuture->GetRoot();
		m_PrgmFuture->SetRoot(NULL);
		}
		///end add by wangmeng for frame
		*/

		GetPresent(pTree->GetChild1());

		//右边是if语句则将整个程序变为右边的if语句
		if (pTree->GetChild1()->GetNType() == IF_ELSE_STA)
		{
			//yangkai 这里区分是不是在and和pal中主要是为了提高效率
			if (m_InAndPal > 0)
			{
				CSyntaxNode* temp_Child_1 = pTree->GetChild1();
				pTree->SetChild1(NULL);
				pTree->CopyTree(temp_Child_1);
			}
			else
			{
				m_RdcOk = 1;
			}
		}
		else if (pTree->GetChild1()->GetNType() == WHILE_DO_STA)
		{
			m_RdcOk = 1;
		}
	}
	else if (m_More == 1)
	{
		//yangkai 如果chop的左边是if语句并且当前的chop语句处于and语句中，则调用__ChopIf()函数
		if (pTree->GetChild0()->GetNType() == IF_ELSE_STA && m_InAndPal > 0)
		{

			__ChopIf(pTree);

			m_IfTree = pTree;
		}
		///p;q  处理p得到pf， m_PrgmFuture:  pf;q
		else if (m_PrgmFuture->MergeTree(pTree->GetChild1(), CHOP_STA, VOIDTYPE))
		{
			pTree->SetChild1(NULL);
			m_RdcOk = 1;
		}
		else
		{
			fout << "__Chop() MergeTree() error!" << endl;
			m_RdcOk = 0;
		}
	}
	else if (m_More == -1)
	{
		fout << "__Chop():Interval length error!" << endl;
		m_RdcOk = 0;
	}
}

void CMSV::__If(CSyntaxNode* pTree)
{
	CSyntaxNode* temp_IRTree = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);
	temp_IRTree->SetChild0(pTree->GetChild0());
	pTree->SetChild0(NULL);

	///2015-3-20 add by wangmeng 保存一下符号表，在处理else的时候需要恢复。
	SymbolTable* tmpNFrm = new SymbolTable();
	tmpNFrm->CopyTable(m_NFrmSymbl);
	SymbolTable* tmpFrm = new SymbolTable();
	tmpFrm->CopyTable(m_FrmSymbl);
	///end add by wangmeng

	m_More = -1;
	m_Program->SetRoot(pTree->GetChild1());
	pTree->SetChild1(NULL);

	m_IRTree->SetRoot(NULL);

	///2015-3-24 add by wangmeng 保存m_FreeFrm的值
	int tmpFreeFrm = m_FreeFrm;
	m_FreeFrm = 0;
	///end add by wangmeng

	Pre();



	temp_IRTree->SetChild1(m_IRTree->GetRoot());
	//pTree->SetChild1(m_IRTree->GetRoot());

	///2015-3-20 add by wangmeng 恢复符号表
	if (m_NFrmSymbl != NULL)
	{
		delete m_NFrmSymbl;
	}
	if (m_FrmSymbl != NULL)
	{
		delete m_FrmSymbl;
	}

	m_NFrmSymbl = tmpNFrm;
	m_FrmSymbl = tmpFrm;
	///end add by wangmeng

	m_FreeFrm = 0;
	m_More = -1;
	m_Program->SetRoot(pTree->GetChild2());
	pTree->SetChild2(NULL);

	m_IRTree->SetRoot(NULL);
	Pre();
	temp_IRTree->SetChild2(m_IRTree->GetRoot());

	delete pTree;
	pTree = NULL;

	m_IRTree->SetRoot(temp_IRTree);



	///恢复m_FreeFrm的值
	m_FreeFrm = tmpFreeFrm;

	m_More = 0;
	m_RdcOk = 1;

	delete m_Program->GetRoot();
	m_Program->SetRoot(NULL);
}

///
void CMSV::__While(CSyntaxNode* pTree)
{
	CSyntaxNode* temp_IRTree = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);
	temp_IRTree->SetChild0(pTree->GetChild0());
	pTree->SetChild0(NULL);

	m_More = -1;
	m_Program->SetRoot(pTree->GetChild1());
	pTree->SetChild1(NULL);

	m_IRTree->SetRoot(NULL);
	Pre();

	temp_IRTree->SetChild1(m_IRTree->GetRoot());

	delete pTree;
	pTree = NULL;

	m_IRTree->SetRoot(temp_IRTree);

	m_More = 0;
	m_RdcOk = 1;

	delete m_Program->GetRoot();
	m_Program->SetRoot(NULL);
}

///2015-3-18 add by yubin,处理for语句：for(p1,b,p2){q}-->p1;while(b){q;p2}
void CMSV::__For(CSyntaxNode* pTree)
{
	CSyntaxNode* initNode = pTree->GetChild0()->GetChild0();//获得for语句的初始条件
	CSyntaxNode* conditionNode = pTree->GetChild0()->GetChild1();//获得for语句的判断条件
	CSyntaxNode* controlNode = pTree->GetChild0()->GetChild2();//获得for语句的控制语句
	CSyntaxNode* forStatNode = pTree->GetChild0()->GetChild2();//获得for语句的真正的内容
	CSyntaxNode* trsltToStatement = new CSyntaxNode(CHOP_STA, VOIDTYPE);//转成之后的最上层的chop语句
	trsltToStatement->SetChild0(initNode);
	CSyntaxNode* whileStatement = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);//转成之后的while语句
	whileStatement->SetChild0(conditionNode);
	CSyntaxNode* whileChopStatement = new CSyntaxNode(CHOP_STA, VOIDTYPE);//while语句中的chop语句
	whileChopStatement->SetChild0(forStatNode);
	whileChopStatement->SetChild1(controlNode);
	whileStatement->SetChild1(whileChopStatement);
	trsltToStatement->SetChild1(whileStatement);
	GetPresent(pTree);//处理完之后，是不是应该在调用一次GetPresent
	return;
}

///2015-3-18 add by yubin,处理switch语句，将switch语句变成if语句
void CMSV::__Switch(CSyntaxNode* pTree)
{
	vector <CSyntaxNode*> ifStatVec;//存储if语句，最后在一起变成if_else语句
	CSyntaxNode* switchCondition = new CSyntaxNode();
	switchCondition->CopyTree(pTree->GetChild0());//switch语句的判断条件，这一句放在最开始，因为switch的条件是不会变的
	CSyntaxNode* caseStat = new CSyntaxNode();
	caseStat->CopyTree(pTree->GetChild1());//第一条case语句


	while (caseStat != NULL)
	{
		if (caseStat->GetNType() == CASE_STA)
		{
			CSyntaxNode* IfStat = new CSyntaxNode(IF_ELSE_STA, VOIDTYPE);//最上层的if语句
			CSyntaxNode* IfCondtionStat = new CSyntaxNode(EQU_EXP, pTree->GetChild0(), caseStat->GetChild0(), BOOLTYPE);
			IfStat->SetChild0(IfCondtionStat);//设置if的判断条件
			CSyntaxNode* chopStat = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			chopStat->SetChild0(new CSyntaxNode());
			chopStat->GetChild0()->CopyTree(caseStat->GetChild1());
			CSyntaxNode* tmpcaseStat = caseStat;

			while (tmpcaseStat->GetChild2() != NULL && tmpcaseStat->GetChild2()->GetRType() == VOIDTYPE)//不是break语句，需要继续添加到chop语句
			{
				tmpcaseStat = tmpcaseStat->GetChild2()->GetChild0();
				CSyntaxNode* tmpChopStat = new CSyntaxNode(CHOP_STA, VOIDTYPE);
				tmpChopStat->SetChild0(new CSyntaxNode());
				if (tmpcaseStat->GetNType() == DEFAULT_STA)//add by yubin 2015/4/10增加default情况
				{
					tmpChopStat->GetChild0()->CopyTree(tmpcaseStat->GetChild0());
				}
				else//case的情况
				{
				tmpChopStat->GetChild0()->CopyTree(tmpcaseStat->GetChild1());
				}
				CSyntaxNode* tmpNode = chopStat;
				while (tmpNode->GetChild1() != NULL)
				{
					tmpNode = tmpNode->GetChild1();
				}
				tmpNode->SetChild1(tmpChopStat);
			}
			tmpcaseStat = chopStat;
			while (tmpcaseStat->GetChild1() != NULL)
			{
				tmpcaseStat = tmpcaseStat->GetChild1();
			}

			CSyntaxNode* tmp = tmpcaseStat->GetChild0();
			tmpcaseStat->SetChild0(NULL);
			tmpcaseStat->CopyTree(tmp);//将最后一个chop的孩子0放在父节点的孩子1,不能直接tmpcaseStat->CopyTree(tmpcaseStat->GetChild0())
			tmp = NULL;

			IfStat->SetChild1(chopStat);//设置if语句的then的内容
			ifStatVec.push_back(IfStat);//将if语句压入vector中
		}
		else//default语句
		{
			CSyntaxNode* defaultStat = new CSyntaxNode();
			defaultStat->CopyTree(caseStat->GetChild0());
			ifStatVec.push_back(defaultStat);//将if语句压入vector中
		}
		if (caseStat->GetChild2() != NULL)
		{
			caseStat = caseStat->GetChild2()->GetChild0();//将case语句变成下一条case语句
		}
		else
			break;
	}

	//下面开始将vector中的if语句，组装成最后的if_else语句
	CSyntaxNode* totalIfStat = ifStatVec.front();//最上层的if语句

	vector <CSyntaxNode*>::iterator iter = ifStatVec.begin();
	iter++;
	CSyntaxNode* tmpTotalIfStat = totalIfStat;
	for (; iter != ifStatVec.end(); iter++)
	{
		tmpTotalIfStat->SetChild2(*iter);
		tmpTotalIfStat = tmpTotalIfStat->GetChild2();
	}
	pTree = totalIfStat;//讲总的if语句赋给pTree
	GetPresent(pTree);//将switch语句变成if语句后，再调用GetPresent函数
}

/* 可支持frame(x) and frame(y) 这种形式的版本
///2015-3-13 add by wangmeng
void CMSV::__Frm(CSyntaxNode* pTree)
{
if(__SearchAndInsert(UNDEFINE,pTree))
{
CSyntaxNode * UnFrame=new CSyntaxNode(UNFRAME_STA,VOIDTYPE);

UnFrame->SetChild0(pTree->GetChild0());
pTree->SetChild0(NULL);
m_UnFrm.push(UnFrame);
m_RdcOk=1;
m_Frm++;
return;
}
else
{
m_RdcOk=0;
fout<<"__Frm() insert symbol error!"<<endl;
return;
}
}*/

/**
* Get present tree of frame statement: frame(c)
* @param the syntax tree to be handled
* @return void
*/
/// frame(c) and P
/// 1.先处理c，将c中包含的符号加入到m_FrmSymbl符号表中，如果符号表中已经有该变量，
/// 其标记frame次数的变量m_nFrm加1.
/// 2.处理P
/// 3.(1)如果有下一状态，unframe(c) and Pf加入到下一状态，其中Pf是处理P得到的下一状态
///   (2)没有下一状态，将unframe(c)加入到栈m_PreUnFrm中，准备释放
///2015-3-16 add by wangmeng
void CMSV::__Frm(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL || pTree->GetChild2() == NULL)
	{
		fout << "__Frm() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}


	if (__SearchAndInsert(UNDEFINE, pTree)) ///加入符号表
	{
		CSyntaxNode * UnFrame = new CSyntaxNode(UNFRAME_STA, VOIDTYPE);

		UnFrame->SetChild0(pTree->GetChild0());
		UnFrame->SetChild1(pTree->GetChild1());
		pTree->SetChild0(NULL);
		pTree->SetChild1(NULL);


		GetPresent(pTree->GetChild2()); ///frame(x) and P 处理P

		///2015-3-12 add by wangmeng for keep,fin,alw
		if ((m_Keep || m_Fin || m_Alw) && m_RdcOk)
		{
			__KpFnAl();
		}

		if (m_RdcOk == 0 || m_More == -1)
		{
			fout << "__Frm() handle child 2 error!" << endl;
			return;
		}

		if (m_More == 1) ///有下一状态，将unframe(x) and Pf加入到下一状态
		{
			UnFrame->SetChild2(m_PrgmFuture->GetRoot());
			m_PrgmFuture->SetRoot(UnFrame);
		}
		else
		{
			if (m_More == 0)///没有下一状态，UnFrame压入栈中，准备释放符号表
			{
				m_PreUnFrm.push(UnFrame);
				m_FreeFrm = 1;
			}

		}

		m_RdcOk = 1;
		//m_Frm++;
		return;
	}
	else
	{
		m_RdcOk = 0;
		fout << "__Frm() insert symbol error!" << endl;
		return;
	}

}

/**
* Get present tree of unframe statement: unframe(c)
* @param the syntax tree to be handled
* @return void
*/
/// unframe(c) and P
/// 1.处理P
/// 2.(1)如果有下一状态，unframe(c) and Pf加入到下一状态，其中Pf是处理P得到的下一状态
///   (2)没有下一状态，将unframe(c)加入到栈m_PreUnFrm中，准备释放
///2015-3-16 add by wangmeng
void CMSV::__UnFrm(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL/*||pTree->GetChild2()==NULL*/)
	{
		fout << "__UnFrm() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}


	CSyntaxNode * UnFrame = new CSyntaxNode(UNFRAME_STA, VOIDTYPE);

	UnFrame->SetChild0(pTree->GetChild0());
	UnFrame->SetChild1(pTree->GetChild1());
	pTree->SetChild0(NULL);
	pTree->SetChild1(NULL);

	if (pTree->GetChild2() == NULL)////在if语句是frame的最后一条语句时，会出现这种情况
	{
		m_PreUnFrm.push(UnFrame);
		m_FreeFrm = 1;
		m_More = 0;
		m_RdcOk = 1;
		return;
	}

	GetPresent(pTree->GetChild2()); ///frame(x) and P 处理P

	///2015-3-12 add by wangmeng for keep,fin,alw
	if ((m_Keep || m_Fin || m_Alw) && m_RdcOk == 1)
	{
		__KpFnAl();
	}


	if (m_RdcOk == 0 || m_More == -1)
	{
		fout << "__UnFrm() handle child 2 error!" << endl;
		return;
	}

	if (m_More == 1) ///有下一状态，将unframe(x) and Pf加入到下一状态
	{
		UnFrame->SetChild2(m_PrgmFuture->GetRoot());
		m_PrgmFuture->SetRoot(UnFrame);
	}
	else
	{
		if (m_More == 0) ///没有下一状态，UnFrame压入栈中，准备释放符号表
		{
			m_PreUnFrm.push(UnFrame);
			m_FreeFrm = 1;
		}
	}

	m_RdcOk = 1;
	return;
}


///
void CMSV::__Or(CSyntaxNode* pTree)
{

}

//2015-3-16,add by yubin,处理存在变量
//注意，修改了语法分析的inner_optional_define_identifier，以前的语法分析识别不了exists x:{x<==1 and skip},以前只能识别exists x,int y:{x<==1 and skip},
void CMSV::__Ext(CSyntaxNode* pTree)
{
	CSyntaxNode* p = pTree;
	while (p != NULL)
	{
		//遇见一个变量，先在自己定义的map中的查找，这个map的作用是存储每个变量已经出现过的exist的次数
		string varName = p->GetChild0()->GetNName();
		map < string, int >::iterator iter;
		iter = existVarTimes.find(varName);
		if (iter == existVarTimes.end())
		{
			//如果没有找到该变量，表明该变量之前没有出现过exist，将该变量的次数变成1，并且插入到map中
			existVarTimes.insert(make_pair(varName, 1));
			iter = existVarTimes.find(varName);
		}
		else
		{
			iter->second = iter->second + 1;//如果出现过，就将次数加1
		}
		char* tmpNam = (char*)malloc(2);
		itoa(iter->second, tmpNam, 10);//将次数变成字符串的形式，然后调用replaceExistName函数进行函数名替换
		pTree->replaceExistName(iter->first, currFunc, tmpNam);//如果是全局变量，函数名写成空

		p = p->GetChild1();
	}
	pTree = pTree->GetChild2();//将exist x:{p}中的p内容付给pTree，然后再调用getPresent函数
	GetPresent(pTree);
}

bool CMSV::__isRValue(CSyntaxNode* pTree)
{
	CSyntaxNode* tmp = pTree->GetChild0();
	while (tmp != NULL)
	{
		if (tmp->GetNName() == "RValue")
		{
			tmp->SetNName("");
			return 1;
		}
		tmp = tmp->GetChild0();
	}
	return 0;
}
///2015-3-16,add by yubin,处理函数定义
void CMSV::__FuncDef(CSyntaxNode* pTree)
{
	bool existRValue = false;
	CSyntaxNode *tmpExtNode = new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(IDENT_EXP, "$$Ext", NULL, VOIDTYPE), INTTYPE);
	tmpExtNode->SetChild1(pTree->GetChild0());
	pTree->SetChild0(tmpExtNode);//将$$Ext变量添加到形参列表的最开始
	currFunc = pTree->GetNName();//获得函数名，付给currFunc变量，用于存在量词的变名
	m_Program->SetRoot(pTree->GetChild1());
	m_More = -1;

	///2015-4-15 add by wangmeng函数的参数加入到符号表中
	CSyntaxNode* VstPar = pTree->GetChild0();

	while (VstPar != NULL)
	{
		SymbolType type;
		type = SymbolTable::GetSymbolType(VstPar->GetRType());
		string name = "";
		if (VstPar->GetRType() == STRUCTTYPE)
			name = VstPar->GetNName();

		if (!__SearchAndInsert(type, VstPar->GetChild0(), name, true))
		{
			fout << "__FuncDef(: declaration of parameter : " << VstPar->TreeToString().data() << endl;
			m_RdcOk = 0;
			return;
		}
		VstPar = VstPar->GetChild1();
	} 
	
	///end 2015-4-15 add by wangmeng

	CSyntaxNode *paraList = pTree->GetChild0();//获得函数的形参列表
	while (paraList->GetChild1() != NULL)
	{
		//if ((paraList->GetChild1()->GetChild0()->GetNName() == "RValue") && (paraList->GetChild1()->GetChild1() == NULL))
		if ((__isRValue(paraList->GetChild1())) && (paraList->GetChild1()->GetChild1() == NULL))
		{
			CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, paraList->GetChild1(), VOIDTYPE);
			paraList->SetChild1(NULL);
			retNode->SetChild1(pTree->GetChild0());
			pTree->SetChild0(retNode);
			existRValue = 1;
			break;
		}
		else if ((__isRValue(paraList->GetChild1())) && (paraList->GetChild1()->GetChild1() != NULL))
		{
			fout << "In function" << pTree->GetNName() << " definition, RValue must be the last parameter!!!" << endl;
			exit(1);
		}
		paraList = paraList->GetChild1();
	}
	if (existRValue == false)
	{
		CSyntaxNode* retNode = new CSyntaxNode(FUNC_RETURN_TYPE, VOIDTYPE);
		retNode->SetChild1(pTree->GetChild0());
		pTree->SetChild0(retNode);
	}

	//将函数体转成IR树
	Pre();
	//添加最后一个状态的输出
	CSyntaxNode* output = __OutputState();
	m_IRTree->MergeTree(output, CHOP_STA, VOIDTYPE);

	pTree->SetChild1(m_IRTree->GetRoot());//将转成IR之后的函数体代码，作为函数树的右子树
	m_IRTree->SetRoot(NULL);
	
	currFunc = "";//处理完一个函数后，将此变量置为空串

	///2015-4-15 add by wangmeng 删除符号表中的所有变量
	m_FrmSymbl->Clear();
	m_NFrmSymbl->Clear();
	__ClearUnFrm();	 
	///end by wangmeng

	if (pTree->GetChild2() != NULL)//处理下一个函数定义
	{
		__FuncDef(pTree->GetChild2());
	}
}

//用户调用函数时，如果没有写extern，则先在函数树上找，如果找到了，则按照内部函数调用，即输出状态序列，如果没有找到，则当成是库函数
//如果写了extern，也是先在函数树上找，如果找到了，则按照外部函数调用，即不输出状态序列，如果没有找到，则当成是库函数
//内部函数调用的情况：f(x)(内部函数调用不能写区间),y:=f(x)(内部函数调用赋值必须是next赋值)
//外部函数调用的情况：extern f(x) and skip, malloc(x) and skip,y<==extern f(x) and skip,y<==malloc(x) and skip
///2015-3-17，add by yubin,处理内部函数调用,记着修改状态输出时的判断，如果是内部调用，进行状态输出，如果是外部函数调用，不进行输出
//flag=1时，表示merge到m_IRTree上，flag=0时，表示在赋值的右边，不用merge到m_IRTree上
void CMSV::__FuncCall(CSyntaxNode* pTree, int flag)
{
	CSyntaxNode* p = function_tree;//p指向函数定义语法树
	while (p != NULL && p->GetNName() != pTree->GetNName())//while循环用于在函数定义树查找当前调用的函数
	{
		p = p->GetChild2();
	}
	if (p != NULL)//函数语法树中有当前函数的定义,就需要将$$ext变量设置为0，表示内部函数调用，输出每个状态
	{
		CSyntaxNode *tmpExtNode = new CSyntaxNode(ACTUAL_PARAMETER_EXP, new CSyntaxNode(INTEGER_EXP, 0, VOIDTYPE), VOIDTYPE);

	tmpExtNode->SetChild1(pTree->GetChild0());
	pTree->SetChild0(tmpExtNode);//因为是内部函数调用，因此将$$Ext变量添加到实参列表的最开始，并且值为0

		//判断实参类型，如果是函数调用则添加内部或外部调用标志
		CSyntaxNode* tmpPar = pTree->GetChild0();
		while (tmpPar != NULL)
		{
			if (tmpPar->GetChild0()->GetNType() == FUNCTION_STA)
			{
				__FuncCall(tmpPar->GetChild0(), 0);
			}
			else if (tmpPar->GetChild0()->GetNType() == EXT_FUNCTION_STA)
			{
				__EFuncCall(tmpPar->GetChild0(), 0);
			}
			tmpPar = tmpPar->GetChild1();
		}

		CSyntaxNode *paraList = pTree->GetChild0();//获得函数的形参列表
		while (paraList->GetChild1() != NULL)
		{
			if ((__isRValue(paraList->GetChild1())) && (paraList->GetChild1()->GetChild1() == NULL))
			{
				paraList->SetChild1(NULL);
				break;
			}
			else if ((__isRValue(paraList->GetChild1())) && (paraList->GetChild1()->GetChild1() != NULL))
			{
				fout << "In function" << pTree->GetNName() << " call, RValue must be the last parameter!!!" << endl;
				exit(1);
			}
			paraList = paraList->GetChild1();
		}

		if (flag == 1)//flag=1时，表示merge到m_IRTree上
		{
	CSyntaxNode* copyTree = new CSyntaxNode();
	copyTree->CopyTree(pTree);
	__InsrtAss(copyTree);

	m_RdcOk = 1;
			m_More = 1;
}
		
	}
	else//如果用户没定义此函数，则当成是外部函数调用
	{
		__EFuncCall(pTree, flag);
	}
}

///2015-3-17，add by yubin,处理外部函数调用
//flag=1时，表示merge到m_IRTree上，flag=0时，表示在赋值的右边，不用merge到m_IRTree上
void CMSV::__EFuncCall(CSyntaxNode* pTree, int flag)
{
	CSyntaxNode* p = function_tree;//p指向函数定义语法树
	while (p != NULL && p->GetNName() != pTree->GetNName())//while循环用于在函数定义树查找当前调用的函数
	{
		p = p->GetChild2();
	}
	if (p != NULL)//函数语法树中有当前函数的定义,就需要将$$ext变量设置为1，表示外部函数调用，不用输出每个状态，如果用户没定义此函数，则将此函数当做是系统函数
	{
		CSyntaxNode *tmpExtNode = new CSyntaxNode(ACTUAL_PARAMETER_EXP, new CSyntaxNode(INTEGER_EXP, 1, VOIDTYPE), VOIDTYPE);
	tmpExtNode->SetChild1(pTree->GetChild0());
	pTree->SetChild0(tmpExtNode);//因为是内部函数调用，因此将$$Ext变量添加到实参列表的最开始，并且值为0

		//判断实参类型，如果是函数调用则添加内部或外部调用标志
		CSyntaxNode* tmpPar = pTree->GetChild0();
		while (tmpPar != NULL)
		{
			if (tmpPar->GetChild0()->GetNType() == FUNCTION_STA)
			{
				__FuncCall(tmpPar->GetChild0(), 0);
			}
			else if (tmpPar->GetChild0()->GetNType() == EXT_FUNCTION_STA)
			{
				__EFuncCall(tmpPar->GetChild0(), 0);
			}
			tmpPar = tmpPar->GetChild1();
		}

		CSyntaxNode *paraList = pTree->GetChild0();//获得函数的形参列表
		while (paraList->GetChild1() != NULL)
		{
			if ((__isRValue(paraList->GetChild1())) && (paraList->GetChild1()->GetChild1() == NULL))
			{
				paraList->SetChild1(NULL);
				break;
			}
			else if ((__isRValue(paraList->GetChild1())) && (paraList->GetChild1()->GetChild1() != NULL))
			{
				fout << "In function" << pTree->GetNName() << " call, RValue must be the last parameter!!!" << endl;
				exit(1);
			}
			paraList = paraList->GetChild1();
		}
	}
	if (flag == 1)//flag=1时，表示merge到m_IRTree上
	{
	CSyntaxNode* copyTree = new CSyntaxNode();
	copyTree->CopyTree(pTree);
	__InsrtAss(copyTree);
	m_RdcOk = 1;
		m_More = -1;
}
}

///add by yubin 2015-3-24，await(b)-->while(!b){skip}
void CMSV::__Await(CSyntaxNode* pTree)
{
	CSyntaxNode * whileStat = new CSyntaxNode(WHILE_DO_STA, VOIDTYPE);//将await语句变成while语句，这是最上层的while语句
	CSyntaxNode * negStat = new CSyntaxNode(NEGATION_EXP, BOOLTYPE);
	negStat->SetChild0(pTree->GetChild0());
	whileStat->SetChild0(negStat);
	CSyntaxNode * skipStat = new CSyntaxNode(SKIP_STA, VOIDTYPE);
	whileStat->SetChild1(skipStat);
	pTree = whileStat;
	GetPresent(pTree);//将await语句变成while语句后，再调用GetPresent函数
}

/**
* Get present tree of final statement: fin(p)
* @param the syntax tree to be handled
* @return void
*/
///2015-3-12 add by wangmeng
void CMSV::__Fin(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		fout << "__Fin syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}
	///仍然有下一状态，则将fin(p)加入到下一状态语法树中
	if (m_More == 1)  
	{
		CSyntaxNode* Fin = new CSyntaxNode(FINAL_STA, VOIDTYPE);
		Fin->SetChild0(pTree->GetChild0());
		pTree->SetChild0(NULL);

		if (m_IfTree != NULL)		///if()then{}else{} and fin(p)
		{
			__AndRIf(NULL, Fin);
		}
		else
		{
			m_PrgmFuture->MergeTree(Fin, AND_EXP, VOIDTYPE);
		}
		m_RdcOk = 1;
		return;
	}
	else if (m_More == 0) ///最后一个状态，执行fin(p)中的p
	{
		m_More = -1;
		GetPresent(pTree->GetChild0());

		///2015-3-30 add by wangmeng
		if (pTree->GetChild0()->GetNType() == IF_ELSE_STA)
		{
			//m_More=0;
			CSyntaxNode*tmpIf = m_IfTree;
			tmpIf = new CSyntaxNode(AND_EXP, VOIDTYPE);
			tmpIf->SetChild0(pTree->GetChild0());
			pTree->SetChild0(NULL);
			tmpIf->SetChild1(new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
			__AndIf(tmpIf);
			return;
		}

		if (m_More == 1)
		{
			m_RdcOk = 0;
			fout << "__Fin() empty and next error!" << endl;
			return;
		}
		else
		{
			m_More = 0;
			return;
		}
	}
	else  ///没有确定是否是最后一个状态，则将fin(p)加入到m_FinTree中，稍后再做处理
	{
		//CSyntaxNode* Fin=new CSyntaxNode(FINAL_STA,VOIDTYPE);
		//Fin->SetChild0();

		m_FinTree.push(pTree->GetChild0());
		pTree->SetChild0(NULL);

		m_Fin++;
		m_RdcOk = 1;
		return;

	}
}

/**
* Get present tree of always statement: always(p)
* @param the syntax tree to be handled
* @return void
*/
///2015-3-12 add by wangmeng
void CMSV::__Alw(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		fout << "__Alw() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}

	CSyntaxNode* Alw = new CSyntaxNode(ALWAYS_STA, VOIDTYPE); ///拷贝alw(p)
	Alw->SetChild0(pTree->GetChild0());
	pTree->SetChild0(NULL);

	m_AlwTree.push(Alw);
	m_Alw++;
	Alw = NULL;
	return;
}

/**
* Get present tree of always statement: always(p)
* @param the syntax tree to be handled
* @return void
*/
///2015-3-24 add by wangmeng
void CMSV::__AlwPost(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		fout << "__AlwPost() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}

	if (m_IfTree != NULL) ////if()then{}else{} and alw(p)
	{
		CSyntaxNode *tmpAlw = new CSyntaxNode(ALWAYS_STA, VOIDTYPE);
		tmpAlw->SetChild0(pTree->GetChild0());
		pTree->SetChild0(NULL);
		__AndRIf(NULL, tmpAlw);
		m_RdcOk = 1;
		return;
	}

	CSyntaxNode* Alw = new CSyntaxNode(); ///拷贝alw(p)
	Alw->CopyTree(pTree);

	int tmpMore = m_More;
	m_More = -1;

	CSyntaxNode* Future = m_PrgmFuture->GetRoot();
	m_PrgmFuture->SetRoot(NULL);

	GetPresent(pTree->GetChild0());


	if (m_RdcOk == 0)
	{
		delete Alw;
		Alw = NULL;
		fout << "__AlwPost() gets present error!" << endl;
		return;
	}


	if (tmpMore == -1)
	{
		if (m_More == 1)   /// alw(p)中p有下一状态，外边没有给定状态  pf and alw(p)加入到下一状态
		{
			if (!m_PrgmFuture->MergeTree(Alw, AND_EXP, VOIDTYPE))
			{
				fout << "__AlwPost() MergeTree() error 1!" << endl;
				m_RdcOk = 0;
				return;
			}
		}
		else if (m_More == -1) /// alw(p)中p与外边均没有给定状态 alw(p)加入到m_AlwTree。
		{
			fout << "__AlwPost() The interval does not determined." << endl;
			m_RdcOk = 0;
			return;
		}
		else  ///alw(p)中p没有下一状态，外边没有给定状态  pf 加入到下一状态
		{
			delete Alw;
			Alw = NULL;
			return;
		}
	}
	else if (tmpMore == 1)
	{
		if (m_More == 0)  ///外边给定的区间与alw(p)中p的区间不一致，报错
		{
			delete Alw;
			Alw = NULL;
			fout << "__AlwPost() empty and next error 1!" << endl;
			m_RdcOk = 0;
			return;
		}
		else    ///有下一状态，pf and alw(p)加入到下一状态中
		{
			if (!m_PrgmFuture->MergeTree(Alw, AND_EXP, VOIDTYPE))
			{
				fout << "__AlwPost() MergeTree() error 2!" << endl;
				m_RdcOk = 0;
				return;
			}

			CSyntaxNode *tmpFuture = new CSyntaxNode(AND_EXP, VOIDTYPE);
			tmpFuture->SetChild0(Future);
			tmpFuture->SetChild1(m_PrgmFuture->GetRoot());
			m_PrgmFuture->SetRoot(tmpFuture);

			///alw(if()then{}else{})
			if (pTree->GetChild0()->GetNType() == IF_ELSE_STA)
			{
				CSyntaxNode* Prgm = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				Prgm->SetChild0(m_PrgmFuture->GetRoot());
				m_PrgmFuture->SetRoot(NULL);

				__AndRIf(pTree->GetChild0(), Prgm);
				pTree->SetChild0(NULL);
			}

			tmpFuture = NULL;
			Future = NULL;
			m_More = 1;
			return;
		}
	}
	else if (tmpMore == 0)
	{
		delete Alw;
		Alw = NULL;
		if (m_More == 0)  ///alw(p)中p与外边均没有下一状态 pf加入到下一状态中
		{
			return;
		}
		else if (m_More == 1) ///外边给定的区间与alw(p)中p的区间不一致，报错
		{
			if (pTree->GetChild0()->GetNType() == IF_ELSE_STA)
			{
				__AndRIf(NULL, new CSyntaxNode(EMPTY_EXP, VOIDTYPE));
				pTree->SetChild0(NULL);
				return;
			}

			fout << "__AlwPost() empty and next error 2!" << endl;
			m_RdcOk = 0;
			return;
		}
		else ///没有下一状态
		{
			m_More = 0;
		}
	}
}

/**
* Get present tree of keep statement: keep(p)
* @param the syntax tree to be handled
* @return void
*/
///2015-3-12 add by wangmeng
void CMSV::__Keep(CSyntaxNode* pTree)
{
	if (pTree->GetChild0() == NULL)
	{
		fout << "__Keep() syntax tree error!" << endl;
		m_RdcOk = 0;
		return;
	}

	if (m_More == 1)  ///有下一状态，pf and keep(p)加入到future中
	{
		///if()then{}else{} and keep(p)
		if (m_IfTree != NULL)
		{
			CSyntaxNode* Prgm = new CSyntaxNode(KEEP_STA, VOIDTYPE);
			Prgm->SetChild0(pTree->GetChild0());
			pTree->SetChild0(NULL);
			__AndRIf(NULL, Prgm);
			m_RdcOk = 1;
			return;
		}

		CSyntaxNode* Future = m_PrgmFuture->GetRoot();
		m_PrgmFuture->SetRoot(NULL);

		m_More = -1;
		CSyntaxNode* tmp = new CSyntaxNode(); ///tmp记录keep(p)
		tmp->CopyTree(pTree);

		//CSyntaxNode* tmpIf=m_IfTree;
		GetPresent(pTree->GetChild0()); ///处理p

		if (m_More == 0) ///p中区间长度是0则报错
		{
			delete tmp;
			tmp = NULL;
			fout << "__Keep() empty and next error!" << endl;
			m_RdcOk = 0;
			return;
		}
		else  ///p中也有下一状态pf，将pf and keep(p)加入到future中
		{
			m_PrgmFuture->MergeTree(tmp, AND_EXP, VOIDTYPE); ///pf and keep(p)加到future中
			CSyntaxNode *tmpFutr = m_PrgmFuture->GetRoot();
			m_PrgmFuture->SetRoot(Future);
			m_PrgmFuture->MergeTree(tmpFutr, AND_EXP, VOIDTYPE);

			///keep(if()then{}else{})
			if (pTree->GetChild0()->GetNType() == IF_ELSE_STA)
			{
				m_IfTree = NULL;
				CSyntaxNode* Future = new CSyntaxNode(NEXT_STA, VOIDTYPE);
				Future->SetChild0(m_PrgmFuture->GetRoot());
				m_PrgmFuture->SetRoot(NULL);

				__AndRIf(pTree->GetChild0(), Future);
				pTree->SetChild0(NULL);
			}

			tmpFutr = NULL;
			tmp = NULL;
			m_More = 1;
			m_RdcOk = 1;
			return;
		}

	}
	else if (m_More == -1)  ////没有确定区间长度，keep(p)加入m_KeepTree中
	{
		CSyntaxNode* Keep = new CSyntaxNode(KEEP_STA, VOIDTYPE);
		Keep->SetChild0(pTree->GetChild0());
		pTree->SetChild0(NULL);

		m_KeepTree.push(Keep);
		m_Keep++;
		m_RdcOk = 1;
		return;
	}
	else
	{
		m_RdcOk = 1;
		return;
	}
}

///2015-4-13 add by yubin
void CMSV::__Return(CSyntaxNode* pTree)
{
	///2015-4-16 add by wangmeng
	CSyntaxNode* RTree = new CSyntaxNode();
	RTree->CopyTree(pTree);
	__InsrtAss(RTree);
	///end 2015-4-16 add by wangmeng

	m_RdcOk = 1;
	m_More = 0;
}


/**
* Insert the present assignment to the present vector.
* @param the syntax tree to be handled
* @return void
*/
///2015-3-9 add by wangmeng
void CMSV::__InsrtAss(CSyntaxNode*pTree)
{
	m_PrgmPrsnt.push_back(pTree);

}

///输出当前状态赋值语句
///2015-3-10 add by wangmeng
void CMSV::PrintPrsnt()
{
	vector<CSyntaxNode*>::iterator iter;
	for (iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++)
	{
		fout << (*iter)->TreeToString().data() << "  ";
	}
}

///将当前状态赋值的链表加入到IRTree中
///2015-3-10 add by wangmeng
void CMSV::__AdPrst2IR()
{
	vector<CSyntaxNode*>::iterator iter;
	for (iter = m_PrgmPrsnt.begin(); iter != m_PrgmPrsnt.end(); iter++)
	{
		m_IRTree->MergeTree((*iter), CHOP_STA, VOIDTYPE);
	}
	m_PrgmPrsnt.clear();
	//	m_IRTree->MergeTree(new CSyntaxNode(TRUE_EXP,VOIDTYPE),CHOP_STA,VOIDTYPE);
}

///输出m_IRTree
///2015-3-11 add by wangmeng
string CMSV::PrintIRTree()
{
	return m_IRTree->GetRoot()->TreeToString();
}

///当得到区间长度后处理对应的keep语句
///2015-3-11 add by wangmeng
void CMSV::__KeepMore()
{
	if (m_More == -1)///如果没有定义区间长度，则我们认为是最后一个状态处理
	{
		m_More = 0;
	}

	CSyntaxNode*iter;
	if (m_More == 0)  ///没有下一状态，直接将m_KeepTree清空；
	{
		for (; m_Keep != 0 && !m_KeepTree.empty(); m_Keep--)
		{
			iter = m_KeepTree.top();
			delete iter;
			m_KeepTree.pop();
		}
		return;
	}

	///有下一状态
	for (; m_Keep != 0 && !m_KeepTree.empty(); m_Keep--) ///遍历m_KeepTree,处理每个元素
	{
		iter = m_KeepTree.top();
		__Keep(iter); ///处理keep(p)中的p

		if (m_RdcOk == 0)
		{
			fout << "__KeepMore() handles keep error!" << endl;
			for (; m_Keep != 0 && !m_KeepTree.empty(); m_Keep--)
			{
				iter = m_KeepTree.top();
				delete iter;
				m_KeepTree.pop();
			}
			return;
		}
		delete iter;
		m_KeepTree.pop();
	}
}

///当得到区间长度后处理对应的fin语句
///2015-3-11 add by wangmeng
void CMSV::__FinMore()
{
	CSyntaxNode*iter;
	if (m_More == 0 || m_More == 1)  ///区间长度确定，直接调用__Fin()函数处理
	{
		CSyntaxTree* FinTree = new CSyntaxTree();
		for (; m_Fin != 0 && !m_FinTree.empty(); m_Fin--)
		{
			iter = m_FinTree.top();
			FinTree->MergeTree(iter, AND_EXP, VOIDTYPE);
			m_FinTree.pop();
		}
		CSyntaxNode* tmpFin = new CSyntaxNode(FINAL_STA, VOIDTYPE);
		tmpFin->SetChild0(FinTree->GetRoot());
		FinTree->SetRoot(NULL);
		delete FinTree;
		__Fin(tmpFin);

		return;
	}
	else ///区间长度不确定，返回错误信息
	{
		m_RdcOk = 0;
		fout << "__FinMore() does not have the interval length!" << endl;
		for (; m_Fin != 0 && !m_FinTree.empty(); m_Fin--)
		{
			iter = m_FinTree.top();
			delete iter;
			m_FinTree.pop();
		}
		return;

	}

}

///当得到区间长度后处理对应的alw语句
///2015-3-12 add by wangmeng
void CMSV::__AlwMore()
{
	/*	if (m_More == -1)///如果没有定义区间长度，则我们认为是最后一个状态处理
	{
	m_More = 0;
	}
	*/
	CSyntaxNode*iter;
	/*	if(m_More==0)  ///没有下一状态，直接将m_AlwTree清空；
	{
	for (; m_More!=0&&!m_AlwTree.empty(); m_Alw--)
	{
	iter = m_AlwTree.top();
	delete iter;
	m_AlwTree.pop();
	}
	return;
	}
	*/
	///有下一状态
	for (; m_Alw != 0 && !m_AlwTree.empty(); m_Alw--) ///遍历m_AlwTree,处理每个元素
	{
		iter = m_AlwTree.top();

		__AlwPost(iter);

		delete iter;

		/*if (!m_PrgmFuture->MergeTree(iter,AND_EXP,VOIDTYPE))
		{
		m_RdcOk=0;
		fout << "__AlwMore() MergeTree() error!" << endl;
		for (; m_Alw!=0&&!m_AlwTree.empty();m_Alw--)
		{
		iter=m_AlwTree.top();
		delete iter;
		m_AlwTree.pop();
		}
		return;
		}*/
		m_AlwTree.pop();
	}

}

///可支持frame(x) and frame(y) 这种形式的版本
///当得到区间长度后处理对应的unframe语句
///2015-3-13 add by wangmeng
void CMSV::__AddUnFrm()
{
	/*	CSyntaxNode*iter;

	for (;  m_Frm!=0&&!m_UnFrm.empty();m_Frm--) ///遍历m_UnFrm,处理每个元素
	{
	iter=m_UnFrm.top();

	if (!m_PrgmFuture->MergeTree(iter,CHOP_STA,VOIDTYPE))
	{
	m_RdcOk=0;
	fout << "__AddUnFrm() MergeTree() error!" << endl;
	for (; m_Frm!=0&&!m_UnFrm.empty();m_Frm--)
	{
	iter=m_UnFrm.top();
	delete iter;
	m_UnFrm.pop();
	}
	return;
	}
	m_UnFrm.pop();
	}
	*/

}

///在得到是否有下一状态后，统一处理keep，fin，alw
///2015-3-12 add by wangmeng
void CMSV::__KpFnAl()
{
	if (m_Keep) ///如果有keep，处理keep
	{
		__KeepMore();
	}
	if (m_Alw)
	{
		__AlwMore();
	}
	if (m_Fin)///如果有fin，处理fin
	{
		__FinMore();
	}
}

///释放frame变量
///2015-3-16 add by wangmeng
void CMSV::__FreeSymbl()
{
	CSyntaxNode* iter = NULL;
	while (!m_UnFrm.empty())///将m_UnFrm中的项弹栈，挨个frame释放
	{
		iter = m_UnFrm.top();

		while (iter != NULL)///释放frame中的每个变量
		{
			if (!m_FrmSymbl->DelFrm(iter->GetChild0()->GetNName()))
			{
				fout << "__FreeSymbl() " << iter->GetChild0()->GetNName().data() << " error!" << endl;
				m_RdcOk = 0;
				return;
			}
			iter = iter->GetChild1();
		}

		m_UnFrm.pop();
	}
	m_FreeFrm = 0;
}

/**
* 获得节点后代中的所有标识符
* @param pTree要遍历的节点，right存放获得的标识符
*/
///2015-03-10 add by shiyifang
void CMSV::__GetRightId(CSyntaxNode*pTree, vector<string> &right)
{
	if (pTree == NULL)
		return;

	//节点类型为标识符时，查找符号表中是否有此名称的标识符
	//有则加入存放右值的vector，否则报错返回
	if (pTree->GetNType() == IDENT_EXP)
	{
		///2015-4-21 add by wangmeng 加入全局符号表
		Symbol* symbol = m_GlbSymbl->FindSymbol(pTree->GetNName());

		///end by wangmeng

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNName());
		}		

		if (symbol == NULL)
		{
			symbol = m_NFrmSymbl->FindSymbol(pTree->GetNName());
		}
			
		if (symbol == NULL)
		{
			fout << "__GetRightId: the symbol does not exist:" << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}

		right.push_back(pTree->GetNName());
		return;
	}

	//节点类型为数组时，查找符号表中是否有此名称的标识符
	//有则判断下标是否越界是否为整数，无则报错返回
	//下标正确则加入存放右值的vector，否则报错返回
	if (pTree->GetNType() == LIST_SYMBOL_STA)
	{
		///2015-4-21 add by wangmeng 加入全局符号表
		Symbol* symbol = m_GlbSymbl->FindSymbol(pTree->GetNName());

		///end by wangmeng

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNName());
		}

		if (symbol == NULL)
		{
			symbol = m_NFrmSymbl->FindSymbol(pTree->GetNName());
		}
			
		if (symbol == NULL)
		{
			fout << "__GetRightId:  the symbol does not exist: " << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}

		if (!(symbol->IsArray() || SymbolTable::IsPointer(symbol->GetSymbolType())))
		{
			fout << "__GetRightId:  the symbol is not an array: " << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}

		CSyntaxNode * child = pTree->GetChild0();
		switch (child->GetNType())
		{
		case INTEGER_EXP: {
			if (child->GetiValue() < 0)
			{
				fout << "__GetRightId: array overflow : " << pTree->GetNName().data() << endl; m_RdcOk = 0; return;
			}
			char buff[32];  _itoa(child->GetiValue(), buff, 10);
			right.push_back(pTree->GetNName().append("[").append(buff).append("]")); return;
		}
		case FLOATLITERAL_EXP: {fout << "__GetRightId: the index of array should be an integer: " << pTree->GetNName().data() << endl; m_RdcOk = 0; return; }

		default: {  __GetRightId(child, right);
			if (m_RdcOk == 0) return;
			right.push_back(pTree->TreeToString()); return; }///add by syf 2015-4-9
		}

	}

	///add by syf 2015-4-9
	if (pTree->GetNType() == STRUCT_IDENT_EXP || pTree->GetNType() == STRUCT_LIST_EXP)
	{
		__GetRightId(pTree->GetChild0(), right);
		if (m_RdcOk == 0) return;

		CSyntaxNode * child = pTree->GetChild1();
		while (child != NULL)
		{
			if (child->GetNType() == LIST_SYMBOL_STA)
				__GetRightId(child->GetChild0(), right);
			child = child->GetChild0();
		}
		if (m_RdcOk == 0) return;

		right.push_back(pTree->TreeToString());
		return;
	}

	__GetRightId(pTree->GetChild0(), right);
	__GetRightId(pTree->GetChild1(), right);
}



/*赋值符号左边为指针时，取得左边所有指针,并将其余标识符加入右边
*@param  pTree(VALUE_EXP的孩子) stru(存放标识符的结构体)
*/
///2015-03-12 add by shiyifang
void CMSV::__GetLeftId(CSyntaxNode*pTree, Ass_Struct &stru)
{
	if (pTree == NULL)
		return;

	if (pTree->GetNType() == IDENT_EXP)
	{
		///2015-4-21 add by wangmeng 加入全局符号表
		Symbol* symbol = m_GlbSymbl->FindSymbol(pTree->GetNName());

		///end by wangmeng

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNName());
		}

		if (symbol == NULL)
		{
			symbol = m_NFrmSymbl->FindSymbol(pTree->GetNName());
		}
			
		if (symbol == NULL)
		{
			fout << "__GetRightId: the symbol does not exist: " << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}

		if (symbol->IsArray() || SymbolTable::IsPointer(symbol->GetSymbolType()))
			stru.left.push_back(pTree->GetNName());
		else
			stru.right.push_back(pTree->GetNName());
		return;
	}

	if (pTree->GetNType() == LIST_SYMBOL_STA || pTree->GetNType() == VALUE_EXP)
	{
		__GetRightId(pTree, stru.right);
		return;
	}

	///add by shiyifang 2015-4-9
	if (pTree->GetNType() == STRUCT_IDENT_EXP || pTree->GetNType() == STRUCT_LIST_EXP)
	{
		string name = "";///结构体变量名
		CSyntaxNode *left = pTree->GetChild0();
		if (left->GetNType() == IDENT_EXP || left->GetNType() == LIST_SYMBOL_STA)
			name = left->GetNName();
		else if (left->GetNType() == VALUE_EXP)
		{

		}
		if (name == "")
		{
			fout << "__GetLeftId():找不到此种格式的结构体！" << pTree->TreeToString() << endl;
			m_RdcOk = 0;
			return;
		}
		
		///2015-4-21 add by wangmeng 加入全局符号表
		Symbol* symbol = m_GlbSymbl->FindSymbol(name);

		///end by wangmeng

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(name);
		}

		if (symbol == NULL)
		{
			symbol = m_NFrmSymbl->FindSymbol(name);
		}
			
		if (symbol == NULL || symbol->GetSName() == "")
		{
			fout << "__GetLeftId: the symbol does not exist: " << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}

		name = symbol->GetSName();///结构体类型名
		symbol = struct_map.find(name)->second->FindSymbol(pTree->GetChild1()->GetNName());
		if (symbol == NULL)
		{
			fout << "__GetLeftId: the symbol does not exist: " << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}
		if (symbol->IsArray() || SymbolTable::IsPointer(symbol->GetSymbolType()))///结构体成员是否为指针
			stru.left.push_back(pTree->TreeToString());
		else
			stru.right.push_back(pTree->TreeToString());
		return;
	}


	__GetLeftId(pTree->GetChild0(), stru);
	__GetLeftId(pTree->GetChild1(), stru);
}

/**2015-3-9 add by shiyifang
*
* 对赋值语句排序的判断条件，算法类中sort函数的参数之一
* @paragram：vector中带比较的两个节点
* return : 比较之后的布尔值
*/
bool Comp(const Ass_Struct &a, const Ass_Struct &b)
{
	if (b.right.empty())
		return false;
	if (a.right.empty())
		return true;
	return (find(a.right.begin(), a.right.end(), b.left.at(0)) == a.right.end());
}

/*节点类型为赋值时 获得赋值的左值和右值
* @param pTree(赋值节点)  stru(存放标识符的结构体)
*/
///2015-03-12 add by shiyifang
void CMSV::__GetAssStruct(CSyntaxNode*pTree, Ass_Struct &stru)
{
	//将右边的值加入右值vector中
	__GetRightId(pTree->GetChild1(), stru.right);

	//将左边待赋值的值加入左值vector中，其余加入右值vector
	//如*(p+a+b[10])
	CSyntaxNode* left = pTree->GetChild0();
	switch (left->GetNType())
	{
		//左边以指针开始时，遍历掉所有开始的指针，调用__GetLeftId()
	case VALUE_EXP: {
		/*left = left->GetChild0();
		while (left->GetNType() == VALUE_EXP)
		left = left->GetChild0();
		__GetLeftId(left, stru);*/

		__GetRightId(left, stru.right);
		stru.left.push_back(left->TreeToString());
		break;
	}

					//左边以数组开始时，可以确定左值为此数组
					//还需判断是否有此数组和是否越界
	case LIST_SYMBOL_STA:
	{

		///2015-4-21 add by wangmeng 加入全局变量
		Symbol* symbol = m_GlbSymbl->FindSymbol(left->GetNName());

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(left->GetNName());
		}

		///end by wangmeng

		if (symbol == NULL)
			symbol = m_NFrmSymbl->FindSymbol(left->GetNName());
		if (symbol == NULL)
		{
			fout << "__GetAssAtruct: the symbol does not exist 1: " << left->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}
		if (!(symbol->IsArray() || SymbolTable::IsPointer(symbol->GetSymbolType())))
		{
			fout << "__GetAssStruct:  the symbol is not an array: " << pTree->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}
		CSyntaxNode * child = left->GetChild0();
		switch (child->GetNType())
		{
		case INTEGER_EXP: {
			if (child->GetiValue() < 0)
			{
				fout << "__GetAssAtruct:  array overflow : " << left->GetNName().data() << endl; m_RdcOk = 0; return;
			}
			char buff[32];  _itoa(child->GetiValue(), buff, 10);
			stru.left.push_back(left->GetNName().append("[").append(buff).append("]")); break;
		}
		case FLOATLITERAL_EXP: {fout << "__GetRightId: the index of array should be an integer: " << left->GetNName().data() << endl; m_RdcOk = 0; return; }
		default: { __GetRightId(child, stru.right);
			if (m_RdcOk == 0) return;
			stru.left.push_back(left->TreeToString()); break; }///add by syf 2015-4-9
		}

		break;
	}

	//左边以标识符开始时，左值就是此标识符
	//还需判断标识符是否存在
	case IDENT_EXP: {

		///2015-4-21 add by wangmeng 加入全局变量
		Symbol* symbol = m_GlbSymbl->FindSymbol(left->GetNName());

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(left->GetNName());
		}

		///end by wangmeng
		

		if (symbol == NULL)
		{
			symbol = m_NFrmSymbl->FindSymbol(left->GetNName());
		}
			
		if (symbol == NULL)
		{
			fout << "__GetAssAtruct: the symbol does not exist :" << left->GetNName().data() << endl;
			m_RdcOk = 0;
			return;
		}

		stru.left.push_back(left->GetNName());
		break;
	}

					///add by shiyifang 2015-4-9
	case STRUCT_IDENT_EXP:
	case STRUCT_LIST_EXP:{
		__GetRightId(left->GetChild0(), stru.right);///结构体.左边加入right
		if (m_RdcOk == 0) return;

		CSyntaxNode * child = left->GetChild1();
		while (child != NULL)///结构体.右边加入right
		{
			if (child->GetNType() == LIST_SYMBOL_STA)
				__GetRightId(child->GetChild0(), stru.right);
			child = child->GetChild0();
		}
		if (m_RdcOk == 0) return;

		stru.left.push_back(left->TreeToString());///结构体加入left
		break;
	}

	default: {fout << "__the type has not been considered !!!" << endl; m_RdcOk = 0; return; }
	}

	//如果左值不为1，结果错误
	if (stru.left.size() != 1)
	{
		fout << "__GetAssAtruct:the left value is error, too more or too less" << endl;
		m_RdcOk = 0;
	}

}


/*
* 为获得的所有当前状态排序,声明并检查变量是否存在
* 排序顺序: EX_ASS 声明 ASS_EXP
* 排序规则：使用快排，比较左值是否包含右值
*/
///2015-03-13 add by shiyifang
void CMSV::__SortPrsnt()
{
	vector<CSyntaxNode*> others, prsnt_ass, return_sta, result, function_call, efunction_call, input_sta, output_sta;//others:除了赋值语句的所有语句，result：排序结果
	vector<Ass_Struct> ex_id, ass_id;//辅助存储结构体
	int i;//循环变量

	//将m_PrgmPrsnt分类存储
	for (i = 0; i < m_PrgmPrsnt.size(); i++)
	{
		CSyntaxNode* pTree = m_PrgmPrsnt.at(i);

		switch (pTree->GetNType())
		{
		case EX_ASS_EXP: {Ass_Struct stru; stru.seq = i; __GetAssStruct(pTree, stru);
			if (m_RdcOk == 0) { fout << pTree->TreeToString().data() << endl; return; }
			ex_id.push_back(stru); break; }

		case ASS_EQU_EXP: {prsnt_ass.push_back(pTree); break; }

		case RETURN_STA: {return_sta.push_back(pTree); break; }
		case FUNCTION_STA: {function_call.push_back(pTree); break; }
		case EXT_FUNCTION_STA: {efunction_call.push_back(pTree); break; }

		case REQUEST_STA:{input_sta.push_back(pTree); break; }

		case DISPLAY_STA:{output_sta.push_back(pTree); break; }

		default: {others.push_back(pTree);  break; }
	}
	}

	//排序
	sort(ex_id.begin(), ex_id.end(), Comp);
	//删除上一状态非Frame符号表

	m_NFrmSymbl->Clear();


	///删除上一状态Frame符号表
	///2015-4-16 add by wangmeng 
	if (!m_UnFrm.empty())
	{
		__FreeSymbl();
		if (m_RdcOk == 0)
		{
			fout << "__SortPrsnt() delete frame variable error!" << endl;
			return;
		}
	}
	///2015-3-17 add by wangmeng
	if (m_FreeFrm == 1)
	{
		CSyntaxNode* iter = NULL;
		while (!m_PreUnFrm.empty())///将m_UnFrm中的项弹栈，挨个frame释放
		{
			iter = m_PreUnFrm.top();
			m_UnFrm.push(iter);
			m_PreUnFrm.pop();
		}
	}
	


	vector<Ass_Struct>::iterator it1;
	vector<CSyntaxNode*>::iterator it2;

	//根据序号将Ex_ASS_EXP加入到结果中
	for (i = ex_id.size() - 1; i >= 0; i--)
		result.push_back(m_PrgmPrsnt.at(ex_id.at(i).seq));

	//处理声明语句，调用__SearchAndInsert()插入声明的变量到符号表中
	for (it2 = others.begin(); it2 < others.end(); it2++)
	{
		if ((*it2)->GetNType() == DECLARE_STA || (*it2)->GetNType() == UNSIGN_DECLARATION_STA)
		{
			if (!__InstDec2ST(*it2))
			{
				return;
			}
		}
		result.push_back(*it2);
	}

	//处理输入语句，输入语句需要放在声明语句之后执行
	for (it2 = input_sta.begin(); it2 < input_sta.end(); it2++)
	{
		result.push_back(*it2);
	}

	//将ASS_EXP的结点存入结构体中
	for (it2 = prsnt_ass.begin(), i = 0; it2 != prsnt_ass.end(); it2++, i++)
	{
		CSyntaxNode* pTree = *it2;
		Ass_Struct stru;
		stru.seq = i;
		
		
		__GetAssStruct(pTree, stru);
		
		
		if (m_RdcOk == 0)
		{
			fout << (*it2)->TreeToString().data() << endl;
			return;
		}
		ass_id.push_back(stru);
	}

	//对结构体进行排序
	sort(ass_id.begin(), ass_id.end(), Comp);

	///按序号将排序后的结果加入到result中
	for (it1 = ass_id.begin(); it1 < ass_id.end(); it1++)
		result.push_back(prsnt_ass.at(it1->seq));

	//处理输出语句，输出语句需放在最后(return语句之前)执行
	for (it2 = output_sta.begin(); it2 < output_sta.end(); it2++)
	{
		result.push_back(*it2);
	}

	//将return语句加入到最后
	for (it2 = return_sta.begin(); it2 != return_sta.end(); it2++)
	{
		result.push_back(*it2);
	}
	for (it2 = function_call.begin(); it2 != function_call.end(); it2++)
	{
		result.push_back(*it2);
	}
	for (it2 = efunction_call.begin(); it2 != efunction_call.end(); it2++)
	{
		result.push_back(*it2);
	}

	m_PrgmPrsnt.clear();
	m_PrgmPrsnt = result;

}

/*声明变量时 插入所有变量到符号表
* type(声明类型)  pTree(声明节点)
*/
///2015-03-13 add by shiyifang
bool CMSV::__SearchAndInsert(SymbolType type, CSyntaxNode *pTree, string name, bool IsPar, bool IsGlb)
{
	if (pTree == NULL)
		return true;

	//节点类型为标识符时
	if (pTree->GetNType() == IDENT_EXP)
	{
		//变量声明的类型是UNDEFINE，代表将变量插入到Frame中
		if (type == UNDEFINE)
		{
			if (IsGlb)
			{
				return m_GlbSymbl->InstSymbol(pTree->GetNName(), new Symbol());
			}
			else
			{
			return m_FrmSymbl->InstFSymbl(pTree->GetNName());
		}

		}

		Symbol* symbol = NULL;

		if (!g_InFunc)
		{
		///2015-4-21 add by wangmeng 从全局变量表中查找
			symbol = m_GlbSymbl->FindSymbol(pTree->GetNName());
		///end by wangmeng
		}

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNName());
		}		

		//Frame中没有这个变量，直接插入到非Frame中
		if (symbol == NULL)
		{
			if (IsPar)
			{
				return m_FrmSymbl->InstSymbol(pTree->GetNName(), new Symbol(type, name));
			}
			else
			{
				return m_NFrmSymbl->InstSymbol(pTree->GetNName(), type, name);
		}
			
		}
		//Frame中有这个变量，类型是UNDEFINE，改变类型
		else if (symbol->GetSymbolType() == UNDEFINE)
		{
			symbol->SetSymbolType(type);
			symbol->SetSName(name);
			return true;
		}
		//Frame中已有这个变量，类型也确定，不能插入
		else
		{
			fout << "__SearchAndInsert : variable " << pTree->GetNName().data() << " has been declarated in the frame table." << endl;
			return false;
		}
	}

	else if (pTree->GetNType() == ARRAY_DECLARE_STA || pTree->GetNType() == ARRAY_PARAMETER_EXP)
	{
		CSyntaxNode * child = pTree->GetChild0();

		Symbol* symbol = NULL;
		if (!g_InFunc)
		{
		///2015-4-21 add by wangmeng 从全局变量表中查找
			symbol = m_GlbSymbl->FindSymbol(pTree->GetNName());
		///end by wangmeng
		}

		if (symbol == NULL)
		{
			symbol = m_FrmSymbl->FindSymbol(pTree->GetNName());
		}

		//Frame中没有这个变量，直接插入到非Frame中
		if (symbol == NULL)
		{
			if (IsPar)
			{
				return m_FrmSymbl->InstSymbol(child->GetNName(), new Symbol(type, true, name, pTree->GetChild0()->GetiValue()));
			}
			else
			{
			return m_NFrmSymbl->InstSymbol(pTree->GetNName(), new Symbol(type, true, name, pTree->GetChild0()->GetiValue()));
			}
              
		}
			
		//Frame中有这个变量，类型是UNDEFINE，改变类型，数组长度
		else if (symbol->GetSymbolType() == UNDEFINE)
		{
			symbol->SetSymbolType(type);
			symbol->SetIsArray(true);
			symbol->SetSName(name);
			symbol->SetArrayNum(pTree->GetChild0()->GetiValue());
			return true;
		}
		//Frame中已有这个变量，类型也确定，不能插入
		else
		{
			fout << "__SearchAndInsert : variable" << pTree->GetNName().data() << "has been declarated in the frame table";
			return false;
		}
	}

	else if (pTree->GetNType() == ADDRESS_DECLARE_STA || pTree->GetNType() == POINT_PARAMETER_EXP)
	{
		//类型不能是UNDEFINE
		//2015-4-12 处理多个*
		if (type == UNDEFINE || SymbolTable::IsPointer(type))
		{
			fout << "__SearchAndInsert : pointer variable : " << pTree->GetNName().data() << " must be declarated as a pointer type:";
			return false;
		}

		//2015-04-17 syf 多重指针变量声明
		pTree = pTree->GetChild0();
		while (pTree->GetNType() == ADDRESS_DECLARE_STA || pTree->GetNType() == POINT_PARAMETER_EXP)
			pTree = pTree->GetChild0();
		//type+1就是这个类型的指针类型，具体说明在枚举类型中

		return  (__SearchAndInsert((SymbolType)(1 + type), pTree, name, IsPar, IsGlb));
	}


	return  (__SearchAndInsert(type, pTree->GetChild0(), name, IsPar, IsGlb) && __SearchAndInsert(type, pTree->GetChild1(), name, IsPar, IsGlb));
	
}

/**
* 该函数用于将结构体定义插入到符号表中
* @param pTree（结构体定义）
* @return void
*/
///2015-4-9  add by shiyifang
void CMSV::__InitStructDel(CSyntaxNode *pTree)
{
	if (m_RdcOk == 0 || pTree == NULL)
		return;

	if (pTree->GetNType() == DECLARE_STA)
	{
		if (!__SearchAndInsert(SymbolTable::GetSymbolType(pTree->GetRType()), pTree, pTree->GetNName()))
		{
			fout << "__SortPrsnt: declaration of variable fail : " << pTree->TreeToString().data() << endl;
			m_RdcOk = 0;
		}
		return;
	}
	
	__InitStructDel(pTree->GetChild0());
	__InitStructDel(pTree->GetChild1());
}

///2015-3-9 add by yangkai
void CMSV::__AndIf(CSyntaxNode* pTree)
{
	//左边是if语句，右边不是if语句，将右子树分别乘进then和else分支
	if (pTree->GetChild0()->GetNType() == IF_ELSE_STA && pTree->GetChild1()->GetNType() != IF_ELSE_STA)
	{
		CSyntaxNode* tempThen = new CSyntaxNode(AND_EXP, VOIDTYPE);
		CSyntaxNode* tempElse = new CSyntaxNode(AND_EXP, VOIDTYPE);

		tempThen->SetChild0(pTree->GetChild1());
		tempThen->SetChild1(pTree->GetChild0()->GetChild1());
		pTree->GetChild0()->SetChild1(NULL);

		tempElse->SetChild0(new CSyntaxNode());
		tempElse->GetChild0()->CopyTree(pTree->GetChild1());
		tempElse->SetChild1(pTree->GetChild0()->GetChild2());
		pTree->GetChild0()->SetChild2(NULL);

		pTree->SetNType(IF_ELSE_STA);
		CSyntaxNode* temp_Child_0_0 = pTree->GetChild0()->GetChild0();
		pTree->GetChild0()->SetChild0(NULL);
		delete pTree->GetChild0();

		pTree->SetChild0(temp_Child_0_0);
		pTree->SetChild1(tempThen);
		pTree->SetChild2(tempElse);


		m_IfTree = pTree;
		m_RdcOk = 1;
	}
	//右边是if语句，左边不是if语句，将左子树分别乘进then和else分支
	else if (pTree->GetChild1()->GetNType() == IF_ELSE_STA && pTree->GetChild0()->GetNType() != IF_ELSE_STA)
	{
		CSyntaxNode* tempThen = new CSyntaxNode(AND_EXP, VOIDTYPE);
		CSyntaxNode* tempElse = new CSyntaxNode(AND_EXP, VOIDTYPE);

		tempThen->SetChild0(pTree->GetChild0());
		tempThen->SetChild1(pTree->GetChild1()->GetChild1());
		pTree->GetChild1()->SetChild1(NULL);

		tempElse->SetChild0(new CSyntaxNode());
		tempElse->GetChild0()->CopyTree(pTree->GetChild0());
		tempElse->SetChild1(pTree->GetChild1()->GetChild2());
		pTree->GetChild1()->SetChild2(NULL);

		pTree->SetNType(IF_ELSE_STA);
		pTree->SetChild0(pTree->GetChild1()->GetChild0());
		pTree->GetChild1()->SetChild0(NULL);
		delete pTree->GetChild1();
		pTree->SetChild1(tempThen);
		pTree->SetChild2(tempElse);


		m_IfTree = pTree;
		m_RdcOk = 1;
	}
	//左右都是if语句，将右子树分别和左子树的then和else分支相乘
	else
	{
		CSyntaxNode* tempThen = new CSyntaxNode(AND_EXP, VOIDTYPE);
		CSyntaxNode* tempElse = new CSyntaxNode(AND_EXP, VOIDTYPE);

		tempThen->SetChild0(pTree->GetChild0()->GetChild1());
		pTree->GetChild0()->SetChild1(NULL);
		tempThen->SetChild1(pTree->GetChild1());

		tempElse->SetChild0(pTree->GetChild0()->GetChild2());
		pTree->GetChild0()->SetChild2(NULL);
		tempElse->SetChild1(new CSyntaxNode());
		tempElse->GetChild1()->CopyTree(pTree->GetChild1());

		pTree->SetNType(IF_ELSE_STA);
		CSyntaxNode* temp_Child_0_0 = pTree->GetChild0()->GetChild0();
		pTree->GetChild0()->SetChild0(NULL);
		delete pTree->GetChild0();

		pTree->SetChild0(temp_Child_0_0);
		pTree->SetChild1(tempThen);
		pTree->SetChild2(tempElse);

		m_IfTree = pTree;
		m_RdcOk = 1;
	}
}

///2015-3-9 add by yangkai
void CMSV::__PalIf(CSyntaxNode* pTree)
{
	//左边是if语句，右边不是if语句
	if (pTree->GetChild0()->GetNType() == IF_ELSE_STA && pTree->GetChild1()->GetNType() != IF_ELSE_STA)
	{
		CSyntaxNode* tempThen = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);
		CSyntaxNode* tempElse = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);

		tempThen->SetChild0(pTree->GetChild1());
		tempThen->SetChild1(pTree->GetChild0()->GetChild1());
		pTree->GetChild0()->SetChild1(NULL);

		tempElse->SetChild0(new CSyntaxNode());
		tempElse->GetChild0()->CopyTree(pTree->GetChild1());
		tempElse->SetChild1(pTree->GetChild0()->GetChild2());
		pTree->GetChild0()->SetChild2(NULL);

		pTree->SetNType(IF_ELSE_STA);
		CSyntaxNode* temp_Child_0_0 = pTree->GetChild0()->GetChild0();
		pTree->GetChild0()->SetChild0(NULL);
		delete pTree->GetChild0();

		pTree->SetChild0(temp_Child_0_0);
		pTree->SetChild1(tempThen);
		pTree->SetChild2(tempElse);


		m_IfTree = pTree;
		m_RdcOk = 1;
	}
	//右边是if语句，左边不是if语句
	else if (pTree->GetChild1()->GetNType() == IF_ELSE_STA && pTree->GetChild0()->GetNType() != IF_ELSE_STA)
	{
		CSyntaxNode* tempThen = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);
		CSyntaxNode* tempElse = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);

		tempThen->SetChild0(pTree->GetChild0());
		tempThen->SetChild1(pTree->GetChild1()->GetChild1());
		pTree->GetChild1()->SetChild1(NULL);

		tempElse->SetChild0(new CSyntaxNode());
		tempElse->GetChild0()->CopyTree(pTree->GetChild0());
		tempElse->SetChild1(pTree->GetChild1()->GetChild2());
		pTree->GetChild1()->SetChild2(NULL);

		pTree->SetNType(IF_ELSE_STA);
		pTree->SetChild0(pTree->GetChild1()->GetChild0());
		pTree->GetChild1()->SetChild0(NULL);
		delete pTree->GetChild1();
		pTree->SetChild1(tempThen);
		pTree->SetChild2(tempElse);


		m_IfTree = pTree;
		m_RdcOk = 1;
	}
	//左右都是if语句
	else
	{
		CSyntaxNode* tempThen = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);
		CSyntaxNode* tempElse = new CSyntaxNode(PARALLEL_STA, VOIDTYPE);

		tempThen->SetChild0(pTree->GetChild0()->GetChild1());
		pTree->GetChild0()->SetChild1(NULL);
		tempThen->SetChild1(pTree->GetChild1());

		tempElse->SetChild0(pTree->GetChild0()->GetChild2());
		pTree->GetChild0()->SetChild2(NULL);
		tempElse->SetChild1(new CSyntaxNode());
		tempElse->GetChild1()->CopyTree(pTree->GetChild1());

		pTree->SetNType(IF_ELSE_STA);
		CSyntaxNode* temp_Child_0_0 = pTree->GetChild0()->GetChild0();
		pTree->GetChild0()->SetChild0(NULL);
		delete pTree->GetChild0();

		pTree->SetChild0(temp_Child_0_0);
		pTree->SetChild1(tempThen);
		pTree->SetChild2(tempElse);

		m_IfTree = pTree;
		m_RdcOk = 1;
	}
}


///左边不是if语句，右边是if语句
///2015-3-19 add by wangmeng
void CMSV::__AndRIf(CSyntaxNode*IfTree, CSyntaxNode*LPrgm)
{
	//int tmpFreeFrm=m_FreeFrm;
	//__SortPrsnt();
	//m_FreeFrm=tmpFreeFrm;
	//__AdPrst2IR(); ///将当前状态加入到IR树中

	//左边有future部分，则将左边的future部分加上next节点后调用__AndIf函数
	if (LPrgm != NULL)
	{
		CSyntaxNode* tmpIf = m_IfTree;

		if (IfTree != NULL&&IfTree->GetNType() == IF_ELSE_STA)
		{
			CSyntaxNode*pTree = new CSyntaxNode(AND_EXP, VOIDTYPE);
			pTree->SetChild0(LPrgm);
			pTree->SetChild1(IfTree);
			__AndIf(pTree);
			LPrgm = pTree;
		}

		if (tmpIf != NULL&& tmpIf != IfTree)
		{
			CSyntaxNode* tmpTree = new CSyntaxNode(AND_EXP, VOIDTYPE);
			tmpTree->SetChild0(LPrgm);
			tmpTree->SetChild1(tmpIf);
			__AndIf(tmpTree);
		}

		//m_PrgmPrsnt.clear();

		m_More = 1;
		m_RdcOk = 1;
		return;

	}
}
//add by yangkai 2015/3/19
void CMSV::__ChopIf(CSyntaxNode* pTree)
{
	///该函数的作用是将chop左边的if语句和右边相乘，此函数只处理chop左边是if语句的情况

	//语法树结构有错
	if (pTree->GetNType() != CHOP_STA || pTree->GetChild0() == NULL || pTree->GetChild1() == NULL)
	{
		fout << "__ChopIf error 1 !" << endl;
		m_RdcOk = 0;
		return;
	}

	if (pTree->GetChild0()->GetNType() == IF_ELSE_STA)
	{
		CSyntaxNode* temp_Then = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp_Then->SetChild0(pTree->GetChild0()->GetChild1());
		pTree->GetChild0()->SetChild1(NULL);
		temp_Then->SetChild1(pTree->GetChild1());

		CSyntaxNode* temp_Else = new CSyntaxNode(CHOP_STA, VOIDTYPE);
		temp_Else->SetChild0(pTree->GetChild0()->GetChild2());
		pTree->GetChild0()->SetChild2(NULL);
		temp_Else->SetChild1(new CSyntaxNode());
		temp_Else->GetChild1()->CopyTree(pTree->GetChild1());

		CSyntaxNode* temp_Cond = pTree->GetChild0()->GetChild0();
		pTree->GetChild0()->SetChild0(NULL);

		delete pTree->GetChild0();

		pTree->SetNType(IF_ELSE_STA);
		pTree->SetChild0(temp_Cond);
		pTree->SetChild1(temp_Then);
		pTree->SetChild2(temp_Else);
	}
}

void CMSV::__While2If(CSyntaxNode* pTree)
{
	///该函数将while(b){P}转为if(b)then{p;while(b){P}} else {empty}

	//p; while(b){P}
	CSyntaxNode* Then = new CSyntaxNode(CHOP_STA, VOIDTYPE);

	Then->SetChild0(pTree->GetChild1());
	Then->SetChild1(new CSyntaxNode());
	Then->GetChild1()->CopyTree(pTree);

	CSyntaxNode* Else = new CSyntaxNode(EMPTY_EXP, VOIDTYPE);

	//if(b)then{p&more;while(b){P}} else {empty}
	pTree->SetNType(IF_ELSE_STA);
	pTree->SetChild1(Then);
	pTree->SetChild2(Else);
}


///add by yangkai
string CMSV::__DetectLoop()
{
	//该函数的作用是:当程序中有多个while语句并行时，在IR树中相应的位置添加Label标志和goto语句
	//该函数的返回值为一个字符串类型，该字符串表示需要在IR树上添加的Label名称，只有在当前状态
	//出现多个while并行的语句时，返回的字符串不为空

	string newLabel = "";
	//m_MutlWhile不为空时表示当前状态遇到了多个while并行的语句
	if (m_MutlWhile != "")
	{
		m_WhileTree = NULL;
		//如果栈不为空，则将m_MutlWhile和栈顶比较，如果相等，说明当前语句之前已经出现过，则
		//在IR树中添加goto语句，如果不等，则需要生成新的Label，并压入栈中
		if (!m_loopStack.empty() && m_MutlWhile == m_loopStack.top().prgm)
		{
			//当前程序之前出现过，则添加一个goto语句
			CSyntaxNode* Goto = new CSyntaxNode(GOTO_STA, m_loopStack.top().label, VOIDTYPE);
			m_loopStack.pop();
			m_IfTree = NULL;//必须置为NULL，否则会死循环

			///yangkai 处理while相关
			//遇到循环添加goto语句

			CSyntaxNode* temp_IR = new CSyntaxNode(CHOP_STA, VOIDTYPE);

			temp_IR->SetChild0(m_IRTree->GetRoot());
			temp_IR->SetChild1(Goto);

			m_IRTree->SetRoot(temp_IR);

			m_More = 0;
		}
		else
		{
			//当前语句之前没有出现过，则压入栈中
			//首先生成当前Label的名称，该名称为“Label”+数字
			char labelNum[3];
			_itoa(m_Label, labelNum, 10);
			string sLabelNum = labelNum;
			newLabel = "Label_" + sLabelNum;

			Loop_Block block;
			block.label = newLabel;
			block.prgm = m_MutlWhile;
			m_loopStack.push(block);
		}
	}

	return newLabel;
}

///add by yangkai
void CMSV::__PreIfWhile()
{
	//该函数用于处理当前状态遇到的while和if语句

	//判断当前状态是否是多个while语句并行的情况
	string newLabel = __DetectLoop();

	//将当前状态已经得到的present部分加到IR树上
	__SortPrsnt();
	__AdPrst2IR();///将当前状态加入到IR树中

	if (m_IfTree != NULL)
	{
		//保存之前得到的future部分和more标志
		CSyntaxNode* temp_Futr = m_PrgmFuture->GetRoot();
		int temp_More = m_More;

		m_PrgmFuture->SetRoot(NULL);
		//之前的m_IRTree已经有内容，则将整个if语句得到的IR和之前的m_IRTree用chop连接起来
		if (m_IRTree->GetRoot() != NULL)
		{
			CSyntaxNode* temp_IR = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			temp_IR->SetChild0(m_IRTree->GetRoot());
			m_IRTree->SetRoot(NULL);
			__If(m_IfTree);

			temp_IR->SetChild1(m_IRTree->GetRoot());
			m_IRTree->SetRoot(temp_IR);
		}
		//之前m_IRTree为空，则将整个if语句得到的IR作为m_IRTree
		else
		{
			__If(m_IfTree);
		}

		///yangkai 处理while相关
		//如果出现多个while并行的语句则添加label节点
		if (newLabel != "")
		{
			CSyntaxNode* newBlock = new CSyntaxNode(LABEL_STA, newLabel, VOIDTYPE);
			m_Label++;
			newBlock->SetChild0(m_IRTree->GetRoot());
			m_IRTree->SetRoot(newBlock);
		}

		//恢复之前的future部分和more标志
		m_PrgmFuture->SetRoot(temp_Futr);
		m_More = temp_More;
	}
	else if (m_WhileTree != NULL)//yangkai
	{
		//保存之前得到的future部分和more标志
		CSyntaxNode* temp_Futr = m_PrgmFuture->GetRoot();
		int temp_More = m_More;

		m_PrgmFuture->SetRoot(NULL);
		//之前的m_IRTree已经有内容，则将整个while语句得到的IR和之前的m_IRTree用chop连接起来
		if (m_IRTree->GetRoot() != NULL)
		{
			CSyntaxNode* temp_IR = new CSyntaxNode(CHOP_STA, VOIDTYPE);
			temp_IR->SetChild0(m_IRTree->GetRoot());
			m_IRTree->SetRoot(NULL);
			__While(m_WhileTree);
			temp_IR->SetChild1(m_IRTree->GetRoot());
			m_IRTree->SetRoot(temp_IR);
		}
		//之前m_IRTree为空，则将整个if语句得到的IR作为m_IRTree
		else
		{
			__While(m_WhileTree);
		}

		//恢复之前的future部分和more标志
		m_PrgmFuture->SetRoot(temp_Futr);
		m_More = temp_More;
	}
}

void  CMSV::Simulation()
{
	///2015-4-21 add by wangmeng  加入全局变量
	CSyntaxNode* GlbTree = m_Program->GetRoot();
	if (GlbTree->GetNType() == FRAME_STA)
	{
		if (__SearchAndInsert(UNDEFINE, GlbTree, "", false, true))
		{
			m_Program->SetRoot(GlbTree->GetChild2());
			GlbTree->SetChild2(NULL);
			delete GlbTree;
			GlbTree = NULL;
		}
	}
	///end by wangmeng

	
    Pre();

	m_FrmSymbl->Clear();
	m_NFrmSymbl->Clear();

	///2015-3-31 add by wangmeng
	CSyntaxNode* output = __OutputState();
	m_IRTree->MergeTree(output, CHOP_STA, VOIDTYPE);
	
	return;
}

///2015-3-31 add by wangmeng
CSyntaxNode* CMSV::__OutputState()
{
	vector<string> SymTbl;

	map<string, Symbol*>::iterator iter;

	map<string, Symbol*>* Smap = m_NFrmSymbl->GetSymbolMap();
	map<string, Symbol*>* FSmap = m_FrmSymbl->GetSymbolMap();

	map<string, Symbol*>* GSmap = m_GlbSymbl->GetSymbolMap();

	string print = "";

	//函数调用时根据是内部还是外部调用判断是否需要输出每个状态
	if (g_InFunc)
	{
		print += "if(!$$Ext){";
	}

	print += "printf(\"state %d:\", $$state_num);\n";

	for (iter = GSmap->begin(); iter != GSmap->end(); ++iter)
	{
		string tmps = iter->first;
		SymTbl.push_back(tmps);
		if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
		{
			print += iter->second->PrntSym(iter->first);
		}
	}

	for (iter = Smap->begin(); iter != Smap->end(); ++iter)
	{
		string tmps = iter->first;
		SymTbl.push_back(tmps);
		if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
		{
		print += iter->second->PrntSym(iter->first);
		}				
	}

	for (iter = FSmap->begin(); iter != FSmap->end(); ++iter)
	{
		SymTbl.push_back(iter->first);
		if (!(*(iter->first.c_str()) == '$' && *(iter->first.c_str() + 1) == '$'))
		{
			print += iter->second->PrntSym(iter->first);
	}
		
	}

	print += "printf(\"\\n\");\n";
	print += "$$state_num++";

	if (g_InFunc)
	{
		print += ";\n}";
	}
	return new CSyntaxNode(STATE_OUTPUT_STA, VOIDTYPE, print, SymTbl);
}




void CMSV::__Input(CSyntaxNode* pTree)
				 {					 
	//先根据要输入的变量名在frame符号表(这里只查找frame符号表)找到该变量
	string varName = pTree->GetChild0()->GetNName();	

	///2015-4-21 add by wangmeng 加入全局符号表
	Symbol* Var = m_GlbSymbl->FindSymbol(varName);
	if (Var == NULL)
				 {
		Var = m_FrmSymbl->FindSymbol(varName);
		 }
	///end by wangmeng
		 

	string scanfStr = "scanf(";
	if (Var == NULL)
	{
		fout << "__Input error (3674)!" << endl;
		return;
	}
	else
	{
		//根据变量类型进行格式化输入
		switch (Var->GetSymbolType())
		{
		case SYMBOLINT:
			scanfStr += "\"%d\""; break;
		case SYMBOLFLOAT:
			scanfStr += "\"%f\""; break;
		case SYMBOLCHAR:
			scanfStr += "\"%c\""; break;
		default:
			break;
		}
	}
	scanfStr += ",";
	scanfStr += "&";
	scanfStr += varName;
	scanfStr += ")";

	CSyntaxNode* inputStmt = new CSyntaxNode(REQUEST_STA, scanfStr, VOIDTYPE);

	//将输入语句加入到当前状态中
	__InsrtAss(inputStmt);

	while (pTree->GetChild1() != NULL)
	{
		__Input(pTree->GetChild1());
		pTree = pTree->GetChild1();
	}

	m_More = -1;
	m_RdcOk = 1;
}

void CMSV::__Output(CSyntaxNode* pTree)
{
	string printfStr = "";
	switch (pTree->GetChild0()->GetNType())
	{
	case STR_EXP:
		printfStr = __OutputStr(pTree->GetChild0());
		break;
	case IDENT_EXP://output(x)
		printfStr = __OutputVar(pTree->GetChild0());
		break;
	case LIST_SYMBOL_STA://output(a[3])
		printfStr = __OutputArrayMember(pTree->GetChild0());
		break;
	default:
		break;
	}

	CSyntaxNode* outputStmt = new CSyntaxNode(DISPLAY_STA, printfStr, VOIDTYPE);

	//将输出语句加入到当前状态中
	__InsrtAss(outputStmt);

	while (pTree->GetChild1() != NULL)
	{
		__Output(pTree->GetChild1());
		pTree = pTree->GetChild1();
	}

	m_More = -1;
	m_RdcOk = 1;
}

string CMSV::__OutputStr(CSyntaxNode* pTree)
{
	string printfStr = "printf(";

		//输出字符串 如：output（“hello”） output("\n")
		printfStr += "\"";
	if (pTree->GetsValue() == "\n")
		{
			//转义字符
			printfStr += "\\n";
		}
		else
		{
		printfStr += pTree->GetsValue();
		}
		printfStr += "\"";
		printfStr += ")";

	return printfStr;
	}

string CMSV::__OutputVar(CSyntaxNode* pTree)
	{
	string printfStr = "printf(";

		//输出的是变量，先在符号表中找到该变量
		string varName = pTree->GetChild0()->GetNName();

		///2015-4-21 add by wangmeng 加入全局符号表
		Symbol* Var = m_GlbSymbl->FindSymbol(varName);
		if (Var == NULL)
		{
			Var = m_FrmSymbl->FindSymbol(varName);
		}
		///end by wangmeng

		if (Var == NULL)
		{
			fout << "__Output error (3714)!" << endl;
		return "";
		}
		else
		{
			//根据变量类型进行格式化输出
			switch (Var->GetSymbolType())
			{
			case SYMBOLINT:
				printfStr += "\"%d\""; break;
			case SYMBOLFLOAT:
				printfStr += "\"%f\""; break;
			case SYMBOLCHAR:
				printfStr += "\"%c\""; break;
			default:
				break;
			}
		}
		printfStr += ",";
		printfStr += varName;
		printfStr += ")";
	return printfStr;
	}

string CMSV::__OutputArrayMember(CSyntaxNode* pTree)
{
	string printfStr = "printf(";

	//输出的是变量，先在符号表中找到该变量
	string varName = pTree->GetNName();
	Symbol* Var = m_FrmSymbl->FindSymbol(varName);

	if (Var == NULL)
	{
		fout << "__Output error (3714)!" << endl;
		return "";
	}
	else
	{
		//根据数组类型进行格式化输出
		switch (Var->GetSymbolType())
	{
		case SYMBOLINT:
			printfStr += "\"%d\""; break;
		case SYMBOLFLOAT:
			printfStr += "\"%f\""; break;
		case SYMBOLCHAR:
			printfStr += "\"%c\""; break;
		default:
			break;
		}
	}

	printfStr += ",";
	printfStr += pTree->TreeToString();
	printfStr += ")";
	return printfStr;

	m_More = -1;
	m_RdcOk = 1;
}

/**
* 处理赋值号右边的函数调用,如果有函数掉用，处理后，将m_HasFunCal置为true
* @param1 赋值号右边的语法树
* @param2 是不是当前状态赋值
* @return void
*/
///2015-4-17 add by wangmeng
void CMSV::__AssRight(CSyntaxNode* RAssTree, bool IsPAss)
{
	if (RAssTree == NULL)
	{
		return;
	}
	switch (RAssTree->GetNType())
	{
	     case FUNCTION_STA:
	     {
			 if (IsPAss)
		{//modified by yubin 2015/4/22
			if (isFuncExist(RAssTree))//如果函数存在，并且是当前赋值，则提示错误
			 {
				 fout << "The right current assign cannot exist inner function call" << endl;
				 m_RdcOk = 0;
				 return;
			 }
			else//如果函数不存在，则当成是库函数调用
			{
				__FuncCall(RAssTree, 0);
				m_RdcOk = 1;
				return;
			}

		}
			 else
			 {
				 m_HasFunCal = true;
				 __FuncCall(RAssTree, 0);
				 m_RdcOk = 1;
				 return;
			 }
	     }
		 case EXT_FUNCTION_STA:
		 {
			 if (IsPAss)
			 {
				 __EFuncCall(RAssTree, 0);				 
			 }
			 m_RdcOk = 1;
			 return;
		 }
		 default:
		 {
			 if (RAssTree->GetChild0() != NULL)
			 {
			__AssRight(RAssTree->GetChild0(), IsPAss);
			 }
		if (RAssTree->GetChild1() != NULL)
			 {
				 __AssRight(RAssTree->GetChild1(), IsPAss);
			 }

	}
	}
}
			 
//add by yubin 2015/4/22,判断函数pTree是否出现过
bool CMSV::isFuncExist(CSyntaxNode* pTree)
{
	CSyntaxNode* p = function_tree;//p指向函数定义语法树
	while (p != NULL)//while循环用于在函数定义树查找当前调用的函数
	{
		if (p->GetNName() == pTree->GetNName())
		{
			return 1;
		 }
		p = p->GetChild2();
	}
	return 0;
}

/**
* 将变量声明加入到符号表中
* @param 待加入的变量
* @return void
*/
///2015-4-22 add by wangmeng 
bool CMSV::__InstDec2ST(CSyntaxNode*pTree)
{
		SymbolType type;
		if (pTree->GetNType() == UNSIGN_DECLARATION_STA)
			type = SymbolType(SymbolTable::GetSymbolType(pTree->GetChild0()->GetRType()) + 12);//改动枚举类型SymbolType需改动这里
		else
			type = SymbolTable::GetSymbolType(pTree->GetRType());
		string name = "";
		if (pTree->GetRType() == STRUCTTYPE)
			name = pTree->GetNName();


		if (!__SearchAndInsert(type, pTree, name))
		{
		fout << "__InstDec2ST: declaration of variable fail : " << pTree->TreeToString().data() << endl;
			m_RdcOk = 0;
			return false;
	}
		return true;

}

///清空m_UnFrm和m_PreUnFrm
///2015-4-22 add by wangmeng 
void CMSV::__ClearUnFrm()
{
	CSyntaxNode* delTree = NULL;
	while (!m_UnFrm.empty())
	{
		delTree = m_UnFrm.top();
		delete delTree;
		m_UnFrm.pop();
	}
	while (!m_PreUnFrm.empty())
	{
		delTree = m_PreUnFrm.top();
		delete delTree;
		m_PreUnFrm.pop();
	}

}

///输出全局变量
///2015-4-22 add by wangmeng
void CMSV::PrintGlbVar()
{
	fout << m_GlbVarTree->GetRoot()->TreeToString() << ";"<< endl;
}