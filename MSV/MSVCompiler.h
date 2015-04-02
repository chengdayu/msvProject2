/*******************************************************************************
*@file  MSVCompiler.h
*@author
*@remarks  Modified by
*@version  1.0
*@section  Brief
*Modified: 2015/03/06  11:51
*
*
*
*Copyright (C) ICTT.,XiDian univ
*All rights reserved.
*
*******************************************************************************/

//add by wm2013.04.18
#ifndef MSVCOMPILER_H_INCLUDED
#define MSVCOMPILER_H_INCLUDED

#include<stack>
#include<vector>
#include "SyntaxNode.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"

/**2015-3-9 add by shiyifang
*
*赋值语句结构体，用于排序
*seq：在present中的序号
*left: 赋值左边的标识符,多余1个会处理
*right：应属于右边的标识符
*/
typedef struct {
	int seq;
	vector<string> left;
	vector<string> right;
} Ass_Struct;

/**2015-3-9 add by yangkai
*用于存储多个while并行的程序片段
*label: 给该程序片段添加的label名
*prgm: 程序片段
*/
typedef struct{
	string label;
	string prgm;
}Loop_Block;

/**
*Compiler class
*/
class CMSV
{
public:
	///constructor
	CMSV();
	///destructor
	virtual  ~CMSV();

	/**
	* Get present tree of pTree, including the assignment statement at present state
	* @param the syntax tree to be handled
	* @return void
	*/
	void GetPresent(CSyntaxNode* pTree);

	///Preprocess the tree
	void Pre();

	///输出当前状态赋值语句
	void PrintPrsnt();

	///输出m_IRTree
	void PrintIRTree();

	void Simulation();

	inline CSyntaxTree* GetIRTree()
	{
		return m_IRTree;
	}


private:
	/**
	* Get present tree of declaration statement: int x,y<==x
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Declr(CSyntaxNode* pTree);
	/**
	* Get present tree of assignment statement: x<==e
	* @param the syntax tree to be handled
	* @return void
	*/
	void __ASS(CSyntaxNode* pTree);

	/**
	* Get present tree of unit assignment statement: x:=e
	* @param the syntax tree to be handled
	* @return void
	*/
	void __UASS(CSyntaxNode* pTree);

	/**
	* Get present tree of empty statement: empty
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Emp(CSyntaxNode* pTree);

	/**
	* Get present tree of more statement: more
	* @param the syntax tree to be handled
	* @return void
	*/
	void __More(CSyntaxNode* pTree);

	/**
	* Get present tree of true statement: true
	* @param the syntax tree to be handled
	* @return void
	*/
	void __True(CSyntaxNode* pTree);

	/**
	* Get present tree of skip statement: skip
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Skip(CSyntaxNode* pTree);

	/**
	* Get present tree of length statement: len(n);
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Len(CSyntaxNode* pTree);

	/**
	* Get present tree of next statement: next p
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Next(CSyntaxNode* pTree);

	/**
	* Get present tree of and statement: p and q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __And(CSyntaxNode* pTree);

	/**
	* Get present tree of parallel statement: p||q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Pal(CSyntaxNode* pTree);

	/**
	* Get present tree of projection statement: {p1,p2,...}prj q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Prj(CSyntaxNode* pTree);

	/**
	* Handle left of prj: {p1,p2,...}prj q
	* @param the syntax tree to be handled
	* @return 投影左子树处理到的结点位置，如果p1 是p and empty形式，而p2有下一状态，则返回p2节点的位置
	*/
	CSyntaxNode* __PrjLeft(CSyntaxNode*pTree);

	/**
	* Handle right of prj: {p1,p2,...}prj q
	* @param the syntax tree to be handled
	* @param whether the left of prj is the last state
	* @return void
	*/
	void __PrjRight(CSyntaxNode*pTree, int LMore);

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
	CSyntaxNode* __PrjObFutr(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* RFutr, CSyntaxNode* visit, int LMore, int RMore);

	/**
	* Handle the next state program of prj when both the left and right have next state
	* @param1  处理的投影树{p1,p2,...}prj q
	* @param2  投影左边处理后得到的下一状态语法树
	* @param3   投影右边处理后得到的下一状态语法树
	* @param4   投影左边处理到的节点位置
	* @return  处理以后得到的prj的下一状态语法树
	*/
	CSyntaxNode * __PrjLRRmn(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* RFutr, CSyntaxNode* visit);


