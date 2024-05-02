
%{
#include <stdio.h>
#include <stdlib.h>

int lineNo = 1;

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
{number}   { sscanf( yytext, "%lf", &yylval ); return (NUMBER); } 
if         { return (IF); }
while      { return (WHILE); }
do         { return (DO); }
break      { return (BREAK); }
else       { return (ELSE); }
\n         { lineNo++; }
{id}       { return (ID); }


"<"        { yylval = RELOP_LT; return(RELOP); }
"<="       { yylval = RELOP_LE; return(RELOP); }
"=="       { yylval = RELOP_EQ; return(RELOP); }
"<>"       { yylval = RELOP_NE; return(RELOP); }
">"        { yylval = RELOP_GT; return(RELOP); }
">="       { yylval = RELOP_GE; return(RELOP); }

[\/][\*]([^\*])*[\*]([^\*\/](([^\*])*)[\*]|[\*])*(\/)  { printf("备注:%s\n",yytext); }

.          { return yytext[0]; }

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









