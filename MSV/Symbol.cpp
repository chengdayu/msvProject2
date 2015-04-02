/*
* Symbol.cpp
*
*  Created on: 2015-3-17
*      Author: wangmeng
*/
#include"function.h"
#include"Symbol.h"
#include<iostream>
using namespace std;

///构造函数
///2015-03-20 add by wangmeng
Symbol::Symbol() :m_ArrayNum(0), m_nFrm(0), m_SymbolType(UNDEFINE){ }
/*
*构造函数
*@param 符号类型
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type) :m_ArrayNum(0), m_nFrm(0)
{
	m_SymbolType = type;
}

/*
* 构造函数
*@param 符号类型 数组长度
*/
///2015-03-12 add by shiyifang
Symbol::Symbol(SymbolType type, int num) :m_nFrm(0)
{
	m_SymbolType = type;
	m_ArrayNum = num;

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
	m_ArrayNum = pSymbl->GetArrayNum();///数组长度
	m_nFrm = pSymbl->GetNFrm(); ///变量frame的次数
}