	/**
	* Handle the next state program of prj when both the left and right are if statement
	* @param1  处理的投影树{p1,p2,...}prj q
	* @param2  投影左边处理到的节点位置
	* @return  void
	*/
	void __PrjLRIf(CSyntaxNode* pTree, CSyntaxNode* visit);

	/**
	* Handle the next state program of prj when only the left is if statement
	* @param1  处理的投影树{p1,p2,...}prj q
	* @param2   投影右边处理后得到的下一状态语法树
	* @param3  投影左边处理到的节点位置
	* @return  void
	*/
	void __PrjLIf(CSyntaxNode* pTree, CSyntaxNode* RFutr, CSyntaxNode* visit);

	/**
	* Handle the next state program of prj when only the right is if statement
	* @param1  处理的投影树{p1,p2,...}prj q
	* @param2  投影左边处理后得到的下一状态语法树
	* @param3  投影左边处理到的节点位置
	* @return  void
	*/
	void __PrjRIf(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* visit);

	/**
	* Handle the next state program of prj when only the left has next state
	* @param1  处理的投影树{p1,p2,...}prj q
	* @param2  投影左边处理后得到的下一状态语法树
	* @param3   投影左边处理到的节点位置
	* @return  处理以后得到的prj的下一状态语法树
	*/
	CSyntaxNode * __PrjLRmn(CSyntaxNode* pTree, CSyntaxNode* LFutr, CSyntaxNode* visit);

	/**
	* Get present tree of chop statement: p;q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Chop(CSyntaxNode* pTree);

	/**
	* Get present tree of if statement: if(b)then{p}else{q}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __If(CSyntaxNode* pTree);

	/**
	* Get present tree of while statement: while(b){p}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __While(CSyntaxNode* pTree);

	/**
	* Get present tree of for statement: for(p1,b,p2){q}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __For(CSyntaxNode* pTree);

	/**
	* Get present tree of switch statement: switch(e){case n1: (p1)...}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Switch(CSyntaxNode* pTree);

	/**
	* Get present tree of frame statement: frame(c)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Frm(CSyntaxNode* pTree);

	/**
	* Get present tree of unframe statement: unframe(c)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __UnFrm(CSyntaxNode* pTree);

	/**
	* Get present tree of or statement: p or q
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Or(CSyntaxNode* pTree);

	/**
	* Get present tree of exist statement: exists(x,y,...){P}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Ext(CSyntaxNode* pTree);

	/**
	* Get present tree of function definition statement: fuction fname(e1,e2,...){P}
	* @param the syntax tree to be handled
	* @return void
	*/
	void __FuncDef(CSyntaxNode* pTree);

	/**
	* Get present tree of function call statement: fname(e1,e2,...)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __FuncCall(CSyntaxNode* pTree);

	/**
	* Get present tree of extern function call statement: extern fname(e1,e2,...)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __EFuncCall(CSyntaxNode* pTree);

	/**
	* Get present tree of await statement: await(b)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Await(CSyntaxNode* pTree);

	/**
	* Get present tree of final statement: fin(p)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Fin(CSyntaxNode* pTree);

	/**
	* Get present tree of always statement: always(p)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Alw(CSyntaxNode* pTree);

	/**
	* Get present tree of always statement: always(p)
	* @param the syntax tree to be handled
	* @return void
	*/
	///2015-3-24 add by wangmeng
	void __AlwPost(CSyntaxNode* pTree);

	/**
	* Get present tree of keep statement: keep(p)
	* @param the syntax tree to be handled
	* @return void
	*/
	void __Keep(CSyntaxNode* pTree);

	/**
	* Insert the present assignment to the present vector.
	* @param the syntax tree to be handled
	* @return void
	*/
	void __InsrtAss(CSyntaxNode*pTree);

	///将当前状态赋值的链表加入到IRTree中
	void __AdPrst2IR();

	///当得到区间长度后处理对应的keep语句
	void __KeepMore();

	///当得到区间长度后处理对应的fin语句
	void __FinMore();

	///当得到区间长度后处理对应的alw语句
	void __AlwMore();

	///当得到区间长度后处理对应的unframe语句
	void __AddUnFrm();

	///统一处理keep，fin，alw
	void __KpFnAl();

	///释放frame变量
	void __FreeSymbl();

	/**
	* 获得节点后代中的所有标识符
	* @param pTree要遍历的节点，right存放获得的标识符
	*/
	///2015-03-10
	void __GetRightId(CSyntaxNode*pTree, vector<string> &right);

	/*赋值符号左边为指针时，取得左边所有指针,并将其余标识符加入右边
	*@param  pTree(VALUE_EXP的孩子) stru(存放标识符的结构体)
	*/
	///2015-03-12
	void __GetLeftId(CSyntaxNode*pTree, Ass_Struct &stru);

