%{
#include "function.h"
#include "MyMap.h"
#include <malloc.h>
#include <string.h>      // strcpy

//#include "lex.h"
#include "SyntaxNode.h"
#include "SyntaxTree.h"
//#include "inherit.h"
//#include <afxtempl.h>
CMAP g_StructNameMap;

//��Ĳ�ι�ϵ
//typedef CArray<Inherit,Inherit> ClassHier;//���д����൱�ڶ�����һ�����飬�����е�ÿ��Ԫ�ض���һ��Inherit����
//ClassHier classarray;//����һ��������������Tempura�������ඨ��ļ̳й�ϵ

//******************  �����洢�����﷨���Լ������� *******************************/
/***************************************************************************/
//�����﷨���ڵ�ָ��
CSyntaxNode *g_syntaxTree = NULL;  
//�����﷨���ڵ�ָ��
CSyntaxNode *g_propositionTree=NULL; 

CSyntaxNode *function_tree=NULL, *propositionDefineTree=NULL;

CSyntaxNode *struct_tree=NULL;//�洢�ṹ��Ķ���

CSyntaxTree *g_tree=NULL, *g_proptree=NULL;     

map<string, string> structName; //�洢����Ľṹ�������

//g_tree=new CSyntaxTree(g_syntaxTree);  
//g_proptree=new CSyntaxTree(g_propositionTree); 

int g_nErrors=0;

extern void yyerror(const char *msg );

extern int yylex(void);

#define YYDEBUG 1	          // Generate debug code; needed for YYERROR_VERBOSE
#define YYERROR_VERBOSE       // Give a more specific parse error message

%}

%union{
   char     * cString;   /* A character string */
   int       nInteger;   /* An intliteral value */
   float floatval;	/* A float value added by Jane 2012-06-06 */
   char character;	/* A character added by Jane 2012-06-06*/
   CSyntaxNode *tnode;    /* Node in the syntax tree */
   enum NODETYPE  nodetype;
   enum RETURNTYPE  returntype;
} 

%token SWITCH CASE BREAK DEFAULT RETURN//add by yubin 2015-3-23
%token IF ELSE EMPTY AWAIT PROJECTION  PBEGIN PEND POINTERNULL ARRAY STRUCT DOT UNION FILEDECLARATION
%token MORE DEFINE MY_TRUE MY_FALSE EXIST FRAME FOR WHILE DO 
%token TRUE FALSE
%token REPEAT UNTIL DISPLAY SKIP THEN COMMA COLON 
%token INTDECLARATION STRDECLARATION POINTERDECLARATION CHARDECLARATION VOIDDECLARATION
%token FLOATDECLARATION LISTDECLARATION           
%token OPEN_PAR CLOSE_PAR TIMES OPEN_BPAR CLOSE_BPAR OPEN_MPAR CLOSE_MPAR FUNCTION
%token OVER CYLINDER
%token OVEREP
%token INTER_OR NON_DETERMINED 
%token SEND RECEIVE PROCESS CHANNEL PUT GET ERROR_TOKEN
%token STRFUNCHEAD STRFUNCTAIL STRFUNCCAT STRFUNCCMP STRFUNCCPY STRFUNCLEN 
%token EXTERN
%token UNSIGNED SIGNED
%left COMMA 
%left CHOP
%left IMPLY IFF PROPIMPLY
%left OR PARALLEL CYLINDER
%left AND 
%right ASS_P ASS_N 
%left INTER_OR	//��λ���	
%left OVEREP	
%left CON   //��λ���
%left ADDRESS //��λ��
%left NE EQ  
%left GE LE  GT LT 
%left LST RST  //��������
%left ADD SUB 
%left MUL DIV MOD 
%left BNE  //��λȡ��




%nonassoc CHOPSTAR NEXT NEGATION LENGTH REQUEST FINAL KEEP ALWAYS HALT W_NEXT PREFIX /*ADDRESS*/ SOMETIMES 

%token<cString> ID STRLITERAL STRUCT_TYPE
%token<nInteger> INTLITERAL
%token<floatval> FLOATLITERAL 
%token<character> CHARLITERAL 

//-----------%type<cString> idstatement       //Annotation-Class 
//%type<tnode> cylinder_statement over_statement rep_statement fomer_rep_statement
//%type<tnode> option_fomer_rep_statement option_rep_statement

//added for cylinder computing  
//2015-3-7 %type<tnode> CYLINDER_STATEMENT OVER_STATEMENT INTERVAL_STATEMENT
//2015-3-7 %type<tnode> SIG_INTERVAL
 


%type<tnode> type_cast  /*--------cdstatement mdstatement new atrbstatement-----*/ //Annotation-Class   
%type<tnode> statement identifier strliteral intliteral ass_statement bool_exp option_exp_else_statement leftaddrshift rightaddrshift
%type<tnode> if_statement option_else_statement while_statement extend_for_statement ass_left imply_pre for_sta_init for_sta_condition for_sta_control
%type<tnode> ari_exp for_statement option_frame_identifier option_projection type_cast_alg_exp
%type<tnode> prime_bool_exp option_function_parameter_list inner_option_define_identifier
%type<tnode> option_function_identifier inner_option_function_identifier empty_statement
%type<tnode> option_list_value inner_option_list_value option_ari_exp ass_right array_exp
%type<tnode> declaration option_identifier_array_declaration inner_option_identifier_array_declaration point_exp array option_struct_declaration
%type<tnode> address_exp//added by Jane
%type<tnode> option_array_declaration inner_option_array_declaration//added by Jane

%type<tnode> statement_bpar bool_par option_output option_input output_statement input_statement function//add by yubin 2015/4/15    
%type<tnode> member_in_exp simple_ari_exp  
%type<tnode> charliteral floatliteral 
%type<tnode> struct_define_statement struct_identifier
%type<tnode> option_struct_list_value //�ṹ��ĳ�ʼ���б�{{1,"hello"},{2,"hi"}}
%type<tnode> sign_declaration//����unsigned�Ķ������

//%type<tnode>array_exp option_array_exp 

%type<tnode> casted_element 

//�ṹ�崮��
%type<tnode> struct_member_exp  option_struct_member_exp


%type<nodetype>		assign_operator relation_operator bi_operator  ari_operator
%type<returntype>  type_define sign_type_define
%type<returntype> all_type_define
%type<tnode> program gComplexProposition complexProposition poptional_projection 
%type<tnode> switch_statement case_par init_case_par //add by yubin 2015-3-23


//%expect 1  /* shift/reduce conflict: dangling ELSE declaration */

%%
program      //Annotation-Class   
       :gComplexProposition /*---cdstatement---*/ statement 
	   {
			
			//��֤��Ҫ�����
			g_propositionTree=$1;
			g_proptree=new CSyntaxTree(g_propositionTree);    
			
			
			
			//$2�����ж���ķ������������function_tree�У�ʵ���Ǻϲ�������
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

			
			//ִ�еĻ������
			g_syntaxTree=$2;
			g_tree=new CSyntaxTree(g_syntaxTree);          //2013-5-3 add by YY[fixed]
          
		}
	   ;
//��������֤�����������﷨
gComplexProposition
	   :PBEGIN complexProposition PEND  {$$=$2;}
	   |								{$$=NULL;}
	   ;

complexProposition
	   :complexProposition AND complexProposition   {$$=new CSyntaxNode(AND_EXP, $1, $3, VOIDTYPE);}   
	   |complexProposition OR complexProposition    {$$=new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE);}    
	   |complexProposition IFF complexProposition   {$$=new CSyntaxNode(IFF_STA, $1, $3, BOOLTYPE);}    
	   |complexProposition PROPIMPLY complexProposition {$$=new CSyntaxNode(IMPLY_STA, $1, $3, VOIDTYPE);}    
	   |complexProposition CHOP complexProposition  
	   {
			if($1==NULL)                            $$=$3;
			else                                    $$=new CSyntaxNode(CHOP_STA, $1, $3, VOIDTYPE);       
	   }
	   |ALWAYS complexProposition					{$$=new CSyntaxNode(ALWAYS_STA, $2, VOIDTYPE);}    
	   |KEEP complexProposition					    {$$=new CSyntaxNode(KEEP_STA, $2, VOIDTYPE);}     
	   |SOMETIMES complexProposition				{$$=new CSyntaxNode(SOMETIMES_STA, $2, VOIDTYPE);}
	   |W_NEXT complexProposition					{$$=new CSyntaxNode(W_NEXT_STA, $2, VOIDTYPE);}    
	   |NEXT complexProposition			            {$$=new CSyntaxNode(NEXT_STA, $2, VOIDTYPE);}     
	   |SKIP										{$$=new CSyntaxNode(SKIP_STA, VOIDTYPE);}         
	   |MY_TRUE										{$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}       
	   |MY_FALSE									{$$=new CSyntaxNode(FALSE_EXP, VOIDTYPE);}         
	   |LENGTH ari_exp								{$$=new CSyntaxNode(LENGTH_STA, $2, VOIDTYPE);}   
	   
	   
	   
