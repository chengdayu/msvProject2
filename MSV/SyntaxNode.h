#ifndef SYNTAXNODE_H_INCLUDED
#define SYNTAXNODE_H_INCLUDED

#include <string>
//#include"List.h"


/// VALUE结构在类外面定义
struct VALUE
{
	int iValue;
	float fValue;
	char cValue;
	std::string sValue;
};


enum NODETYPE
{

	NEXT_STA,   ///必须有下一个状态,执行
	PREVIOUS_STA,///
	W_NEXT_STA,///可以有下一个状态,也可以没有.如果有,就执行
	MORE_STA,  ///还有更多程序要执行的标志

	CHOP_STA,///
	SKIP_STA,///
	ALWAYS_STA,///一直执行
	SOMETIMES_STA,///
	LENGTH_STA,///

	IF_STA,///
	IF_ELSE_STA,///
	FOR_TIMES_STA,///
	FOR_STA,
	FOR_STA_CON,
	SWITCH_STA,
	WHILE_DO_STA,///
	WHEN_DO_STA,///
	REPEAT_UNTIL_STA,///

	DECLARE_STA,///
	FRAME_STA,///
		PARALLEL_STA,///并行语句
		PROJECTION_STA,///投影语句

		FINAL_STA,///结束语句
		AUX_IDENT_EXP,/// fin标记
		HALT_STA,///暂停语句,即当满足条件是终止执行
		KEEP_STA,///除了终态,一直执行

		AWAIT_STA,///等待语句
		IMPLY_STA,///蕴涵
		IFF_STA,///	当且仅当

		DISPLAY_STA,///
		REQUEST_STA,////

		TYPE_CAST_STA,/// node type for type casting 强制类型转换 2012-06-21 Jane
		DOUBLE_TYPE_CAST_STA,///针对二重指针强制类型转换
		FUNCTION_STA,///函数调用
		FUNCTION_DEFINE_STA,///无返回值的函数定义
		RETURN_FUNCTION_DEFINE_ST,///有返回值的函数定义
		STRUCT_DEFINE_STA,///结构体定义语句
		STRUCT_DEC,///结构体类型变量的声明
		STRUCT_IDENT_EXP,

		STRUCTP_IDENT_EXP,///add by wm 2013/11/05  10:23
		PREFIX_STA,
		EXIST_STA,///局部变量声明
		ELE_GUARD_STA,///???????
		GUARD_STA,///??????
		OPTION_GUARD_STA,///?????
		RETURN_STA,


		IDENT_EXP,///
		STR_EXP,///
		INTEGER_EXP,///
		ARRAY_EXP,///
		FLOATLITERAL_EXP,
		CHARLITERAL_EXP,
		ADDRESS_EXP,///
		ADDR_VAR,///地址变量，add by yubin 2013-12-19
		ADDR_VAR_LIST,///地址变量偏移，add by yubin 2014-6-5
		VALUE_EXP,///
		VALUE_LIST_EXP,///
		SYMBOL_EXP,///

		PARAMETER_EXP,///
		ADDRESS_PARAMETER_EXP,
		ARRAY_PARAMETER_EXP,
		TRUE_EXP,///
		FALSE_EXP,///
		IF_ELSE_EXP,///

		ADD_EXP,///
		SUB_EXP,///
		MUL_EXP,///
		DIV_EXP,///
		MOD_EXP,///
		LST_EXP,///左移 left shift简称 add by yubin 2013-12-19
		RST_EXP,///右移 rigth shift简称 add by yubin 2013-12-19
		BAN_EXP,///按位与 bit and简称 add by yubin 2013-12-19
		BOR_EXP,///按位或 bit or简称 add by yubin 2013-12-19
		XOR_EXP,///异或 add by yubin 2013-12-19
		BNE_EXP,///按位非 add by yubin 2013-12-20
		CON_EXP,///
		MINUS_EXP,///
		ARITHMETIC_EXP,///算术表达式节点 Jane

		LT_EXP,///
		GT_EXP,///
		LE_EXP,///
		GE_EXP,///
		NE_EXP,///
		NEGATION_EXP,///

		AND_EXP,///
		OR_EXP,///

		EMPTY_EXP,///

		EQU_EXP,   ///=
		ASS_EQU_EXP,///  <==

		EX_ASS_EXP,///由:=得到的<==其节点类型

