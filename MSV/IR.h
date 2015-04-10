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

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"


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
	* @return 转换之后的结果
	*/
	///2015-4-7 add by wangmeng
	Value * __Expr2IR(CSyntaxNode* pTree);

	/**
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

	/**
	* 加法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value * __Add2IR(CSyntaxNode* pTree);

	/**
	* 减法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value * __Sub2IR(CSyntaxNode* pTree);

	/**
	* 乘法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value * __Mul2IR(CSyntaxNode* pTree);

	/**
	* 除法操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value * __Div2IR(CSyntaxNode* pTree);

	/**
	* 取余操作转成对应的IR代码
	* @param 待处理的语法树
	* @return 转之后的结果
	*/
	Value * __Mod2IR(CSyntaxNode* pTree);

	/**
	* if语句转成对应的IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	void __If2IR(CSyntaxNode* pTree);

	/**
	* 将if语句和while语句中的条件表达式转为IR代码
	* @param 传入待分析的语法树
	* @return void
	*/
	Value* __Cond2IR(CSyntaxNode* pTree);

	/**
	* 强制转换语句转为IR代码
	* @param 传入待分析的语法树
	* @return 转换后的IR代码
	*/
	Value* __Cast2IR(Value *value, Type *type);

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