//2015-3-9	   |LENGTH intliteral							    {$$=new CSyntaxNode(LENGTH_STA, $2, VOIDTYPE);}   
	   |EMPTY										{$$=new CSyntaxNode(EMPTY_EXP, VOIDTYPE);}      
	   |MORE										{$$=new CSyntaxNode(MORE_STA, VOIDTYPE);}        
	   |HALT complexProposition                     {$$=new CSyntaxNode(HALT_STA, $2, VOIDTYPE);}  
	   |FINAL complexProposition                    {$$=new CSyntaxNode(FINAL_STA, $2, VOIDTYPE);}   
	   |NEGATION complexProposition                 {$$=new CSyntaxNode(NEGATION_EXP, $2, VOIDTYPE);}  
	   |OPEN_BPAR complexProposition poptional_projection CLOSE_BPAR PROJECTION OPEN_BPAR complexProposition CLOSE_BPAR
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, $3, $7, VOIDTYPE);                         
	   }
	   |ID 											{$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE)}     //{$$=new CSyntaxNode(IDENT_EXP, $1, 0);}  
	   |DEFINE ID COLON prime_bool_exp              
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

//�����ǿ�ִ�л��������﷨
statement
	   :statement  AND statement        {$$=new CSyntaxNode(AND_EXP, $1, $3, VOIDTYPE);}

	   |statement  OR  statement        {$$=new CSyntaxNode(OR_EXP, $1, $3, VOIDTYPE);}
	   
	   //**********************************************************************************
	   //   �Ѻ����������ŵ�function_tree�н��д洢  
	   //   ����ԭʼ�Ķ��壬����������ȷʵҪ���ʼ���� 
	   //   �����������һ����� ʹ�õĺ����������κεط�����
	   //**********************************************************************************
	   |statement CHOP statement        
	   {
			//�ṹ��(struct)���� ��ʱ�涨structֻ�����ʼ����
			
			// ��������struct�������
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
			// ��һ����struct������䣬�ڶ�����ִ�����
			if(NULL!=$1 && $1->GetNType()==STRUCT_DEFINE_STA && NULL!=$3 && $3->GetNType()!=STRUCT_DEFINE_STA )
			{
				struct_tree=$1;
				$$=$3;
			}
			// ��һ����ִ����䣬�ڶ�����struct�������
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
			
			//�������Ǻ����������
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
			//��һ���Ǻ���������䣬�ڶ�����ִ�����
			if(NULL!=$1 && $1->GetNType()==FUNCTION_DEFINE_STA && NULL!=$3 && $3->GetNType()!=FUNCTION_DEFINE_STA)
			{
				function_tree=$1;
                $$=$3;
			}

			//��������ִ�����
			if( NULL!=$1 && $1->GetNType()!=FUNCTION_DEFINE_STA &&  $1->GetNType()!=STRUCT_DEFINE_STA && 
			    NULL!=$3 && $3->GetNType()!=FUNCTION_DEFINE_STA &&  $3->GetNType()!=STRUCT_DEFINE_STA)
			{
				$$=new CSyntaxNode(CHOP_STA, $1, $3, VOIDTYPE);
			}

			//��һ����ִ����䣬�ڶ������������  
			//problem: ��������ĵ�һ��������������ſ���    
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
	   
	  // |PREFIX OPEN_PAR statement CLOSE_PAR   {$$=new CSyntaxNode(PREFIX_STA, $3, VOIDTYPE);}
       |ass_statement                   {$$=$1;}
	   |sign_declaration                    {$$=$1;}
	   
	   |switch_statement              {$$=$1;}   //add 2015-3-18
	   |RETURN ID					  {$$=new CSyntaxNode(RETURN_STA, $2,NULL,VOIDTYPE);}   //add by yubin,2015-4-13
	   |if_statement                    {$$=$1;}
	   |while_statement                 {$$=$1;}
	   |for_statement                   {$$=$1;}
	   |extend_for_statement            {$$=$1;}//������C�����е�for���
	   |NEXT OPEN_PAR statement CLOSE_PAR    {$$=new CSyntaxNode(NEXT_STA, $3, VOIDTYPE);}
//2015-3-7	   |W_NEXT OPEN_PAR statement CLOSE_PAR    {$$=new CSyntaxNode(W_NEXT_STA, $3, VOIDTYPE);}
	   |KEEP OPEN_PAR statement CLOSE_PAR       {$$=new CSyntaxNode(KEEP_STA, $3, VOIDTYPE);}
	   |ALWAYS OPEN_PAR statement CLOSE_PAR      {$$=new CSyntaxNode(ALWAYS_STA, $3, VOIDTYPE);}
//2015-3-7	   |HALT OPEN_PAR bool_exp CLOSE_PAR   {$$=new CSyntaxNode(HALT_STA, $3, VOIDTYPE);}
	   |AWAIT OPEN_PAR bool_exp CLOSE_PAR  {$$=new CSyntaxNode(AWAIT_STA, $3, VOIDTYPE);}
	   |REPEAT OPEN_BPAR statement CLOSE_BPAR UNTIL OPEN_PAR bool_exp CLOSE_PAR  {$$=new CSyntaxNode(REPEAT_UNTIL_STA, $3, $7, VOIDTYPE);}
	   |SKIP                            {$$=new CSyntaxNode(SKIP_STA, VOIDTYPE);}
       |EMPTY                           {$$=new CSyntaxNode(EMPTY_EXP, VOIDTYPE);}
	   |MORE                            {$$=new CSyntaxNode(MORE_STA, VOIDTYPE);}
	   |LENGTH OPEN_PAR ari_exp CLOSE_PAR   {$$=new CSyntaxNode(LENGTH_STA, $3, VOIDTYPE);}
//2013-5-7 
//	   
//	   |LENGTH OPEN_PAR intliteral CLOSE_PAR  {$$=new CSyntaxNode(LENGTH_STA, $3, VOIDTYPE);}
//annotated by YY 2013/06/17 16:20
///	   |REQUEST OPEN_PAR identifier CLOSE_PAR             {$$=new CSyntaxNode(REQUEST_STA, $3, VOIDTYPE);}
	   |input_statement               {$$=$1;}
	   |output_statement               {$$=$1;}
	   |TRUE                         {$$=new CSyntaxNode(TRUE_EXP, VOIDTYPE);}         //2015-3-7
	   |FALSE                        {$$=new CSyntaxNode(FALSE_EXP, VOIDTYPE);}        //2015-3-7
	   |FINAL OPEN_PAR statement CLOSE_PAR   {$$=new CSyntaxNode(FINAL_STA, $3, VOIDTYPE);}
	   | statement PARALLEL  statement  	{$$=new CSyntaxNode(PARALLEL_STA, $1, $3, VOIDTYPE);}

	   |OPEN_PAR imply_pre CLOSE_PAR IMPLY OPEN_MPAR statement CLOSE_MPAR       
			{$$=new CSyntaxNode(IMPLY_STA, $2, $6, VOIDTYPE);}     	   	   
	   
	   |FRAME OPEN_PAR identifier option_frame_identifier CLOSE_PAR AND OPEN_PAR statement CLOSE_PAR
	   {
			$$=new CSyntaxNode(FRAME_STA, $3, $4, $8, VOIDTYPE);
	   }

//prj 
	   |OPEN_BPAR statement option_projection CLOSE_BPAR PROJECTION statement_bpar
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, $3, $6,VOIDTYPE);
	   }
	   |OPEN_BPAR statement CLOSE_BPAR PROJECTION statement_bpar
	   {
			$$=new CSyntaxNode(PROJECTION_STA, $2, NULL, $5, VOIDTYPE);
	   }

	   	   
		|FUNCTION ID OPEN_PAR option_function_parameter_list CLOSE_PAR  OPEN_BPAR statement CLOSE_BPAR//2015-4-13,�ڱ��޸�
	   {
			$$=new CSyntaxNode(FUNCTION_DEFINE_STA, $2, $4, $7, NULL, VOIDTYPE);
	   }

	   //***********************************************************************
	      //�޷���ֵ�ĺ�������      ���̣���������  
	   //***********************************************************************
	   |DEFINE ID OPEN_PAR option_function_parameter_list CLOSE_PAR ASS_P OPEN_BPAR empty_statement CLOSE_BPAR 
	   { 
			$$=new CSyntaxNode(FUNCTION_DEFINE_STA , $2, $4, $8, NULL, VOIDTYPE);
	   }
	   
	   //*******************************************************************************************
	       //�з���ֵ�ĺ�������function, ������Ϊ���Ը�ֵ������ʽ
		      
	   //*******************************************************************************************
	    
       |struct_define_statement//�ṹ�嶨�����
	   
	   |function                         {$$=$1;}