		UNITASSIGN_EXP,///  :=
		//	 FASS_EQU_EXP,  //删除不必要的赋值符号 2013-4-19 add by YY
		//  TASSIGN_EXP,//
		//	 FTASSIGN_EXP,//
		//	 NEXTASSIGN_EXP,//
		//	 FNEXTASSIGN_EXP,//
		//	 FUNITASSIGN_EXP,//

		STRFUNCHEAD_EXP,/// node type for string function head 2012-06-21 Jane
		STRFUNCTAIL_EXP,/// node type for string function tail 2012-06-21 Jane
		STRFUNCCAT_EXP,/// node type for string function strcat 2012-06-21 Jane
		STRFUNCCMP_EXP,/// node type for string function strcmp 2012-06-21 Jane
		STRFUNCCPY_EXP,/// node type for string function strcpy 2014-02-24 yangkai
		STRFUNCLEN_EXP,

		//ARRARFUNCSIZE_EXP,//node type for array function sizeof 2013-11-06 yangkai
		SIZEOF_EXP,
		///add by YY 2013/06/06 20:16
		CEIL_EXP,
		FLOOR_EXP,
		ROUND_EXP,

		///add by YY 2013/11/22 15:37
		SIN_EXP,
		COS_EXP,
		TAN_EXP,
		ASIN_EXP,
		ACOS_EXP,
		ATAN_EXP,
		SINH_EXP,
		COSH_EXP,
		TANH_EXP,
		EXP_EXP,
		LOG_EXP,
		LOG10_EXP,
		SQRT_EXP,
		ATAN2_EXP,
		POW_EXP,
		ABS_EXP,
		FABS_EXP,
		LABS_EXP,
		FMOD_EXP,
		MODF_EXP,
		LDEXP_EXP,

		///add by yubin 2014/3/7,文件类型
		FOPEN_EXP,
		FCLOSE_EXP,
		FGETS_EXP,
		FPUTS_EXP,
		FGETC_EXP,
		FPUTC_EXP,

		LIST_DECLARE_STA,/// node type for list declaration list型变量的声明
		LIST_SYMBOL_STA,/// node type for symbol of array or list 数组元素
		LIST_SUBSCRIPT_EXP,/// node type for LIST SUBSCRIPT数组下标
		LIST_ASSVALUE_STA,/// node type for array assign 形如[1,2,3]
		LOCAL_ARRAY_VALUE,///add by Jane for local list_value
		ARRAY_DECLARE_STA,/// node type for array declaration 数组变量的声明
		DOUBLEARRAY_PARAMETER_EXP, ///add by YY 2014/01/14 17:56 二维数组函数形参节点

		ADDRESS_DECLARE_STA,///add by wm 2013/10/28  16:09

		OPEN_PAR_EXP,///
		CLOSE_PAR_EXP,///
		CHOPSTAR_STA,///新定义的CHOP_STAR
		FIN_EXP,

		/*	 OVER_STA,//2010-07-07 add
		REP_STA,//2010-07-07 add
		//annotator: Jane
		*/

		//Jane实现的东东~  2013-4-27 add by YY
		//2011-08-10 Jane begin
		OVER_STA,
		INTERVAL_CAT,
		INTERVAL_OR,
		INTERVAL_REP,
		INTERVAL_REP_NDET,
		/* node type for cylinder computing
		2011-08-10 Jane*/

		S_AWAIT,///add by mdp
		S_UNIT_ASSIGN,///add by mdp
		S_ASSIGN,///add by mdp
		R_AWAIT,///add by mdp
		R_UNIT_ASSIGN,///add by mdp
		R_ASSIGN,///add by mdp
		S_IF_ELSE,
		R_IF_ELSE,



		///add by YY 2013/08/09 20:15  仿真empty节点的下一节点
		EMPTYNULL_EXP,
		///add by YY 2013/08/10 9:43 建模化简失败节点
		FALSE_REDU_EXP,


		FRAME_FREE_STA,
		///add by YY 2013/08/22 10:50
		UNFRAME_STA,

		NULL_EXP, ///add by wm 2013/10/29  10:36

		FUNCTION_END,
		STRUCT_PARAMETER_EXP,
		STRUCTP_PARAMETER_EXP,
		DOUBLESTRUCTP_PARAMETER_EXP,
		POINT_PARAMETER_EXP,
		STRUCT_LIST_EXP,

		ARRAY_DECLARE_NULL_STA,///int a[]<==[1,2,3]记录a


