%{
#include"function.h"
#include <malloc.h>
#include <string.h>      // strcpy
//#include"MSV.scanner.cc"
#include "SyntaxNode.h"
#include "SyntaxTree.h"

//******************  用来存储程序语法树以及性质树 *******************************/
/***************************************************************************/
//程序语法树节点指针
CSyntaxNode *g_syntaxTree = NULL;
//性质语法树节点指针
CSyntaxNode *g_propositionTree=NULL;

CSyntaxNode *function_tree=NULL, *propositionDefineTree=NULL;

CSyntaxNode *struct_tree=NULL;//存储结构体的定义

CSyntaxTree *g_tree=NULL, *g_proptree=NULL;     //2013-5-3 add by YY[fixed]

int g_nErrors=0;

extern void yyerror(const char *msg);

extern int yylex (void);

#define YYDEBUG 1	          // Generate debug code; needed for YYERROR_VERBOSE
#define YYERROR_VERBOSE       // Give a more specific parse error message



%}

%union{
   char     * cString;   /* A character string */
   int       nInteger;   /* An integer value */
   float floatval;	/* A float value added by Jane 2012-06-06 */
   char character;	/* A character added by Jane 2012-06-06*/
   CSyntaxNode *tnode;    /* Node in the syntax tree */
   enum NODETYPE  nodetype;
   enum RETURNTYPE  returntype;
};


%token IF ELSE EMPTY AWAIT PROJECTION  PBEGIN PEND POINTERNULL ARRAY STRUCT DOT UNION FOPEN FCLOSE FILEDECLARATION FGETS FPUTS FGETC FPUTC
%token MORE DEFINE MY_TRUE MY_FALSE EXIST FRAME FOR WHILE DO
%token REPEAT UNTIL DISPLAY SKIP THEN COMMA COLON
%token INTDECLARATION STRDECLARATION POINTERDECLARATION CHARDECLARATION VOIDDECLARATION//added by Jane 2012-06-06
%token FLOATDECLARATION LISTDECLARATION           //added by Jane 2012-06-06
%token OPEN_PAR CLOSE_PAR TIMES OPEN_BPAR CLOSE_BPAR OPEN_MPAR CLOSE_MPAR FUNCTION
%token OVER CYLINDER//2010-04-23 add Jane
%token OVEREP//2010-07-07 add Jane
%token INTER_OR NON_DETERMINED //2010-07-07 add Jane
%token SEND RECEIVE PROCESS CHANNEL PUT GET ERROR_TOKEN
%token STRFUNCHEAD STRFUNCTAIL STRFUNCCAT STRFUNCCMP STRFUNCCPY STRFUNCLEN //added by Jane 2012-06-21
%token SIZEOF
%token SYSTEM
%token SWITCH BREAK CASE DEFAULT//add by yubin 2015-3-23

//add by YY 2013-6-6
%token CEIL FLOOR ROUND
%token SIN COS TAN ASIN ACOS ATAN SINH COSH TANH EXP LOG LOG10 SQRT ATAN2 POW ABS FABS LABS FMOD MODF LDEXP FREXP
%token UNSIGNED SIGNED
%token MALLOC FREE//add by yk 2014-2-13
%left COMMA
%left CHOP
%left IMPLY IFF PROPIMPLY
%left OR PARALLEL CYLINDER
%left AND
%right ASS_EQU UNITASSIGN
%left INTER_OR	//added by Jane 2011-08-22	按位异或
%left OVEREP	//added by Jane 2011-08-22
%left CON //add by yubin 按位异或
%left ADDRESS //add by yubin 按位与
%left NE EQU
%left GE LE  GT LT
%left LST RST//add by yubin  左移右移
%left ADD SUB
%left MUL DIV MOD
%left BNE//add by yubin 按位取反




%nonassoc CHOPSTAR NEXT NEGATION LENGTH REQUEST FINAL KEEP ALWAYS HALT W_NEXT PREFIX /*ADDRESS*/ PREVIOUS SOMETIMES

%token<cString> ID STR STRUCT_TYPE
%token<nInteger> INTEGER
%token<floatval> FLOATLITERAL //added by Jane 2012-06-06
%token<character> CHARLITERAL //added by Jane 2012-06-06

//-----------%type<cString> idstatement       //Annotation-Class   2013-5-6 add by YY[fixed]
//%type<tnode> cylinder_statement over_statement rep_statement fomer_rep_statement //2010-07-07 add
//%type<tnode> optional_fomer_rep_statement optional_rep_statement//2010-07-07 add
//annotator: Jane 2011-08-10
//added for cylinder computing  Jane 2011-08-10 begin
%type<tnode> CYLINDER_STATEMENT OVER_STATEMENT INTERVAL_STATEMENT
%type<tnode> SIG_INTERVAL
//added for cylinder computing  Jane 2011-08-10 end


%type<tnode> type_cast  /*--------cdstatement mdstatement new atrbstatement-----*/ //Annotation-Class   2013-5-6 add by YY[fixed]
%type<tnode> program statement identifier string integer ass_statement bool_statement optional_exp_else_statement leftaddrshift rightaddrshift
%type<tnode> if_statement optional_else_statement while_statement extend_for_statement ass_ident imply_pre for_sta_init for_sta_condition for_sta_control
%type<tnode> alg_exp for_statement optional_frame_identifier optional_projection type_cast_alg_exp
%type<tnode> prime_bool_statement option_function_parameter_list inner_optional_define_identifier
%type<tnode> option_function_identifier inner_option_function_identifier empty_statement
%type<tnode> option_list_value inner_option_list_value option_alg_exp ass_exp list_exp
%type<tnode> declaration option_identifier_array_declaration inner_option_identifier_array_declaration value_exp list option_struct_declaration
%type<tnode> address_exp//added by Jane
%type<tnode> option_array_declaration inner_option_array_declaration//added by Jane
//add by YY 2013/06/17 16:30
%type<tnode> big_par bool_par option_output option_input display_statement request_statement function   pre_next  //2013-6-6 add by YY
%type<tnode> factor simple_alg_exp pbool_expression
%type<tnode> charliteral floatliteral //added by Jane
%type<tnode> size_of
%type<tnode> struct_define_statement struct_identifier
%type<tnode> option_struct_list_value //add by YY 结构体的初始化列表{{1,"hello"},{2,"hi"}}
%type<tnode> sign_declaration//add by YY 带有unsigned的定义语句

%type<tnode> Float_Function //float_exp //add by YY 2013-6-6
//%type<tnode>array_exp option_array_exp //add by YY 2013-6-7

//字符处理函数 begin Jane
%type<tnode> String_Function String_Function_head String_Function_tail String_Function_cat String_Function_cpy String_Function_len
%type<tnode> String_Function_cmp str_func_parameter
//字符处理函数 end Jane
%type<tnode> casted_element

//add by YY 2014/01/17 结构体串联
%type<tnode> struct_member_exp  option_struct_member_exp


%type<nodetype>		assign_operator relation_operator alg_operator mul_operator
%type<returntype>  type_define sign_type_define
%type<returntype> all_type_define all_sizeof_type
%type<tnode> gComplexProposition complexProposition poptional_projection file_statement

%type<tnode> switch_statement case_par init_case_par //add by yubin 2015-3-23


//%expect 1  /* shift/reduce conflict: dangling ELSE declaration */

%%
program      //Annotation-Class   2013-5-6 add by YY[fixed]
       :gComplexProposition /*---cdstatement---*/ statement
	   {

			//验证需要的语句
			g_propositionTree=$1;
			g_proptree=new CSyntaxTree(g_propositionTree);    //2013-5-3 add by YY[fixed]



			//$2是类中定义的方法，将其加入function_tree中，实质是合并两个链
			//Annotation-Class   2013-5-6 add by YY[fixed]
			/*---------------------------------------------------------------------
			SyntaxTree  temp;
			temp = $2;
			if (temp!=NULL)
			{
					while(temp->GetChild2()!=NULL){ temp=temp->GetChild2(); };
					temp->SetChild2(function_tree);
					function_tree = $2;
			}
			---------------------------------------------------------------------*/


			//执行的基本语句
			g_syntaxTree=$2;
			g_tree=new CSyntaxTree(g_syntaxTree);          //2013-5-3 add by YY[fixed]

		}
	   ;
//下面是验证条件的语句的语法
gComplexProposition
	   :PBEGIN complexProposition PEND  {$$=$2;}
	   |								{$$=NULL;}
	   ;

