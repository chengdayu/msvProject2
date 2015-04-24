/*
* Symbol.cpp
*
*  Created on: 2015-3-17
*      Author: wangmeng
*/
#include"function.h"
#include"Symbol.h"
#include<iostream>
#include <sstream>

using namespace std;

///构造函数
///2015-03-20 add by wangmeng
Symbol::Symbol() :m_isArray(false), m_nFrm(0), m_SymbolType(UNDEFINE), m_name(""), m_ArrayNum(0),m_AddrNum(0){}
/*
*构造函数
*@param 符号类型
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type) :m_isArray(false), m_nFrm(0), m_name(""), m_ArrayNum(0), m_AddrNum(0)
{
	m_SymbolType = type;
}

/*
* 构造函数
*@param 符号类型 数组长度
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type, bool isArray) :m_nFrm(0), m_name(""), m_ArrayNum(0), m_AddrNum(0)
{
	m_SymbolType = type;
	m_isArray = isArray;

}



/*
* 构造函数
*@param 符号类型 数组长度
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type, string name) :m_nFrm(0), m_isArray(false), m_ArrayNum(0), m_AddrNum(0)
{
	m_SymbolType = type;
	m_name = name;

}

/*
* 构造函数
*@param 符号类型 数组长度
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type, int AddrNum, string name) :m_nFrm(0), m_isArray(false), m_ArrayNum(0)
{
	m_SymbolType = type;
	m_name = name;
	m_AddrNum = AddrNum;
}


/*
* 构造函数
*@param 符号类型
*@param 结构体类型名
*@param 被frame次数
*/
///2015-4-9 add by shiyifang
Symbol::Symbol(SymbolType type, string name, int nFrm) :m_isArray(false), m_ArrayNum(0), m_AddrNum(0)
{
	m_SymbolType = type;
	m_name = name;
	m_nFrm = nFrm;
}


/*
* 构造函数
*@param 符号类型 数组长度
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type, bool isArray, string name) :m_nFrm(0), m_ArrayNum(0), m_AddrNum(0)
{
	m_SymbolType = type;
	m_isArray = isArray;
	m_name = name;
}


/*
* 构造函数
*@param 
*/
///2015-03-12 add by wangmeng
Symbol::Symbol(SymbolType type, bool isArray, string name, int ArrayNum) :m_nFrm(0), m_AddrNum(0)
{
	m_SymbolType = type;
	m_isArray = isArray;
	m_name = name;
	m_ArrayNum = ArrayNum;
}

/*
* 构造函数
*@param
*/
///2015-03-12 add by wangmeng
Symbol::Symbol(SymbolType type, bool isArray, string name, int ArrayNum,int AddrNum) :m_nFrm(0)
{
	m_SymbolType = type;
	m_isArray = isArray;
	m_name = name;
	m_ArrayNum = ArrayNum;
	m_AddrNum = AddrNum;
}


/*
* 拷贝符号函数
*@param 待拷贝的符号
*return void
*/
///考虑数据区域的copy
///2015-3-20 add by wangmeng
void Symbol::Copy(Symbol *pSymbl)//复制变量
{
	if (this == NULL)
	{
		fout << "Symbol::Copy error!" << endl;
		return;
	}
	if (pSymbl == NULL)
	{
		return;
	}
	m_SymbolType = pSymbl->GetSymbolType();///type of symbol
	m_isArray = pSymbl->IsArray();///数组长度
	m_nFrm = pSymbl->GetNFrm(); ///变量frame的次数
	m_ArrayNum = pSymbl->GetArrayNum();
	m_name = pSymbl->GetSName();
}


/*
* 输出符号转换成字符串
*@param 根据不同类型对应的printf里边的类型，如int对应"%d"
*return void
*/
string Symbol::__PrntFixSym(string flag,string name)
{
	string result = "printf(";
	if (!m_isArray)
	{
		result += "\"" + name + "=" + flag + " \","+ name;
	}
	else
	{
		if (m_ArrayNum==0)
		{
			return "";
		}
		result += "\"" + name + "[";

		stringstream ss;
		ss << m_ArrayNum;
		result += ss.str() + "]";
		ss.clear();

		result += "= [";

		string tmp = "";
		for (int i = 0; i < m_ArrayNum; i++)
		{
			result += flag+ ",";
			tmp += "," + name + "[";

			stringstream ss;
			ss << i;
			tmp += ss.str();
			ss.clear();
			tmp += "]";
		}
		result = result.substr(0, (result.length() - 1));
		result += "]\"";
		result += tmp;
	}

	result += ");\n";
	return result;
}