		STRUCT_LIST_ASSVALUE_STA,///add by YY 2013/12/25 10:05 结构体初始化列表{{1,"he"},{2,"hello"}}
		UNSIGN_DECLARATION_STA,///add by YY 2013/12/31 14:45 unsigned或者signed定义语句

		///add by yubin 2013-12-27 15:31 参数类型是函数指针
		FUNCP_PARAMETER_EXP,
		DOUBLEPOINT_PARAMETER_EXP,

		///add by YY 2014/01/17 11:07
		ESCAPECHARNULL_EXP,   ///转义字符 '\0'
		///add by YY 2014/01/17 15:51
		STRUCT_ADDRESS_EXP,
		STRUCT_VALUE_EXP,
		STRUCTP_LIST_EXP,

		MALLOC_STA,///yangkai malloc相关
		FREE_STA,

		///add by YY 2014/02/19 10:47
		FRAME_END_STA, ///标志节点，用来表示frame块程序执行结束，类似于empty
		STRUCT_NAME,
		SYSTEM_STA,
		DLL_CALL_STA,
		NEG_EXPONENT_EXP,
		POS_EXPONENT_EXP,
		STRUCT_FUNC_EXP,
		STRUCTP_FUNC_EXP,

		GOTO_STA,//yangkai 处理while相关
		LABEL_STA//yangkai 处理while相关
};
enum RETURNTYPE
{
	VOIDTYPE,        ///return no type
	UNKNOWNTYPE,     ///return unknown type
	INTTYPE,         ///return integer type
	STRTYPE,         ///return string type
	FILETYPE,		///add by yubin, return file type

	FLOATTYPE,       ///2013-4-26 add by YY[fixed]
	CHARTYPE,        ///2013-4-26 add by YY[fixed]
	ARITHMETICTYPE,  ///2013-4-26 add by YY[fixed]

	LISTTYPE,		 /// return list value
	BOOLTYPE,        ///return bool type
	POINTERTYPE,     ///return pointer value
	SYMBOLTYPE,		 /// return symbol value
	CHANNELRTYPE,		///add by mdp 2010-07-09


	///add by wm 2013/10/25  22:17
	INTPTYPE,         ///return integer pointer type
	UINTPTYPE,
	FLOATPTYPE,
	UCHARPTYPE,
	STRUCTTYPE,
	STRUCTPTYPE,

	DOUBLEINTPTYPE,
	DOUBLEUINTPTYPE,
	DOUBLEFLOATPTYPE,
	DOUBLEUCHARPTYPE,
	DOUBLESTRUCTPTYPE,
	VOIDPTYPE,
	///add by yubin 2013-12-23 15:31 函数指针
	FUNCPTYPE,

	///add by YY 2014/01/07 10:57 强制转换
	CHARPTYPE,
	DOUBLECHARPTYPE,
	UINTTYPE,
	UCHARTYPE,
	STRUCT_NAME_TYPE,
	FUNCPPTYPE
};


