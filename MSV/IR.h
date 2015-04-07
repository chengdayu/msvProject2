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
	Module* module;
	llvm::IRBuilder<> *builder;

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

	void Stmt2IR(CSyntaxNode* pTree); //��ÿ�����ת�ɶ�Ӧ��IR����

	void __Declr2IR(CSyntaxNode *pTree);

	void __DeclrInt2IR(CSyntaxNode *pTree);//����int���͵�����

	void __Chop2IR(CSyntaxNode *pTree);//����chop���͵�����


};