/*
* 输出符号转换成字符串
*@param 变量名
*return void
*/
string Symbol::PrntSym(string name)
{
	switch (m_SymbolType)
	{
	case UNDEFINE:{return ""; }
	case SYMBOLINT:{ return __PrntFixSym("%d", name); }
	case SYMBOLFLOAT:{return __PrntFixSym("%f", name); }
	case SYMBOLINTP:
	case SYMBOLFLOATP:{return __PrntFixSym("%p", name); }
	default: {return ""; }
	}
}


///根据节点类型得到返回值类型
///2015-4-24 add by wangmeng
RETURNTYPE Symbol::GetRType(bool &isUnsign)
{
	switch (m_SymbolType)
	{
	case SYMBOLINT:     	   { return INTTYPE; }     //return integer type
	case SYMBOLSTR:     		{ return STRTYPE; }   	//return string type
	case SYMBOLFILE:	 		{ return FILETYPE; }		//add by yubin, return file type

	case SYMBOLFLOAT:   		{ return FLOATTYPE; }     //2013-4-26 add by YY[fixed]
	case SYMBOLCHAR:    		{ return CHARTYPE; }     //2013-4-26 add by YY[fixed]
							//case ARITHMETICTYPE:  	{ return ;} 	//2013-4-26 add by YY[fixed]

	case SYMBOLLIST:		 	{ return LISTTYPE; }		// return list value
	case SYMBOLBOOL:        	{ return BOOLTYPE; }		//return bool type
							//case POINTERTYPE:    	{ return SYMBOLPOINTER;} 	//return pointer value
							//case SYMBOLTYPE:		{ return ;}		// return symbol value
							//case CHANNELRTYPE:		{ return ;}		//add by mdp 2010-07-09


	case SYMBOLINTP:         	{ return INTTYPE; }		//return integer pointer type
	case SYMBOLUINT:		   	{isUnsign = true; return UINTTYPE; }
	case SYMBOLFLOATP:		{ return FLOATTYPE; }
	case SYMBOLUCHARP:		{isUnsign = true; return CHARTYPE; }

	case SYMBOLSTRUCT:		{ return STRUCTTYPE; }
	case SYMBOLSTRUCTP:		{ return STRUCTPTYPE; }

							//case DOUBLEINTPTYPE:	{ return ;}
							//case DOUBLEUINTPTYPE:	{ return ;}
							//case DOUBLEFLOATPTYPE:	{ return ;}
							//case DOUBLEUCHARPTYPE:	{ return ;}
							//case DOUBLESTRUCTPTYPE:	{ return ;}
	case SYMBOLVOID:			{ return VOIDTYPE; }
	case SYMBOLVOIDP:			{ return VOIDPTYPE; }
							//add by yubin 2013-12-23 15:31 函数指针
							//case FUNCPTYPE:			{ return SYMBOLFUNCP;}

							//add by YY 2014/01/07 10:57 强制转换
	case SYMBOLCHARP:			{ return CHARTYPE; }
							//case DOUBLECHARPTYPE:	{ return ;}
	case SYMBOLUINTP:			{ isUnsign = true; return INTTYPE; }
	case SYMBOLUCHAR:			{ isUnsign = true; return CHARTYPE; }
							//case STRUCT_NAME_TYPE:	{ return ;}
							//case FUNCPPTYPE:		{ return ;}
	default:   			return  UNKNOWNTYPE;
	}
}


/*
* 将符号转成变量定义语法树
*@param 变量名
*return 生成的语法树
*/
CSyntaxNode* Symbol::GetDecTree(string name)
{
	bool IsUnsigned = false;
	RETURNTYPE RType = GetRType(IsUnsigned);
	if (RType == UNKNOWNTYPE || name == "")
	{
		return NULL;
	}
		
	CSyntaxNode* DecTree = new CSyntaxNode(DECLARE_STA, new CSyntaxNode(PARAMETER_EXP, VOIDTYPE), RType);
	CSyntaxNode* visit = DecTree->GetChild0();
	if (IsUnsigned)
	{	
		CSyntaxNode*tmp = new CSyntaxNode(UNSIGN_DECLARATION_STA, DecTree,VOIDTYPE);
		DecTree = tmp;
	}

	for (int i = 0; i < m_AddrNum; i++)
	{
		visit->SetChild0(new CSyntaxNode(ADDRESS_DECLARE_STA, VOIDTYPE));
		visit = visit->GetChild0();
	}
	visit->SetChild0(new CSyntaxNode(IDENT_EXP, name, NULL, VOIDTYPE));
	return DecTree;	
	
}