complexProposition
	   :complexProposition AND complexProposition   {$$=new CSyntaxNode(AND_EXP, $1, $3, VOIDTYPE);}    //{$$=new CSyntaxNode(AND_EXP, "", 0, $1, $3);}
	   |complexProposition OR complexProposition    {$$=new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE);}     //{$$=new CSyntaxNode(OR_EXP, "", 0, $1, $3);}
	   |complexProposition IFF complexProposition   {$$=new CSyntaxNode(IFF_STA, $1, $3, BOOLTYPE);}    //{$$=new CSyntaxNode(IFF_STA, "", 0, $1, $3);}
	   |complexProposition PROPIMPLY complexProposition {$$=new CSyntaxNode(IMPLY_STA, $1, $3, VOIDTYPE);}     //{$$=new CSyntaxNode(IMPLY_STA, "", 0, $1, $3);}
	   |complexProposition CHOP complexProposition
	   {
			if($1==NULL)                            $$=$3;
			else                                    $$=new CSyntaxNode(CHOP_STA, $1, $3, VOIDTYPE);       //$$=new CSyntaxNode(CHOP_STA, "", 0, $1, $3);
	   }
	   |ALWAYS complexProposition					{$$=new CSyntaxNode(ALWAYS_STA, $2, VOIDTYPE);}    //{$$=new CSyntaxNode(ALWAYS_STA, "", 0, $2);}
	   |KEEP complexProposition					    {$$=new CSyntaxNode(KEEP_STA, $2, VOIDTYPE);}      //{$$=new CSyntaxNode(KEEP_STA, "", 0, $2);}
	   |SOMETIMES complexProposition				{$$=new CSyntaxNode(SOMETIMES_STA, $2, VOIDTYPE);} //{$$=new CSyntaxNode(SOMETIMES_STA, "", 0, $2);}
	   |W_NEXT complexProposition					{$$=new CSyntaxNode(W_NEXT_STA, $2, VOIDTYPE);}    //{$$=new CSyntaxNode(W_NEXT_STA, "", 0, $2);}
	   |NEXT complexProposition			            {$$=new CSyntaxNode(NEXT_STA, $2, VOIDTYPE);}      //{$$=new CSyntaxNode(NEXT_STA, "", 0, $2);}
	   |SKIP										{$$=new CSyntaxNode(SKIP_STA, VOIDTYPE);}          //{$$=new CSyntaxNode(SKIP_STA, "", 0);}
	   |MY_TRUE										{$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}         //{$$=new CSyntaxNode(TRUE_EXP, "TRUE", 0);}
	   |MY_FALSE									{$$=new CSyntaxNode(FALSE_EXP, VOIDTYPE);}        //{$$=new CSyntaxNode(FALSE_EXP, "FALSE", 0);}
	   |LENGTH alg_exp								{$$=new CSyntaxNode(LENGTH_STA, $2, VOIDTYPE);}    //{$$=new CSyntaxNode(LENGTH_STA, "", 0, $2);}


	   //2013-5-7 add by YY[fixed]
	   |LENGTH integer							    {$$=new CSyntaxNode(LENGTH_STA, $2, VOIDTYPE);}    //{$$=new CSyntaxNode(LENGTH_STA, "", 0, $2);}
	   |EMPTY										{$$=new CSyntaxNode(EMPTY_EXP, VOIDTYPE);}        //{$$=new CSyntaxNode(EMPTY_EXP, "EMPTY", 0);}
	   |MORE										{$$=new CSyntaxNode(MORE_STA, VOIDTYPE);}        //{$$=new CSyntaxNode(MORE_EXP, "MORE", 0);}
	   |HALT complexProposition                     {$$=new CSyntaxNode(HALT_STA, $2, VOIDTYPE);}    //{$$=new CSyntaxNode(HALT_STA, "", 0, $2);}
	   |FINAL complexProposition                    {$$=new CSyntaxNode(FINAL_STA, $2, VOIDTYPE);}    //{$$=new CSyntaxNode(FINAL_STA, "", 0, $2);}
	   |NEGATION complexProposition                 {$$=new CSyntaxNode(NEGATION_EXP, $2, VOIDTYPE);}   //{$$=new CSyntaxNode(NEGATION_EXP, "", 0, $2);}
	   |OPEN_BPAR complexProposition poptional_projection CLOSE_BPAR PROJECTION OPEN_BPAR complexProposition CLOSE_BPAR
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, $3, $7, VOIDTYPE);                                //$$=new CSyntaxNode(PROJECTION_STA, "", 0, $2, $3, $7);
	   }
	   |ID 											{$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE)}     //{$$=new CSyntaxNode(IDENT_EXP, $1, 0);}
	   |DEFINE ID COLON prime_bool_statement//pbool_expression
	   {
			$$=NULL;
			propositionDefineTree=new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, propositionDefineTree, NULL, VOIDTYPE);
	   }
       |complexProposition  CHOPSTAR  {$$=new CSyntaxNode(CHOPSTAR_STA, $1, VOIDTYPE);}   //{$$=new CSyntaxNode(CHOPSTAR_STA, "", 0, $1);}
	   |OPEN_PAR complexProposition CLOSE_PAR       {$$=$2;}
	   ;

poptional_projection
	   :COMMA complexProposition poptional_projection
	   {
			$$ = new CSyntaxNode(PROJECTION_STA, $2, $3, VOIDTYPE);                      //$$ = new CSyntaxNode(PROJECTION_STA, "", 0, $2, $3);
	   }
	   |								{$$=NULL;}
	   ;
pbool_expression
		:alg_exp relation_operator alg_exp  {$$=new CSyntaxNode($2, $1, $3, BOOLTYPE);}
//		|alg_exp ASS_EQU alg_exp        {$$=new CSyntaxNode(ASS_EQU_EXP, $1, $3, BOOLTYPE);}
		;

//下面是可执行基本语句的语法
statement
	   :statement  AND statement        {$$=new CSyntaxNode(AND_EXP, $1, $3, VOIDTYPE);}

	   |statement  OR  statement        {$$=new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE);}

	   //**********************************************************************************
	   //   把函数声明语句放到function_tree中进行存储  2013-4-17 add by YY
	   //   按照原始的定义，函数的声明确实要在最开始进行
	   //   我添加了下面一块代码 使得的函数可以在任何地方声明
	   //**********************************************************************************
	   |statement CHOP statement
	   {
			//结构体(struct)定义 暂时规定struct只能在最开始定义

			//add by YY 2013/11/20 两个都是struct定义语句
			if(NULL!=$1 && $1->GetNType()==STRUCT_DEFINE_STA && NULL!=$3 && $3->GetNType()==STRUCT_DEFINE_STA)
			{
			    if($1->FindSameName($3->GetNName()) )
					{  yyerror("Structs have the same name");
					   $$=$1;
					}
				else
				    {
					  $3->SetChild1($1);
					  $$=$3;
					}
			}
			//add by YY 2013/11/20 第一个是struct定义语句，第二个是执行语句
			if(NULL!=$1 && $1->GetNType()==STRUCT_DEFINE_STA && NULL!=$3 && $3->GetNType()!=STRUCT_DEFINE_STA )
			{
				struct_tree=$1;
				$$=$3;
			}
			//add by YY 2013/11/20 第一个是执行语句，第二个是struct定义语句
			if( NULL!=$1 && $1->GetNType()!=STRUCT_DEFINE_STA  && $1->GetNType()!=FUNCTION_DEFINE_STA &&
			    NULL!=$3 && $3->GetNType()==STRUCT_DEFINE_STA)
			{
				        if(NULL==struct_tree)
						{
							struct_tree=$3;
						}
					    else
						{
							if(struct_tree->FindSameName($3->GetNName()))
							{
									yyerror("Structs have the same name");
								    $$=$1;
							}
							else
							{
								CSyntaxNode* p = struct_tree;
								while(p->GetChild1()!=NULL)
								p=p->GetChild1();
								p->SetChild1($3);
								$$=$1;
							}
						}
			}

			//两个都是函数声明语句
			if(NULL!=$1 && $1->GetNType()==FUNCTION_DEFINE_STA && NULL!=$3 && $3->GetNType()==FUNCTION_DEFINE_STA )
			{
				if($1->FindSameName($3->GetNName()) )
					{  yyerror("Functions or predicates have the same name");
					   $$=$1;
					}
				else
				    {
					  $3->SetChild2($1);
					  $$=$3;
					}
			}
			//第一个是函数声明语句，第二个是执行语句
			if(NULL!=$1 && $1->GetNType()==FUNCTION_DEFINE_STA && NULL!=$3 && $3->GetNType()!=FUNCTION_DEFINE_STA)
			{
				function_tree=$1;
                $$=$3;
			}

			//两个都是执行语句
			if( NULL!=$1 && $1->GetNType()!=FUNCTION_DEFINE_STA &&  $1->GetNType()!=STRUCT_DEFINE_STA &&
			    NULL!=$3 && $3->GetNType()!=FUNCTION_DEFINE_STA &&  $3->GetNType()!=STRUCT_DEFINE_STA)
			{
				$$=new CSyntaxNode(CHOP_STA, $1, $3, VOIDTYPE);
			}

			//第一个是执行语句，第二个是声明语句  2013-4-17 add by YY[fixed]
			//problem: 整个程序的第一句必须是声明语句才可以
			if( NULL!=$1 && $1->GetNType()!=FUNCTION_DEFINE_STA   &&  $1->GetNType()!=STRUCT_DEFINE_STA &&
			    NULL!=$3 && $3->GetNType()==FUNCTION_DEFINE_STA)
				{
				        if(NULL==function_tree)
						{
							function_tree=$3;
						}
					    else
						{
							if(function_tree->FindSameName($3->GetNName()))
							{
									yyerror("Functions or predicates have the same name");
								    $$=$1;
							}
							else
							{
								CSyntaxNode* p = function_tree;
								while(p->GetChild2()!=NULL)
								p=p->GetChild2();
								p->SetChild2($3);
								$$=$1;
							}
						}

				}
	   }

	   |PREFIX OPEN_PAR statement CLOSE_PAR   {$$=new CSyntaxNode(PREFIX_STA, $3, VOIDTYPE);}
       |ass_statement                   {$$=$1;}
	   |file_statement                  {$$=$1;}//add by yubin 2014/3/3 文件的相关操作
	   |sign_declaration                    {$$=$1;}
//	   |return_statement                {$$=$1;}

	   |if_statement                    {$$=$1;}
	   |while_statement                 {$$=$1;}
	   |for_statement                   {$$=$1;}
	   |extend_for_statement            {$$=$1;}//类似于C语言中的for语句
	   |NEXT OPEN_PAR statement CLOSE_PAR    {$$=new CSyntaxNode(NEXT_STA, $3, VOIDTYPE);}
	   |W_NEXT OPEN_PAR statement CLOSE_PAR    {$$=new CSyntaxNode(W_NEXT_STA, $3, VOIDTYPE);}
	   |KEEP OPEN_PAR statement CLOSE_PAR       {$$=new CSyntaxNode(KEEP_STA, $3, VOIDTYPE);}
	   |ALWAYS OPEN_PAR statement CLOSE_PAR      {$$=new CSyntaxNode(ALWAYS_STA, $3, VOIDTYPE);}
	   |HALT OPEN_PAR bool_statement CLOSE_PAR   {$$=new CSyntaxNode(HALT_STA, $3, VOIDTYPE);}
	   |AWAIT OPEN_PAR bool_statement CLOSE_PAR  {$$=new CSyntaxNode(AWAIT_STA, $3, VOIDTYPE);}
	   |REPEAT OPEN_BPAR statement CLOSE_BPAR UNTIL OPEN_PAR bool_statement CLOSE_PAR  {$$=new CSyntaxNode(REPEAT_UNTIL_STA, $3, $7, VOIDTYPE);}
	   |SKIP                            {$$=new CSyntaxNode(SKIP_STA, VOIDTYPE);}
       |EMPTY                           {$$=new CSyntaxNode(EMPTY_EXP, VOIDTYPE);}
	   |MORE                            {$$=new CSyntaxNode(MORE_STA, VOIDTYPE);}
	   |LENGTH OPEN_PAR alg_exp CLOSE_PAR   {$$=new CSyntaxNode(LENGTH_STA, $3, VOIDTYPE);}
//2013-5-7 add by YY[fixed]
//
//	   |LENGTH OPEN_PAR integer CLOSE_PAR  {$$=new CSyntaxNode(LENGTH_STA, $3, VOIDTYPE);}
//annotated by YY 2013/06/17 16:20
///	   |REQUEST OPEN_PAR identifier CLOSE_PAR             {$$=new CSyntaxNode(REQUEST_STA, $3, VOIDTYPE);}
	   |request_statement               {$$=$1;}
	   |display_statement               {$$=$1;}
	   |MY_TRUE                         {$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}
	   |MY_FALSE                        {$$=new CSyntaxNode(FALSE_EXP, VOIDTYPE);}
	   |FINAL OPEN_PAR statement CLOSE_PAR   {$$=new CSyntaxNode(FINAL_STA, $3, VOIDTYPE);}
	   | statement PARALLEL  statement  	{$$=new CSyntaxNode(PARALLEL_STA, $1, $3, VOIDTYPE);}

	   |OPEN_PAR imply_pre CLOSE_PAR IMPLY OPEN_MPAR statement CLOSE_MPAR
			{$$=new CSyntaxNode(IMPLY_STA, $2, $6, VOIDTYPE);}    //add by YY【fixed】

	   |FRAME OPEN_PAR identifier optional_frame_identifier CLOSE_PAR AND OPEN_PAR statement CLOSE_PAR
	   {
			$$=new CSyntaxNode(FRAME_STA, $3, $4, $8, VOIDTYPE);
	   }

