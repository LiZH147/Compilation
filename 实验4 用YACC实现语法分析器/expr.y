%{ 

#include <ctype.h>
#include <stdio.h>

/*YYSTYPE是属性栈的元素的类型，该类型由我们定义,yyparse会使用该类型创建属性栈等等*/
#define YYSTYPE double

int successFlag = 1;

/*yyerror函数是yacc规定的一个函数，在yyparse函数中会调用该函数，但该函数的定义是我们提供的。该函数如下*/
void yyerror( char * ErrStr )
{
    printf("错误信息:%s\n", ErrStr);
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
                        printf("产生式：expr->expr+expr\n"); }
      | expr '-' expr { $$ = $1 - $3; 
                        printf("产生式：expr->expr-expr\n"); }
      | expr '*' expr { $$ = $1 * $3; 
                        printf("产生式：expr->expr*expr\n"); }
      | expr '/' expr { $$ = $1 / $3; 
                        printf("产生式：expr->expr/expr\n"); }
      | '(' expr ')'  { $$ = $2; 
                        printf("产生式：expr->(expr)\n");}
      | '-' expr %prec UMINUS { $$ = - $2; 
                        printf("产生式：expr-> -expr\n");}
      | NUMBER        { $$ = $1;
                        printf("产生式：expr-> NUMBER\n"); 
                        /* "%f"和"%g"都是输出浮点数，"%g"不会输出多余的0, */ }
      | ID            { printf("expr-->id\n"); }
      ;
%%

/*如果把lex.yy.c包含在y.tab.c中，在工程中就只需要y.tab.c，不要再有lex.yy.c，否则yylex等函数会重复定义*/
#include "lex.yy.c"

int yyparse();  /*main函数要调用yyparse()函数，但该函数的定义在后面，所以要先声明(才能引用)*/

int main()
{ char filename[1000];

	printf("请输入要编译的源程序文件名："); gets(filename);
      /*因为lex.yy.c被包含在y.tab.c中，所以可以直接使用BeginCompileOneFile函数。
        否则，就要在main函数前面写声明: void BeginCompileOneFile(const char *); */
	BeginCompileOneFile( filename );

        if( yyparse()==0 && successFlag==1 ) 
            printf("Successful!\n");
        else
            printf("Failed!\n");

	EndCompileOneFile();

	getchar();
      return 0;
}





   