
%{
#include <stdio.h>
#include <stdlib.h>



/*参考网页: http://www.ibm.com/developerworks/cn/linux/sdk/lex/index.html */




%}

delim    [ \t]
ws       {delim}+
letter   [A-Za-z]
digit    [0-9]
id       {letter}({letter}|{digit})*
character "'"."'"
integer  {digit}+ 
number   {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%

{ws}       {/*no action and no return */ }
\n         {LineNo++; /*printf("行号：%d\n",LineNo);*/ }

if         {   return (IF);   }
else       {   return (ELSE); }
while      {   return (WHILE);}
do         {   return (DO);   }
break      {   return (BREAK);}


char       {  yylval.basic.type=CHAR;   return (BASIC);}
int        {  yylval.basic.type=INT;    return (BASIC);}
float      {  yylval.basic.type=FLOAT;  return (BASIC);}
bool       {  yylval.basic.type=BOOL;   return (BASIC);}

{character} { yylval.constval.str[0] = yytext[1]; yylval.constval.str[1]='\0';  /*str用于演示用*/
              yylval.constval.type = CHAR;    
              memset( &yylval.constval.value, 0, sizeof( yylval.constval.value) );
              yylval.constval.value.ch = yytext[1];
              printf("字符常量：%c\n", yylval.constval.value.ch );
              yylval.constval.width = CHAR_WIDTH;
              return CONST; 
            }

{integer}   { strcpy(yylval.constval.str, yytext); /*str用于演示用*/
              yylval.constval.type = INT; 
              memset( &yylval.constval.value, 0, sizeof( yylval.constval.value) );
              sscanf( yytext, "%d",&yylval.constval.value.n);
              printf("整型常量：%d\n", yylval.constval.value.n );
              yylval.constval.width = INT_WIDTH;
              return CONST; 
            }

{number}    { strcpy(yylval.constval.str, yytext);  /*str用于演示用*/
              yylval.constval.type = FLOAT;
              memset( &yylval.constval.value, 0, sizeof( yylval.constval.value) );
              sscanf( yytext, "%lf", &yylval.constval.value.f );
              printf("浮点数常量：%f\n", yylval.constval.value.f );
              yylval.constval.width = FLOAT_WIDTH;
              return CONST; 
            } 

true        { strcpy(yylval.constval.str, yytext); /*str用于演示用*/
              yylval.constval.type = BOOL;
              memset( &yylval.constval.value, 0, sizeof( yylval.constval.value) );
              yylval.constval.value.n = 1;
              printf("布尔常量:true\n");
              yylval.constval.width = BOOL_WIDTH;
              return CONST; 
            }

false       { strcpy(yylval.constval.str, yytext); /*str用于演示用*/
              yylval.constval.type = BOOL;
              memset( &yylval.constval.value, 0, sizeof( yylval.constval.value) );
              yylval.constval.value.n = 0;
              printf("布尔常量:false\n");
              yylval.constval.width = BOOL_WIDTH;
              return CONST; 
            }

{id}       {  strncpy( yylval.id.name, yytext, ID_MAX_LEN );yylval.id.name[ID_MAX_LEN]='\0'; return (ID); }

"<"        {  return(RELOP_LT);  }
"<="       {  return(RELOP_LE);  }
">"        {  return(RELOP_GT);  }
">="       {  return(RELOP_GE);  }

"=="       {  return(RELOP_EQ);  }
"!="       {  return(RELOP_NEQ); }

"||"       {  return OR;  }
"&&"       {  return AND; }



[\/][\*]([^\*])*[\*]([^\*\/](([^\*])*)[\*]|[\*])*(\/)  { printf("备注:%s\n",yytext); }

.       {  return yytext[0]; }

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