//prj
	   |OPEN_BPAR statement optional_projection CLOSE_BPAR PROJECTION big_par
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, $3, $6,VOIDTYPE);
	   }
// 2013-5-7 add by YY[fixed]
	   |OPEN_BPAR statement CLOSE_BPAR PROJECTION big_par
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, NULL, $5, VOIDTYPE);
	   }


	   //***********************************************************************
	      //无返回值的函数调用      过程！！！！！  2013-4-16 add by YY
	   //***********************************************************************
	   |DEFINE ID OPEN_PAR option_function_parameter_list CLOSE_PAR ASS_EQU OPEN_BPAR empty_statement CLOSE_BPAR
	   {
			$$=new CSyntaxNode(FUNCTION_DEFINE_STA , $2, $4, $8, NULL, VOIDTYPE);
	   }

	   //*******************************************************************************************
	       //有返回值的函数调用function, 函数体为可以赋值的算术式alg_exp    2013-4-16 add by YY
	   //*******************************************************************************************
	   |FUNCTION ID OPEN_PAR option_function_parameter_list CLOSE_PAR  OPEN_BPAR statement CLOSE_BPAR
	   {
			$$=new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, $7, NULL, VOIDTYPE);
	   }
	   |PROCESS ID OPEN_PAR option_function_parameter_list CLOSE_PAR ASS_EQU OPEN_BPAR empty_statement CLOSE_BPAR
	   {
			$$=new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, $8, NULL, VOIDTYPE);
	   }//add by mdp
       |struct_define_statement//结构体定义语句
	   |FREE OPEN_PAR identifier CLOSE_PAR//yangkai malloc相关
	   {
            $$=new CSyntaxNode(FREE_STA, $3, VOIDTYPE);
	   }
	   |SYSTEM OPEN_PAR str_func_parameter CLOSE_PAR//yangkai 调用可执行文件
	   {
	        $$=new CSyntaxNode(SYSTEM_STA, $3, VOIDTYPE);
	   }
	   |String_Function
	   {
	      $$=$1;
	   }
	   |CHANNEL identifier OPEN_PAR integer CLOSE_PAR //add by mdp
	   {
			$$=new CSyntaxNode(DECLARE_STA, $2, $4, CHANNELRTYPE);
	   }//add by mdp
	   |SEND OPEN_PAR ass_ident COMMA ass_exp CLOSE_PAR
	   {
			CSyntaxNode *t1=new CSyntaxNode(S_AWAIT, $3, $5, VOIDTYPE);
			CSyntaxNode *t2=new CSyntaxNode(S_UNIT_ASSIGN, $3, $5, VOIDTYPE);
			$$=new CSyntaxNode(CHOP_STA, t1, t2, VOIDTYPE);
	   }//add by mdp
	   |PUT OPEN_PAR ass_ident COMMA ass_exp CLOSE_PAR
	   {
			CSyntaxNode *t1=new CSyntaxNode(S_IF_ELSE, $3, $5, VOIDTYPE);
			CSyntaxNode *t2=new CSyntaxNode(S_UNIT_ASSIGN, $3, $5, VOIDTYPE);
			CSyntaxNode *t3=new CSyntaxNode(SKIP_STA, VOIDTYPE);
			$$=new CSyntaxNode(IF_ELSE_STA, t1, t2, t3, VOIDTYPE);
	   }
	   |RECEIVE OPEN_PAR ass_ident COMMA identifier CLOSE_PAR
	   {
			CSyntaxNode *t1=new CSyntaxNode(R_AWAIT, $3, $5, VOIDTYPE);
			CSyntaxNode *t2=new CSyntaxNode(R_UNIT_ASSIGN, $3, $5, VOIDTYPE);
			$$=new CSyntaxNode(CHOP_STA, t1, t2, VOIDTYPE);
	   }//add by mdp
	   |GET OPEN_PAR ass_ident COMMA identifier CLOSE_PAR
	   {
			CSyntaxNode *t1=new CSyntaxNode(R_IF_ELSE, $3, $5, VOIDTYPE);
			CSyntaxNode *t2=new CSyntaxNode(R_UNIT_ASSIGN, $3, $5, VOIDTYPE);
			CSyntaxNode *t3=new CSyntaxNode(SKIP_STA, VOIDTYPE);
			$$=new CSyntaxNode(IF_ELSE_STA, t1, t2, t3, VOIDTYPE);
	   }//add by mdp

	   |function                         {$$=$1;}
//	   |new                              {$$=$1;}    //Annotation-Class   2013-5-6 add by YY[fixed]
	   |EXIST identifier inner_optional_define_identifier COLON OPEN_BPAR statement CLOSE_BPAR
	   {
			CSyntaxNode * p;
//			$6->par_change($2->m_NodeName, LOCAL_IDENT_EXP);
			p=$3;

			while(p!=NULL)
			{
//				$6->par_change(p->GetChild0()->GetNName(), LOCAL_IDENT_EXP);
				p=p->GetChild0();
			}

			$$=new CSyntaxNode(EXIST_STA, $2, $3, $6,VOIDTYPE);
	   }
	   |OPEN_PAR statement CLOSE_PAR    {$$=$2;}
	   |CYLINDER_STATEMENT				{$$=$1;} //2011-08-10 Jane
	   ;
option_struct_declaration
       :sign_declaration
	   {
	      $$=$1;
		}
       |sign_declaration AND option_struct_declaration
		{
		   $$=new CSyntaxNode(AND_EXP,$1,$3,VOIDTYPE);
		}
                         ;

//2011-8-10 Jane begin
CYLINDER_STATEMENT : CYLINDER_STATEMENT CYLINDER CYLINDER_STATEMENT
				 		{$$= new CSyntaxNode(PARALLEL_STA, $1, $3, VOIDTYPE);}
					| OVER_STATEMENT
						{$$=$1;}
					| OPEN_PAR OVER_STATEMENT CLOSE_PAR
						{$$=$2;}
					;//CYLINDER_STATEMENT 的定义和原有的定义一样

OVER_STATEMENT:	big_par OVER OPEN_PAR INTERVAL_STATEMENT CLOSE_PAR
					{$$=new CSyntaxNode(OVER_STA,$1,$4,VOIDTYPE);}
				|big_par OVER OPEN_PAR CLOSE_PAR
					{yyerror("INTERVAL CAN NOT BE EMPTY SET");$$=NULL;}
				; //OVER_STATEMENT 的定义和原有的定义差不多

INTERVAL_STATEMENT:	SIG_INTERVAL
						{$$=$1;}
					| INTERVAL_STATEMENT COMMA INTERVAL_STATEMENT
						{$$=new CSyntaxNode(INTERVAL_CAT, $1, $3, VOIDTYPE);}
					| INTERVAL_STATEMENT INTER_OR INTERVAL_STATEMENT
						{$$=new CSyntaxNode(INTERVAL_OR, $1, $3, VOIDTYPE);}
					|INTERVAL_STATEMENT OVEREP alg_exp
						{$$=new CSyntaxNode(INTERVAL_REP, $1, $3, VOIDTYPE);}
					|INTERVAL_STATEMENT OVEREP NON_DETERMINED
						{$$=new CSyntaxNode(INTERVAL_REP_NDET, $1, VOIDTYPE);}
					|OPEN_PAR INTERVAL_STATEMENT CLOSE_PAR
						{$$=$2;}
					;//主要的改变在时间区间的改变上，以及重复节点的改变上

SIG_INTERVAL:	alg_exp
					{$$=new CSyntaxNode(LENGTH_STA, $1, VOIDTYPE);}
//				| integer
//					{$$=new CSyntaxNode(LENGTH_STA, $1, VOIDTYPE);}
				;


//2011-8-10 Jane end
//syntax for cylinder computing Jane 2011-08-10


empty_statement
	   :statement                       {$$=$1;}
	   |                                {$$=NULL;}
	   ;

//调用谓词和函数时的语法
//annotated by YY 2013-6-6
function
       :ID OPEN_PAR option_function_identifier CLOSE_PAR
	   {
	        //Annotate by YY 2013/11/20 暂时不考虑带成员函数 .的函数调用
			$$=new CSyntaxNode(FUNCTION_STA, $1, $3, NULL, NULL, VOIDTYPE);//构造调用函数的程序语法树子树
	   }
	   |list OPEN_PAR option_function_identifier CLOSE_PAR
	   {
	        //Annotate by YY 2013/11/20 暂时不考虑带成员函数 .的函数调用
			$$=new CSyntaxNode(FUNCTION_STA, $1, $3, NULL, FUNCPTYPE);//通过函数指针数组调用函数，add by yubin 2014-5-26
	   }
	   ;


struct_define_statement
       :struct_identifier OPEN_BPAR option_struct_declaration CLOSE_BPAR
	   {
           CSyntaxNode* temp=$1;
		   string structName=temp->GetNName();
           $$=new CSyntaxNode(STRUCT_DEFINE_STA, structName, $3, VOIDTYPE);
	   }
       ;
struct_identifier
       :STRUCT ID
	   {
/*			if(!g_StructNameMap.insert($2))
			{
			    yyerror("Redefinition of struct");
			}
			else
			{
			    $$=new CSyntaxNode(STRUCT_NAME, $2, STRUCT_NAME_TYPE);
			}*/
	   }
       |UNION  ID
	   {
/*	       if(!g_StructNameMap.insert($2))
			{
			    yyerror("Redefinition of union");
			}
			else
			{
			    $$=new CSyntaxNode(STRUCT_NAME, $2, STRUCT_NAME_TYPE);
			}*/
	   }
	   ;
//处理unsigned和signed的问题，最终的定义语句
sign_declaration
	   :SIGNED declaration   {$$=$2;} //默认为signed
	   |UNSIGNED declaration {$$=new CSyntaxNode(UNSIGN_DECLARATION_STA, $2, VOIDTYPE); }
	   |declaration          {$$=$1;}
	   ;
