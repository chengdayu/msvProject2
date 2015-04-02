/*******************************************************************************
*@file     SyntaxNode.cpp
*@author
*@remarks  Modified by YY
*@version  1.0
*@section  Brief
*Modified: 2013/04/22  16:21
*
*
*
*Copyright (C) ICTT.,XiDian univ
*All rights reserved.
*
*******************************************************************************/

#include <stdlib.h>
#include "function.h"
#include "SyntaxNode.h"
#include <sstream>


using namespace std;
//----------------------------------------------------------------------
CSyntaxNode::CSyntaxNode()
{
	m_NodeType = TRUE_EXP;
	m_ReturnType = VOIDTYPE;
	m_child[0] = NULL;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";
}

//----------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = NULL;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}

//-----------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, CSyntaxNode *p0, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}

//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, CSyntaxNode *p0, CSyntaxNode *p1, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}

//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, CSyntaxNode *p0, CSyntaxNode *p1, CSyntaxNode *p2, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = p2;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}

//------------------------------------------------------------------------------------------------------------------------
//2013-4-28 add by YY[fixed]
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, int piValue, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = NULL;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = piValue;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}
//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, float pfValue, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = NULL;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = pfValue;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}
//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, char pcValue, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = NULL;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = "";
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = pcValue;
	m_Value.sValue = "";

}
//attention------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &psValue, RETURNTYPE preturntype)
{
	m_NodeName = "";

	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = NULL;
	m_child[1] = NULL;
	m_child[2] = NULL;

	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = psValue;
	if (preturntype == STRUCT_NAME_TYPE)
	{
		m_NodeName = psValue;
		m_Value.sValue = "";
	}
}


//-----------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pname, CSyntaxNode * p0, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = NULL;
	m_child[2] = NULL;
	m_NodeName = pname;
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pname, CSyntaxNode * p0, CSyntaxNode * p1, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = NULL;
	m_NodeName = pname;
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";

}
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pname, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = p2;
	m_NodeName = pname;
	m_Value.iValue = 0;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";


}

//add by yubin 2014-6-5
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, int piValue, CSyntaxNode * p0, CSyntaxNode * p1, CSyntaxNode * p2, RETURNTYPE preturntype)
{
	m_NodeType = pnodetype;
	m_ReturnType = preturntype;
	m_child[0] = p0;
	m_child[1] = p1;
	m_child[2] = p2;
	m_NodeName = "";
	m_Value.iValue = piValue;
	m_Value.fValue = 0.0;
	m_Value.cValue = 0;
	m_Value.sValue = "";
}



//------------------------------------------------------------------------------------------------------------------------

//性质节点用到的构造函数  2013-4-28 add by YY [fixed]
/*
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, string pstr, int piValue)
{
m_NodeType=pnodetype;
m_ReturnType=VOIDTYPE;
m_child[0]=NULL;
m_child[1]=NULL;
m_child[2]=NULL;
m_NodeName=pstr;
m_Value.iValue=piValue;
m_Value.fValue=0.0;
m_Value.cValue=0;
m_Value.sValue="";
}

//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, string pstr, int piValue, CSyntaxNode * s1)
{
m_NodeType=pnodetype;
m_ReturnType=VOIDTYPE;
m_child[0]=s1;
m_child[1]=NULL;
m_child[2]=NULL;
m_NodeName=pstr;
m_Value.iValue=piValue;
m_Value.fValue=0.0;
m_Value.cValue=0;
m_Value.sValue="";
}

//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, string pstr, int piValue, CSyntaxNode * p1, CSyntaxNode * p2)
{
m_NodeType=pnodetype;
m_ReturnType=VOIDTYPE;
m_child[0]=p1;
m_child[1]=p2;
m_child[2]=NULL;
m_NodeName=pstr;
m_Value.iValue=piValue;
m_Value.fValue=0.0;
m_Value.cValue=0;
m_Value.sValue="";
}

//------------------------------------------------------------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, string pstr, int piValue, CSyntaxNode * p1, CSyntaxNode * p2, CSyntaxNode * p3)
{
m_NodeType=pnodetype;
m_ReturnType=VOIDTYPE;
m_child[0]=p1;
m_child[1]=p2;
m_child[2]=p3;
m_NodeName=pstr;
m_Value.iValue=piValue;
m_Value.fValue=0.0;
m_Value.cValue=0;
m_Value.sValue="";
}
*/
//===========性质用到的构造函数  //add by YY[fixed] 2013/05/09 12:06   ==============






