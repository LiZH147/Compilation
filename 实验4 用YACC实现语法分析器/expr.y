%{ 

#include <ctype.h>
#include <stdio.h>

/*YYSTYPE������ջ��Ԫ�ص����ͣ������������Ƕ���,yyparse��ʹ�ø����ʹ�������ջ�ȵ�*/
#define YYSTYPE double

int successFlag = 1;

/*yyerror������yacc�涨��һ����������yyparse�����л���øú��������ú����Ķ����������ṩ�ġ��ú�������*/
void yyerror( char * ErrStr )
{
    printf("������Ϣ:%s\n", ErrStr);
}

%}

%start  program

%token  NUMBER

%left   '+' '-'
%left   '*' '/'
%right  UMINUS

%token ID
%token RELOP_TKN  
%token RELOP_LT    
%token RELOP_LE    
%token RELOP_EQ    
%token RELOP_NE   
%token RELOP_GT    
%token RELOP_GE   


%token IF  
%token ELSE
%token DO
%token WHILE
%token RELOP
%token BREAK 

%%


program : block {printf("program-->block\n");}
         ;
block : '{' stmts '}' {printf("block-->{stmts}\n");}
      ;
stmts : stmt stmts {printf("stmts-->stmt stmts\n");}
      | /*empty*/  { printf("stmts->empty\n");}
      ;
stmt  : ID '=' expr ';' {printf("stmt-->id-expr\n");}
      | IF '(' bool ')' stmt {printf("stmt-->if(bool)stmt\n");}
      | IF '(' bool ')' stmt ELSE stmt {printf("stmt-->if(bool)stmt else stmt\n");}
      | WHILE '(' bool ')' stmt {printf("stmt-->while(bool) stmt\n");}
      | DO stmt WHILE '(' bool ')' ';' {printf("stmt-->do stmt while(bool);\n");}
      | BREAK ';' {printf("stmt-->break;\n");}
      | block {printf("stmt-->block\n");}
      ;
bool  : expr RELOP expr {printf("bool-->expr relop expr\n");}
      | expr {printf("bool-->expr\n");}
      ;
expr  : expr '+' expr { $$ = $1 + $3;
                        printf("����ʽ��expr->expr+expr\n"); }
      | expr '-' expr { $$ = $1 - $3; 
                        printf("����ʽ��expr->expr-expr\n"); }
      | expr '*' expr { $$ = $1 * $3; 
                        printf("����ʽ��expr->expr*expr\n"); }
      | expr '/' expr { $$ = $1 / $3; 
                        printf("����ʽ��expr->expr/expr\n"); }
      | '(' expr ')'  { $$ = $2; 
                        printf("����ʽ��expr->(expr)\n");}
      | '-' expr %prec UMINUS { $$ = - $2; 
                        printf("����ʽ��expr-> -expr\n");}
      | NUMBER        { $$ = $1;
                        printf("����ʽ��expr-> NUMBER\n"); 
                        /* "%f"��"%g"���������������"%g"������������0, */ }
      | ID            { printf("expr-->id\n"); }
      ;
%%

/*�����lex.yy.c������y.tab.c�У��ڹ����о�ֻ��Ҫy.tab.c����Ҫ����lex.yy.c������yylex�Ⱥ������ظ�����*/
#include "lex.yy.c"

int yyparse();  /*main����Ҫ����yyparse()���������ú����Ķ����ں��棬����Ҫ������(��������)*/

int main()
{ char filename[1000];

	printf("������Ҫ�����Դ�����ļ�����"); gets(filename);
      /*��Ϊlex.yy.c��������y.tab.c�У����Կ���ֱ��ʹ��BeginCompileOneFile������
        ���򣬾�Ҫ��main����ǰ��д����: void BeginCompileOneFile(const char *); */
	BeginCompileOneFile( filename );

        if( yyparse()==0 && successFlag==1 ) 
            printf("Successful!\n");
        else
            printf("Failed!\n");

	EndCompileOneFile();

	getchar();
      return 0;
}





   