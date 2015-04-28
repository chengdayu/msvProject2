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

#include "IRStruct.h"
#include "SyntaxTree.h"


using namespace llvm;

static map<string, IRStruct*> m_IRStruct;///�ṹ���� �� IRStruct��ӳ��



#include "SyntaxTree.h"

using namespace llvm;


class IRSymbol
{
public:
	///���캯��
	IRSymbol();
	IRSymbol(AllocaInst* InstVar, bool sign);
	~IRSymbol();
	inline void SetIsSigned(bool sign)
	{
		IsSigned = sign;
	}
	inline bool GetIsSigned()
	{
		return IsSigned;
	}
	inline void SetAllocaInstVar(AllocaInst* InstVar)
	{
		AllocaInstVar = InstVar;
	}
	inline AllocaInst* GetAllocaInstVar()
	{
		return AllocaInstVar;
	}


private:
	bool IsSigned;
	AllocaInst* AllocaInstVar;
};

class IR
{
public:
	///���캯��
	IR();
	~IR();

	/**add by yubin 2015-4-7
	* ����m_IRTree,����ÿ����㣬����Stmt2IR����
	* @param �����������m_IRTree
	* @return void
	*/
	void Trslt2IR(CSyntaxTree *IRTree);

	/**add by yubin 2015-4-7
	* ��ÿ�����ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void Stmt2IR(CSyntaxNode* pTree); //


	/*
	*���ݽṹ�嶨���ģ���declare���õ���Ӧ��llvm type
	*@param module (�����ṹ���ģ��) pTree(�������)
	*@return ��������Ӧ��llvm type
	*/
	//add by shiyifang 2015-04-16
	static Type* GetType(Module *m_module, CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	///changed by shiyifang 2015-04-17
	void __Declr2IR(CSyntaxNode *pTree, bool sign);

	/*
	*����һ�������ı���
	*@param  type(��������)  name(������)  alignment(���뷽ʽ)
	*/
	void __DeclrIdent(Type *type, CSyntaxNode *pTree, int alignment, bool sign);

	/*
	*����һ��ָ�����ͱ���
	*@param  type(��������)  name(������)  alignment(���뷽ʽ)
	*/
	void __DeclrPtr(Type *type, CSyntaxNode *pTree, int alignment, bool sign);

	/*
	*����һ���������ͱ���
	*@param  type(��������)  name(������)  alignment(���뷽ʽ)
	*/
	void __DeclrArray(Type *type, CSyntaxNode *pTree, int alignment, bool sign);

	/**
	* ��ֵ���ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Ass2IR(CSyntaxNode* pTree);

	/**add by yubin 2015-4-7
	* ˳�����ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Chop2IR(CSyntaxNode *pTree);

	/**
	* ���ʽת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return ת��֮��Ľ��
	*/
	///2015-4-7 add by wangmeng
	Value * __Expr2IR(CSyntaxNode* pTree);

	/**
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Out2IR(CSyntaxNode *pTree);

	/**
	* �����ԼӲ���ת�ɶ�Ӧ��IR����
	* @param ������ı���
	* @return void
	*/
	void __AddOne2IR(AllocaInst * alloc);

	/**
	* �ӷ�����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value * __Add2IR(CSyntaxNode* pTree);

	/**
	* ��������ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value * __Sub2IR(CSyntaxNode* pTree);

	/**
	* �˷�����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value * __Mul2IR(CSyntaxNode* pTree);

	/**
	* ��������ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value * __Div2IR(CSyntaxNode* pTree);

	/**
	* ȡ�����ת�ɶ�Ӧ��IR����
	* @param ��������﷨��
	* @return ת֮��Ľ��
	*/
	Value * __Mod2IR(CSyntaxNode* pTree);

	/**
	* if���ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __If2IR(CSyntaxNode* pTree);

	/**
	* ��if����while����е��������ʽתΪIR����
	* @param ������������﷨��
	* @return void
	*/
	Value* __Cond2IR(CSyntaxNode* pTree);

	/**
	* ǿ��ת�����תΪIR����
	* @param ������������﷨��
	* @return ת�����IR����
	*/
	Value* __Cast2IR(Value *value, Type *type);

	/**
	* �����������תΪIR����
	* @param ������������﷨��
	* @return ת�����IR����
	*/
	///2015-4-14 add by daichunchun
	Value* IR::__Call2IR(CSyntaxNode *pTree);

	bool InstIRSymbol(string name, AllocaInst* InstVar, bool sign);

private:
	///
	llvm::IRBuilder<> *m_builder;

	///
	Module* m_module;

	///���ű�
	//map<string, AllocaInst *> m_IRSTable;
	map<string, IRSymbol *> m_IRSTable;

	///add by syf 2015-04-16
	map<string, string> m_SVtable;//�ṹ����������ṹ������ӳ��

	///״̬��
	AllocaInst *m_StNum;

};

#endif