	/*节点类型为赋值时 获得赋值的左值和右值
	* @param pTree(赋值节点)  stru(存放标识符的结构体)
	*/
	///2015-03-12
	void __GetAssStruct(CSyntaxNode*pTree, Ass_Struct &stru);

	/*声明变量时 插入所有变量
	* type(声明类型)  pTree(声明节点)
	*/
	///2015-03-13
	bool __SearchAndInsert(SymbolType type, CSyntaxNode *pTree);

	/*
	* 为获得的所有当前状态排序,声明并检查变量
	* 排序顺序: EX_ASS 声明 ASS_EXP
	* 排序规则：使用快排，比较左值是否包含右值
	*/
	///2015-03-13
	void __SortPrsnt();


	/**
	* Deal with the situation that one subtree of AND_EXP node is IF_ELSE_STA statement
	* @param the syntax tree to be handled
	* @return void
	*/
	void __AndIf(CSyntaxNode* pTree);

	/**
	* Deal with the situation that one subtree of PARALLEL_STA node is IF_ELSE_STA statement
	* @param the syntax tree to be handled
	* @return void
	*/
	void __PalIf(CSyntaxNode* pTree);


	/**
	* Deal with the situation that right subtree of AND_EXP node is IF_ELSE_STA statement
	* @param if tree
	* @param left future tree
	* @return void
	*/
	///左边不是if语句，右边是if语句
	///2015-3-19 add by wangmeng
	void __AndRIf(CSyntaxNode*IfTree, CSyntaxNode*LFutr);


	/**
	* Deal with the situation that left subtree of CHOP_STA node is IF_ELSE_STA statement and
	* the CHOP_STA node is a subtree of AND_EXP or PARALLEL_STA node
	* @param the syntax tree to be handled
	* @return void
	*/
	void __ChopIf(CSyntaxNode* pTree);


	/**
	* Transforming an if statement to a while statement
	* @param the syntax tree to be dealed with
	* @return void
	*/
	void __While2If(CSyntaxNode* pTree);

	/**
	* 处理多个while并行的情况，这种情况下可能出现循环
	* @param
	* @return bool
	*/
	string __DetectLoop();

	/**
	* 该函数用于对if语句和while语句的预处理
	* @param
	* @return void
	*/
	void __PreIfWhile();


	///输出语句
	CSyntaxNode* __Output();


private:
	///the syntax tree of program
	CSyntaxTree  *m_Program;

	///the syntax tree of next state program
	CSyntaxTree  *m_PrgmFuture;

	///present 链表
	vector <CSyntaxNode*> m_PrgmPrsnt;

	///存储待处理keep语句
	stack <CSyntaxNode*> m_KeepTree;
	///存储待处理fin语句
	stack <CSyntaxNode*> m_FinTree;
	///存储待处理alw语句
	stack <CSyntaxNode*> m_AlwTree;

	///存储待处理unframe语句
	stack <CSyntaxNode*> m_UnFrm;


	///IRTree
	CSyntaxTree  *m_IRTree;

	///记录遇到的if
	CSyntaxNode* m_IfTree;

	///记录遇到的while
	CSyntaxNode* m_WhileTree;

	///frame符号表
	SymbolTable *m_FrmSymbl;

	///没有进行frame的变量的符号表
	SymbolTable * m_NFrmSymbl;



	///current interval terminates when m_Done=1
	int m_Done;

	///initial value:-1; exist next state when m_More=1; the final state when m_More=0
	int m_More;

	///initial value:-1; program get present tree successfully when m_RdcOk=1; failed when m_RdOk=0;
	int m_RdcOk;

	int m_Keep;  ///m_KeepTree需要想外边弹出几个keep语句
	int m_Fin;
	int m_Alw;

	//int m_Frm;
	int m_FreeFrm; ///m_FreeFrm值为0则不删除frame变量，m_FreeFrm值为1则准备在下一状态删除。m_FreeFrm值为2删除frame变量

	int m_InAndPal;//yangkai 记录当前处理的语句是否在and或pal语句中

	string m_MutlWhile;//yangkai 存储当前状态遇到的多个while并行的语句

	stack<Loop_Block> m_loopStack;//yangkai 存储多个while并行的语句

	int m_Label;//yangkai 处理while相关

	//2015-3-16,add by yubin,定义一个map，里面存储每个变量之前出现的次数
	map <string, int> existVarTimes;
};

#endif
