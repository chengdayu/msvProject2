#include"function.h"
#include "SymbolTable.h"


SymbolTable::SymbolTable()
{
	m_SymbolMap = map<string, Symbol*>();
}


/*
*是否包含符号
*@param string(也就是key)
*return bool
*/
///2015-03-11 add by shiyifang
bool SymbolTable::HasSymbol(string symbol)
{
	return (m_SymbolMap.find(symbol) != m_SymbolMap.end());
}

/*
* 检查此类型是否为指针
*@param 符号类型
*return bool
*/
///2015-3-12 add by shiyifang
///若SymbolType改变，应考虑此函数是否需要改变
bool SymbolTable::IsPointer(SymbolType type)
{
	if (type>SYMBOLINT && type < SYMBOLNULL && type % 2 == 0)
		return true;
	return false;
}

/*
* 向符号表中插入数据
* @param string (结点名) SymbolType(符号类型)  int(数组长度)
* return bool
*/
///2015-03-13 add by shiyifang
bool SymbolTable::InstSymbol(string NName, SymbolType type, int array_num)
{
	return m_SymbolMap.insert(make_pair(NName, new Symbol(type, array_num))).second;
}

/*
* 向符号表中插入数据
* @param string (结点名) SymbolType(符号类型)  int(数组长度)
* return bool
*/
///2015-03-17 add by wangmeng
bool SymbolTable::InstFSymbl(string NName)
{
	Symbol* symbl = FindSymbol(NName);
	if (symbl != NULL)
	{
		symbl->SetNFrm(symbl->GetNFrm() + 1);
		return true;
	}
	return m_SymbolMap.insert(make_pair(NName, new Symbol(UNDEFINE))).second;
}

/*
* 判断节点声明的符号类型
* @param RETURNTYPE
* return SymbolType
*/
///2015-03-11 add by shiyifang
SymbolType SymbolTable::GetSymbolType(RETURNTYPE type)
{
	switch (type)
	{
	case INTTYPE:     		{ return SYMBOLINT; }     //return integer type
	case STRTYPE:     		{ return SYMBOLSTR; }   	//return string type
	case FILETYPE:	 		{ return SYMBOLFILE; }		//add by yubin, return file type

	case FLOATTYPE:   		{ return SYMBOLFLOAT; }     //2013-4-26 add by YY[fixed]
	case CHARTYPE:    		{ return SYMBOLCHAR; }     //2013-4-26 add by YY[fixed]
							//case ARITHMETICTYPE:  	{ return ;} 	//2013-4-26 add by YY[fixed]

	case LISTTYPE:		 	{ return SYMBOLLIST; }		// return list value
	case BOOLTYPE:        	{ return SYMBOLBOOL; }		//return bool type
							//case POINTERTYPE:    	{ return SYMBOLPOINTER;} 	//return pointer value
							//case SYMBOLTYPE:		{ return ;}		// return symbol value
							//case CHANNELRTYPE:		{ return ;}		//add by mdp 2010-07-09


	case INTPTYPE:         	{ return SYMBOLINTP; }		//return integer pointer type
	case UINTPTYPE:		   	{ return SYMBOLUINT; }
	case FLOATPTYPE:		{ return SYMBOLFLOATP; }
	case UCHARPTYPE:		{ return SYMBOLCHAR; }
	case STRUCTTYPE:		{ return SYMBOLSTRUCT; }
	case STRUCTPTYPE:		{ return SYMBOLSTRUCTP; }

							//case DOUBLEINTPTYPE:	{ return ;}
							//case DOUBLEUINTPTYPE:	{ return ;}
							//case DOUBLEFLOATPTYPE:	{ return ;}
							//case DOUBLEUCHARPTYPE:	{ return ;}
							//case DOUBLESTRUCTPTYPE:	{ return ;}
	case VOIDTYPE:			{ return SYMBOLVOID; }
	case VOIDPTYPE:			{ return SYMBOLVOIDP; }
							//add by yubin 2013-12-23 15:31 函数指针
							//case FUNCPTYPE:			{ return SYMBOLFUNCP;}

							//add by YY 2014/01/07 10:57 强制转换
	case CHARPTYPE:			{ return SYMBOLCHARP; }
							//case DOUBLECHARPTYPE:	{ return ;}
	case UINTTYPE:			{ return SYMBOLUINTP; }
	case UCHARTYPE:			{ return SYMBOLUCHARP; }
							//case STRUCT_NAME_TYPE:	{ return ;}
							//case FUNCPPTYPE:		{ return ;}
	default:   			return UNDEFINE;
	}
}

/*
* 查找符号
* @param string
* return Symbol*
*/
///2015-03-12 add by shiyifang
Symbol*  SymbolTable::FindSymbol(string symbol)
{
	map<string, Symbol*>::iterator it = m_SymbolMap.find(symbol);
	if (it == m_SymbolMap.end())
		return NULL;
	return (*it).second;
}

/*
* 删除符号表中所有元素
*/
///2015-03-13 add by shiyifang
void SymbolTable::Clear()
{
	map<string, Symbol*>::iterator it;
	for (it = m_SymbolMap.begin(); it != m_SymbolMap.end(); it++)
		delete (*it).second;

	m_SymbolMap.clear();
}

/*
* 删除符号表中指定符号
* @param  string
*/
///2015-03-13 add by shiyifang
void SymbolTable::Delete(string symbol)
{
	Symbol* symbl = FindSymbol(symbol);
	if (symbl != NULL)
	{
		delete symbl;
		m_SymbolMap.erase(symbol);
	}
}

/*
* 删除符号表中指定frame变量
* @param  string 待删除的变量
* return void
*/
///2015-03-17 add by wangmeng
bool SymbolTable::DelFrm(string symbol)
{
	Symbol* symbl = FindSymbol(symbol);

	if (symbl != NULL)
	{
		int i = symbl->GetNFrm();
		if (i == 0)
		{
			delete symbl;
			m_SymbolMap.erase(symbol);
		}
		else if (i>0)
		{
			symbl->SetNFrm(i - 1);
		}
		else
		{
			fout << "Delete frame symbol error!" << endl;
			return false;
		}
		return true;
	}
	{
		fout << "Symbol " << symbol.data() << " does not exist!" << endl;
		return false;
	}

}


/*
* 拷贝符号表
* @param 待拷贝的符号表
* return void
*/
///2015-03-20 add by wangmeng
void SymbolTable::CopyTable(SymbolTable* pTable)
{
	if (NULL == pTable || NULL == this)
	{
		fout << " CopySt error!" << endl;
		return;
	}
	map<string, Symbol*>::iterator iter_1;
	for (iter_1 = m_SymbolMap.begin(); iter_1 != m_SymbolMap.end(); iter_1++)
	{
		if ((*iter_1).second != NULL)
		{
			delete (*iter_1).second;
			(*iter_1).second = NULL;
		}
	}
	m_SymbolMap.clear();//注意这里一定要clear
	map<string, Symbol*>::iterator iter;
	map<string, Symbol*> tempSt = pTable->GetSymbolMap();
	for (iter = tempSt.begin(); iter != tempSt.end(); iter++)
	{
		string sbName = (*iter).first;
		Symbol* newSb = new Symbol();
		newSb->Copy((*iter).second);
		m_SymbolMap.insert(pair<string, Symbol*>(sbName, newSb));
		newSb = NULL;
	}
}