//	   |new                              {$$=$1;}    //Annotation-Class   
	   |EXIST identifier inner_option_define_identifier COLON OPEN_BPAR statement CLOSE_BPAR
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
//2015-3-7	   |CYLINDER_STATEMENT				{$$=$1;} 
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

empty_statement
	   :statement                       {$$=$1;}
	   |                                {$$=NULL;}
	   ;

//����ν�ʺͺ���ʱ���﷨
function
       :ID OPEN_PAR option_function_identifier CLOSE_PAR
	   {
	        // ��ʱ�����Ǵ���Ա���� .�ĺ�������
			$$=new CSyntaxNode(FUNCTION_STA, $1, $3, NULL, NULL, VOIDTYPE);//������ú����ĳ����﷨������          
	   }
	   |EXTERN ID OPEN_PAR option_function_identifier CLOSE_PAR
	   {
	        // ��ʱ�����Ǵ���Ա���� .�ĺ�������
			$$=new CSyntaxNode(EXT_FUNCTION_STA, $2, $4, NULL, NULL, VOIDTYPE);//������ú����ĳ����﷨������          
	   }
	   |array OPEN_PAR option_function_identifier CLOSE_PAR
	   {
	        //Annotate by YY 2013/11/20 ��ʱ�����Ǵ���Ա���� .�ĺ�������
			$$=new CSyntaxNode(FUNCTION_STA, $1, $3, NULL, FUNCPTYPE);//ͨ������ָ��������ú���
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
			if(!g_StructNameMap.insert($2))
			{
			    yyerror("Redefinition of struct");
			}
			else
			{
			    $$=new CSyntaxNode(STRUCT_NAME, $2, STRUCT_NAME_TYPE);
			}
	   }
       |UNION  ID 
	   {
	       if(!g_StructNameMap.insert($2))
			{
			    yyerror("Redefinition of union");
			}
			else
			{
			    $$=new CSyntaxNode(STRUCT_NAME, $2, STRUCT_NAME_TYPE);
			}
	   }
	   ;
//����unsigned��signed�����⣬���յĶ������
sign_declaration
	   :SIGNED declaration   {$$=$2;} //Ĭ��Ϊsigned
	   |UNSIGNED declaration {$$=new CSyntaxNode(UNSIGN_DECLARATION_STA, $2, VOIDTYPE); }
	   |declaration          {$$=$1;}
	   ;

declaration
	   :type_define  option_identifier_array_declaration	
	   {
	      $$=new CSyntaxNode(DECLARE_STA, $2, $1);
		}
       |STRUCT_TYPE option_identifier_array_declaration 	 
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$1,$2,STRUCTTYPE);
		} //�ṹ������ yangkai
	   |STRUCT ID option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//�ṹ������
       |UNION ID option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//����������
	   |STRUCT STRUCT_TYPE option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//�ṹ������
       |UNION STRUCT_TYPE option_identifier_array_declaration
	   {
	     $$=new CSyntaxNode(DECLARE_STA,$2,$3,STRUCTTYPE);
	   }//����������
	//2015-3-20    |POINTERDECLARATION   option_function_parameter_list {$$=new CSyntaxNode(DECLARE_STA, $2, POINTERTYPE);}  
	  //2015-3-20 |LISTDECLARATION   option_array_declaration		{$$=new CSyntaxNode(DECLARE_STA, $2, LISTTYPE);}
	    // ����ָ�� 
	   |OPEN_PAR MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR	{$$=new CSyntaxNode(DECLARE_STA, $3, FUNCPTYPE);}
	   //����ָ���ָ��
	   |OPEN_PAR MUL MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR {$$=new CSyntaxNode(DECLARE_STA, $4, FUNCPPTYPE);}
	   	//����ָ������ 
	   |OPEN_PAR MUL ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR	
	   {
	       CSyntaxNode *pChild0=new CSyntaxNode(PARAMETER_EXP,new CSyntaxNode(ARRAY_DECLARE_STA, $3, $5, $6, NULL, LISTTYPE), VOIDTYPE);
	       $$=new CSyntaxNode(DECLARE_STA, pChild0, FUNCPTYPE);
	   }
       |OPEN_PAR MUL ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR ASS_P array_exp
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
		|FILEDECLARATION    			{$$=FILETYPE;} 
		;



//��option_declaration��Ϊoption_identifier_array_declaration      
option_identifier_array_declaration
		:identifier inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, $1, $2, VOIDTYPE);
		}
	    |ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR inner_option_identifier_array_declaration
		{
	       $$=new CSyntaxNode(PARAMETER_EXP,new CSyntaxNode(ARRAY_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE), $6, VOIDTYPE);
		}
	    |identifier ASS_P ass_right inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, $1, $3, VOIDTYPE), $4, VOIDTYPE);
		}
		// int a[]<==[1,2,3]
		|ID ARRAY ASS_P array_exp inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, new CSyntaxNode(ARRAY_DECLARE_NULL_STA, $1, NULL, NULL, NULL, LISTTYPE), $4, VOIDTYPE), $5, VOIDTYPE);
		}
		
		// char a[]<=="hallo"
		|ID ARRAY ASS_P strliteral inner_option_identifier_array_declaration
		{
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, new CSyntaxNode(ARRAY_DECLARE_NULL_STA, $1, NULL, NULL, NULL, LISTTYPE), $4, VOIDTYPE), $5, VOIDTYPE);
		}
		// int a[3]<==[1,2,3]   char a[2,6]<==["he","she"]
	    |ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR ASS_P array_exp inner_option_identifier_array_declaration
		{
	       CSyntaxNode *pChild0=new CSyntaxNode(ARRAY_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE);
		   $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ASS_EQU_EXP, pChild0, $7, VOIDTYPE), $8, VOIDTYPE);
		   pChild0=NULL;
	    } 
	   // a[3]<=="ni";
	   |ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR ASS_P strliteral inner_option_identifier_array_declaration
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
//��list_option_declaration�޸�Ϊoption_array_declaration 
option_array_declaration
	   :ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR inner_option_array_declaration
	   {
	        $$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(LIST_DECLARE_STA, $1, $3, $4, NULL, LISTTYPE), $6, VOIDTYPE);
	   }
	   ;
//��list_inner_option_declaration�޸�Ϊinner_option_array_declaration
inner_option_array_declaration
	   :COMMA ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR inner_option_array_declaration
	   {
			$$=new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(LIST_DECLARE_STA, $2, $4, $5, NULL, LISTTYPE), $7, VOIDTYPE);
	   }
	   | /* empty */                    {$$=NULL;}
	   |COMMA error                     {$$=NULL;}
	   ;

//������������ end Jane

//�丳ֵ���  

ass_statement
       :ass_left assign_operator ass_right       
										{$$=new CSyntaxNode($2, $1, $3, VOIDTYPE);}	
	   ;