/**
*  程序结点类，用来存储结点数据
*/
class CSyntaxNode
{
public:
	///无参构造函数
	CSyntaxNode();
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的child0结点
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, CSyntaxNode * p0, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的child0结点
	* @param3 结点的child1结点
	* @param4 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, CSyntaxNode * p0, CSyntaxNode * p1, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的child0结点
	* @param3 结点的child1结点
	* @param4 结点的child2结点
	* @param5 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype);

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点整形值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, int piValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的浮点值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, float pfValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的字符值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, char pcValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点的字符串值
	* @param3 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &psValue, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结构体名
	* @param3 变量名
	* @param4 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &pstructname, const std::string pname, RETURNTYPE preturntype);

	/**
	* 构造函数
	* @param1 结点类型
	* @param2 变量名
	* @param3 结点的child0结点
	* @param4 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &pname, CSyntaxNode * p0, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 变量名
	* @param3 结点的child0结点
	* @param4 结点的child1结点
	* @param5 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pndoetype, const std::string &pname, CSyntaxNode * p0, CSyntaxNode * p1, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 变量名
	* @param3 结点的child0结点
	* @param4 结点的child1结点
	* @param5 结点的child2结点
	* @param6 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, const std::string &pname, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype);
	/**
	* 构造函数
	* @param1 结点类型
	* @param2 结点整型值
	* @param3 结点的child0结点
	* @param4 结点的child1结点
	* @param5 结点的child2结点
	* @param6 返回值类型
	* @return
	*/
	CSyntaxNode(NODETYPE pnodetype, int piValue, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype);//add by yubin 2014-6-5


	/**
	* 判断函数是否重名
	* @param 要查找的函数名
	* @return bool
	*/
	bool FindSameName(std::string pname);

	/**
	* 在一棵树中查找另一棵树是否存在
	* @param 查找树 目标树 节点类型
	* @return bool
	*/
	bool FindInTree(CSyntaxNode * ptree, CSyntaxNode* target, NODETYPE ntype);

	/**
	* 拷贝结点
	* @param 待拷贝的结点
	* @return void
	*/
	void CopyNode(CSyntaxNode* pnode);

	/**
	* 拷贝树
	* @param 待拷贝的树的根结点
	* @return void
	*/
	void CopyTree(CSyntaxNode* pnode);

	/**
	* 比较结点是否相同
	* @param 待比较的结点
	* @return bool
	*/
	bool CompareNode(CSyntaxNode* pnode);

	/**
	* 比较树是否相同
	* @param 待比较的结点
	* @return bool
	*/
	bool CompareTree(CSyntaxNode* pnode);


	/**
	* 将树转化为string形式以供输出
	* @param 待比较的结点
	* @return bool
	*/
	string TreeToString();

	///得到变量名
	inline std::string GetNName()
	{
		return m_NodeName;
	}

	///设置变量名
	inline void  SetNName(const std::string NName)
	{
		m_NodeName = NName;
	}
	///得到结点类型
	inline NODETYPE GetNType()
	{
		return m_NodeType;
	}
	///设置结点类型
	inline void SetNType(NODETYPE NType)
	{
		m_NodeType = NType;
	}

	///得到返回类型
	inline RETURNTYPE GetRType()
	{
		return m_ReturnType;
	}

	///设置返回类型
	inline void SetRType(RETURNTYPE RType)
	{
		m_ReturnType = RType;
	}

	/// 得到孩子结点0
	inline CSyntaxNode* GetChild0()
	{
		return m_child[0];
	}
	///设置孩子结点0
	inline void SetChild0(CSyntaxNode * Child)
	{
		m_child[0] = Child;
	}

	/// 得到孩子结点1
	inline CSyntaxNode* GetChild1()
	{
		return m_child[1];
	}
	///设置孩子结点1
	inline void SetChild1(CSyntaxNode * Child)
	{
		m_child[1] = Child;
	}

	/// 得到孩子结点2
	inline CSyntaxNode* GetChild2()
	{
		return m_child[2];
	}
	//设置孩子结点2
	inline void SetChild2(CSyntaxNode * Child)
	{
		m_child[2] = Child;
	}
	///设置整型值
	inline void SetiValue(int ivalue)
	{
		m_Value.iValue = ivalue;
	}

	///得到整形值
	inline int GetiValue()
	{
		return m_Value.iValue;
	}

	///设置浮点型值
	inline void SetfValue(float fvalue)
	{
		m_Value.fValue = fvalue;
	}
	///得到浮点型值
	inline float GetfValue()
	{
		return m_Value.fValue;
	}

	///设置字符串值
	inline void SetsValue(std::string svalue)
	{
		m_Value.sValue = svalue;
	}
	///得到字符串值
	inline std::string GetsValue()
	{
		return m_Value.sValue;
	}

	///设置字符值
	inline void SetcValue(char cvalue)
	{
		m_Value.cValue = cvalue;
	}
	///得到字符值
	inline char GetcValue()
	{
		return m_Value.cValue;
	}

	///得到PreLen的标志位
	inline bool GetPreLen()
	{
		return m_PreLen;
	}
	///设置PreLen的标志位
	inline void SetPreLen(bool PreLen)
	{
		m_PreLen = PreLen;
	}

	///析构函数
	virtual ~CSyntaxNode();

private:
	///值
	VALUE m_Value;
	///变量名
	std::string  m_NodeName;
	///结点类型
	NODETYPE  m_NodeType;
	///返回类型
	RETURNTYPE m_ReturnType;
	///孩子结点
	CSyntaxNode * m_child[3];
	///是否对Len进行Pre的标志位
	bool m_PreLen;

};
typedef CSyntaxNode* SyntaxTree;



#endif // SYNTAXNODE_H_INCLUDED
