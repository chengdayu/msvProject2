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

//----------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype, vector<string> ST)
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

	m_Value.ST = ST;

}

//----------------------------------------------------------------------
CSyntaxNode::CSyntaxNode(NODETYPE pnodetype, RETURNTYPE preturntype, string&psValue, vector<string> ST)
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
	m_Value.sValue = psValue;

	m_Value.ST = ST;

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
	m_Value.ST = pnode->m_Value.ST;

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
			m_Value.sValue == pnode->m_Value.sValue&&
			m_Value.ST == pnode->m_Value.ST)
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
		Stree += "{";
		Stree += m_child[0]->TreeToString();
		CSyntaxNode *search = m_child[1];
		while (NULL != search)
		{
			Stree += ',';
			Stree += search->GetChild0()->TreeToString();
			search = search->GetChild1();
		}
		Stree += "}";
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
		break; 
	}
	case CHARLITERAL_EXP:// char
		Stree += '\'';
		Stree += m_Value.cValue;
		Stree += '\'';
		break;
	case ASS_EQU_EXP: //"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "=";
		Stree += m_child[1]->TreeToString();
		break;
	case EX_ASS_EXP://"<=="
		Stree += m_child[0]->TreeToString();
		Stree += "=";
		Stree += m_child[1]->TreeToString();
		break;
	case EQU_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += "==";
		Stree += m_child[1]->TreeToString();
		break;
	case UNITASSIGN_EXP://:=
		Stree += m_child[0]->TreeToString();
		Stree += ":=";
		Stree += m_child[1]->TreeToString();
		break;
		//add by YY[fixed] 2013/05/24 16:11
	case CHOP_STA://";"
		Stree += m_child[0]->TreeToString();
		Stree += ";\n";
		Stree += m_child[1]->TreeToString();
		break;
	case NEGATION_EXP:
		Stree += "!";
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
	case IF_ELSE_STA://"if_else"
	{
		Stree += "if (";
		Stree += m_child[0]->TreeToString();
		Stree += "){";
		Stree += m_child[1]->TreeToString();
		Stree += ";\n";
		if (m_child[2] != NULL)
		{
		Stree += "} else {";
		Stree += m_child[2]->TreeToString();
		Stree += ";\n";
		}		
		Stree += "}";
		Stree += "\n";
		break;
	}

	case IF_ELSE_EXP:
	{
		Stree += "if (";
		Stree += m_child[0]->TreeToString();
		Stree += ") ";
		Stree += m_child[1]->TreeToString();
		Stree += " else ";
		Stree += m_child[2]->TreeToString();
		Stree += " ";

		Stree += "\n";
		break;
	}
	//Annotated by YY  2013/06/09 9:30
	case WHILE_DO_STA://"while_do"
		Stree += "while(";
		Stree += m_child[0]->TreeToString();
		Stree += "){";
		Stree += m_child[1]->TreeToString();
		Stree += ";\n";
		Stree += "}";

		Stree += "\n";
		break;
	case FOR_TIMES_STA://for
		Stree += "for ";
		Stree += m_child[0]->TreeToString();
		Stree += " times do{";
		Stree += m_child[1]->TreeToString();
		Stree += "}";
		break;
	case AND_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += " && ";
		Stree += m_child[1]->TreeToString();
		break;
	case OR_EXP:
		Stree += m_child[0]->TreeToString();
		Stree += " || ";
		Stree += m_child[1]->TreeToString();
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
	case REQUEST_STA://程序中的input()语句
	{
		Stree += m_Value.sValue;
		break;
	}
	case DISPLAY_STA://程序中的output()语句
	{
		Stree += m_Value.sValue;
		break;
	}
	case STATE_OUTPUT_STA://每个状态的输出语句
	{
		Stree += m_Value.sValue.data();
		break;
	}
	case FUNCTION_STA:
	case EXT_FUNCTION_STA:
	{
		//函数调用语句
		Stree += m_NodeName;
		Stree += "(";
		CSyntaxNode* par = m_child[0];
		while (par != NULL)
		{
			Stree += par->TreeToString();
			par = par->m_child[1];
			if (par != NULL)
			{
				Stree += ",";
			}
		}
		Stree += ")";
		break;
	}
	case FUNCTION_DEFINE_STA:
	{
		//函数返回值类型
		if (m_child[0]->m_child[0] != NULL)
		{
			Stree += m_child[0]->m_child[0]->TreeToString();
		}

		Stree += " ";
		//函数名
		Stree += m_NodeName;

		//参数列表
		Stree += "(";

		Stree += m_child[0]->m_child[1]->TreeToString();
		
		Stree += ")";
		//函数体
	    Stree += "{\n";
		if (m_child[0]->m_child[0] != NULL)
		{
			Stree += m_child[0]->m_child[0]->TreeToString();
			Stree += " RValue;";
		}
		Stree += m_child[1]->TreeToString();
		Stree += ";";

		if (m_child[0]->m_child[0] != NULL)
		{
			Stree += "return RValue;";
		}
		Stree += "\n}\n";
		break;
	}
	case RETURN_STA:
	{
		Stree += "return ";
		//modified by yubin 2015/4/17,以前return的情况只能是变量，现在也可以是表达式，因此输出的时候，输出child0
		Stree += m_child[0]->TreeToString();
		Stree += ";";
		break;
	}
	case PARAMETER_EXP://参数表达式
	{
		CSyntaxNode* par = this;
		while (par != NULL)
		{
			switch (par->m_ReturnType)
			{
			case INTTYPE:
			{
				Stree += "int ";
				break;
			}
			case FLOATTYPE:
			{
				Stree += "float";
				break;
			}
			case CHARTYPE:
			{
				Stree += "char";
				break;
			}
			default:
				break;
			}
			Stree += par->m_child[0]->TreeToString();
			par = par->m_child[1];
			if (par != NULL)
			{
				Stree += ",";
			}
		}
		break;
	}
	case ACTUAL_PARAMETER_EXP:
	{
		switch (m_ReturnType)
		{
		//考虑到函数sizeof的参数可能是int,float等，所以添加以下判断语句
		case INTTYPE:
			Stree += "int";
			break;
		case FLOATTYPE:
			Stree += "float";
			break;
		case CHARTYPE:
			Stree += "char";
			break;
		case UINTTYPE:
			Stree += "unsigned int";
			break;
		case UCHARTYPE:
			Stree += "unsigned char";
			break;
		//返回值不是以上类型时，则为普通实参即字面量或标示符
		default:
			Stree += m_child[0]->TreeToString();
			break;
		}
		break;
	}
	case POINT_PARAMETER_EXP:
	{
		Stree += "* ";
		Stree += m_child[0]->TreeToString();
		break;
	}
	case DOUBLEPOINT_PARAMETER_EXP:
	{
		Stree += "**";
		Stree += m_child[0]->TreeToString();
		break;
	}
	case STRUCT_PARAMETER_EXP:
	{
		Stree += m_NodeName;
		Stree += m_child[0]->TreeToString();
		break;
	}
	case STRUCTP_PARAMETER_EXP:
	{
		Stree += m_NodeName;
		Stree += "* ";
		Stree += m_child[0]->m_NodeName;
		break;
	}
	case DOUBLESTRUCTP_PARAMETER_EXP:
	{
		Stree += m_NodeName;
		Stree += "** ";
		Stree += m_child[0]->m_NodeName;
		break;
	}
	case ARRAY_PARAMETER_EXP:
	{
		Stree += m_child[0]->TreeToString();
		Stree += "[]";
		break;
	}
	case POINTER_ARRAY_PARAMETER_EXP:
	{
		Stree += "*";
		Stree += m_child[0]->TreeToString();
		Stree += "[]";
		break;
	}
	case STRUCT_ARRAY_PARAMETER_EXP:
	{
		Stree += m_NodeName;
		Stree += " ";
		Stree += m_child[0]->TreeToString();
		Stree += "[]";
		break;
	}
	case TYPE_CAST_STA:
	{
		//要转换为的类型
		Stree += "(";
		switch (m_ReturnType)
		{
		case INTTYPE:
			Stree += "int";
			break;
		case FLOATTYPE:
			Stree += "float";
			break;
		case CHARTYPE:
			Stree += "char";
			break;
		case UINTTYPE:
			Stree += "unsigned int";
			break;
		case UCHARTYPE:
			Stree += "unsigned char";
			break;
		case INTPTYPE:
			Stree += "int *";
			break;
		case FLOATPTYPE:
			Stree += "float *";
			break;
		case CHARPTYPE:
			Stree += "char *";
			break;
		case UINTPTYPE:
			Stree += "unsigned int *";
			break;
		case UCHARPTYPE:
			Stree += "unsigned char *";
			break;
		}
		Stree += ")";
		Stree += "(";
		Stree += m_child[0]->TreeToString();
		Stree += ")";
		break;
	}
	case ADDRESS_DECLARE_STA:
	{
		Stree += "*";
		Stree += m_child[0]->m_NodeName;
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

//2015-3-16 add by yubin, 用于exists， 如果节点的名字等于origNam，如果是在函数中，将结点的名字换成#函数名_变量名_块名，如果是全局的，将节点名字换成#变量名_块名
void CSyntaxNode::replaceExistName(string origNam, string funcNam, string blockNam)
{
	if (m_NodeName == origNam)
	{
		if (funcNam == "")
		{
			m_NodeName = "$" + m_NodeName + "_" + blockNam;
		}
		else
		{
			m_NodeName = "$" + funcNam + "_" + m_NodeName + "_" + blockNam;
		}

	}

	if (m_child[0] != NULL)
	{
		GetChild0()->replaceExistName(origNam, funcNam, blockNam);
	}
	if (m_child[1] != NULL)
	{
		GetChild1()->replaceExistName(origNam, funcNam, blockNam);
	}
	if (m_child[2] != NULL)
	{
		GetChild2()->replaceExistName(origNam, funcNam, blockNam);
	}
}