ass_left
       :identifier                      {$$=$1;}
	   |struct_member_exp               {$$=$1;}	   
       |array                            {$$=$1;}   //a[2,3,4]  
	   |point_exp                       {$$=$1;}
	   |OPEN_PAR ass_left CLOSE_PAR    {$$=$2;}
	   |NEXT ass_left                  {$$=new CSyntaxNode(NEXT_STA, $2, UNKNOWNTYPE);} 
	   |leftaddrshift                   {$$=$1;}  
	   ;
leftaddrshift
	   :OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $6, $9, $10, NULL, CHARTYPE);}   //((char*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $7, $10, $11, NULL, UCHARTYPE);}   //((unsigned char*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $6, $9, $10, NULL, INTTYPE);}   //((int*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR FLOATDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $6, $9, $10, NULL, FLOATTYPE);}   //((float*)0x01)[2]=5
	   |OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, CHARTYPE);}   //((char*)((char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, CHARTYPE);}   //((char*)((unsigned char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, CHARTYPE);}   // ((char*)((int*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, UCHARTYPE);}   //((unsigned char*)((char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $13, $17, $18, NULL, UCHARTYPE);}   //((unsigned char*)((unsigned char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, UCHARTYPE);}   //((unsigned char*)((int*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, INTTYPE);}   //((int*)((char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $12, $16, $17, NULL, INTTYPE);}   // ((int*)((unsigned char*)0x01))[2]=5
	   |OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR INTLITERAL CLOSE_PAR CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR		
	  								{$$=new CSyntaxNode(ADDR_VAR_LIST, $11, $15, $16, NULL, INTTYPE);}   // ((int*)((int*)0x01))[2]=5
		;

assign_operator
	   :ASS_P                         {$$=ASS_EQU_EXP;}    // <==
	   |ASS_N      {$$=UNITASSIGN_EXP;}   // :=
	   ;                
ass_right
	   :ari_exp                         {$$=$1;}
	   |array_exp                        {$$=$1;}   // [2,3,5,9] ����[1,3]^[2,4]
	   |strliteral                          {$$=$1;}     
//2015-3-9	   |bool_exp                  {$$=$1;} //bool���ʽ       
//2015-3-7	   |struct_member_exp               {$$=$1;}
	   |OPEN_PAR strliteral CLOSE_PAR OPEN_PAR type_define CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR function {$$=new CSyntaxNode(DLL_CALL_STA, $2, $10, $5);}
	   |OPEN_PAR ass_right CLOSE_PAR      {$$=$2;} 
	   |rightaddrshift                  {$$=$1;}
	   ;
rightaddrshift
       :MUL OPEN_PAR leftaddrshift CLOSE_PAR {$$=$3;}
	   ;

array  
       :ID OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR  {$$=new CSyntaxNode(LIST_SYMBOL_STA, $1, $3, $4, NULL, VOIDTYPE);}
		
	   // ����((unsigned char*)a)[2]='b',����Ϊ��ֵ������ֵ
	   //|casted_element OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR {$$=new CSyntaxNode(LIST_SYMBOL_STA, $1, $3, $4, NULL, VOIDTYPE);}

	   | OPEN_PAR OPEN_PAR INTDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR 
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $6, $9, $10, NULL, VOIDTYPE);}  
	   | OPEN_PAR OPEN_PAR UNSIGNED INTDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR 
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $7, $10, $11, NULL, VOIDTYPE);}  
	   | OPEN_PAR OPEN_PAR FLOATDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR  
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $6, $9, $10, NULL, VOIDTYPE);}  
	   | OPEN_PAR OPEN_PAR CHARDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR 
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $6, $9, $10, NULL, VOIDTYPE);}  
	   | OPEN_PAR OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR ID CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR 
				{$$=new CSyntaxNode(LIST_SYMBOL_STA, $7, $10, $11, NULL, VOIDTYPE);}  
	   | OPEN_PAR array CLOSE_PAR   {$$=$2;}
	   ;

option_ari_exp
	   :COMMA ari_exp option_ari_exp
	                                    {$$=new CSyntaxNode(LIST_SUBSCRIPT_EXP, $2, $3, VOIDTYPE);}
	   |COMMA error                     {$$=NULL;}
	   | /* empty */                    {$$=NULL;}
	   ;


//ʵ������ָ�루*p��
point_exp
	   :MUL identifier                  {$$=new CSyntaxNode(VALUE_EXP, $2, UNKNOWNTYPE);}    //*x
	   |MUL point_exp 					{$$=new CSyntaxNode(VALUE_EXP, $2, UNKNOWNTYPE);}    //**x
	   |MUL array                              //*a[2]
									    {$$=new CSyntaxNode(VALUE_EXP, $2,  UNKNOWNTYPE);} //add by wm 2013/10/28
	   |OPEN_PAR point_exp CLOSE_PAR OPEN_MPAR ari_exp option_ari_exp CLOSE_MPAR                     //(*a)[2]
									    {$$=new CSyntaxNode(VALUE_LIST_EXP, $2, $5, $6, UNKNOWNTYPE);}
	   |MUL OPEN_PAR all_type_define MUL CLOSE_PAR ari_exp
	   		{$$=new CSyntaxNode(ADDR_VAR, $6, $3);} //  *(char*)0x01=5
	   |MUL OPEN_PAR ari_exp CLOSE_PAR	                                    
	        {$$=new CSyntaxNode(VALUE_EXP, $3, UNKNOWNTYPE);}
	   |OPEN_PAR point_exp CLOSE_PAR    {$$=$2;}   // ʹ��a<==(*p)+
	   //|MUL error                       {$$=NULL;}
       ;

//�������ʽ
ari_exp     
	   :simple_ari_exp                  {$$=$1;}
	   |simple_ari_exp relation_operator ari_exp {$$=new CSyntaxNode($2, $1, $3, BOOLTYPE);} //�������ʽ�п�����bool���ʽ����a+(b>c)
	 //  |simple_ari_exp bi_operator ari_exp    {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}   
       |ari_exp ari_operator simple_ari_exp    {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}   // +|-
	   |IF bool_exp THEN  ari_exp option_exp_else_statement 
	   {
            $$ = new CSyntaxNode(IF_ELSE_EXP, $2, $4, $5, ARITHMETICTYPE);
       }   	   
	   ;
type_cast_alg_exp
       :simple_ari_exp                  {$$=$1;}
	   |simple_ari_exp ari_operator simple_ari_exp    {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}   // +|- 	   
	   ;

option_exp_else_statement
       : ELSE ari_exp					                   {$$=$2;}
       | /* empty */                                       {$$=NULL;}
       ;

simple_ari_exp
	   :member_in_exp   {$$=$1;}
	   |simple_ari_exp bi_operator member_in_exp  {$$=new CSyntaxNode($2, $1, $3, ARITHMETICTYPE);}
	   ; 

member_in_exp
       :intliteral                         {$$=$1;}
	   |SUB member_in_exp						{$$=new CSyntaxNode(MINUS_EXP, $2, ARITHMETICTYPE);}
	   |BNE member_in_exp                      {$$=new CSyntaxNode(BNE_EXP, $2, ARITHMETICTYPE);}
	   |POINTERNULL                     {$$=new CSyntaxNode(NULL_EXP, VOIDTYPE);}
	   |floatliteral					{$$=$1;}     
	   //|ID                            {$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE);}    
       |identifier						{$$=$1;}
	   |array                            {$$=$1;}
	   |point_exp                       {$$=$1;}
	   |function                        {$$=$1;}
	   |OPEN_PAR ari_exp CLOSE_PAR      {$$=$2;}   
	   |charliteral						{$$=$1;}
	   |address_exp						{$$=$1;} // ��ʼ��ָ�������õ�	   
	   |type_cast                       {$$=$1;} //  ǿ��ת�����Բ�����������	  	   	   
	   |struct_member_exp               {$$=$1;} //  �ṹ�崮��
	   ;
bi_operator      
	   :MUL								{$$=MUL_EXP;}
	   |DIV								{$$=DIV_EXP;}
	   |MOD								{$$=MOD_EXP;}
	   |LST						     	{$$=LST_EXP;}//  ���� left shift
	   |RST	 						    {$$=RST_EXP;}//  ���� right shift
	   |ADDRESS 	 					{$$=BAN_EXP;}//  ��λ��
	   |INTER_OR	 					{$$=BOR_EXP;}//  ��λ��
	   |CON	 		         			{$$=XOR_EXP;}//  ���	
	   ;
