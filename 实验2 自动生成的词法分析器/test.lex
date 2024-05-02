
%{
#include <stdio.h>
#include <stdlib.h>


#define ID_TKN      500
#define NUM_TKN     510
#define INT_TKN		511

#define RELOP_TKN   520
#define RELOP_LT    521
#define RELOP_LE    522
#define RELOP_EQ    523
#define RELOP_NE    524
#define RELOP_GT    525
#define RELOP_GE    526
#define RELOP_NGE	527
#define RELOP_GGE	528


#define IF_TKN      610
#define ELSE_TKN    612
#define WHILE_TKN	615
#define DO_TKN		616
#define FALSE_TKN	617
#define TRUE_TKN	618

/*书上使用该变量yylval，但该变量不是flex自带的，所以我们要自己定义这个变量*/
int yylval;

/*该函数可以将变量添加到符号表中，在这里我们没有实现该功能*/
int installID() ;

/*该函数可以将数值添加到数值表中，在这里我们没有实现该功能*/
int installNum() ;

/*请参考下面网页中的说明*/
/* http://www.ibm.com/developerworks/cn/linux/sdk/lex/index.html */

int line = 1 ;

%}

delim    [" "\t\r\n]
ws       {delim}+
letter   [A-Za-z]
digit    [0-9]
id       {letter}({letter}|{digit})*
int_num	 {digit}+([eE][+-]?{digit}+)?
number   {digit}+(\.{digit}+)([Ee][+-]?{digit}+)?

%%

\n		   { ++line; }
{ws}       {/*no action and no return */ }
if         { printf("%d, IF:%s\n",line,yytext);   return (IF_TKN); }
else       { printf("%d, ELSE:%s\n",line,yytext); return (ELSE_TKN); }
while 	   { printf("%d, WHILE:%s\n",line,yytext); return (WHILE_TKN); } 
do 		   { printf("%d, DO:%s\n",line,yytext); return (DO_TKN); }
true 	   { printf("%d, TRUE:%s\n",line,yytext); return (TRUE_TKN); }
false 	   { printf("%d, FALSE:%s\n",line,yytext); return (FALSE_TKN); }
{id}       { yylval = (int) installID(); printf("%d, ID:%s\n",line,yytext); return (ID_TKN); }
{int_num}  { yylval = (int) installNum(); printf("%d, Int:%d\n",line,(int)strtod(yytext, NULL)); return (INT_TKN);}
{number}   { yylval = (int) installNum();printf("%d, Float:%lf\n",line,strtod(yytext, NULL)); return (NUM_TKN); } 

"<"        { yylval = RELOP_LT; printf("%d, LT:%s\n",line,yytext);  return(RELOP_TKN); }
"<="       { yylval = RELOP_LE; printf("%d, LE:%s\n",line,yytext);  return(RELOP_TKN); }
"="        { yylval = RELOP_EQ; printf("%d, ASSIGN:%s\n",line,yytext);  return(RELOP_TKN); }
"!="       { yylval = RELOP_NE; printf("%d, NE:%s\n",line,yytext);  return(RELOP_TKN); }
">"        { yylval = RELOP_GT; printf("%d, GT:%s\n",line,yytext);  return(RELOP_TKN); }
">="       { yylval = RELOP_GE; printf("%d, GE:%s\n",line,yytext);  return(RELOP_TKN); }
"=="       { yylval = RELOP_GGE; printf("%d, GGE:%s\n",line,yytext);  return(RELOP_TKN); }

[\/][\*]([^\*])*[\*]([^\*\/](([^\*])*)[\*]|[\*])*(\/)  { printf("%d, Remark:%s\n",line,yytext); }

.          { printf("%d, %c:%c\n",line,yytext[0],yytext[0]); return yytext[0]; }

%%

/*该函数设置yyin变量，fflex对yyin变量所对应的文件进行词法分析*/
void BeginCompileOneFile( const char * filename )
{  
   yyin = fopen( filename, "r");
   fseek( yyin, 0, SEEK_SET );

/* 缺省情况下，yyin 和 yyout 都指向标准输入和输出。
   yyout = fopen( outputfilename, "w");
   fseek( yyout, 0, SEEK_SET );
*/
}

void EndCompileOneFile(void) 
{
   fclose(yyin);
   yyin = 0;
}

/*如果函数的返回值是1，就停止解析。 因此它可以用来解析多个文件。*/
int yywrap()
{
	return 1;
}

int installID() 
{
   return 1;
}

int installNum() 
{
   return 1;
}


/*因为lex.yy.c是C语言写的，下面这些函数是以C语言的函数调用方式编译的
#ifdef __cplusplus
    extern "C" int yylex(void);
    extern "C" void BeginCompileOneFile( const char * filename );
    extern "C" void EndCompileOneFile( void);
#endif
*/


int main()
{ int token;
char filename[1000];

	printf("请输入要编译的源程序文件名："); gets(filename);
	BeginCompileOneFile( filename );

	//当flex扫描到文件末尾，yylex函数返回0
    while( ( token = yylex() ) > 0 ) ;

	EndCompileOneFile();

	getchar();
        return 0;
}









