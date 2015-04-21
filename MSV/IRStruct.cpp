#include "IR.h"
#include <iostream>

IRStruct::IRStruct(Module *module, string name, CSyntaxNode *pTree)
{
	s_name = name;
	isDef = true;

	CSyntaxNode* visit = pTree;
	while (isDef && visit != NULL)
	{
		switch (visit->GetNType())
		{
			case AND_EXP:{
				__Init(module, visit->GetChild0());
				visit = visit->GetChild1();
				break;
			}

			case DECLARE_STA:
			case UNSIGN_DECLARATION_STA:{
				__Init(module, visit);
				visit = NULL;
				break;
			}

			default: {
				cout << "IRStruct : invalid case " << endl;
				isDef = false;
				break;
			}
		}
	}

	s_type = StructType::create(module->getContext(),types,s_name);

}

IRStruct::~IRStruct()
{

}

void IRStruct::__Init(Module *module, CSyntaxNode *pTree)
{
	Type* type = IR::GetType(module, pTree);

	if (type == NULL)
	{
		cout << "__Init : struct define failed";
		isDef = false;
		return;
	}

	CSyntaxNode* visit = pTree->GetChild0();
	while (isDef && visit != NULL)
	{
		
		switch (visit->GetChild0()->GetNType())
		{
		case IDENT_EXP:{
			__DeclrIdent(type, visit->GetChild0()->GetNName());
			break;
		}

		case ADDRESS_DECLARE_STA:{
			__DeclrPtr(type, visit->GetChild0());
			break;
		}

		case ARRAY_DECLARE_STA:{
			__DeclrArray(type, visit->GetChild0());
			break;
		}

		default: {
			cout << "__Init : Type " << visit->GetChild0()->GetNType() << "has not considered " << endl;
			isDef = false;
			break;
		}

		}

		visit = visit->GetChild1();
	}

}

void IRStruct::__DeclrIdent(Type *type, string name)
{
	types.push_back(type);
	if (position.insert(make_pair(name, types.size())).second == false)
	{
		cout << "__Init : variable " << name << "has already defined" << endl;
		isDef = false;
	}
}

void IRStruct::__DeclrPtr(Type *type, CSyntaxNode *pTree)
{
	while (pTree != NULL)
	{
		if (pTree->GetNType() == ADDRESS_DECLARE_STA)
			type = PointerType::get(type, 0);//ADDRESS_SAPCE_GENERIC ??

		else if (pTree->GetNType() == ARRAY_DECLARE_STA)
		{
			__DeclrArray(type, pTree);
			return;
		}

		else if (pTree->GetNType() == IDENT_EXP)
		{
			__DeclrIdent(type, pTree->GetNName());
			return;
		}

		pTree = pTree->GetChild0();
	}
}

void IRStruct::__DeclrArray(Type *type, CSyntaxNode *pTree)
{
	string name = pTree->GetNName();

	vector<int> subscript;
	while (pTree != NULL)
	{
		if (pTree->GetChild0()->GetNType() == INTEGER_EXP)
			subscript.push_back(pTree->GetChild0()->GetiValue());
		else if (pTree->GetChild0()->GetChild0() != NULL)
		{
			//算术表达式的情况暂时不考虑
		}
		else
		{
			cout << "__DeclrArray: invalid array subscript " << endl;
			isDef = false;
		}

		pTree = pTree->GetChild1();
	}

	for (int i = subscript.size() - 1; i >= 0; i--)
		type = ArrayType::get(type,subscript.at(i));

	__DeclrIdent(type,name);
}