//add by wm 2013/10/25 22:18
declaration
	   :type_define  option_identifier_array_declaration
	   {
	      $$=new CSyntaxNode(DECLARE_STA, $2, $1);
		}
       |STRUCT_TYPE option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$1,$2,STRUCTTYPE);
		} //结构体声明 yangkai
	   |STRUCT ID option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//结构体声明
       |UNION ID option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//联合体声明
	   |STRUCT STRUCT_TYPE option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//结构体声明
       |UNION STRUCT_TYPE option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//联合体声明
	   |POINTERDECLARATION   option_function_parameter_list {$$=new CSyntaxNode(DECLARE_STA, $2, POINTERTYPE);}  //add by YY 有问题，暂时先不管pointer类型
	   |LISTDECLARATION   option_array_declaration		{$$=new CSyntaxNode(DECLARE_STA, $2, LISTTYPE);}
	    //add by yubin 函数指针
	   |OPEN_PAR MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR	{$$=new CSyntaxNode(DECLARE_STA, $3, FUNCPTYPE);}
	   //add by yangkai 函数指针的指针
	   |OPEN_PAR MUL MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR {$$=new CSyntaxNode(DECLARE_STA, $4, FUNCPPTYPE);}
	   	//add by yubin 函数指针数组
	   |OPEN_PAR MUL ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR
	   {
	       CSyntaxNode *pChild0=new CSyntaxNode(PARAMETER_EXP,new CSyntaxNode(ARRAY_DECLARE_STA, $3, $5, $6, NULL, LISTTYPE), VOIDTYPE);
	       $$=new CSyntaxNode(DECLARE_STA, pChild0, FUNCPTYPE);
	   }
       |OPEN_PAR MUL ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR ASS_EQU list_exp
	   {
	       CSyntaxNode *pChild0_Child0=new CSyntaxNode(ARRAY_DECLARE_STA, $3, $5, $6, NULL, LISTTYPE);
		   CSyntaxNode *pChild0=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, pChild0_Child0, $13, VOIDTYPE), VOIDTYPE);
		   $$=new CSyntaxNode(DECLARE_STA, pChild0, FUNCPTYPE);
		   pChild0=NULL;
		   pChild0_Child0=NULL;
	   }
	   ;


type_define
		:INTDECLARATION					{$$=INTTYPE;}
		|FLOATDECLARATION				{$$=FLOATTYPE;}
		|CHARDECLARATION				{$$=CHARTYPE;}
//		|UNSIGNED INTDECLARATION        {$$=UINTTYPE;}
//	    |UNSIGNED CHARDECLARATION       {$$=UCHARTYPE;}
		|STRDECLARATION					{$$=STRTYPE;}
		|VOIDDECLARATION                {$$=VOIDTYPE;}
		|FILEDECLARATION    			{$$=FILETYPE;}//add by yubin 2014/3/7
		;



//将option_declaration改为option_identifier_array_declaration     2013-4-19 add by YY[fixed]
option_identifier_array_declaration
		:identifier inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, $1, $2, VOIDTYPE);
		}
	    |ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR inner_option_identifier_array_declaration
		{
	       $$=new CSyntaxNode(PARAMETER_EXP,new CSyntaxNode(ARRAY_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE), $6, VOIDTYPE);
		}
	    |identifier ASS_EQU ass_exp inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, $1, $3, VOIDTYPE), $4, VOIDTYPE);
		}
		//add by YY 2013/12/19
		|identifier ASS_EQU string inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, $1, $3, VOIDTYPE), $4, VOIDTYPE);
		}
		//add by YY 2013/12/30 int a[]<==[1,2,3]
		|ID ARRAY ASS_EQU list_exp inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, new CSyntaxNode(ARRAY_DECLARE_NULL_STA, $1, NULL, NULL, NULL, LISTTYPE), $4, VOIDTYPE), $5, VOIDTYPE);
		}

		//add by YY 2013/12/30 char a[]<=="hallo"
		|ID ARRAY ASS_EQU string inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, new CSyntaxNode(ARRAY_DECLARE_NULL_STA, $1, NULL, NULL, NULL, LISTTYPE), $4, VOIDTYPE), $5, VOIDTYPE);
		}
		//add by YY 2014/01/13  int a[3]<==[1,2,3]   char a[2,6]<==["he","she"]
	    |ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR ASS_EQU list_exp inner_option_identifier_array_declaration
		{
	       CSyntaxNode *pChild0=new CSyntaxNode(ARRAY_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE);
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, pChild0, $7, VOIDTYPE), $8, VOIDTYPE);
		   pChild0=NULL;
	    }
	   //add by YY 2013/12/19 a[3]<=="ni";
	   |ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR ASS_EQU string inner_option_identifier_array_declaration
	   {
	       CSyntaxNode *pChild0=new CSyntaxNode(ARRAY_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE);
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, pChild0, $7, VOIDTYPE), $8, VOIDTYPE);
		   pChild0=NULL;
	   }
	   |MUL option_identifier_array_declaration
	   {
	      CSyntaxNode* pChild0=new CSyntaxNode(ADDRESS_DECLARE_STA,$2->GetChild0(),VOIDTYPE);
          $$=$2;
		  $2->SetChild0(pChild0);
		  pChild0=NULL;
	   }
	   ;


inner_option_identifier_array_declaration
	   :COMMA option_identifier_array_declaration
	                  {$$=$2;}
	   |COMMA error   {$$=NULL;}
	   |{$$=NULL;}
	   ;
//将list_option_declaration修改为option_array_declaration
option_array_declaration
	   :ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR inner_option_array_declaration
	   {
	        $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(LIST_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE), $6, VOIDTYPE);
	   }
	   ;
//将list_inner_option_declaration修改为inner_option_array_declaration
inner_option_array_declaration
	   :COMMA ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR inner_option_array_declaration
	   {
			$$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(LIST_DECLARE_STA, $2, $4, $5, NULL, LISTTYPE), $7, VOIDTYPE);
	   }
	   | /* empty */                    {$$=NULL;}
	   |COMMA error                     {$$=NULL;}
	   ;

//变量类型声明 end Jane

//add by yubin 2014/3/3 文件操作
file_statement
	   :FOPEN OPEN_PAR ass_ident COMMA string CLOSE_PAR
										{$$=new CSyntaxNode(FOPEN_EXP, $3, $5, VOIDTYPE);}  //add by yubin 2014/3/4文件打开操作
       |FGETS OPEN_PAR identifier COMMA integer COMMA identifier CLOSE_PAR
										{$$=new CSyntaxNode(FGETS_EXP,$3,$5,$7,VOIDTYPE);}
	   |FGETC OPEN_PAR identifier CLOSE_PAR
										{$$=new CSyntaxNode(FGETC_EXP,$3,VOIDTYPE);}
	   |FPUTS OPEN_PAR identifier COMMA identifier CLOSE_PAR
										{$$=new CSyntaxNode(FPUTS_EXP,$3,$5,VOIDTYPE);}
	   |FPUTC OPEN_PAR identifier COMMA identifier CLOSE_PAR
										{$$=new CSyntaxNode(FPUTC_EXP,$3,$5,VOIDTYPE);}
	   |FPUTC OPEN_PAR list COMMA identifier CLOSE_PAR
										{$$=new CSyntaxNode(FPUTC_EXP,$3,$5,VOIDTYPE);}
	   |FCLOSE OPEN_PAR identifier CLOSE_PAR
										{$$=new CSyntaxNode(FCLOSE_EXP,$3,VOIDTYPE);}
	   ;






//变赋值语句 begin Jane

ass_statement
       :ass_ident assign_operator ass_exp
										{$$=new CSyntaxNode($2, $1, $3, VOIDTYPE);}
	   ;
ass_ident
       :identifier                      {$$=$1;}
	   |struct_member_exp               {$$=$1;}
       |list                            {$$=$1;}   //a[2,3,4]
	   |value_exp                       {$$=$1;}
	   |OPEN_PAR ass_ident CLOSE_PAR    {$$=$2;}
	   |NEXT ass_ident                  {$$=new CSyntaxNode(NEXT_STA, $2, UNKNOWNTYPE);}
	   |leftaddrshift                   {$$=$1;}  //add by yubin 2014-6-6
	   ;
leftaddrshift
	   :OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $6, $9, $10, NULL, CHARTYPE);}   //add by yubin 2014-6-5: ((char*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $7, $10, $11, NULL, UCHARTYPE);}   //add by yubin 2014-6-5: ((unsigned char*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $6, $9, $10, NULL, INTTYPE);}   //add by yubin 2014-6-5: ((int*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR FLOATDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $6, $9, $10, NULL, FLOATTYPE);}   //add by yubin 2014-6-5: ((float*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, CHARTYPE);}   //add by yubin 2014-6-5: ((char*)((char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, CHARTYPE);}   //add by yubin 2014-6-5: ((char*)((unsigned char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, CHARTYPE);}   //add by yubin 2014-6-5: ((char*)((int*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, UCHARTYPE);}   //add by yubin 2014-6-5: ((unsigned char*)((char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $13, $17, $18, NULL, UCHARTYPE);}   //add by yubin 2014-6-5: ((unsigned char*)((unsigned char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, UCHARTYPE);}   //add by yubin 2014-6-5: ((unsigned char*)((int*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, INTTYPE);}   //add by yubin 2014-6-5: ((int*)((char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, INTTYPE);}   //add by yubin 2014-6-5: ((int*)((unsigned char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTEGER CLOSE_PAR CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, INTTYPE);}   //add by yubin 2014-6-5: ((int*)((int*)0x01))[2]=5


assign_operator
	   :ASS_EQU                         {$$=ASS_EQU_EXP;}    // <==
	   |UNITASSIGN                      {$$=UNITASSIGN_EXP;}   // :=
	   ;
ass_exp
	   :alg_exp                         {$$=$1;}
	   |list_exp                        {$$=$1;}   // [2,3,5,9] 或者[1,3]^[2,4]
	   |string                          {$$=$1;}
	   |bool_statement                  {$$=$1;} //add by YY bool表达式
	   |struct_member_exp               {$$=$1;}
	   |file_statement                  {$$=$1;}
	   |OPEN_PAR string CLOSE_PAR OPEN_PAR type_define CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR function {$$=new CSyntaxNode(DLL_CALL_STA, $2, $10, $5);}
	   |OPEN_PAR ass_exp CLOSE_PAR      {$$=$2;}
	   |rightaddrshift                  {$$=$1;}
	   ;
rightaddrshift
       :MUL OPEN_PAR leftaddrshift CLOSE_PAR {$$=$3;}
	   ;
//将option_list_parameter改为option_alg_exp  2013-4-19 add by YY[fixed]

list
       :ID OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR  {$$=new CSyntaxNode(LIST_SYMBOL_STA, $1, $3, $4, NULL, VOIDTYPE);}

	   //add by YY 2014/01/08 增加((unsigned char*)a)[2]='b',可作为左值或者右值
	   //|casted_element OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR {$$=new CSyntaxNode(LIST_SYMBOL_STA, $1, $3, $4, NULL, VOIDTYPE);}

	   | OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $6, $9, $10, NULL, VOIDTYPE);}
	   | OPEN_PAR OPEN_PAR UNSIGNED INTDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $7, $10, $11, NULL, VOIDTYPE);}
	   | OPEN_PAR OPEN_PAR FLOATDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $6, $9, $10, NULL, VOIDTYPE);}
	   | OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $6, $9, $10, NULL, VOIDTYPE);}
	   | OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $7, $10, $11, NULL, VOIDTYPE);}
	   | OPEN_PAR list CLOSE_PAR   {$$=$2;}
	   ;