//------------------------------------------------------------------------------------------------------------------------
//  2013-4-28 add by YY[fixed]
//谓词、函数定义时用到：类型、名字，形参列表，函数体，返回值，返回值类型
///CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string &pstr, CSyntaxNode *p1, CSyntaxNode *p2, CSyntaxNode *p3, RETURNTYPE preturntype)
///{
//先不处理
///}



//------------------------------------------------------------------------------------------------------------------------
//是上面的变体，最后多了self的输入，并且设置rewrited为0，表示以后要重写
///CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, const string & pstr, CSyntaxNode *p1, CSyntaxNode *p2, CSyntaxNode *p3, RETURNTYPE preturntype, const string &pself)
///{
//先不处理
///}
//------------------------------------------------------------------------------------------------------------------------
void CSyntaxNode::CopyNode(CSyntaxNode* pnode)
{
	m_NodeType = pnode->m_NodeType;
	m_ReturnType = pnode->m_ReturnType;
	//	m_child[0]=pnode->m_child[0];
	//	m_child[1]=pnode->m_child[1];
	//	m_child[2]=pnode->m_child[2];
	m_NodeName = pnode->m_NodeName;
	m_Value.iValue = pnode->m_Value.iValue;
	m_Value.fValue = pnode->m_Value.fValue;
	m_Value.cValue = pnode->m_Value.cValue;
	m_Value.sValue = pnode->m_Value.sValue;

}


