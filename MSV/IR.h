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
	///构造函数
	IR();
	~IR();

	/**add by yubin 2015-4-7
	* 遍历m_IRTree,遇到每个结点，调用Stmt2IR函数
	* @param 传入待分析的m_IRTree
	* @return void
	*/
	void Trslt2IR(CSyntaxTree *IRTree);

	void Stmt2IR(CSyntaxNode* pTree); //将每个结点转成对应的IR代码

	void __Declr2IR(CSyntaxNode *pTree);

	void __DeclrInt2IR(CSyntaxNode *pTree);//处理int类型的声明

	void __Chop2IR(CSyntaxNode *pTree);//处理chop类型的声明


};