option_alg_exp
	   :COMMA alg_exp option_alg_exp
	                                    {$$=new CSyntaxNode(LIST_SUBSCRIPT_EXP, $2, $3, VOIDTYPE);}
	   |COMMA error                     {$$=NULL;}
	   | /* empty */                    {$$=NULL;}
	   ;


//实际上是指针（*p）
value_exp
	   :MUL identifier                  {$$=new CSyntaxNode(VALUE_EXP, $2, UNKNOWNTYPE);}    //*x
	   |MUL value_exp 					{$$=new CSyntaxNode(VALUE_EXP, $2, UNKNOWNTYPE);}    //**x
	   |MUL list                              //*a[2]
									    {$$=new CSyntaxNode(VALUE_EXP, $2,  UNKNOWNTYPE);} //add by wm 2013/10/28
	   |OPEN_PAR value_exp CLOSE_PAR OPEN_MPAR alg_exp option_alg_exp CLOSE_MPAR                     //(*a)[2]
									    {$$=new CSyntaxNode(VALUE_LIST_EXP, $2, $5, $6, UNKNOWNTYPE);}
	   |MUL OPEN_PAR all_type_define MUL CLOSE_PAR alg_exp
	   		{$$=new CSyntaxNode(ADDR_VAR, $6, $3);} //add by yubin 2013-12-19: *(char*)0x01=5
	   |MUL OPEN_PAR alg_exp CLOSE_PAR
	        {$$=new CSyntaxNode(VALUE_EXP, $3, UNKNOWNTYPE);}
	   |OPEN_PAR value_exp CLOSE_PAR    {$$=$2;}   //add by yubin 2013-12-20 使得a<==(*p)+
	   //|MUL error                       {$$=NULL;}
       ;

//算术表达式
alg_exp
	   :simple_alg_exp                  {$$=$1;}
	   |simple_alg_exp relation_operator alg_exp {$$=new CSyntaxNode($2, $1, $3, BOOLTYPE);} //算术表达式中可以有bool表达式，如a+(b>c)
	   |alg_exp alg_operator simple_alg_exp    {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}   // +|-
	   |IF bool_statement THEN  alg_exp optional_exp_else_statement
	   {
            $$ = new CSyntaxNode(IF_ELSE_EXP, $2, $4, $5, ARITHMETICTYPE);
       }
	   ;
type_cast_alg_exp
       :simple_alg_exp                  {$$=$1;}
	   |simple_alg_exp alg_operator simple_alg_exp    {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}   // +|-
	   ;

optional_exp_else_statement
       : ELSE alg_exp					                   {$$=$2;}
       | /* empty */                                       {$$=NULL;}
       ;

simple_alg_exp
	   :factor   {$$=$1;}
	   |simple_alg_exp mul_operator factor  {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}
       |String_Function                 {$$=$1;}
	   ;

factor
       :integer                         {$$=$1;}
	   |SUB factor						{$$=new CSyntaxNode(MINUS_EXP, $2, ARITHMETICTYPE);}
	   |BNE factor                      {$$=new CSyntaxNode(BNE_EXP, $2, ARITHMETICTYPE);}
	   |POINTERNULL                     {$$=new CSyntaxNode(NULL_EXP, VOIDTYPE);}
	   |floatliteral					{$$=$1;}    //add by Jane 2012-06-11
	   //|ID                            {$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE);}    //[fixed]  2013-4-22 add by YY
       |identifier						{$$=$1;}
	   |list                            {$$=$1;}
	   |pre_next						{$$=$1;}
	   |value_exp                       {$$=$1;}
	   |function                        {$$=$1;}
	   |OPEN_PAR alg_exp CLOSE_PAR      {$$=$2;}
	   |Float_Function                  {$$=$1}
	   |charliteral						{$$=$1;}
	   |address_exp						{$$=$1;} //add by YY 2014/01/06 初始化指针数组用到
	   |type_cast                       {$$=$1;} //add by YY 2014/01/15 强制转换可以参与算数运算
	   |struct_member_exp               {$$=$1;} //add by YY 2014/01/17 结构体串联
	   |size_of							{$$=$1;}
	   ;

//problem [already fixed] 2013-4-18 add by YY    注释原因：算术表达式中，应该不能出现next

pre_next
	   :PREVIOUS OPEN_PAR identifier CLOSE_PAR            {$$=new CSyntaxNode(PREVIOUS_STA, $3, UNKNOWNTYPE);}
//	   |NEXT identifier                 {$$=new CSyntaxNode(NEXT_STA, $2, UNKNOWNTYPE);}
//	   |PREVIOUS pre_next				{$$=new CSyntaxNode(PREVIOUS_STA, $2, UNKNOWNTYPE);}
//	   |NEXT pre_next					{$$=new CSyntaxNode(NEXT_STA, $2, UNKNOWNTYPE);}
	   ;

mul_operator
	   :MUL								{$$=MUL_EXP;}
	   |DIV								{$$=DIV_EXP;}
	   |MOD								{$$=MOD_EXP;}
	   |LST						     	{$$=LST_EXP;}//add by yubin 2013-12-19 左移 left shift
	   |RST	 						    {$$=RST_EXP;}//add by yubin 2013-12-19 右移 right shift
	   |ADDRESS 	 					{$$=BAN_EXP;}//add by yubin 2013-12-19 按位与
	   |INTER_OR	 					{$$=BOR_EXP;}//add by yubin 2013-12-19 按位或
	   |CON	 		         			{$$=XOR_EXP;}//add by yubin 2013-12-19 异或
	   ;
alg_operator
	   :ADD								{$$=ADD_EXP;}
	   |SUB								{$$=SUB_EXP;}
	   ;

list_exp
	   :list_exp CON list_exp           {$$=new CSyntaxNode(CON_EXP, $1, $3, LISTTYPE);}
	   |option_list_value               {$$=$1;}   // [2,3,5,9]
	   |option_struct_list_value         {$$=$1;}   // {1,'C',"HELLO"}
	   |OPEN_PAR list_exp CLOSE_PAR     {$$=$2;}
	   ;


//可能是对于数组赋值[1,2,4]的形式 Jane  【int float str char list】
option_list_value
	   :OPEN_MPAR alg_exp inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
		}
	   |OPEN_MPAR string inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   |OPEN_MPAR charliteral inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   |OPEN_MPAR list_exp inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   /*|OPEN_MPAR CLOSE_MPAR / * empty * /
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, LISTTYPE);
   	   }*/
	   ;

//add by YY 结构体的初始化列表{{1,"hello"},{2,"hi"}}
option_struct_list_value
	   :OPEN_BPAR alg_exp inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
		}
	   |OPEN_BPAR string inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   |OPEN_BPAR charliteral inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   |OPEN_BPAR list_exp inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   /*|OPEN_BPAR CLOSE_BPAR / * empty * /
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, LISTTYPE);
   	   }*/
	   ;

//此处含有STR_EXP,CHARLITERAL_EXP的不同含义定义 返回值类型为LISTTYPE  2013-3-12 add by YY
inner_option_list_value
	   :COMMA alg_exp inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);//INTEGER_EXP Jane
	   }
	   |COMMA string inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   |COMMA charliteral inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   |COMMA list_exp inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   | /* emtpy */									   {$$=NULL;}
	   |COMMA error                                        {$$=NULL;}
	   ;

//[need fixed]   2013-4-22 add by YY [need fixed]
address_exp:
            ADDRESS identifier
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);
			}
            |ADDRESS list
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);
			}
            |ADDRESS struct_member_exp
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE );
			}
			//add by YY 2013/11/28 多重地址引用暂时不考虑
		    |ADDRESS address_exp
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);
			}
			|OPEN_PAR address_exp CLOSE_PAR  {$$=$2;}   //add by YY 2014/01/17
			;

//rewrited by Jane


//加入强制类型转换语句 begin Jane
type_cast:
			//add by YY 2014/01/07 10:57 强制转换 (int) (float) (char)
		      OPEN_PAR INTDECLARATION CLOSE_PAR type_cast_alg_exp     {$$=new CSyntaxNode(TYPE_CAST_STA, $4, INTTYPE);}      //char->int   float->int  [fixed] 2013-4-22 add by YY
			| OPEN_PAR FLOATDECLARATION CLOSE_PAR type_cast_alg_exp {$$=new CSyntaxNode(TYPE_CAST_STA, $4, FLOATTYPE);}    //int->float   //char->float [fixed] 2013-6-4 add by YY
			| OPEN_PAR CHARDECLARATION CLOSE_PAR type_cast_alg_exp   {$$=new CSyntaxNode(TYPE_CAST_STA, $4, CHARTYPE);}     //int->char float->char

			//add by YY 2014/01/07 10:57 强制转换 (signed int) (signed char)
			| OPEN_PAR SIGNED INTDECLARATION CLOSE_PAR type_cast_alg_exp     {$$=new CSyntaxNode(TYPE_CAST_STA, $5, INTTYPE);}      //char->int   float->int  [fixed] 2013-4-22 add by YY
			| OPEN_PAR SIGNED CHARDECLARATION CLOSE_PAR type_cast_alg_exp   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, CHARTYPE);}     //int->char float->char

			//add by YY 2014/01/07 10:57 强制转换 (unsigned int) (unsigned char)
			| OPEN_PAR UNSIGNED INTDECLARATION CLOSE_PAR type_cast_alg_exp     {$$=new CSyntaxNode(TYPE_CAST_STA, $5, UINTTYPE);}      //char->int   float->int  [fixed] 2013-4-22 add by YY
			| OPEN_PAR UNSIGNED CHARDECLARATION CLOSE_PAR type_cast_alg_exp   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, UCHARTYPE);}     //int->char float->char

			//add by YY 2014/01/07 10:57 强制转换 (int*) (unsigned int*) (float*) (char*) (unsigned char*)
			| OPEN_PAR INTDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, INTPTYPE);}
			| OPEN_PAR UNSIGNED INTDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $6, UINTPTYPE);}
			| OPEN_PAR FLOATDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, FLOATPTYPE);}
			| OPEN_PAR CHARDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, CHARPTYPE);}
			| OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $6, UCHARPTYPE);}
			| OPEN_PAR VOIDDECLARATION MUL CLOSE_PAR casted_element {$$=new CSyntaxNode(TYPE_CAST_STA, $5, VOIDPTYPE);}

			//二重指针强制类型转换
			| OPEN_PAR INTDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, DOUBLEINTPTYPE);}
			| OPEN_PAR UNSIGNED INTDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $7, DOUBLEUINTPTYPE);}
			| OPEN_PAR FLOATDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, DOUBLEFLOATPTYPE);}
			| OPEN_PAR CHARDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, DOUBLECHARPTYPE);}
			| OPEN_PAR UNSIGNED CHARDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $7, DOUBLEUCHARPTYPE);}
			| OPEN_PAR VOIDDECLARATION MUL MUL CLOSE_PAR casted_element {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, VOIDPTYPE);}
			//yangkai malloc相关
			| OPEN_PAR STRUCT STRUCT_TYPE MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $3, $6, STRUCTPTYPE);}
			| OPEN_PAR STRUCT_TYPE MUL CLOSE_PAR casted_element  {$$=new CSyntaxNode(TYPE_CAST_STA, $2, $5, STRUCTPTYPE);}
            //结构体类型二重指针转换
			| OPEN_PAR STRUCT STRUCT_TYPE MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $3, $7, DOUBLESTRUCTPTYPE);}
			| OPEN_PAR STRUCT_TYPE MUL MUL CLOSE_PAR casted_element  {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $2, $6, DOUBLESTRUCTPTYPE);}
			;
