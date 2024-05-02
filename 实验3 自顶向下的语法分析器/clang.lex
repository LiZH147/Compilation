
%{

#include "lex.yy.h"

/*书上使用该变量yylval，但该变量不是flex自带的，所以我们要自己定义这个变量*/
int yylval;
int lineNo=1;

/*请参考下面网页中的说明*/
/* http://www.ibm.com/developerworks/cn/linux/sdk/lex/index.html */

%}

delim    [" "\t]
ws       {delim}+
letter   [A-Za-z]
digit    [0-9]
id       {letter}({letter}|{digit})*
number   {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%

{ws}       {/*no action and no return */ }
"\n"       {lineNo++;}
if         { printf("IF:%s\n",yytext);   return (IF_TKN); }
else       { printf("ELSE:%s\n",yytext); return (ELSE_TKN); }
break      { printf("BREAK:%s\n",yytext); return (BREAK_TKN); }
while      { printf("WHILE:%s\n",yytext); return (WHILE_TKN); }
do         { printf("DO:%s\n",yytext); return (DO_TKN); }
{id}       { printf("ID:%s\n",yytext); return (ID_TKN); }
{number}   { printf("Num:%s\n",yytext); return (NUM_TKN); } 

"<"        { yylval = RELOP_LT; printf("LT:%s\n",yytext);  return(RELOP_TKN); }
"<="       { yylval = RELOP_LE; printf("LE:%s\n",yytext);  return(RELOP_TKN); }
"=="       { yylval = RELOP_EQ; printf("EQ:%s\n",yytext);  return(RELOP_TKN); }
"!="       { yylval = RELOP_NE; printf("NE:%s\n",yytext);  return(RELOP_TKN); }
">"        { yylval = RELOP_GT; printf("GT:%s\n",yytext);  return(RELOP_TKN); }
">="       { yylval = RELOP_GE; printf("GE:%s\n",yytext);  return(RELOP_TKN); }

[\/][\*]([^\*])*[\*]([^\*\/](([^\*])*)[\*]|[\*])*(\/)  { printf("备注:%s\n",yytext); }

.          { printf("TKN:%s\n",yytext); return yytext[0]; }

%%

/*该函数设置yyin变量，fflex对yyin变量所对应的文件进行词法分析*/
void BeginCompileOneFile( const char * filename )
{  
   yyin = fopen( filename, "r");
   fseek( yyin, 0, SEEK_SET );

}

void EndCompileOneFile(void) 
{
   fclose(yyin);
   yyin = 0;
}

int yywrap()
{
	return 1;
}

/*返回：词法分析当前处理到的行号，一般在句法分析报错时使用*/
int GetLineNo(void)
{
    return lineNo;
}










