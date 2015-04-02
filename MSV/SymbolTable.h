/*
* SymbolTable.h
*
*  Created on: 2015-3-7
*      Author: yubin
*/

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include"Symbol.h"
#include<map>
#include <set>
#include "SyntaxNode.h"
#include<string>

class SymbolTable
{
public:

	/*
	* 构造函数
	*/
	///2015-3-12 add by shiyifang
	SymbolTable();


	/*
	*是否包含符号
	*@param string(也就是key)
	*return bool
	*/
	///2015-03-11 add by shiyifang
	bool HasSymbol(string symbol);

	/*
	* 检查此类型是否为指针
	*@param 符号类型
	*return bool
	*/
	///2015-3-12 add by shiyifang
	static bool IsPointer(SymbolType type);

	/*
	* 判断节点声明的符号类型
	* @param RETURNTYPE
	* return SymbolType
	*/
	///2015-03-11 add by shiyifang
	static	SymbolType GetSymbolType(RETURNTYPE type);

	/*
	* 向符号表中插入数据
	* @param string (结点名) SymbolType(符号类型)  int(数组长度)
	* return bool
	*/
	///2015-03-13 add by shiyifang
	bool InstSymbol(string NName, SymbolType type, int array_num = 0);

	/*
	* 向符号表中插入frame变量
	* @param string (结点名) SymbolType(符号类型)
	* return bool
	*/
	///2015-03-17 add by wangmeng
	bool InstFSymbl(string NName);


	/*
	* 查找符号
	* @param string
	* return Symbol*
	*/
	///2015-03-12 add by shiyifang
	Symbol* FindSymbol(string symbol);

	/*
	* 拷贝符号表
	* @param 待拷贝的符号表
	* return void
	*/
	///2015-03-20 add by wangmeng
	void CopyTable(SymbolTable* pTable);

	/*
	* 删除符号表中所有元素
	*/
	///2015-03-13 add by shiyifang
	void Clear();

	/*
	* 删除符号表中指定符号
	* @param  string
	*/
	///2015-03-13 add by shiyifang
	void Delete(string symbol);

	/*
	* 删除符号表中指定frame变量
	* @param  string 待删除的变量
	* return 是否成功删除
	*/
	///2015-03-17 add by wangmeng
	bool DelFrm(string symbol);


	inline map<string, Symbol*> GetSymbolMap()
	{
		return m_SymbolMap;
	}


private:
	map<string, Symbol*> m_SymbolMap;

};


#endif /* SYMBOLTABLE_H_ */