casted_element:	type_cast_alg_exp	{$$=$1;}   //yangkai malloc相关
			  | MALLOC OPEN_PAR alg_exp CLOSE_PAR {$$=new CSyntaxNode(MALLOC_STA, $3, VOIDTYPE);}
			  ;
//加入强制类型转换语句 end Jane

all_sizeof_type:
			all_type_define{$$=$1;}
		   |INTDECLARATION MUL {$$=INTPTYPE;}
		   |FLOATDECLARATION MUL {$$=FLOATPTYPE;}
		   |CHARDECLARATION MUL {$$=CHARPTYPE;}
		   |UNSIGNED INTDECLARATION MUL {$$=UINTPTYPE;}
		   |UNSIGNED CHARDECLARATION MUL {$$=UCHARPTYPE;}
		   |STRUCT STRUCT_TYPE MUL {$$=STRUCTPTYPE;}
		   |STRUCT_TYPE MUL{$$=STRUCTPTYPE;}
		   //|STRUCT_TYPE {$$=$1;}
		   ;

size_of :
           SIZEOF OPEN_PAR alg_exp CLOSE_PAR   {$$=new CSyntaxNode(SIZEOF_EXP, $3, VOIDTYPE);}
		   |SIZEOF OPEN_PAR all_sizeof_type CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $3);}
		   |SIZEOF OPEN_PAR STRUCT_TYPE CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $3, STRUCTTYPE);}
		   |SIZEOF OPEN_PAR string CLOSE_PAR {$$=new CSyntaxNode(SIZEOF_EXP, $3, STRTYPE);}
		   ;

//加入对字符串操作的函数 begin Jane

String_Function:
                 String_Function_head   {$$=$1;}
			   | String_Function_tail	{$$=$1;}
			   | String_Function_cat	{$$=$1;}
			   | String_Function_cpy    {$$=$1;}
			   | String_Function_cmp    {$$=$1;}
			   | String_Function_len	{$$=$1;}
			   ;

String_Function_head:
                    STRFUNCHEAD OPEN_PAR str_func_parameter CLOSE_PAR
					{
					   $$=new CSyntaxNode(STRFUNCHEAD_EXP, $3, STRTYPE);
					}
					;

String_Function_tail:
				    STRFUNCTAIL OPEN_PAR str_func_parameter CLOSE_PAR
					{
					   $$=new CSyntaxNode(STRFUNCTAIL_EXP, $3, STRTYPE);
					}
					;

String_Function_cat:
					STRFUNCCAT OPEN_PAR str_func_parameter COMMA str_func_parameter CLOSE_PAR
					{
					   $$=new CSyntaxNode(STRFUNCCAT_EXP, $3, $5, STRTYPE);
					}
					;

String_Function_cmp:
					STRFUNCCMP OPEN_PAR str_func_parameter COMMA str_func_parameter CLOSE_PAR
					{
					   $$=new CSyntaxNode(STRFUNCCMP_EXP, $3, $5, STRTYPE);
					}
					;

String_Function_cpy:
					STRFUNCCPY OPEN_PAR str_func_parameter COMMA str_func_parameter CLOSE_PAR
					{
					   $$=new CSyntaxNode(STRFUNCCPY_EXP, $3, $5, STRTYPE);
					}
					;

String_Function_len:
					STRFUNCLEN OPEN_PAR str_func_parameter CLOSE_PAR
					{
						$$=new CSyntaxNode(STRFUNCLEN_EXP, $3, STRTYPE);
					}
					;


str_func_parameter:  identifier					{$$=$1;}
					|list						{$$=$1;}
					|value_exp					{$$=$1;}
					|string						{$$=$1;}
					|String_Function			{$$=$1;}
					|struct_member_exp          {$$=$1;}
					;

//加入对字符串操作的函数 end Jane

//add by YY math.h库中的函数
Float_Function:
				 ROUND alg_exp   {$$=new CSyntaxNode(ROUND_EXP, $2, FLOATTYPE);}
				|CEIL alg_exp     {$$=new CSyntaxNode(CEIL_EXP, $2, FLOATTYPE);}
				|FLOOR alg_exp    {$$=new CSyntaxNode(FLOOR_EXP, $2, FLOATTYPE);}
				//add by YY 2013/11/22 math.h  part added
				|SIN alg_exp      {$$=new CSyntaxNode(SIN_EXP, $2, FLOATTYPE);}
				|COS alg_exp      {$$=new CSyntaxNode(COS_EXP, $2, FLOATTYPE);}
				|TAN alg_exp      {$$=new CSyntaxNode(TAN_EXP, $2, FLOATTYPE);}
				|ASIN alg_exp     {$$=new CSyntaxNode(ASIN_EXP, $2, FLOATTYPE);}
				|ACOS alg_exp     {$$=new CSyntaxNode(ACOS_EXP, $2, FLOATTYPE);}
				|ATAN alg_exp     {$$=new CSyntaxNode(ATAN_EXP, $2, FLOATTYPE);}
				|SINH alg_exp     {$$=new CSyntaxNode(SINH_EXP, $2, FLOATTYPE);}
				|COSH alg_exp     {$$=new CSyntaxNode(COSH_EXP, $2, FLOATTYPE);}
				|TANH alg_exp     {$$=new CSyntaxNode(TANH_EXP, $2, FLOATTYPE);}
				|EXP alg_exp      {$$=new CSyntaxNode(EXP_EXP, $2, FLOATTYPE);}
				|LOG alg_exp      {$$=new CSyntaxNode(LOG_EXP, $2, FLOATTYPE);}
				|LOG10 alg_exp    {$$=new CSyntaxNode(LOG10_EXP, $2, FLOATTYPE);}
				|SQRT alg_exp     {$$=new CSyntaxNode(SQRT_EXP, $2, FLOATTYPE);}

				|ATAN2 OPEN_PAR alg_exp COMMA alg_exp CLOSE_PAR {$$=new CSyntaxNode(ATAN2_EXP, $3, $5, FLOATTYPE);}
				|POW OPEN_PAR alg_exp COMMA alg_exp CLOSE_PAR   {$$=new CSyntaxNode(POW_EXP, $3, $5, FLOATTYPE);}
				|FMOD OPEN_PAR alg_exp COMMA alg_exp CLOSE_PAR   {$$=new CSyntaxNode(FMOD_EXP, $3, $5, FLOATTYPE);}
				|MODF OPEN_PAR alg_exp COMMA alg_exp CLOSE_PAR   {$$=new CSyntaxNode(MODF_EXP, $3, $5, FLOATTYPE);}  //add by YY 未完全实现
				|LDEXP OPEN_PAR alg_exp COMMA alg_exp CLOSE_PAR   {$$=new CSyntaxNode(LDEXP_EXP, $3, $5, FLOATTYPE);}

				|ABS alg_exp     {$$=new CSyntaxNode(ABS_EXP, $2, INTTYPE);}
				|FABS alg_exp     {$$=new CSyntaxNode(FABS_EXP, $2, INTTYPE);}
				|LABS alg_exp     {$$=new CSyntaxNode(LABS_EXP, $2, INTTYPE);}
				;



bool_statement
	   :prime_bool_statement            {$$=$1;}
//	   |alg_exp EQU alg_exp         {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}//add by YY[fixed]   2013-4-22 删除原因：因为relation_operator中已经添加了相关的操作符


	   |alg_exp EQU string          {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}
	   |alg_exp NE string			{$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);}

	   //add by YY 2013/12/19
	   |string EQU alg_exp            {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}
	   |string NE alg_exp   			{$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);}

	   //add by YY 2013/12/19
	   |alg_exp EQU POINTERNULL          {$$=new CSyntaxNode(EQU_EXP, $1, new CSyntaxNode(NULL_EXP, VOIDTYPE), BOOLTYPE);}
	   |alg_exp NE POINTERNULL			{$$=new CSyntaxNode(NE_EXP, $1, new CSyntaxNode(NULL_EXP, VOIDTYPE), BOOLTYPE);}

	    //add by YY
	   |POINTERNULL EQU alg_exp           {$$=new CSyntaxNode(EQU_EXP, new CSyntaxNode(NULL_EXP, VOIDTYPE), $3, BOOLTYPE);}
	   |POINTERNULL NE alg_exp  		  {$$=new CSyntaxNode(NE_EXP, new CSyntaxNode(NULL_EXP, VOIDTYPE), $3, BOOLTYPE);}


	   |alg_exp EQU charliteral     {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}
	   |alg_exp NE  charliteral     {$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);}

	   |NEGATION bool_statement         {$$=new CSyntaxNode(NEGATION_EXP, $2, BOOLTYPE);}
	   |bool_statement AND bool_statement    {$$=new CSyntaxNode(AND_EXP, $1, $3, BOOLTYPE);}
	   |bool_statement OR bool_statement     {$$=new CSyntaxNode(OR_EXP, $1, $3, BOOLTYPE);}
	   |OPEN_PAR bool_statement CLOSE_PAR    {$$=$2;}

	   ;


prime_bool_statement
       :alg_exp relation_operator alg_exp {$$=new CSyntaxNode($2, $1, $3, BOOLTYPE);}
	   |NEGATION prime_bool_statement   {$$=new CSyntaxNode(NEGATION_EXP, $2, BOOLTYPE);}
	   |MY_TRUE                         {$$=new CSyntaxNode(TRUE_EXP, BOOLTYPE);}
	   |MY_FALSE                        {$$=new CSyntaxNode(FALSE_EXP, BOOLTYPE);}
	   |MORE                            {$$=new CSyntaxNode(MORE_STA, BOOLTYPE);}
	   |EMPTY                           {$$=new CSyntaxNode(EMPTY_EXP, BOOLTYPE);}
	   |alg_exp                         {$$=$1;}
	   |String_Function_cmp				{$$=$1;}//added by Jane 2012-06-21
	   |file_statement			     	{$$=$1;}//add by yubin 2014/3/6 文件相关函数可以作为布尔表达式
	   ;


