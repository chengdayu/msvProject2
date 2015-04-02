// stdafx.cpp : source file that includes just the standard includes
//	MSV.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "function.h"


extern int g_nErrors;
extern int lineno;

ofstream fout(OutPath);//定义流(文件缓冲对象)

void error(const char *cFormat, ...)
{
	va_list args;

	++g_nErrors;
	//	fprintf( stderr, "Line %d: ", lineno );
	fout << "Line " << lineno << ": ";
	fout.close();
	va_start(args, cFormat);

	//	vfprintf( stderr, cFormat, args );
	FILE * temp;
	temp = fopen(OutPath, "a");
	vfprintf(temp, cFormat, args);
	fclose(temp);
	fout.open(OutPath, ios::app);

	va_end(args);
	fout << endl;
}

void errorSummary()  //错误概况
{
	fout << g_nErrors << " error(s) were found." << endl;//将出错情况输出与OutPath关联的流文件
}

void yyerror(const char *msg)
{
	error(msg);
}

extern "C" int yywrap(void)
{
	return 1;
}