ari_operator:
		ADD								{$$=ADD_EXP;}
	   |SUB								{$$=SUB_EXP;}
	   ;	  

array_exp
	   :array_exp CON array_exp           {$$=new CSyntaxNode(CON_EXP, $1, $3, LISTTYPE);}
	   |option_list_value               {$$=$1;}   // [2,3,5,9]
	   |option_struct_list_value         {$$=$1;}   // {1,'C',"HELLO"}
	   |OPEN_PAR array_exp CLOSE_PAR     {$$=$2;}
	   ;


//�����Ƕ������鸳ֵ[1,2,4]����ʽ  ��int float str char array�� 
option_list_value
	   :OPEN_MPAR ari_exp inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
		}	
	   |OPEN_MPAR strliteral inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	  /*2015-3-9 |OPEN_MPAR charliteral inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }*/
	   |OPEN_MPAR array_exp inner_option_list_value CLOSE_MPAR
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   /*|OPEN_MPAR CLOSE_MPAR / * empty * /				   
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, LISTTYPE);
   	   }*/
	   ;     

// �ṹ��ĳ�ʼ���б�{{1,"hello"},{2,"hi"}}
option_struct_list_value
	   :OPEN_BPAR ari_exp inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
		}	
	   |OPEN_BPAR strliteral inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   /*2015-3-9
	   |OPEN_BPAR charliteral inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }*/
	   |OPEN_BPAR array_exp inner_option_list_value CLOSE_BPAR
	   {
			$$ = new CSyntaxNode(STRUCT_LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   /*|OPEN_BPAR CLOSE_BPAR / * empty * /				   
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, LISTTYPE);
   	   }*/
	   ;  

//�˴�����STR_EXP,CHARLITERAL_EXP�Ĳ�ͬ���嶨�� ����ֵ����ΪLISTTYPE 
inner_option_list_value
	   :COMMA ari_exp inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);//INTEGER_EXP Jane
	   }
	   |COMMA strliteral inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	  /*2015-3-9 |COMMA charliteral inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }*/
	   |COMMA array_exp inner_option_list_value
	   {
			$$ = new CSyntaxNode(LIST_ASSVALUE_STA, $2, $3, LISTTYPE);
	   }
	   | /* emtpy */									   {$$=NULL;}
	   |COMMA error                                        {$$=NULL;}
	   ;
 
address_exp:	 
            ADDRESS identifier	
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);
			}
            |ADDRESS array
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);
			}
            |ADDRESS struct_member_exp
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE );
			}
			//add by YY 2013/11/28 ���ص�ַ������ʱ������
		    |ADDRESS address_exp 
			{
			   $$=new CSyntaxNode(ADDRESS_EXP, $2, VOIDTYPE);
			}
			|OPEN_PAR address_exp CLOSE_PAR  {$$=$2;}   //add by YY 2014/01/17
			;

 


//����ǿ������ת����� 
type_cast:
			//add by YY 2014/01/07 10:57 ǿ��ת�� (int) (float) (char)		
		      OPEN_PAR INTDECLARATION CLOSE_PAR type_cast_alg_exp     {$$=new CSyntaxNode(TYPE_CAST_STA, $4, INTTYPE);}      //char->int   float->int   
			| OPEN_PAR FLOATDECLARATION CLOSE_PAR type_cast_alg_exp {$$=new CSyntaxNode(TYPE_CAST_STA, $4, FLOATTYPE);}    //int->float   //char->float  
			| OPEN_PAR CHARDECLARATION CLOSE_PAR type_cast_alg_exp   {$$=new CSyntaxNode(TYPE_CAST_STA, $4, CHARTYPE);}     //int->char float->char
			
			//add by YY 2014/01/07 10:57 ǿ��ת�� (signed int) (signed char)	
			| OPEN_PAR SIGNED INTDECLARATION CLOSE_PAR type_cast_alg_exp     {$$=new CSyntaxNode(TYPE_CAST_STA, $5, INTTYPE);}      //char->int   float->int   
			| OPEN_PAR SIGNED CHARDECLARATION CLOSE_PAR type_cast_alg_exp   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, CHARTYPE);}     //int->char float->char
			
			//add by YY 2014/01/07 10:57 ǿ��ת�� (unsigned int) (unsigned char)		
			| OPEN_PAR UNSIGNED INTDECLARATION CLOSE_PAR type_cast_alg_exp     {$$=new CSyntaxNode(TYPE_CAST_STA, $5, UINTTYPE);}      //char->int   float->int  
			| OPEN_PAR UNSIGNED CHARDECLARATION CLOSE_PAR type_cast_alg_exp   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, UCHARTYPE);}     //int->char float->char						
			
			//add by YY 2014/01/07 10:57 ǿ��ת�� (int*) (unsigned int*) (float*) (char*) (unsigned char*)			
			| OPEN_PAR INTDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, INTPTYPE);}  
			| OPEN_PAR UNSIGNED INTDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $6, UINTPTYPE);}  
			| OPEN_PAR FLOATDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, FLOATPTYPE);}  
			| OPEN_PAR CHARDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $5, CHARPTYPE);}  
			| OPEN_PAR UNSIGNED CHARDECLARATION MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $6, UCHARPTYPE);}   
			| OPEN_PAR VOIDDECLARATION MUL CLOSE_PAR casted_element {$$=new CSyntaxNode(TYPE_CAST_STA, $5, VOIDPTYPE);}

			//����ָ��ǿ������ת��			
			| OPEN_PAR INTDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, DOUBLEINTPTYPE);}  
			| OPEN_PAR UNSIGNED INTDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $7, DOUBLEUINTPTYPE);}  
			| OPEN_PAR FLOATDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, DOUBLEFLOATPTYPE);}  
			| OPEN_PAR CHARDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, DOUBLECHARPTYPE);}  
			| OPEN_PAR UNSIGNED CHARDECLARATION MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $7, DOUBLEUCHARPTYPE);}   
			| OPEN_PAR VOIDDECLARATION MUL MUL CLOSE_PAR casted_element {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $6, VOIDPTYPE);}
			//yangkai malloc���
			| OPEN_PAR STRUCT STRUCT_TYPE MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(TYPE_CAST_STA, $3, $6, STRUCTPTYPE);} 
			| OPEN_PAR STRUCT_TYPE MUL CLOSE_PAR casted_element  {$$=new CSyntaxNode(TYPE_CAST_STA, $2, $5, STRUCTPTYPE);} 
            //�ṹ�����Ͷ���ָ��ת��
			| OPEN_PAR STRUCT STRUCT_TYPE MUL MUL CLOSE_PAR casted_element   {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $3, $7, DOUBLESTRUCTPTYPE);} 
			| OPEN_PAR STRUCT_TYPE MUL MUL CLOSE_PAR casted_element  {$$=new CSyntaxNode(DOUBLE_TYPE_CAST_STA, $2, $6, DOUBLESTRUCTPTYPE);} 
			;
casted_element:	type_cast_alg_exp	{$$=$1;}   // malloc���			  
			  ;		
//����ǿ������ת����� end Jane
 



bool_exp
	   :prime_bool_exp            {$$=$1;}
	   |NEGATION bool_exp         {$$=new CSyntaxNode(NEGATION_EXP, $2, BOOLTYPE);}
	   |bool_exp AND bool_exp    {$$=new CSyntaxNode(AND_EXP, $1, $3, BOOLTYPE);}
	   |bool_exp OR bool_exp     {$$=new CSyntaxNode(OR_EXP, $1, $3, BOOLTYPE);}
	   |OPEN_PAR bool_exp CLOSE_PAR    {$$=$2;}
	   
	   ;


prime_bool_exp
       :ari_exp relation_operator ari_exp {$$=new CSyntaxNode($2, $1, $3, BOOLTYPE);}