relation_operator
	   :GE                              {$$=GE_EXP;}
	   |LE                              {$$=LE_EXP;}
	   |NE                              {$$=NE_EXP;}
	   |GT                              {$$=GT_EXP;}
	   |LT                              {$$=LT_EXP;}

	   //add by YY 2013-6-6
	   |EQU                             {$$=EQU_EXP;}     //[add fixed]
	   ;


//添加类型需要改动的地方 end Jane

//add by YY 2014/01/09
sign_type_define:
	    UNSIGNED INTDECLARATION					{$$=UINTTYPE;}
	   |UNSIGNED CHARDECLARATION				{$$=UCHARTYPE;}
	   ;
//add by YY 2014/01/09 用于函数形参列表option_function_parameter_list，使得可以传递unsigned修饰参数
all_type_define:
		sign_type_define  {$$=$1;}
	   |type_define       {$$=$1;}
	   ;


//定义谓词和函数时使用的形参列表
option_function_parameter_list
	   :all_type_define identifier inner_optional_define_identifier
	   {
			$$ = new CSyntaxNode(PARAMETER_EXP, $2, $3, $1);
	   }
	   |all_type_define inner_optional_define_identifier //add by yubin 2014/2/24,函数指针的形参列表可以没有参数名字
	   {
			$$=NULL;
	   }

	   |STRUCT_TYPE identifier inner_optional_define_identifier //结构体 S s
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $1, $2, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $3, STRUCTTYPE);
			pChild0=NULL;
		}
       |STRUCT_TYPE MUL identifier inner_optional_define_identifier//结构体指针类型的参数S *s
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(STRUCTP_PARAMETER_EXP, $1, $3, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTPTYPE);
            pChild0=NULL;
	   }
       |STRUCT_TYPE MUL MUL identifier inner_optional_define_identifier//结构体指针类型的参数S **s //二重结构体指针
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(DOUBLESTRUCTP_PARAMETER_EXP, $1, $4, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTPTYPE);
            pChild0=NULL;
	   }
	   |STRUCT_TYPE identifier ARRAY inner_optional_define_identifier //结构体数组 S a[]
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
            pChild0=NULL;
		}



	   |ID identifier inner_optional_define_identifier //结构体 S s
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $1, $2, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $3, STRUCTTYPE);
			pChild0=NULL;
		}
	   |ID MUL identifier inner_optional_define_identifier//结构体指针类型的参数S *s
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(STRUCTP_PARAMETER_EXP, $1, $3, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTPTYPE);
            pChild0=NULL;
	   }
       |ID MUL MUL identifier inner_optional_define_identifier//结构体指针类型的参数S **s //二重结构体指针
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(DOUBLESTRUCTP_PARAMETER_EXP, $1, $4, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTPTYPE);
            pChild0=NULL;
	   }
	   |ID identifier ARRAY inner_optional_define_identifier //结构体数组 S a[]
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
            pChild0=NULL;
		}


	   |STRUCT_TYPE  inner_optional_define_identifier //结构体 S s
	   {
	        $$=NULL;
		}
       |STRUCT_TYPE MUL  inner_optional_define_identifier//结构体指针类型的参数S *s
	   {
	        $$=NULL;
	   }
       |STRUCT_TYPE MUL MUL  inner_optional_define_identifier//结构体指针类型的参数S **s //二重结构体指针
	   {
			$$=NULL;
	   }


	   |ID  inner_optional_define_identifier //结构体 S s
	   {
	        $$=NULL;
		}
       |ID MUL  inner_optional_define_identifier//结构体指针类型的参数S *s
	   {
	        $$=NULL;
	   }
       |ID MUL MUL  inner_optional_define_identifier//结构体指针类型的参数S **s //二重结构体指针
	   {
			$$=NULL;
	   }


	   |all_type_define identifier ARRAY inner_optional_define_identifier//数组类型的参数int a[]
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $4, $1);
            pChild0=NULL;
	   }
	   |all_type_define MUL identifier ARRAY inner_optional_define_identifier //指针数组int *a[]
	   {
            CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $1);
            pChild0=NULL;
	   }
	   //add by YY 2014/01/14																		//数组类型的参数a[4]
	   |all_type_define identifier OPEN_MPAR alg_exp CLOSE_MPAR inner_optional_define_identifier

	   {
			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $6, $1);
            pChild0=NULL;
	   }
	   //add by YY 2014/01/14																		//数组类型的参数a[3,2]
	   |all_type_define identifier OPEN_MPAR alg_exp COMMA alg_exp CLOSE_MPAR inner_optional_define_identifier

	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $8, $1);
            pChild0=NULL;
	   }
	   //add by YY 2014/01/14																		//数组类型的参数a[][100]
	   |all_type_define identifier ARRAY OPEN_MPAR alg_exp CLOSE_MPAR inner_optional_define_identifier
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $7, $1);
            pChild0=NULL;
	   }
	   |all_type_define MUL identifier inner_optional_define_identifier//指针类型的参数*p
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(POINT_PARAMETER_EXP, $3, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, $1);
          pChild0=NULL;
		}
	   |all_type_define MUL inner_optional_define_identifier//指针类型的参数*p
	   {
	      $$=NULL;
		}
	   |all_type_define MUL MUL identifier inner_optional_define_identifier//指针类型的参数**p
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(DOUBLEPOINT_PARAMETER_EXP, $4, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $1);
          pChild0=NULL;
		}
		//add by yubin 2013-12-27 函数的参数可以是函数指针
	   |OPEN_PAR MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR inner_optional_define_identifier//函数指针类型的参数
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(FUNCP_PARAMETER_EXP, $3, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $8, FUNCPTYPE);
          pChild0=NULL;
		}
	   | /* empty */                                       {$$ = NULL;}
	   ;
inner_optional_define_identifier
       : COMMA all_type_define identifier inner_optional_define_identifier
	    {
		   $$ = new CSyntaxNode(PARAMETER_EXP, $3, $4, $2);
		}
		|COMMA all_type_define inner_optional_define_identifier  //add by yubin 2014/2/24，函数指针的形参列表可以没有参数名字
	    {
		   $$ = NULL;
		}
	   |COMMA ID identifier inner_optional_define_identifier
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $2, $3, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
			pChild0=NULL;
	   }
	   |COMMA  STRUCT_TYPE identifier inner_optional_define_identifier
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $2, $3, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
			pChild0=NULL;
	   }
	   |COMMA STRUCT_TYPE MUL identifier inner_optional_define_identifier//结构体指针类型的参数
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(STRUCTP_PARAMETER_EXP, $2, $4, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTPTYPE);
            pChild0=NULL;
	   }
	   |COMMA ID identifier ARRAY inner_optional_define_identifier //结构体数组 S a[]
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTTYPE);
            pChild0=NULL;
		}
	   |COMMA STRUCT_TYPE identifier ARRAY inner_optional_define_identifier //结构体数组 S a[]
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTTYPE);
            pChild0=NULL;
		}

	   |COMMA all_type_define identifier ARRAY inner_optional_define_identifier
	   {
		    CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
		    $$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $2);
            pChild0=NULL;
		}
		|COMMA all_type_define MUL identifier ARRAY inner_optional_define_identifier  //指针数组 int *a[]
	    {
		    CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $4, VOIDTYPE);
		    $$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $6, $2);
            pChild0=NULL;
		}
	   |COMMA all_type_define MUL identifier inner_optional_define_identifier
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(POINT_PARAMETER_EXP,$4,VOIDTYPE);

	      $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $2);

		  pChild0=NULL;
		}
		//add by yubin 2013-12-27 函数的参数可以是函数指针
	   |COMMA OPEN_PAR MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR inner_optional_define_identifier//函数指针类型的参数
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(FUNCP_PARAMETER_EXP, $4, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $9, FUNCPTYPE);
          pChild0=NULL;
		}
	   | /* empty */                                       {$$ = NULL;}

	   //add by YY 2014/01/14																		//数组类型的参数a[4]
	   |COMMA all_type_define identifier OPEN_MPAR alg_exp CLOSE_MPAR inner_optional_define_identifier

	   {
			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $7, $2);
            pChild0=NULL;
	   }
	   //add by YY 2014/01/14																		//数组类型的参数a[3,2]
	   |COMMA all_type_define identifier OPEN_MPAR alg_exp COMMA alg_exp CLOSE_MPAR inner_optional_define_identifier

	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $9, $2);
            pChild0=NULL;
	   }
	   //add by YY 2014/01/14																		//数组类型的参数a[][100]
	   |COMMA all_type_define identifier ARRAY OPEN_MPAR alg_exp CLOSE_MPAR inner_optional_define_identifier
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $8, $2);
            pChild0=NULL;
	   }
	   |COMMA all_type_define MUL identifier inner_optional_define_identifier//指针类型的参数*p
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(POINT_PARAMETER_EXP, $4, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $2);
          pChild0=NULL;
		}
	   |COMMA all_type_define MUL inner_optional_define_identifier//指针类型的参数*p
	   {
	      $$=NULL;
		}
		|COMMA identifier inner_optional_define_identifier//2015-3-16,add by yubin,用于使用存在量词时，exist x,y
	    {
		   $$ = new CSyntaxNode(PARAMETER_EXP, $2, $3,VOIDTYPE);
		}
	   |COMMA error                                        {$$=NULL;}
	   ;

//定义谓词和函数时使用的形参列表 end



//调用函数和谓词的实参列表，加入字符串 begin
option_function_identifier
       :ass_exp inner_option_function_identifier
	   {
			$$ = new CSyntaxNode(PARAMETER_EXP, $1, $2, VOIDTYPE);
	   }
	   |ID OPEN_PAR option_function_identifier CLOSE_PAR  inner_option_function_identifier
	   {
	        CSyntaxNode* child0=new CSyntaxNode(FUNCTION_STA, $1, $3, NULL, NULL, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, child0, $5, VOIDTYPE);
			child0=NULL;
	   }
       //|ADDRESS identifier inner_option_function_identifier
	   //{
	   //		$$ = new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE), $3, VOIDTYPE);
	   //}
	   | /* empty */                                       {$$=NULL;}
	   |ADDRESS error                                      {$$=NULL;}
	   ;

inner_option_function_identifier
       :COMMA ass_exp inner_option_function_identifier
	   {
			$$ = new CSyntaxNode(PARAMETER_EXP, $2, $3, VOIDTYPE);
	   }
	   |COMMA ID OPEN_PAR option_function_identifier CLOSE_PAR  inner_option_function_identifier
	   {
	        CSyntaxNode* child0=new CSyntaxNode(FUNCTION_STA, $2, $4, NULL, NULL, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, child0, $6, VOIDTYPE);
			child0=NULL;
	   }
	   //|COMMA ADDRESS identifier inner_option_function_identifier
	   //{
	   //		$$ = new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ADDRESS_EXP, $3, VOIDTYPE), $4, VOIDTYPE);
	   //}
	   | /* empty */                                       {$$=NULL;}
	   |COMMA error                                        {$$=NULL;}
	   ;

