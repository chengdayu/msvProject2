#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include "llvm/ADT/SmallVector.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"


#include "SyntaxTree.h"

using namespace llvm;
class IR
{
public:
	///构造函数
	IR();
	~IR();

	/**add by yubin 2015-4-7
	* 遍历m_IRTree,遇到每个结点，调用Stmt2IR函数
	* @param 传入待分析的m_IRTree
	* @return void
	*/
	void Trslt2IR(CSyntaxTree *IRTree);

	/**add by yubin 2015-4-7
	* 将每个结点转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void Stmt2IR(CSyntaxNode* pTree); //

	/**add by yubin 2015-4-7
	* 将声明转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Declr2IR(CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* 处理int类型的声明
	* @param 传入待分析的语法树
	* @return void
	*/
	void __DeclrInt2IR(CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* 处理float类型的声明
	* @param 传入待分析的语法树
	* @return void
	*/
	void __DeclrFloat2IR(CSyntaxNode *pTree);

	/**
	* 赋值语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Ass2IR(CSyntaxNode* pTree);

	/**add by yubin 2015-4-7
	* 顺序语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Chop2IR(CSyntaxNode *pTree);

	/**
	* 表达式转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	///2015-4-7 add by wangmeng
	Value * __Expr2IR(CSyntaxNode* pTree);

	/**add by yubin 2015-4-9
	* 输出语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __Out2IR(CSyntaxNode *pTree);

	/**
	* 变量自加操作转成对应的IR代码
	* @param 待处理的变量
	* @return void
	*/
	void __AddOne2IR(AllocaInst * alloc);

private:

	///
	llvm::IRBuilder<> *m_builder;

	///
	Module* m_module;

	///符号表
	map<string, AllocaInst *> m_IRSTable;

	///状态数
	AllocaInst *m_StNum;

};

#endif