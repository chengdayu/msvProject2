/*******************************************************************************
*@file     Symbol.cpp
*@author
*@remarks  Modified by
*@version  1.0
*@section  Brief
*Modified: 2013/04/22  16:23
*
*
*
*Copyright (C) ICTT.,XiDian univ
*All rights reserved.
*
*******************************************************************************/


#include "function.h"
#include "MyMap.h"
#include <string>

//using  std::string;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
CMAP::CMAP()
{

}

bool CMAP::find(const string pStructName)
{
	map<string, string>::iterator iter;
	iter = m_structName.find(pStructName);
	if (iter == m_structName.end())
	{
		return false;//没找到
	}
	else
	{
		return true;
	}
}
bool CMAP::insert(const string pStructName)
{
	pair<std::map<string, string>::iterator, bool> ret;
	ret = m_structName.insert(make_pair(pStructName, pStructName));
	if (ret.second == false)
	{
		return false;
	}
	else
	{
		return true;
	}
}
void CMAP::clear()
{
	m_structName.clear();
}
CMAP::~CMAP()
{
	m_structName.clear();
}