//调用函数和谓词的实参列表，加入字符串 end



big_par
       :OPEN_BPAR statement CLOSE_BPAR					   {$$=$2;}
//	   |OPEN_BPAR statement				{yyerror("expecting '}'"); $$=NULL;}
	   |error CLOSE_BPAR                                   {$$=NULL;}
//	   |OPEN_BPAR CLOSE_BPAR            {yyerror("expecting statement"); $$=NULL;}
	   ;



bool_par
       :OPEN_PAR bool_statement CLOSE_PAR				   {$$=$2;}
//	   |OPEN_PAR bool_statement			{yyerror("expecting ')'"); $$=NULL;}
	   |error CLOSE_PAR									   {$$=NULL;}
//	   |OPEN_PAR CLOSE_PAR              {yyerror("expecting statement"); $$=NULL;}
	   ;


identifier
       :ID                              {$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE);}
	   |OPEN_PAR identifier CLOSE_PAR   {$$=$2;}
       ;

string
       : STR                            {$$=new CSyntaxNode(STR_EXP, $1, STRTYPE);}
       ;


integer
       : INTEGER                        {$$=new CSyntaxNode(INTEGER_EXP, $1, INTTYPE);}
       ;


charliteral
			:CHARLITERAL					{$$=new CSyntaxNode(CHARLITERAL_EXP, $1, CHARTYPE);}
			;


floatliteral
			:FLOATLITERAL					{$$=new CSyntaxNode(FLOATLITERAL_EXP, $1, FLOATTYPE);}
			;



optional_frame_identifier
       : COMMA identifier optional_frame_identifier
	    {
			$$ = new CSyntaxNode( FRAME_STA, $2, $3, VOIDTYPE);
		}
	   | /* empty */                                       {$$ = NULL;}
	   |COMMA error                                        {$$=NULL;}
	   ;



optional_projection
	   :                                                   {$$=NULL;}
	   |COMMA statement optional_projection
	   {
			$$ = new CSyntaxNode( PROJECTION_STA, $2, $3, VOIDTYPE);
	   }
	   ;





imply_pre
	   :
	   //2013-6-6 add by YY[fixed]
	   /*
	   ass_statement
	   {
			if($1->GetNType()==ASS_EQU_EXP)
				$1->SetRType(BOOLTYPE);
			$$=$1;
	   }
	   */
	   prime_bool_statement            {$$=$1;}
	   |NEGATION imply_pre              {$$=new CSyntaxNode(NEGATION_EXP, $2, BOOLTYPE);}
	   |imply_pre AND imply_pre
	                                    {$$=new CSyntaxNode(AND_EXP, $1, $3, BOOLTYPE);}
	   |imply_pre OR imply_pre
	                                    {$$=new CSyntaxNode(OR_EXP, $1, $3, BOOLTYPE);}
	   |OPEN_PAR imply_pre CLOSE_PAR    {$$=$2;}
	   ;



while_statement
       :WHILE bool_par big_par			{$$=new CSyntaxNode(WHILE_DO_STA, $2, $3, VOIDTYPE);}
	   ;

//add by yubin 2015-3-23
switch_statement
       :SWITCH OPEN_PAR factor CLOSE_PAR OPEN_BPAR case_par CLOSE_BPAR 
	   {$$=new CSyntaxNode(SWITCH_STA, $3, $6,VOIDTYPE);}
	   ;   
case_par
       :CASE factor COLON OPEN_PAR statement CLOSE_PAR init_case_par 
	   {
	     $$=new CSyntaxNode(CASE_STA, $2, $5, $7, VOIDTYPE);
	   }
	   |DEFAULT COLON OPEN_PAR statement CLOSE_PAR init_case_par
	   {
	     $$=new CSyntaxNode(DEFAULT_STA, $4, $6, VOIDTYPE);
	   }
	//   |  {$$=NULL;}
       ;
init_case_par
       :BREAK case_par
	   {$$=new CSyntaxNode(INIT_CASE_STA, $2, BREAKTYPE);}
	   |case_par    {$$=new CSyntaxNode(INIT_CASE_STA, $1, VOIDTYPE);}//modified by yubin
	   |    {$$=NULL;}
	   ;


for_statement
       :FOR alg_exp TIMES DO big_par
	                                    {$$=new CSyntaxNode(FOR_TIMES_STA, $2, $5, VOIDTYPE);}
//	   |FOR alg_exp DO big_par          {yyerror("expecting TIMES"); $$=NULL;}
//	   |FOR alg_exp TIMES big_par       {yyerror("expecting DO"); $$=NULL;}
//	   |FOR alg_exp big_par             {yyerror("expecting TIMES DO"); $$=NULL;}
	   ;

extend_for_statement
       :FOR OPEN_PAR for_sta_init CHOP for_sta_condition CHOP for_sta_control  CLOSE_PAR  big_par
	   {
	     CSyntaxNode* pchild0=new CSyntaxNode(FOR_STA_CON, $3, $5, $7, VOIDTYPE);
		 $$=new CSyntaxNode(FOR_STA, pchild0, $9, VOIDTYPE);
		 pchild0=NULL;
	   }
       ;
for_sta_init//for语句括号中的初始化语句,可以是赋值语句，也可以是声明语句,也可以不写
       :ass_statement
	   {
	      $$=$1;
	   }
	   |declaration
	   {
	      $$=$1;
	   }
	   |/*empty*/
	   {
	      $$=NULL;
	   }
       ;
for_sta_condition//for语句括号中的条件语句，是一个布尔表达式也可以不写
      :pbool_expression
	  {
	      $$=$1;
	  }
	  |/*empty*/
	  {
	      $$=NULL;
	  }
	  ;
for_sta_control//for语句括号中的控制语句，是一个赋值语句也可以不写
      :ass_statement
	  {
	     $$=$1;
	  }
	  |/*empty*/
	  {
	      $$=NULL;
	  }
	  ;
if_statement
       : IF bool_statement THEN  big_par optional_else_statement
        {
            $$ = new CSyntaxNode( IF_ELSE_STA, $2, $4, $5, VOIDTYPE);
        }
//	   | IF bool_par sma_par optional_else_statement  {yyerror("Expecting THEN");$$=NULL;}
	   ;



optional_else_statement
       : ELSE big_par					                   {$$=$2;}
       | /* empty */                                       {$$=NULL;}
       ;



display_statement
       :DISPLAY OPEN_PAR ass_exp CLOSE_PAR   {$$=new CSyntaxNode(DISPLAY_STA, $3, UNKNOWNTYPE);}
	   |DISPLAY OPEN_PAR ass_exp option_output CLOSE_PAR   {$$=new CSyntaxNode(DISPLAY_STA, $3, $4, UNKNOWNTYPE);}
	   ;

option_output
       :								{$$=NULL;}
	   |COMMA ass_exp option_output     {$$=new CSyntaxNode(DISPLAY_STA, $2, $3, UNKNOWNTYPE);}
	   ;

// add by YY 2013/06/17 16:15
request_statement
		:REQUEST OPEN_PAR identifier CLOSE_PAR {$$=new CSyntaxNode(REQUEST_STA, $3, UNKNOWNTYPE);}
		|REQUEST OPEN_PAR identifier option_input CLOSE_PAR 	{$$=new CSyntaxNode(REQUEST_STA, $3, $4, UNKNOWNTYPE);}
		|REQUEST OPEN_PAR list CLOSE_PAR  {$$=new CSyntaxNode(REQUEST_STA, $3, UNKNOWNTYPE);}
		|REQUEST OPEN_PAR list option_input CLOSE_PAR  {$$=new CSyntaxNode(REQUEST_STA, $3, $4, UNKNOWNTYPE);}
	    |REQUEST OPEN_PAR struct_member_exp CLOSE_PAR  {$$=new CSyntaxNode(REQUEST_STA, $3, UNKNOWNTYPE);}
		;
option_input
		:								{$$=NULL;}
	    |COMMA identifier option_input{$$=new CSyntaxNode(REQUEST_STA, $2, $3, UNKNOWNTYPE);}
	    |COMMA list option_input		{$$=new CSyntaxNode(REQUEST_STA, $2, $3, UNKNOWNTYPE);}
		;


//add by YY  s.a->b[2].a   结构体串联
struct_member_exp
		:identifier	 DOT identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|list        DOT identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|value_exp DOT identifier option_struct_member_exp                {$$=new CSyntaxNode(STRUCT_IDENT_EXP, $1, $3, $4, VOIDTYPE);}

		|identifier	 DOT list option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|list        DOT list option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|value_exp DOT list option_struct_member_exp                {$$=new CSyntaxNode(STRUCT_LIST_EXP, $1, $3, $4, VOIDTYPE);}

		|identifier	 IMPLY identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|list        IMPLY identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|OPEN_PAR address_exp CLOSE_PAR IMPLY identifier option_struct_member_exp                {$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $2, $5, $6, VOIDTYPE);}
		|value_exp IMPLY identifier option_struct_member_exp                {$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $1, $3, $4, VOIDTYPE);}

		|identifier	 IMPLY list option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|list        IMPLY list option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|OPEN_PAR address_exp CLOSE_PAR  IMPLY list option_struct_member_exp     {$$=new CSyntaxNode(STRUCTP_LIST_EXP, $2, $5, $6, VOIDTYPE);}
		|identifier DOT function {$$=new CSyntaxNode(STRUCT_FUNC_EXP, $1, $3, VOIDTYPE);} //结构体成员是函数指针
		|identifier IMPLY function {$$=new CSyntaxNode(STRUCTP_FUNC_EXP, $1, $3, VOIDTYPE);} //结构体成员是函数指针
		|value_exp   IMPLY list option_struct_member_exp                {$$=new CSyntaxNode(STRUCTP_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|OPEN_PAR struct_member_exp CLOSE_PAR {$$=$2;}
		;

option_struct_member_exp
		:	{$$=NULL;}
		|DOT identifier		option_struct_member_exp      {$$=new CSyntaxNode(STRUCT_IDENT_EXP, $2, $3, VOIDTYPE);}
		|DOT list			option_struct_member_exp            {$$=new CSyntaxNode(STRUCT_LIST_EXP, $2, $3, VOIDTYPE);}
		|IMPLY identifier   option_struct_member_exp    {$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $2, $3, VOIDTYPE);}
		|IMPLY list			option_struct_member_exp		  {$$=new CSyntaxNode(STRUCTP_LIST_EXP, $2, $3, VOIDTYPE);}
		|DOT function {$$=new CSyntaxNode(STRUCT_FUNC_EXP, $2, NULL, VOIDTYPE);}    //结构体成员是函数指针
		|IMPLY function {$$=new CSyntaxNode(STRUCTP_FUNC_EXP, $2, NULL, VOIDTYPE);}  //结构体成员是函数指针
		;

