#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.8 (Berkeley) 01/02/91\n\
 Modified 11/4/96 by Wensong Zhang to make getenv() function right\n\
 and to remove the warning of unreferenced yyerrlab and yynewerror labels";
#endif
#define YYBYACC 1
#line 1 "expr.y"
 

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

#line 25 "y.tab.c"
#define NUMBER 257
#define UMINUS 258
#define ID 259
#define RELOP_TKN 260
#define RELOP_LT 261
#define RELOP_LE 262
#define RELOP_EQ 263
#define RELOP_NE 264
#define RELOP_GT 265
#define RELOP_GE 266
#define IF 267
#define ELSE 268
#define DO 269
#define WHILE 270
#define RELOP 271
#define BREAK 272
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    1,    2,    2,    3,    3,    3,    3,    3,    3,
    3,    5,    5,    4,    4,    4,    4,    4,    4,    4,
    4,
};
short yylen[] = {                                         2,
    1,    3,    2,    0,    4,    5,    7,    5,    7,    2,
    1,    3,    1,    3,    3,    3,    3,    3,    2,    1,
    1,
};
short yydefred[] = {                                      0,
    0,    0,    1,    0,    0,    0,    0,    0,   11,    0,
    0,    0,    0,    0,    0,   10,    2,    3,   20,    0,
   21,    0,    0,    0,    0,    0,    0,   19,    0,    0,
    0,    0,    0,    5,    0,    0,    0,    0,   18,    0,
    0,   16,   17,    0,    0,    0,    8,    0,    0,    7,
    9,
};
short yydgoto[] = {                                       2,
    9,   10,   11,   24,   25,
};
short yysindex[] = {                                   -102,
 -109,    0,    0,  -38,  -11, -109,   -9,  -34,    0,  -92,
 -109,  -32,  -32, -232,  -32,    0,    0,    0,    0,  -32,
    0,  -32,  -15,  -36,   -1,    1,    2,    0,    7,  -32,
  -32,  -32,  -32,    0,  -32, -109,  -32, -109,    0,  -25,
  -25,    0,    0,   13, -222,   10,    0, -109,   -6,    0,
    0,
};
short yyrindex[] = {                                      0,
  -66,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -66,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   20,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -41,
  -40,    0,    0,   21, -113,    0,    0,    0,    0,    0,
    0,
};
short yygindex[] = {                                      0,
   63,   53,    9,    4,    5,
};
#define YYTABLESIZE 235
short yytable[] = {                                      14,
   15,   14,   15,   14,   15,   32,   30,   22,   31,    6,
   33,    6,   20,    1,   14,   23,   32,   14,   15,   27,
    1,   33,   12,   28,   16,   29,   32,   30,   13,   31,
   15,   33,   17,   40,   41,   42,   43,   26,   44,   36,
   37,   46,   38,   34,   45,   48,   47,   39,   32,   30,
   49,   31,   51,   33,   32,   30,   50,   31,    4,   33,
   13,   12,    3,   18,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    6,    0,    0,    0,    4,
    0,    0,    0,    6,    0,    6,    6,    5,    6,    6,
    7,    0,    8,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   19,    0,   21,    0,    0,   14,
   15,    0,    0,    0,   35,
};
short yycheck[] = {                                      41,
   41,   43,   43,   45,   45,   42,   43,   40,   45,  123,
   47,  125,   45,  123,    6,   12,   42,   59,   59,   15,
  123,   47,   61,   20,   59,   22,   42,   43,   40,   45,
   40,   47,  125,   30,   31,   32,   33,  270,   35,   41,
   40,   37,   41,   59,   36,  268,   38,   41,   42,   43,
   41,   45,   59,   47,   42,   43,   48,   45,  125,   47,
   41,   41,    0,   11,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  259,   -1,   -1,   -1,  259,
   -1,   -1,   -1,  267,   -1,  269,  270,  267,  272,  269,
  270,   -1,  272,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  257,   -1,  259,   -1,   -1,  271,
  271,   -1,   -1,   -1,  271,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 272
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'",0,"'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,"';'",
0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"NUMBER","UMINUS","ID","RELOP_TKN","RELOP_LT","RELOP_LE","RELOP_EQ",
"RELOP_NE","RELOP_GT","RELOP_GE","IF","ELSE","DO","WHILE","RELOP","BREAK",
};
char *yyrule[] = {
"$accept : program",
"program : block",
"block : '{' stmts '}'",
"stmts : stmt stmts",
"stmts :",
"stmt : ID '=' expr ';'",
"stmt : IF '(' bool ')' stmt",
"stmt : IF '(' bool ')' stmt ELSE stmt",
"stmt : WHILE '(' bool ')' stmt",
"stmt : DO stmt WHILE '(' bool ')' ';'",
"stmt : BREAK ';'",
"stmt : block",
"bool : expr RELOP expr",
"bool : expr",
"expr : expr '+' expr",
"expr : expr '-' expr",
"expr : expr '*' expr",
"expr : expr '/' expr",
"expr : '(' expr ')'",
"expr : '-' expr",
"expr : NUMBER",
"expr : ID",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#ifdef YYSTACKSIZE
#ifndef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#endif
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 600
#define YYMAXDEPTH 600
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 83 "expr.y"

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





   
#line 237 "y.tab.c"
#define YYABORT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("yydebug: state %d, reading %d (%s)\n", yystate,
                    yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("yydebug: state %d, shifting to state %d\n",
                    yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
yynewerror:
#endif
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
yyerrlab:
#endif
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("yydebug: state %d, error recovery shifting\
 to state %d\n", *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("yydebug: error recovery discarding state %d\n",
                            *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("yydebug: state %d, error recovery discards token %d (%s)\n",
                    yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("yydebug: state %d, reducing by rule %d (%s)\n",
                yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 47 "expr.y"
{printf("program-->block\n");}
break;
case 2:
#line 49 "expr.y"
{printf("block-->{stmts}\n");}
break;
case 3:
#line 51 "expr.y"
{printf("stmts-->stmt stmts\n");}
break;
case 4:
#line 52 "expr.y"
{ printf("stmts->empty\n");}
break;
case 5:
#line 54 "expr.y"
{printf("stmt-->id-expr\n");}
break;
case 6:
#line 55 "expr.y"
{printf("stmt-->if(bool)stmt\n");}
break;
case 7:
#line 56 "expr.y"
{printf("stmt-->if(bool)stmt else stmt\n");}
break;
case 8:
#line 57 "expr.y"
{printf("stmt-->while(bool) stmt\n");}
break;
case 9:
#line 58 "expr.y"
{printf("stmt-->do stmt while(bool);\n");}
break;
case 10:
#line 59 "expr.y"
{printf("stmt-->break;\n");}
break;
case 11:
#line 60 "expr.y"
{printf("stmt-->block\n");}
break;
case 12:
#line 62 "expr.y"
{printf("bool-->expr relop expr\n");}
break;
case 13:
#line 63 "expr.y"
{printf("bool-->expr\n");}
break;
case 14:
#line 65 "expr.y"
{ yyval = yyvsp[-2] + yyvsp[0];
                        printf("产生式：expr->expr+expr\n"); }
break;
case 15:
#line 67 "expr.y"
{ yyval = yyvsp[-2] - yyvsp[0]; 
                        printf("产生式：expr->expr-expr\n"); }
break;
case 16:
#line 69 "expr.y"
{ yyval = yyvsp[-2] * yyvsp[0]; 
                        printf("产生式：expr->expr*expr\n"); }
break;
case 17:
#line 71 "expr.y"
{ yyval = yyvsp[-2] / yyvsp[0]; 
                        printf("产生式：expr->expr/expr\n"); }
break;
case 18:
#line 73 "expr.y"
{ yyval = yyvsp[-1]; 
                        printf("产生式：expr->(expr)\n");}
break;
case 19:
#line 75 "expr.y"
{ yyval = - yyvsp[0]; 
                        printf("产生式：expr-> -expr\n");}
break;
case 20:
#line 77 "expr.y"
{ yyval = yyvsp[0];
                        printf("产生式：expr-> NUMBER\n"); 
                        /* "%f"和"%g"都是输出浮点数，"%g"不会输出多余的0, */ }
break;
case 21:
#line 80 "expr.y"
{ printf("expr-->id\n"); }
break;
#line 468 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("yydebug: after reduction, shifting from state 0 to\
 state %d\n", YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("yydebug: state %d, reading %d (%s)\n",
                        YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("yydebug: after reduction, shifting from state %d \
to state %d\n", *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
