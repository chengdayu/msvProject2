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
public:
	///���캯��
	IR();

	///��������
	~IR();

	/**add by yubin 2015-4-7
	* ����m_IRTree,����ÿ����㣬����Stmt2IR����
	* @param �����������m_IRTree
	* @return void
	*/
	void Trslt2IR(CSyntaxTree *IRTree);

	/**
	* ��ÿ�����ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void Stmt2IR(CSyntaxNode* pTree); //

private:
	/**
	* ������ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Declr2IR(CSyntaxNode *pTree);

	/**
	* ����int���͵�����
	* @param ������������﷨��
	* @return void
	*/
	void __DeclrInt2IR(CSyntaxNode *pTree);

	/**
	* ��ֵ���ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Ass2IR(CSyntaxNode* pTree);

	/**
	* ˳�����ת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return void
	*/
	void __Chop2IR(CSyntaxNode *pTree);

	/**
	* ���ʽת�ɶ�Ӧ��IR����
	* @param ������������﷨��
	* @return �õ��ı��ʽֵ
	*/
	Value * __Expr2IR(CSyntaxNode* pTree);

private:

	///
	llvm::IRBuilder<> *builder;

	///
	Module* module;

	///���ű�
	map<string, AllocaInst *> m_IRSTable;
	
};