//------------------------------------------------------------------------------------------------------------------------
//2013-5-2 add by YY[fixed]
//注意copytree函数的参数不可以是调用该函数的对象
void CSyntaxNode::CopyTree(CSyntaxNode* pnode)
{
	if (pnode == NULL || this == NULL)
	{
		return;
	}
	else
	{
		if (pnode->m_child[0] != NULL)
		{
			delete m_child[0];
			m_child[0] = new CSyntaxNode();
			m_child[0]->CopyTree(pnode->m_child[0]);

		}
		else
		{
			delete m_child[0];
			m_child[0] = NULL;
		}

		if (pnode->m_child[1] != NULL)
		{
			delete m_child[1];
			m_child[1] = new CSyntaxNode();
			m_child[1]->CopyTree(pnode->m_child[1]);

		}
		else
		{
			delete m_child[1];
			m_child[1] = NULL;
		}

		if (pnode->m_child[2] != NULL)
		{
			delete m_child[2];
			m_child[2] = new CSyntaxNode();
			m_child[2]->CopyTree(pnode->m_child[2]);

		}
		else
		{
			delete m_child[2];
			m_child[2] = NULL;
		}
		CopyNode(pnode);
	}
}
//------------------------------------------------------------------------------------------------------------------------
bool CSyntaxNode::CompareNode(CSyntaxNode *pnode)
{
	if (this == NULL || pnode == NULL)    //需要特殊处理error吗？  2013-4-28 add by YY[fixed]
	{
		return false;
	}
	else
	{
		//除了m_child指针，各个节点变量都相等  2013-5-2 add by YY[fixed]
		if (m_NodeType == pnode->m_NodeType  &&
			m_ReturnType == pnode->m_ReturnType  &&
			m_NodeName == pnode->m_NodeName  &&
			m_Value.iValue == pnode->m_Value.iValue &&
			m_Value.cValue == pnode->m_Value.cValue  &&
			m_Value.fValue == pnode->m_Value.fValue &&
			m_Value.sValue == pnode->m_Value.sValue)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


}

//------------------------------------------------------------------------------------------------------------------------
//2013-5-2 add by YY[fixed]
/*
bool CSyntaxNode::CompareTree(CSyntaxNode* pnode)
{
if(this==NULL || pnode==NULL)
{
fout<<"CompareTree error!"<<endl;
exit(-1);
//return false;
}
if(!CompareNode(pnode))
{
return false;
}
else if(m_child[0]!=NULL)
{
if(!(m_child[0]->CompareTree(pnode->m_child[0])))
return false;
}
else if(m_child[1]!=NULL)
{
if(!(m_child[1]->CompareTree(pnode->m_child[1])))
return false;
}
else if(m_child[2]!=NULL)
{
if(!(m_child[2]->CompareTree(pnode->m_child[2])))
return false;
}
return true;
}
*/

//--------------------------------------------------------------------------------------
/*
add by YY[fixed] 2013/05/13 9:57
比较两棵树是否相同
包含p and q and r与r and p and q的比较，以及p or q or r 与q or r or p
*/
bool CSyntaxNode::CompareTree(CSyntaxNode* ptree)
{
	CSyntaxNode* temp = NULL;
	CSyntaxNode* temp1 = NULL;
	CSyntaxNode* temp2 = NULL;
	if (this == NULL || ptree == NULL)
	{
		return false;
	}
	if (!CompareNode(ptree))
	{
		return false;
	}

	//add by YY[fixed] 2013/05/21 9:53
	/*
	if(m_child[0]!=NULL && m_child[1]==NULL && m_child[2]==NULL)
	{
	if(!m_child[0]->CompareTree(ptree->m_child[0]))
	return false;
	}
	*/
	//p and q and r与r and p and q
	else if (m_NodeType == AND_EXP && ptree->m_NodeType == AND_EXP)
	{
		temp1 = new CSyntaxNode();
		temp2 = new CSyntaxNode();
		temp1->CopyTree(this);
		temp2->CopyTree(ptree);
		while (temp2->m_NodeType == AND_EXP && temp2->m_child[0] != NULL &&
			temp2->m_child[0]->m_NodeType == AND_EXP)
		{
			if (!(FindInTree(this, temp2->m_child[1], AND_EXP)))
			{
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp2->m_child[0]);
			temp2->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp2->m_NodeType == AND_EXP && temp2->m_child[0] != NULL &&
			temp2->m_child[0]->m_NodeType != AND_EXP)
		{
			if (!(FindInTree(this, temp2->m_child[0], AND_EXP)))
				return false;
			if (!(FindInTree(this, temp2->m_child[1], AND_EXP)))
				return false;
		}
		delete temp2;
		temp2 = NULL;

		while (temp1->m_NodeType == AND_EXP && temp1->m_child[0] != NULL &&
			temp1->m_child[0]->m_NodeType == AND_EXP)
		{
			if (!(FindInTree(ptree, temp1->m_child[1], AND_EXP)))
			{
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->m_child[0]);
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1->m_NodeType == AND_EXP && temp1->m_child[0] != NULL &&
			temp1->m_child[0]->m_NodeType != AND_EXP)
		{
			if (!(FindInTree(ptree, temp1->m_child[0], AND_EXP)))
				return false;
			if (!(FindInTree(ptree, temp1->m_child[1], AND_EXP)))
				return false;
		}
		delete temp1;
		temp1 = NULL;
	}

	//p or q or r 与q or r or p
	else if (m_NodeType == OR_EXP && ptree->m_NodeType == OR_EXP)
	{
		temp1 = new CSyntaxNode();
		temp2 = new CSyntaxNode();
		temp1->CopyTree(this);
		temp2->CopyTree(ptree);
		while (temp2->m_NodeType == OR_EXP && temp2->m_child[0] != NULL &&
			temp2->m_child[0]->m_NodeType == OR_EXP)
		{
			if (!(FindInTree(this, temp2->m_child[1], OR_EXP)))
			{
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp2->m_child[0]);
			temp2->CopyTree(temp);
			delete temp;
			temp = NULL;

		}
		if (temp2->m_NodeType == OR_EXP && temp2->m_child[0] != NULL && temp2->m_child[0]->m_NodeType != OR_EXP)
		{
			if (!(FindInTree(this, temp2->m_child[0], OR_EXP)))
				return false;
			if (!(FindInTree(this, temp2->m_child[1], OR_EXP)))
				return false;
		}
		delete temp2;
		temp2 = NULL;
		while (temp1->m_NodeType == OR_EXP && temp1->m_child[0] != NULL && temp1->m_child[0]->m_NodeType == OR_EXP)
		{
			if (!(FindInTree(ptree, temp1->m_child[1], OR_EXP)))
			{
				return false;
			}
			temp = new CSyntaxNode();
			temp->CopyTree(temp1->m_child[0]);
			temp1->CopyTree(temp);
			delete temp;
			temp = NULL;
		}
		if (temp1->m_NodeType == OR_EXP && temp1->m_child[0] != NULL && temp1->m_child[0]->m_NodeType != OR_EXP)
		{
			if (!(FindInTree(ptree, temp1->m_child[0], OR_EXP)))
				return false;
			if (!(FindInTree(ptree, temp1->m_child[1], OR_EXP)))
				return false;
		}
		delete temp1;
		temp1 = NULL;
	}
	else                    //add by YY[fixed] 2013/05/21 9:53
	{
		if (m_child[0] != NULL /*&& ptree->m_child[0]!=NULL*/)
		{
			if (!(m_child[0]->CompareTree(ptree->m_child[0])))
				return false;
		}
		if (m_child[1] != NULL /*&& ptree->m_child[1]!=NULL*/)
		{
			if (!(m_child[1]->CompareTree(ptree->m_child[1])))
				return false;
		}
		if (ptree->m_child[2] != NULL /*&& ptree->GetChild2()!=NULL*/)
		{
			if (!(m_child[2]->CompareTree(ptree->m_child[2])))
				return false;
		}

	}
	return true;
}



//--------------------------------------------------------------------------------------
//add by YY 2013/05/13 9:52
//programming by YK 2013/05/21 11:22
//在ptree中查找target是否存在，专用于CompareTree函数中
bool CSyntaxNode::FindInTree(CSyntaxNode * ptree, CSyntaxNode* target, NODETYPE ntype)
{
	if (target != NULL && ptree->GetChild0() != NULL &&
		target->CompareTree(ptree->GetChild0()))
	{
		return true;
	}
	else if (target != NULL && ptree->GetChild1() != NULL &&
		target->CompareTree(ptree->GetChild1()))
	{
		return true;
	}

	if (ptree->GetChild0() != NULL && ptree->GetChild0()->GetNType() == ntype)
	{
		if (FindInTree(ptree->GetChild0(), target, ntype))
		{
			return true;
		}
	}
	if (ptree->GetChild1() != NULL && ptree->GetChild1()->GetNType() == ntype)
	{
		if (FindInTree(ptree->GetChild1(), target, ntype))
		{
			return true;
		}
	}
	if (ptree->GetChild2() != NULL && ptree->GetChild2()->GetNType() == ntype)
	{
		if (FindInTree(ptree->GetChild2(), target, ntype))
		{
			return true;
		}
	}
	return false;

}

//------------------------------------------------------------------------------------------------------------------------
//2013-5-2 add by YY 暂时先不处理
bool CSyntaxNode::FindSameName(string pname)
{
	//先不处理
	if (!this->GetNName().compare(pname))
	{
		return true;
	}
	else
		return false;

}


//为了进行测试输出做准备  2013-5-2 add by YY[fixed]
//------------------------------------------------------------------------------------------------------------------------
string CSyntaxNode::TreeToString()
{
	if (NULL == this)//如果待转换的语法树为空
	{
		return "empty tree string";//则返回空字符串
	}
	string Stree = "";
	switch (m_NodeType)//根据待转换语法树节点的类型
	{
	case AUX_IDENT_EXP:
	{
		Stree = m_NodeName.data();
		Stree += "l";
		Stree += Stree;
		break;
	}
	case DECLARE_STA://如果是变量声明语句
	{
		switch (m_ReturnType)//根据节点的返回类型
		{
		case INTTYPE:
			Stree += "int ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case FLOATTYPE:
			Stree += "float ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case CHARTYPE:
			Stree += "char ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		case STRUCTTYPE:
			Stree += "struct ";
			Stree += m_child[0]->TreeToString();//把声明的变量序列转换为可以输出的字符串
			break;
		}
		break;
	}
	case STRUCT_IDENT_EXP:
	case STRUCT_LIST_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += ".";
		Stree += m_child[1]->TreeToString();
		if (m_child[2])
		{
			if (m_child[2]->GetNType() == STRUCT_IDENT_EXP)
			{
				Stree += ".";
				Stree += m_child[2]->TreeToString();
			}
			else if (m_child[2]->GetNType() == STRUCT_LIST_EXP)
			{
				Stree += ".";
				Stree += m_child[2]->TreeToString();
			}
			else if (m_child[2]->GetNType() == STRUCTP_IDENT_EXP)
			{
				Stree += "->";
				Stree += m_child[2]->TreeToString();
			}
			else if (m_child[2]->GetNType() == STRUCTP_LIST_EXP)
			{
				Stree += "->";
				Stree += m_child[2]->TreeToString();
			}
			else
			{
				fout << "TreeToString error!" << endl;
				return"";
			}
		}
		break;
	}
	case STRUCTP_IDENT_EXP:
	case STRUCTP_LIST_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += "->";
		Stree += m_child[1]->TreeToString();
		if (m_child[2])
		{
			if (m_child[2]->GetNType() == STRUCT_IDENT_EXP)
			{
				Stree += ".";
				Stree += m_child[2]->TreeToString();
			}
			else if (m_child[2]->GetNType() == STRUCT_LIST_EXP)
			{
				Stree += ".";
				Stree += m_child[2]->TreeToString();
			}
			else if (m_child[2]->GetNType() == STRUCTP_IDENT_EXP)
			{
				Stree += "->";
				Stree += m_child[2]->TreeToString();
			}
			else if (m_child[2]->GetNType() == STRUCTP_LIST_EXP)
			{
				Stree += "->";
				Stree += m_child[2]->TreeToString();
			}
			else
			{
				fout << "TreeToString error!" << endl;
				return"";
			}
		}
		break;
	}

	case PARAMETER_EXP://参数表达式
	{
		Stree += m_child[0]->TreeToString();
		if (m_child[1])
		{
			Stree += ',';
			Stree += m_child[1]->TreeToString();
		}
		break;
	}

	//add by YY 2013/06/09 9:34
	case ARRAY_DECLARE_STA:
	case LIST_DECLARE_STA:
	case LIST_SYMBOL_STA://LIST_VALUE
	{
		Stree = m_NodeName.data();
		Stree += '[' + m_child[0]->TreeToString();
		CSyntaxNode *search = m_child[1];
		while (search != NULL)
		{
			Stree += ',';
			Stree += search->GetChild0()->TreeToString();
			search = search->GetChild1();
		}
		Stree += ']';
		break;
	}
	case IDENT_EXP://identifier
	{
		//add by YY [fixed] 2013/05/07 10:45
		//以下两种赋值形式都可以：将string类型赋值给cstring
		Stree = m_NodeName.data();
		break;
	}
	case LIST_ASSVALUE_STA://[2, 7, 'd', "hello", 6.31]
	{
		Stree += '[';
		Stree += m_child[0]->TreeToString();
		CSyntaxNode *search = m_child[1];
		while (NULL != search)
		{
			Stree += ',';
			Stree += search->GetChild0()->TreeToString();
			search = search->GetChild0();
		}
		Stree += ']';
		break;
	}
	case ADDRESS_EXP:
	{
		Stree += '&';
		Stree += m_child[0]->TreeToString();
		break;
	}
	case VALUE_EXP://*()
	{
		Stree += '*';
		Stree += m_child[0]->TreeToString();
		break;
	}
	case VALUE_LIST_EXP://(*p)[2]
	{
		Stree = '(' + m_child[0]->TreeToString() + ')';
		Stree += '[';
		Stree += m_child[1]->TreeToString();
		CSyntaxNode *search = m_child[2];
		while (NULL != search)
		{
			Stree += ',';
			Stree += search->GetChild0()->TreeToString();
			search = search->GetChild1();
		}
		Stree += ']';
		break;
	}

	case STR_EXP://string
		Stree = m_Value.sValue.c_str();
		Stree += '"' + Stree + '"';
		break;
	case INTEGER_EXP:// interger
	{ stringstream ss;
	ss << m_Value.iValue;
	Stree = ss.str();
	ss.clear();
	break; }
	case FLOATLITERAL_EXP:// float
	{
		stringstream ss;
		ss << m_Value.fValue;
		Stree = ss.str();
		ss.clear();
		break; }
	case CHARLITERAL_EXP:// char
		Stree = m_Value.cValue;
		break;
	case EMPTY_EXP:// empty
		Stree += "empty";
		break;
	case SKIP_STA://skip
		Stree += "skip";
		break;
	case ASS_EQU_EXP://"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "<==";
		Stree += m_child[1]->TreeToString();
		break;
	case EX_ASS_EXP://"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "<==";
		Stree += m_child[1]->TreeToString();
		break;

	case EQU_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += '=';
		Stree += m_child[1]->TreeToString();
		break;
	case UNITASSIGN_EXP://:=
		Stree += m_child[0]->TreeToString();
		Stree += ":=";
		Stree += m_child[1]->TreeToString();
		break;
		//add by YY[fixed] 2013/05/24 16:11
	case CHOP_STA://";"
		if (m_child[0] && (m_child[0]->m_NodeType == PARALLEL_STA || m_child[0]->m_NodeType == CHOP_STA))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += ';';

		if (m_child[1] && (m_child[1]->m_NodeType == PARALLEL_STA || m_child[1]->m_NodeType == CHOP_STA))
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		break;

		//add by YY[fixed] 2013/05/24 16:11
	case AND_EXP:// " and "	双操作数
		if (m_child[0]->m_NodeType == CHOP_STA || m_child[0]->m_NodeType == PARALLEL_STA ||
			m_child[0]->m_NodeType == IMPLY_STA || m_child[0]->m_NodeType == IFF_STA ||
			m_child[0]->m_NodeType == OR_EXP || m_child[0]->m_NodeType == AND_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
		{
			Stree += m_child[0]->TreeToString();
			Stree += ' ';
		}

		Stree += " and ";

		if (m_child[1]->m_NodeType == CHOP_STA || m_child[1]->m_NodeType == PARALLEL_STA ||
			m_child[1]->m_NodeType == IMPLY_STA || m_child[1]->m_NodeType == IFF_STA ||
			m_child[1]->m_NodeType == OR_EXP || m_child[1]->m_NodeType == AND_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
		{
			Stree += ' ';
			Stree += m_child[1]->TreeToString();
		}
		break;

		//add by YY[fixed] 2013/05/24 16:11
	case OR_EXP://"or"
		if (m_child[0]->m_NodeType == CHOP_STA || m_child[0]->m_NodeType == PARALLEL_STA ||
			m_child[0]->m_NodeType == IMPLY_STA || m_child[0]->m_NodeType == IFF_STA ||
			m_child[0]->m_NodeType == OR_EXP || m_child[0]->m_NodeType == AND_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
		{
			Stree += m_child[0]->TreeToString();
			Stree += ' ';
		}

		Stree += " or ";

		if (m_child[1]->m_NodeType == CHOP_STA || m_child[1]->m_NodeType == PARALLEL_STA ||
			m_child[1]->m_NodeType == IMPLY_STA || m_child[1]->m_NodeType == IFF_STA ||
			m_child[1]->m_NodeType == OR_EXP || m_child[1]->m_NodeType == AND_EXP)
		{
			Stree += "(";
			Stree += m_child[1]->TreeToString();
			Stree += ")";
		}
		else
		{
			Stree += ' ';
			Stree += m_child[1]->TreeToString();
		}
		break;
	case NEGATION_EXP:
		Stree += '~';
		if (!(m_child[0]->m_NodeType == IDENT_EXP
			|| m_child[0]->m_NodeType == INTEGER_EXP
			|| m_child[0]->m_NodeType == TRUE_EXP
			|| m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case NEXT_STA://"next"
		Stree += "next ";
		if (!(m_child[0]->m_NodeType == IDENT_EXP
			|| m_child[0]->m_NodeType == EMPTY_EXP || m_child[0]->m_NodeType == TRUE_EXP
			|| m_child[0]->m_NodeType == FALSE_EXP || m_child[0]->m_NodeType == SKIP_STA
			|| m_child[0]->m_NodeType == DISPLAY_STA || m_child[0]->m_NodeType == REQUEST_STA))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";//加括号是为了表示出优先级和结合性吗？//?
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
		//Annotated by YY  2013/06/09 9:29
	case IF_ELSE_STA://"if_else"
	{
		Stree += "if (";
		Stree += m_child[0]->TreeToString();
		Stree += ") then {";
		Stree += m_child[1]->TreeToString();
		Stree += "} else {";
		Stree += m_child[2]->TreeToString();
		Stree += "}";
		break;
	}

	case IF_ELSE_EXP:
	{
		Stree += "if (";
		Stree += m_child[0]->TreeToString();
		Stree += ") then ";
		Stree += m_child[1]->TreeToString();
		Stree += " else ";
		Stree += m_child[2]->TreeToString();
		Stree += " ";
		break;
	}
	//Annotated by YY  2013/06/09 9:30
	case WHILE_DO_STA://"while_do"
		Stree += "while(";
		Stree += m_child[0]->TreeToString();
		Stree += "){";
		Stree += m_child[1]->TreeToString();
		Stree += "}";
		break;
	case FOR_TIMES_STA://for
		Stree += "for ";
		Stree += m_child[0]->TreeToString();
		Stree += " times do{";
		Stree += m_child[1]->TreeToString();
		Stree += "}";
		break;
	case LENGTH_STA://len
		Stree += "len ";
		if (!(m_child[0]->m_NodeType == IDENT_EXP || m_child[0]->m_NodeType == INTEGER_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case GT_EXP://">"
		Stree += m_child[0]->TreeToString();
		Stree += ">";
		Stree += m_child[1]->TreeToString();
		break;
	case LT_EXP://"<"
		Stree += m_child[0]->TreeToString();
		Stree += "<";
		Stree += m_child[1]->TreeToString();
		break;
	case GE_EXP://">="
		Stree += m_child[0]->TreeToString();
		Stree += ">=";
		Stree += m_child[1]->TreeToString();
		break;
	case LE_EXP://"<="
		Stree += m_child[0]->TreeToString();
		Stree += "<=";
		Stree += m_child[1]->TreeToString();
		break;
	case NE_EXP://"!="
		Stree += m_child[0]->TreeToString();
		Stree += "!=";
		Stree += m_child[1]->TreeToString();
		break;
	case CON_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += "^";
		Stree += m_child[1]->TreeToString();
		break;
	case ADD_EXP://"+"
		Stree += m_child[0]->TreeToString();
		Stree += "+";
		Stree += m_child[1]->TreeToString();
		break;
	case SUB_EXP://"-"
		Stree += m_child[0]->TreeToString();
		Stree += "-";
		Stree += m_child[1]->TreeToString();
		break;
	case MINUS_EXP:
		Stree += '-';
		if (!(m_child[0]->m_NodeType == IDENT_EXP || m_child[0]->m_NodeType == INTEGER_EXP || m_child[0]->m_NodeType == TRUE_EXP || m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	case MUL_EXP://"*"
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '*';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		break;
	case DIV_EXP://"/"
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '/';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		break;
	case MOD_EXP://"%"
		if (m_child[0]->m_NodeType == ADD_EXP || m_child[0]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();

		Stree += '%';

		if (m_child[1]->m_NodeType == ADD_EXP || m_child[1]->m_NodeType == SUB_EXP)
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[1]->TreeToString();
		break;
	case TRUE_EXP://"true"
		Stree += "True";
		break;
	case FALSE_EXP://"false"
		Stree += "False";
		break;
	case REQUEST_STA://"request"
	{
		if (m_child[1])
		{
			Stree += "input(";
			Stree += m_child[0]->TreeToString();

			CSyntaxNode *search = m_child[1];
			while (search)
			{
				Stree += ',';
				Stree += search->GetChild0()->TreeToString();
				search = search->GetChild0();
			}
			Stree += ')';
		}
		else
		{
			Stree += "input(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		break;
	}

	case DISPLAY_STA://"display"
	{
		if (m_child[1])
		{
			Stree += "output(";
			Stree += m_child[0]->TreeToString();

			CSyntaxNode *search = m_child[1];
			while (search)
			{
				Stree += ',';
				Stree += search->GetChild0()->TreeToString();
				search = search->GetChild0();
			}
			Stree += ')';
		}
		else
		{
			Stree += "output(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		break;
	}

	case MORE_STA://more    //?
		Stree += "more";
		break;
	case FINAL_STA://fin    //?
		Stree += "fin(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	case KEEP_STA://keep     //?
		Stree += "keep(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	case ALWAYS_STA://always
		Stree += "always(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	case HALT_STA://halt
		Stree += "halt(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	case IMPLY_STA://
		Stree += '(' + m_child[0]->TreeToString() + ')';
		Stree += "->";
		Stree += '[' + m_child[1]->TreeToString() + ']';
		break;
	case IFF_STA://"<->
		Stree += m_child[0]->TreeToString();
		Stree += "<->";
		Stree += m_child[1]->TreeToString();
		break;
	case FRAME_STA:
	case UNFRAME_STA:
	{
		Stree += "frame(";
		CSyntaxNode *search = NULL;
		if (m_child[0]->GetNType() == FRAME_FREE_STA)
		{
			Stree += m_child[0]->m_child[0]->TreeToString();
			search = m_child[0]->m_child[1];

		}
		else
		{
			Stree += m_child[0]->TreeToString();
			search = m_child[1];

		}
		while (NULL != search)
		{
			Stree += ',';
			Stree += search->m_child[0]->TreeToString();
			search = search->m_child[1];
		}
		Stree += ")";
		Stree += " and (";
		Stree += m_child[2]->TreeToString();
		Stree += ")";
		break;
	}
	case AWAIT_STA:
	{
		Stree += "await(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	}
	case PROJECTION_STA:
	{
		Stree += "{";
		Stree += m_child[0]->TreeToString();

		CSyntaxNode *search = m_child[1];
		while (NULL != search)
		{
			Stree += ',';
			Stree += search->m_child[0]->TreeToString();
			search = search->m_child[1];
		}
		Stree += "} prj (";
		Stree += m_child[2]->TreeToString();
		Stree += ")";
		break;
	}
	case PARALLEL_STA:
	{
		Stree += m_child[0]->TreeToString();
		Stree += " || ";
		Stree += m_child[1]->TreeToString();
		break;
	}

	case  SOMETIMES_STA:
	{
		Stree += "som ";
		if (!(m_child[0]->m_NodeType == IDENT_EXP
			|| m_child[0]->m_NodeType == INTEGER_EXP
			|| m_child[0]->m_NodeType == TRUE_EXP
			|| m_child[0]->m_NodeType == FALSE_EXP))
		{
			Stree += "(";
			Stree += m_child[0]->TreeToString();
			Stree += ")";
		}
		else
			Stree += m_child[0]->TreeToString();
		break;
	}

	//add by YY[fixed] 2013/05/06 17:51
	case CHOPSTAR_STA:
	{
		string stemp = "";
		CSyntaxNode *temp = this;
		while (temp->GetNType() == CHOPSTAR_STA)
		{
			Stree += '#';
			temp = temp->GetChild0();
		}
		stemp += '(' + temp->TreeToString() + ')';
		stemp += Stree;
		Stree = stemp;
		temp = NULL;
		stemp = "";
		break;
	}

	//add by YY 2013/06/06 21:09
	case STRFUNCHEAD_EXP:
	{
		Stree += "head( ";
		Stree += GetChild0()->TreeToString();
		Stree += " )";
		break;
	}
	case STRFUNCTAIL_EXP:
	{
		Stree += "tail( ";
		Stree += GetChild0()->TreeToString();
		Stree += " )";
		break;
	}
	case STRFUNCCAT_EXP:
	{
		Stree += "strcat( ";
		Stree += GetChild0()->TreeToString();
		Stree += ",";
		Stree += GetChild1()->TreeToString();
		Stree += " )";
		break;
	}

	case STRFUNCCMP_EXP:
	{
		Stree += "strcmp( ";
		Stree += GetChild0()->TreeToString();
		Stree += ",";
		Stree += GetChild1()->TreeToString();
		Stree += " )";
		break;
	}
	//add by YY 2013/12/13 16:54 math函数库
	case SIN_EXP:
	case COS_EXP:
	case TAN_EXP:
	case ASIN_EXP:
	case ACOS_EXP:
	case ATAN_EXP:
	case SINH_EXP:
	case COSH_EXP:
	case TANH_EXP:
	case EXP_EXP:
	case LOG_EXP:
	case LOG10_EXP:
	case SQRT_EXP:
	case ROUND_EXP:
	case FLOOR_EXP:
	case CEIL_EXP:
	case ABS_EXP:
	case FABS_EXP:
	case LABS_EXP:
	{
		switch (m_NodeType)
		{
		case SIN_EXP:Stree += "sin("; break;
		case COS_EXP:Stree += "cos("; break;
		case TAN_EXP:Stree += "tan("; break;
		case ASIN_EXP:Stree += "asin("; break;
		case ACOS_EXP:Stree += "acos("; break;
		case ATAN_EXP:Stree += "atan("; break;
		case SINH_EXP:Stree += "sinh("; break;
		case COSH_EXP:Stree += "cosh("; break;
		case TANH_EXP:Stree += "tanh("; break;
		case EXP_EXP:Stree += "exp("; break;
		case LOG_EXP:Stree += "log("; break;
		case LOG10_EXP:Stree += "log10("; break;
		case SQRT_EXP:Stree += "sqrt("; break;
		case ROUND_EXP:Stree += "round("; break;
		case FLOOR_EXP:Stree += "floor("; break;
		case CEIL_EXP:Stree += "ceil("; break;
		case ABS_EXP:Stree += "abs("; break;
		case FABS_EXP:Stree += "fabs("; break;
		case LABS_EXP:Stree += "labs("; break;
		default:break;
		}
		Stree += m_child[0]->TreeToString();
		Stree += " )";
		break;
	}
	case LDEXP_EXP:
	case FMOD_EXP:
	case ATAN2_EXP:
	case POW_EXP:
	{
		switch (m_NodeType)
		{
		case LDEXP_EXP:Stree += "ldexp("; break;
		case FMOD_EXP:Stree += "fmod("; break;
		case ATAN2_EXP:Stree += "atan2("; break;
		case POW_EXP:Stree += "pow("; break;
		default:break;
		}
		Stree += m_child[0]->TreeToString();
		Stree += ',';
		Stree += m_child[1]->TreeToString();
		Stree += ')';
		break;
	}
	case EMPTYNULL_EXP:
	{
		Stree += "Hold";
		break;
	}
	case FALSE_REDU_EXP:
	{
		Stree += "FalseNode";
		break;
	}
	//2010-07-15 end summer
	}
	return Stree;
}


//------------------------------------------------------------------------------------------------------------------------
//每次调用delete，都会自动调用析构函数
//2013-5-6 add by YY[fixed()]
CSyntaxNode::~CSyntaxNode()
{
	if (m_child[0] != NULL)
	{
		delete m_child[0];
		m_child[0] = NULL;
	}
	if (m_child[1] != NULL)
	{
		delete  m_child[1];
		m_child[1] = NULL;
	}
	if (m_child[2] != NULL)
	{
		delete  m_child[2];
		m_child[2] = NULL;
	}
}

