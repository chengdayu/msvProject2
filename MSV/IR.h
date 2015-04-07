
#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED

#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "SyntaxTree.h"

using namespace llvm;
class IR
{
private:
	Module* m_module;
	llvm::IRBuilder<> *m_builder;
	map<string, AllocaInst *>m_IRSTable;

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

	/**add by yubin 2015-4-7
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Declr2IR(CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* ����int���͵�����
	* @param ������������﷨��
	* @return void
	*/
	void __DeclrInt2IR(CSyntaxNode *pTree);

	/**add by yubin 2015-4-7
	* ����float���͵�����
	* @param ������������﷨��
	* @return void
	*/
	void __DeclrFloat2IR(CSyntaxNode *pTree);

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
};

#endif 