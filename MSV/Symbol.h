/*
* Symbol.h
*
*  Created on: 2015-3-7
*      Author: yubin
*/

#ifndef SYMBOL_H_
#define SYMBOL_H_

/**
* 枚举类型从零开始,
* 把类型后紧跟它的指针类型，如：SYMBOLINT后面写SYMBOLINTP,使得type+1就可获得他的指针类型
* 有指针的类型放在前面书写，便于用范围判断,SYBOLINT< type < SYMBOLUCHARP，
* SYBOLINT< type < SYMBOLUCHARP，且type为偶数就代表了指针类型
* 尽量按此规则书写   			shiyifang
*/
enum SymbolType
{
	IDENTIFIER,    /// Identifier or variable name

	SYMBOLINT,     /// Integer value
	SYMBOLINTP,
	SYMBOLFLOAT,   ////
	SYMBOLFLOATP,

	SYMBOLCHAR,    ///
	SYMBOLCHARP,

	SYMBOLVOID,
	SYMBOLVOIDP,



	SYMBOLSTRUCT,
	SYMBOLSTRUCTP,
	SYMBOLLIST,    /// List
	SYMBOLLISTP,    ///指向数组的指针

	///unsigned类型
	SYMBOLUINT,
	SYMBOLUINTP,
	SYMBOLUCHAR,
	SYMBOLUCHARP,
	//	 SYMBOLUFLOAT,

	//	 SYMBOLUFLOATP,

	SYMBOLNULL,
	SYMBOLFILE,
	SYMBOLSTR,     /// String constant
	SYMBOLBOOL,
	///未定义
	UNDEFINE,

	NOSUCHSYMBOL,  //符号表中没有这个符号，用于查找符号表时的返回

	SYMBOLPOINTER,  /// Pointer
	SYMBOLEMPTY,   ///用于指针数组，未初始化时的默认指针类型

	STRUCTELEMLISTP,///指向结构体成员数组的指针
	STRUCTELEMP,     ///指向结构体普通成员(非数组)的指针

	SYMBOLLISTVALUE,///用来表示数组列表[1,2,3,4]的valuereduction的返回符号类型
	SYMBOLSTRUCTLISTVALUE,///用来表示结构体初始化列表{{2,"hello"},{4,"kak"}的valuereduction的返回符号类型
	SYMBOLFUNCP,    ///指向函数的函数指针

	///用于指示generatearray函数创建的类型为指针数组元素
	SYMBOLPNT
};


class Symbol
{
public:

	///构造函数
	///add by wangmeng
	Symbol();
	/*
	*构造函数
	*@param 符号类型
	*/
	///2015-03-12 add by shiyifang
	Symbol(SymbolType type);

	/*
	* 构造函数
	*@param 符号类型
	*@param 数组长度
	*/
	///2015-03-12 add by shiyifang
	Symbol(SymbolType type, int num);

	/*
	* 拷贝符号函数
	*@param 待拷贝的符号
	*return void
	*/
	void Copy(Symbol *psb);


	/*
	* 获得符号类型
	* return SymbolType
	*/
	///2015-03-11 add by shiyifang
	inline SymbolType GetSymbolType()
	{
		return m_SymbolType;
	}


	/*
	* 获得数组长度
	* return int
	*/
	///2015-03-12 add by shiyifang
	inline int GetArrayNum()
	{
		return m_ArrayNum;
	}


	/*
	* 设置符号类型
	* @param SymbolType
	*/
	///2015-03-13 add by shiyifang
	inline void SetSymbolType(SymbolType type)
	{
		m_SymbolType = type;
	}


	/*
	* 设置数组长度
	* @param int
	*/
	///2015-03-13 add by shiyifang
	inline void SetArrayNum(int num)
	{
		m_ArrayNum = num;
	}

	/*
	* 设置变量frame的次数
	* @param int
	* return void
	*/
	///2015-03-17 add by wangmeng
	inline void SetNFrm(int num)
	{
		m_nFrm = num;
	}

	///得到变量frame的次数
	inline int GetNFrm()
	{
		return m_nFrm;
	}

private:
	SymbolType   m_SymbolType;///type of symbol
	int			 m_ArrayNum;///数组长度
	int          m_nFrm; ///变量frame的次数
};


#endif /* SYMBOL_H_ */