//2015-3-7	   |NEGATION prime_bool_exp   {$$=new CSyntaxNode(NEGATION_EXP, $2, BOOLTYPE);}
	   |MY_TRUE                         {$$=new CSyntaxNode(TRUE_EXP, BOOLTYPE);}
	   |MY_FALSE                        {$$=new CSyntaxNode(FALSE_EXP, BOOLTYPE);}
//2015-3-7	   |MORE                            {$$=new CSyntaxNode(MORE_STA, BOOLTYPE);}
//2015-3-7	   |EMPTY                           {$$=new CSyntaxNode(EMPTY_EXP, BOOLTYPE);}
	   |ari_exp                         {$$=$1;}
		 |ari_exp EQ strliteral          {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}       
	   |ari_exp NE strliteral			{$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);}

	    
	   |strliteral EQ ari_exp            {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);}       
	   |strliteral NE ari_exp   			{$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);}
	    
	    
	   |ari_exp EQ POINTERNULL          {$$=new CSyntaxNode(EQU_EXP, $1, new CSyntaxNode(NULL_EXP, VOIDTYPE), BOOLTYPE);}       
	   |ari_exp NE POINTERNULL			{$$=new CSyntaxNode(NE_EXP, $1, new CSyntaxNode(NULL_EXP, VOIDTYPE), BOOLTYPE);}

	     
	   |POINTERNULL EQ ari_exp           {$$=new CSyntaxNode(EQU_EXP, new CSyntaxNode(NULL_EXP, VOIDTYPE), $3, BOOLTYPE);}       
	   |POINTERNULL NE ari_exp  		  {$$=new CSyntaxNode(NE_EXP, new CSyntaxNode(NULL_EXP, VOIDTYPE), $3, BOOLTYPE);}
	   

	   |ari_exp EQ charliteral     {$$=new CSyntaxNode(EQU_EXP, $1, $3, BOOLTYPE);} 
	   |ari_exp NE  charliteral     {$$=new CSyntaxNode(NE_EXP, $1, $3, BOOLTYPE);} 
	   ;


relation_operator
	   :GE                              {$$=GE_EXP;}
	   |LE                              {$$=LE_EXP;}
	   |NE                              {$$=NE_EXP;}
	   |GT                              {$$=GT_EXP;}
	   |LT                              {$$=LT_EXP;}
	   |EQ                             {$$=EQU_EXP;}     //[add fixed]
	   ;


//���������Ҫ�Ķ��ĵط� 

 
sign_type_define:
	    UNSIGNED INTDECLARATION					{$$=UINTTYPE;} 
	   |UNSIGNED CHARDECLARATION				{$$=UCHARTYPE;}
	   ;
//  ���ں����β��б�option_function_parameter_list��ʹ�ÿ��Դ���unsigned���β���
all_type_define:
		sign_type_define  {$$=$1;}
	   |type_define       {$$=$1;}
	   ;


//����ν�ʺͺ���ʱʹ�õ��β��б�
option_function_parameter_list
	   :all_type_define identifier inner_option_define_identifier
	   {
			$$ = new CSyntaxNode(PARAMETER_EXP, $2, $3, $1);
	   }
	   |all_type_define inner_option_define_identifier //  ����ָ����β��б����û�в�������
	   {
			$$=NULL;
	   }
	   
	   |STRUCT_TYPE identifier inner_option_define_identifier //�ṹ�� S s	 
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $1, $2, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $3, STRUCTTYPE);
			pChild0=NULL;
		}
       |STRUCT_TYPE MUL identifier inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S *s
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(STRUCTP_PARAMETER_EXP, $1, $3, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTPTYPE);
            pChild0=NULL;
	   }
       |STRUCT_TYPE MUL MUL identifier inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S **s //���ؽṹ��ָ��
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(DOUBLESTRUCTP_PARAMETER_EXP, $1, $4, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTPTYPE);
            pChild0=NULL;
	   }
	   |STRUCT_TYPE identifier ARRAY inner_option_define_identifier //�ṹ������ S a[]	 
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_ARRAY_PARAMETER_EXP, $1, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
            pChild0=NULL;
		}
	   
		
		
	   |ID identifier inner_option_define_identifier //�ṹ�� S s	 
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $1, $2, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $3, STRUCTTYPE);
			pChild0=NULL;
		}
	   |ID MUL identifier inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S *s
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(STRUCTP_PARAMETER_EXP, $1, $3, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTPTYPE);
            pChild0=NULL;
	   }
       |ID MUL MUL identifier inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S **s //���ؽṹ��ָ��
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(DOUBLESTRUCTP_PARAMETER_EXP, $1, $4, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTPTYPE);
            pChild0=NULL;
	   }
	   |ID identifier ARRAY inner_option_define_identifier //�ṹ������ S a[]	 
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_ARRAY_PARAMETER_EXP, $1, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
            pChild0=NULL;
		}

	   
	   |STRUCT_TYPE  inner_option_define_identifier //�ṹ�� S s	 
	   {
	        $$=NULL;
		}
       |STRUCT_TYPE MUL  inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S *s
	   {
	        $$=NULL;
	   }
       |STRUCT_TYPE MUL MUL  inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S **s //���ؽṹ��ָ��
	   {
			$$=NULL;
	   }
	  
       
	   |ID  inner_option_define_identifier //�ṹ�� S s	 
	   {
	        $$=NULL;
		}
       |ID MUL  inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S *s
	   {
	        $$=NULL;
	   }
       |ID MUL MUL  inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���S **s //���ؽṹ��ָ��
	   {
			$$=NULL;
	   }

	   
	   |all_type_define identifier ARRAY inner_option_define_identifier//�������͵Ĳ���int a[]
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $4, $1);
            pChild0=NULL;
	   }
	   |all_type_define MUL identifier ARRAY inner_option_define_identifier //ָ������int *a[]
	   {
            CSyntaxNode* pChild0=new CSyntaxNode(POINTER_ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $1);
            pChild0=NULL;
	   }
	    //�������͵Ĳ���a[][100]
	   |all_type_define identifier ARRAY OPEN_MPAR ari_exp CLOSE_MPAR inner_option_define_identifier
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $2, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $7, $1);
            pChild0=NULL;
	   }
	   |all_type_define MUL identifier inner_option_define_identifier//ָ�����͵Ĳ���*p
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(POINT_PARAMETER_EXP, $3, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, $1);
          pChild0=NULL;
		}
	   |all_type_define MUL inner_option_define_identifier//ָ�����͵Ĳ���*p
	   {
	      $$=NULL;
		}
	   |all_type_define MUL MUL identifier inner_option_define_identifier//ָ�����͵Ĳ���**p
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(DOUBLEPOINT_PARAMETER_EXP, $4, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $1);
          pChild0=NULL;
		}
		// �����Ĳ��������Ǻ���ָ��
	   |OPEN_PAR MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR inner_option_define_identifier//����ָ�����͵Ĳ���
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(FUNCP_PARAMETER_EXP, $3, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $8, FUNCPTYPE);
          pChild0=NULL;
		}
	   | /* empty */                                       {$$ = NULL;}
	   ;
