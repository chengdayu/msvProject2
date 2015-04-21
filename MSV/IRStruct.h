
///create by shiyifang 2015-04-15

#ifndef IRSTRUCT_H_
#define IRSTRUCT_H_

#include <map>
#include <vector>
#include "SyntaxNode.h"
#include "SyntaxTree.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

using namespace llvm;
using namespace std;

/*
 *Translate the struct tree！！pTree to two parts
 *one is vector<llvm::Type*> : store the struct member's types in order
 *the other is map<string,int> : store the map between member's name and member's position
 *then we can get the StructType* of this struct
 *that can help us handle the define 、declar and use of struct in llvm IR.
*/
class IRStruct
{
public:
	/*
	 *更夛痕方
	 *@param module(the module you define the struct) name(struct name)  pTree(struct member tree)
	*/
	IRStruct(Module *module, string name, CSyntaxNode *pTree);
	~IRStruct();

	inline string GetStructName()
	{
		return s_name;
	}

	inline void SetStructName(string name)
	{
		s_name = name;
	}

	inline bool IsDefined()
	{
		return isDef;
	}

	inline std::vector<llvm::Type*> GetAllTypes()
	{
		return types;
	}

	inline StructType* GetStructType()
	{
		return s_type;
	}

	/*
	*
	*/
	llvm::Type* GetElementType(string EleName);

	/*
	 *Get the element's position by its name
	 *@param EleName(the element's name)
	 *return its poision if we find it , else return -1
	*/
	int GetElementPos(string EleName);

private:
	/*
	*
	*/
	void __Init(Module *module, CSyntaxNode *pTree);

	void __DeclrIdent(Type *type, string name);

	void __DeclrPtr(Type *type, CSyntaxNode *pTree);

	void __DeclrArray(Type *type, CSyntaxNode *pTree);

	string s_name;//the name of the struct

	bool isDef;//whether this struct has been defined successfully 

	map <string, int> position; //a map between struct member's name and member's position

	std::vector<llvm::Type*> types;//store struct member's type in order

	StructType* s_type;// type of the struct
};


#endif // !IRSTRUCT_H_