inner_option_define_identifier
       : COMMA all_type_define identifier inner_option_define_identifier        
	    { 
		   $$ = new CSyntaxNode(PARAMETER_EXP, $3, $4, $2);
		}
		|COMMA all_type_define inner_option_define_identifier  // ����ָ����β��б����û�в�������       
	    { 
		   $$ = NULL;
		}
	   |COMMA ID identifier inner_option_define_identifier
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $2, $3, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
			pChild0=NULL;
	   }
	   |COMMA  STRUCT_TYPE identifier inner_option_define_identifier
	   {
	        CSyntaxNode* pChild0=new CSyntaxNode(STRUCT_PARAMETER_EXP, $2, $3, VOIDTYPE);
			$$=new CSyntaxNode(PARAMETER_EXP, pChild0, $4, STRUCTTYPE);
			pChild0=NULL;
	   }
	   |COMMA STRUCT_TYPE MUL identifier inner_option_define_identifier//�ṹ��ָ�����͵Ĳ���
	   {
	        CSyntaxNode* pChild0= new CSyntaxNode(STRUCTP_PARAMETER_EXP, $2, $4, VOIDTYPE);
            $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTPTYPE);
            pChild0=NULL;
	   }
	   |COMMA ID identifier ARRAY inner_option_define_identifier //�ṹ������ S a[]	 
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTTYPE);
            pChild0=NULL;
		}
	   |COMMA STRUCT_TYPE identifier ARRAY inner_option_define_identifier //�ṹ������ S a[]	 
	   {

			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, STRUCTTYPE);
            pChild0=NULL;
		}
	   
	   |COMMA all_type_define identifier ARRAY inner_option_define_identifier        
	   { 
		    CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
		    $$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $2);
            pChild0=NULL;
		}
		|COMMA all_type_define MUL identifier ARRAY inner_option_define_identifier  //ָ������ int *a[]
	    { 
		    CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $4, VOIDTYPE);
		    $$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $6, $2);
            pChild0=NULL;
		}
	  /* |COMMA all_type_define MUL identifier inner_option_define_identifier
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(POINT_PARAMETER_EXP,$4,VOIDTYPE);

	      $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $2);

		  pChild0=NULL;
		} 2015-3-7*/
		//  �����Ĳ��������Ǻ���ָ��
	   |COMMA OPEN_PAR MUL identifier CLOSE_PAR OPEN_PAR option_function_parameter_list CLOSE_PAR inner_option_define_identifier//����ָ�����͵Ĳ���
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(FUNCP_PARAMETER_EXP, $4, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $9, FUNCPTYPE);
          pChild0=NULL;
		}
	   | /* empty */                                       {$$ = NULL;}
	   
	    //�������͵Ĳ���a[4]
	   |COMMA all_type_define identifier OPEN_MPAR ari_exp CLOSE_MPAR inner_option_define_identifier  
	   
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(ARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $7, $2);
            pChild0=NULL;
	   }
	   	 //�������͵Ĳ���a[3,2]
	   |COMMA all_type_define identifier OPEN_MPAR ari_exp COMMA ari_exp CLOSE_MPAR inner_option_define_identifier
	   
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $9, $2);
            pChild0=NULL;
	   }
	    //�������͵Ĳ���a[][100]
	   |COMMA all_type_define identifier ARRAY OPEN_MPAR ari_exp CLOSE_MPAR inner_option_define_identifier
	   {
			CSyntaxNode* pChild0=new CSyntaxNode(DOUBLEARRAY_PARAMETER_EXP, $3, VOIDTYPE);
			$$ = new CSyntaxNode(PARAMETER_EXP, pChild0, $8, $2);
            pChild0=NULL;
	   }
	   |COMMA all_type_define MUL identifier inner_option_define_identifier//ָ�����͵Ĳ���*p
	   {
	      CSyntaxNode* pChild0= new CSyntaxNode(POINT_PARAMETER_EXP, $4, VOIDTYPE);
          $$=new CSyntaxNode(PARAMETER_EXP, pChild0, $5, $2);
          pChild0=NULL;
		}
	   |COMMA all_type_define MUL inner_option_define_identifier//ָ�����͵Ĳ���*p
	   {
	      $$=NULL;
		}
	   |COMMA error                                        {$$=NULL;}
	   ;

 



//���ú�����ν�ʵ�ʵ���б������ַ���  
option_function_identifier
       :ass_right inner_option_function_identifier           
	   {
			$$ = new CSyntaxNode(ACTUAL_PARAMETER_EXP, $1, $2, VOIDTYPE);
	   }
	   /*|ID OPEN_PAR option_function_identifier CLOSE_PAR  inner_option_function_identifier
	   {
	        CSyntaxNode* child0=new CSyntaxNode(FUNCTION_STA, $1, $3, NULL, NULL, VOIDTYPE);
			$$=new CSyntaxNode(ACTUAL_PARAMETER_EXP, child0, $5, VOIDTYPE);
			child0=NULL;
	   }*/
	   //��������������int, float, char�ȣ������������sizeof����
	   |all_type_define                            
	   {
	       $$ = new CSyntaxNode(ACTUAL_PARAMETER_EXP, $1);
	   }
	   | /* empty */                                       {$$=NULL;}
	   |ADDRESS error                                      {$$=NULL;}
	   ;

inner_option_function_identifier
       :COMMA ass_right inner_option_function_identifier
	   {
			$$ = new CSyntaxNode(ACTUAL_PARAMETER_EXP, $2, $3, VOIDTYPE);
	   }
	   /*|COMMA ID OPEN_PAR option_function_identifier CLOSE_PAR  inner_option_function_identifier
	   {
	        CSyntaxNode* child0=new CSyntaxNode(FUNCTION_STA, $2, $4, NULL, NULL, VOIDTYPE);
			$$=new CSyntaxNode(ACTUAL_PARAMETER_EXP, child0, $6, VOIDTYPE);
			child0=NULL;
	   }*/
	   //|COMMA ADDRESS identifier inner_option_function_identifier
	   //{
	   //		$$ = new CSyntaxNode(PARAMETER_EXP, new CSyntaxNode(ADDRESS_EXP, $3, VOIDTYPE), $4, VOIDTYPE);
	   //}
	   | /* empty */                                       {$$=NULL;}
	   |COMMA error                                        {$$=NULL;}
	   ;

//���ú�����ν�ʵ�ʵ���б������ַ��� end



statement_bpar
       :OPEN_BPAR statement CLOSE_BPAR					   {$$=$2;}
//	   |OPEN_BPAR statement				{yyerror("expecting '}'"); $$=NULL;}
	   |error CLOSE_BPAR                                   {$$=NULL;}
//	   |OPEN_BPAR CLOSE_BPAR            {yyerror("expecting statement"); $$=NULL;}
	   ;



bool_par
       :OPEN_PAR bool_exp CLOSE_PAR				   {$$=$2;}
//	   |OPEN_PAR bool_exp			{yyerror("expecting ')'"); $$=NULL;}
	   |error CLOSE_PAR									   {$$=NULL;}
//	   |OPEN_PAR CLOSE_PAR              {yyerror("expecting statement"); $$=NULL;}
	   ;


identifier
       :ID                              {$$=new CSyntaxNode(IDENT_EXP, $1, NULL, VOIDTYPE);}	   
	   |OPEN_PAR identifier CLOSE_PAR   {$$=$2;}
       ;

strliteral
       : STRLITERAL                            {$$=new CSyntaxNode(STR_EXP, $1, STRTYPE);}
       ;


intliteral
       : INTLITERAL                        {$$=new CSyntaxNode(INTEGER_EXP, $1, INTTYPE);}
       ;


charliteral
			:CHARLITERAL					{$$=new CSyntaxNode(CHARLITERAL_EXP, $1, CHARTYPE);}
			;


floatliteral
			:FLOATLITERAL					{$$=new CSyntaxNode(FLOATLITERAL_EXP, $1, FLOATTYPE);}
			;



option_frame_identifier
       : COMMA identifier option_frame_identifier        
	    {
			$$ = new CSyntaxNode( FRAME_STA, $2, $3, VOIDTYPE);
		}
	   | /* empty */                                       {$$ = NULL;}
	   |COMMA error                                        {$$=NULL;}
	   ;



option_projection
	   :                                                   {$$=NULL;}
	   |COMMA statement option_projection               
	   {
			$$ = new CSyntaxNode( PROJECTION_STA, $2, $3, VOIDTYPE);
	   }							
	   ;





imply_pre
	   :
	    
	   /*
	   ass_statement                   
	   {
			if($1->GetNType()==ASS_EQU_EXP)
				$1->SetRType(BOOLTYPE);
			$$=$1;
	   }
	   */
	   prime_bool_exp            {$$=$1;}
	   |NEGATION imply_pre              {$$=new CSyntaxNode(NEGATION_EXP, $2, BOOLTYPE);}
	   |imply_pre AND imply_pre
	                                    {$$=new CSyntaxNode(AND_EXP, $1, $3, BOOLTYPE);}
	   |imply_pre OR imply_pre
	                                    {$$=new CSyntaxNode(OR_EXP, $1, $3, BOOLTYPE);}
	   |OPEN_PAR imply_pre CLOSE_PAR    {$$=$2;}
	   ;


				 
while_statement
       :WHILE bool_par statement_bpar			{$$=new CSyntaxNode(WHILE_DO_STA, $2, $3, VOIDTYPE);}
	   ;



for_statement
       :FOR ari_exp TIMES DO statement_bpar
	                                    {$$=new CSyntaxNode(FOR_TIMES_STA, $2, $5, VOIDTYPE);}
//	   |FOR ari_exp DO statement_bpar          {yyerror("expecting TIMES"); $$=NULL;}
//	   |FOR ari_exp TIMES statement_bpar       {yyerror("expecting DO"); $$=NULL;}
//	   |FOR ari_exp statement_bpar             {yyerror("expecting TIMES DO"); $$=NULL;}
	   ;

extend_for_statement
       :FOR OPEN_PAR for_sta_init COMMA for_sta_condition COMMA for_sta_control  CLOSE_PAR  statement_bpar
	   {
	     CSyntaxNode* pchild0=new CSyntaxNode(FOR_STA_CON, $3, $5, $7, VOIDTYPE); 
		 $$=new CSyntaxNode(FOR_STA, pchild0, $9, VOIDTYPE);
		 pchild0=NULL;
	   }
       ;
for_sta_init//for��������еĳ�ʼ�����,�����Ǹ�ֵ��䣬Ҳ�������������,Ҳ���Բ�д
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
for_sta_condition//for��������е�������䣬��һ���������ʽҲ���Բ�д
      :bool_exp
	  {
	      $$=$1;
	  }
	  |/*empty*/
	  {
	      $$=NULL;
	  }
	  ;
for_sta_control//for��������еĿ�����䣬��һ����ֵ���Ҳ���Բ�д
      :ass_statement
	  {
	     $$=$1;
	  }
	  |/*empty*/
	  {
	      $$=NULL;
	  }
	  ;
//add by yubin 2015-3-23
switch_statement
       :SWITCH OPEN_PAR member_in_exp CLOSE_PAR OPEN_BPAR case_par CLOSE_BPAR 
	   {$$=new CSyntaxNode(SWITCH_STA, $3, $6,VOIDTYPE);}
	   ;   
case_par
       :CASE member_in_exp COLON OPEN_PAR statement CLOSE_PAR init_case_par 
	   {
	     $$=new CSyntaxNode(CASE_STA, $2, $5, $7, VOIDTYPE);
	   }
	   |DEFAULT COLON OPEN_PAR statement CLOSE_PAR //default���ֻ��д�����һ��,�Һ��治�ܼ�break���
	   {
	     $$=new CSyntaxNode(DEFAULT_STA, $4, VOIDTYPE);
	   }
	   //|  {$$=NULL;}
       ;
init_case_par
       :BREAK case_par
	   {
			$$=new CSyntaxNode(INIT_CASE_STA, $2, BREAKTYPE);
		}
	   |case_par  //modified by yubin  
	   {
			$$=new CSyntaxNode(INIT_CASE_STA, $1, VOIDTYPE);
	    }
	   |{$$=NULL;}
	   ;

if_statement
       : IF bool_exp THEN  statement_bpar option_else_statement
        {
            $$ = new CSyntaxNode( IF_ELSE_STA, $2, $4, $5, VOIDTYPE);
        }
//	   | IF bool_par sma_par option_else_statement  {yyerror("Expecting THEN");$$=NULL;}
	   ;



option_else_statement
       : ELSE statement_bpar					                   {$$=$2;}
       | /* empty */                                       {$$=NULL;}
       ;



output_statement
       :DISPLAY OPEN_PAR ass_right CLOSE_PAR   {$$=new CSyntaxNode(DISPLAY_STA, $3, UNKNOWNTYPE);}
	   |DISPLAY OPEN_PAR ass_right option_output CLOSE_PAR   {$$=new CSyntaxNode(DISPLAY_STA, $3, $4, UNKNOWNTYPE);}
	   ;

option_output
       :								{$$=NULL;}
	   |COMMA ass_right option_output     {$$=new CSyntaxNode(DISPLAY_STA, $2, $3, UNKNOWNTYPE);}
	   ;

 
input_statement 
		:REQUEST OPEN_PAR identifier CLOSE_PAR {$$=new CSyntaxNode(REQUEST_STA, $3, UNKNOWNTYPE);}
		|REQUEST OPEN_PAR identifier option_input CLOSE_PAR 	{$$=new CSyntaxNode(REQUEST_STA, $3, $4, UNKNOWNTYPE);}
		|REQUEST OPEN_PAR array CLOSE_PAR  {$$=new CSyntaxNode(REQUEST_STA, $3, UNKNOWNTYPE);}
		|REQUEST OPEN_PAR array option_input CLOSE_PAR  {$$=new CSyntaxNode(REQUEST_STA, $3, $4, UNKNOWNTYPE);}
	    |REQUEST OPEN_PAR struct_member_exp CLOSE_PAR  {$$=new CSyntaxNode(REQUEST_STA, $3, UNKNOWNTYPE);}
		;
option_input
		:								{$$=NULL;}
	    |COMMA identifier option_input{$$=new CSyntaxNode(REQUEST_STA, $2, $3, UNKNOWNTYPE);}
	    |COMMA array option_input		{$$=new CSyntaxNode(REQUEST_STA, $2, $3, UNKNOWNTYPE);}
		;


// s.a->b[2].a   �ṹ�崮��
struct_member_exp 
		:identifier	 DOT identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|array        DOT identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|point_exp DOT identifier option_struct_member_exp                {$$=new CSyntaxNode(STRUCT_IDENT_EXP, $1, $3, $4, VOIDTYPE);}

		|identifier	 DOT array option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|array        DOT array option_struct_member_exp						{$$=new CSyntaxNode(STRUCT_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|point_exp DOT array option_struct_member_exp                {$$=new CSyntaxNode(STRUCT_LIST_EXP, $1, $3, $4, VOIDTYPE);}

		|identifier	 IMPLY identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|array        IMPLY identifier option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $1, $3, $4, VOIDTYPE);}
		|OPEN_PAR address_exp CLOSE_PAR IMPLY identifier option_struct_member_exp                {$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $2, $5, $6, VOIDTYPE);}
		|point_exp IMPLY identifier option_struct_member_exp                {$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $1, $3, $4, VOIDTYPE);}

		|identifier	 IMPLY array option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|array        IMPLY array option_struct_member_exp						{$$=new CSyntaxNode(STRUCTP_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|OPEN_PAR address_exp CLOSE_PAR  IMPLY array option_struct_member_exp     {$$=new CSyntaxNode(STRUCTP_LIST_EXP, $2, $5, $6, VOIDTYPE);}
		|identifier DOT function {$$=new CSyntaxNode(STRUCT_FUNC_EXP, $1, $3, VOIDTYPE);} //�ṹ���Ա�Ǻ���ָ��
		|identifier IMPLY function {$$=new CSyntaxNode(STRUCTP_FUNC_EXP, $1, $3, VOIDTYPE);} //�ṹ���Ա�Ǻ���ָ��
		|point_exp   IMPLY array option_struct_member_exp                {$$=new CSyntaxNode(STRUCTP_LIST_EXP, $1, $3, $4, VOIDTYPE);}
		|OPEN_PAR struct_member_exp CLOSE_PAR {$$=$2;}
		;
		
option_struct_member_exp
		:	{$$=NULL;}
		|DOT identifier		option_struct_member_exp      {$$=new CSyntaxNode(STRUCT_IDENT_EXP, $2, $3, VOIDTYPE);}
		|DOT array			option_struct_member_exp            {$$=new CSyntaxNode(STRUCT_LIST_EXP, $2, $3, VOIDTYPE);}
		|IMPLY identifier   option_struct_member_exp    {$$=new CSyntaxNode(STRUCTP_IDENT_EXP, $2, $3, VOIDTYPE);}
		|IMPLY array			option_struct_member_exp		  {$$=new CSyntaxNode(STRUCTP_LIST_EXP, $2, $3, VOIDTYPE);}
		|DOT function {$$=new CSyntaxNode(STRUCT_FUNC_EXP, $2, NULL, VOIDTYPE);}    //�ṹ���Ա�Ǻ���ָ��
		|IMPLY function {$$=new CSyntaxNode(STRUCTP_FUNC_EXP, $2, NULL, VOIDTYPE);}  //�ṹ���Ա�Ǻ���ָ��
		;
