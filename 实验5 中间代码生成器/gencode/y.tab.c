#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.8 (Berkeley) 01/02/91\n\
 Modified 11/4/96 by Wensong Zhang to make getenv() function right\n\
 and to remove the warning of unreferenced yyerrlab and yynewerror labels";
#endif
#define YYBYACC 1
#line 1 "compile.y"
 
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int LineNo = 1; /*当前lookahead所指向的字符所在的行号，即当前编译到的行号*/

int CompileFailed = 0;

void yyerror( char * ErrStr )
{
    CompileFailed = 1; /*编译失败标志*/
    printf("错误信息:%s, 行号:%d\n", ErrStr, LineNo);
}



/*变量和常量的基本类型BASIC*/
#define CHAR     1
#define INT      2
#define FLOAT    3
#define BOOL     4

#define CHAR_WIDTH  1
#define INT_WIDTH   4
#define FLOAT_WIDTH 8  
#define BOOL_WIDTH  1

/*****************************下面：符号表的定义和相关函数*******************************/

/*变量名长度不超过ID_MAX_LEN 个字符*/
#define ID_MAX_LEN   64

/*存放一个标识符*/
struct SymbolElem {
    char name[ ID_MAX_LEN + 1 ]; /*符号名(例如变量名)长度不超过ID_MAX_LEN 个字符*/
    int type; /*用来存放类型名，例如int, 这个程序只处理简单类型，在实际的编译器中，这里要建立树结构*/
    int  addr;      /*为该变量分配的空间的首地址*/
	int  width;     /*该变量的宽度，即占用多少个字节*/
    struct SymbolElem * next;  /*指向下一个标识符*/
};

/*标识符表*/
typedef struct SymbolList{
    struct SymbolElem * head;  /*指向符号表（用链表实现）的第一个结点，没有头结点,初始化为NULL*/
    struct SymbolList * prev; /*上一层的符号表*/
    int beginaddr; /*该符号表中分配给变量和临时变量空间的开始地址*/
    int endaddr;    /*该符号表中分配给变量和临时变量空间的结束地址*/
                   /*beginaddr~endaddr的空间存放该符号表的所有变量和临时变量*/
} * SymbolList;  /*符号表*/

SymbolList TopSymbolList=NULL; /*全局变量，存放当前的符号表（即当前处于最顶层的符号表）,对应为书上的top*/

/*创建并返回一个新的符号表（SymbolList就是书上的Env），PrevList是其的上一层符号表*/
SymbolList CreateSymbolList( SymbolList PrevList, int StartAddr )
{ SymbolList list;
    list = (SymbolList) malloc( sizeof(struct SymbolList) );
    memset( list, 0, sizeof( struct SymbolList ) );
    list->prev = PrevList;
	list->endaddr = list->beginaddr = StartAddr;

    return list;
}

void DestroySymbolList( SymbolList List )
{struct SymbolElem * p, *q;
    
    if( List == NULL) return;
    p = List->head;
    while( p!=NULL ) {
        q = p->next; free(p); p=q;
    }
    free(List);    
}

/*在符号表List中查找是否存在标识符IdName，如果存在，则返回该结点指针，否则返回空*/
struct SymbolElem * LookUpSymbolList( SymbolList List, char * IdName )
{struct SymbolElem * p;
    if( List==NULL ) return NULL;
    for( p = List->head; p!=NULL; p = p->next ) 
        if( strcmp( p->name, IdName ) == 0 ) break;
    return p;
}

/*从符号表List开始并不断地往上一层符号表中查找是否存在标识符IdName，如果存在，则返回该结点指针，否则返回空*/
struct SymbolElem * LookUpAllSymbolList( SymbolList List, char * IdName )
{ SymbolList env;
struct SymbolElem * p;
    env = List;
    while( env!=NULL ) {
        p = LookUpSymbolList( env, IdName );
        if(  p != NULL ) return p; /*找到该符号*/
        env = env->prev;
    }
    return NULL;
}


/*创建一个新的符号结点,并添加到符号表中，而后返回该结点指针*/
struct SymbolElem * AddToSymbolList( SymbolList List, char * IdName,int IdType, int Width )
{struct SymbolElem * p;

    p = (struct SymbolElem *) malloc( sizeof(struct SymbolElem) );

    strcpy( p->name, IdName );
    p->type = IdType;
	p->width = Width;
	p->addr = List->endaddr;
	List->endaddr += Width;

    p->next = List->head;  /*将该标识符添加到符号表表头*/
    List->head = p;

    return p;    
}

void PrintSymbolList( SymbolList List )
{struct SymbolElem * p;
    printf("***********************变量列表*************************\n");
    if( List ==NULL ) return ;
    for( p=List->head; p!=NULL; p=p->next ) {
        printf("变量名:%s, 类型:", p->name);
		switch( p->type ) {
            case CHAR : printf("char");  break;
            case INT  : printf("int");   break;
            case FLOAT: printf("float"); break;
            case BOOL : printf("bool");  break;
		}
        printf(", 地址:%d, 宽度:%d\n", p->addr, p->width );
	}
    printf("*************该变量列表共占用%d个字节空间***************\n", List->endaddr - List->beginaddr);
}

/*分配一个临时变量,返回临时变量的地址、临时变量的名称*/
int NewTemp( SymbolList List, char Name[], int Width )
{ static int TempID = 1;
  int addr;
    sprintf( Name, "T%d", TempID++ ); /*例如T1，T2等*/
	addr = List->endaddr;
    List->endaddr += Width;
  
    return addr;
}

/*****************************上面：符号表的定义和相关函数*****************************/




/*****************************下面：常数表的定义和相关函数*******************************/

union ConstVal {
        char    ch;    /*存放字符常量*/
        int     n;     /*存放整型常量，或true=1，false=0 */
        double  f;     /*存放浮点数常量*/
};	

/*存放一个常数*/
struct ConstElem {
    char str[ID_MAX_LEN + 1 ]; /*该变量用于存储常数的文本形式，演示的时候用的,实际的编译系统不需要*/	   
    int type; /*用来存放类型名，例如int*/
    union ConstVal value;
    int  addr;      /*为该常量分配的空间的首地址*/
	int  width;     /*该变量的宽度，即占用多少个字节*/
    struct ConstElem * next;  /*指向下一个常量*/
};

/*常量表*/
struct ConstList{
    struct ConstElem * head;  /*指向常量表（用链表实现）的第一个结点，没有头结点,初始化为NULL*/
    int beginaddr;  /*该符号表中分配给常量空间的开始地址*/
    int endaddr;    /*该符号表中分配给常量空间的结束地址*/
                   /*beginaddr~endaddr的空间存放该常量表的所有常量*/
} ConstList ;  /*常量表，全局变量，注意整个程序只需要一个常量表**/


/*创建并返回常量表*/
void CreateConstList( int StartAddr )
{ 
	ConstList.head = NULL;
	ConstList.endaddr = ConstList.beginaddr = StartAddr;
}

void DestroyConstList( void )
{struct ConstElem * p, *q;
    
    p = ConstList.head;
    while( p!=NULL ) {
        q = p->next; free(p); p=q;
    }
	memset( &ConstList, 0, sizeof(struct ConstList) );
}

/*在常量表ConstList中查找是否存在常量，如果存在，则返回该结点指针，否则返回空*/
struct ConstElem * LookUpConstList( int ConstType, union ConstVal ConstValue, int Width )
{struct ConstElem * p;
    for( p = ConstList.head; p!=NULL; p = p->next ) 
        if( p->type == ConstType && memcmp( &p->value,&ConstValue, Width) == 0 )  break;
	
    return p;
}


/*创建一个新的常数结点,并添加到常数表中，而后返回该结点指针*/
struct ConstElem * AddToConstList( char * Str, int ConstType, union ConstVal ConstValue, int Width )
{struct ConstElem * p;

    p = (struct ConstElem *) malloc( sizeof(struct ConstElem) );

    strcpy( p->str, Str );
    p->type = ConstType;
    p->value = ConstValue;
	p->width = Width;

	p->addr = ConstList.endaddr;
	ConstList.endaddr += Width;

    p->next = ConstList.head;  /*将该常量添加到常量表表头*/
    ConstList.head = p;

    return p;    
}

void PrintConstList(void)
{struct ConstElem * p;
    printf("***********************常量列表*************************\n");
    for( p=ConstList.head; p!=NULL; p=p->next ) {
	    printf("常量:%s, 类型:", p->str);
		switch( p->type ) {
            case CHAR : printf("char");  break;
            case INT  : printf("int");   break;
            case FLOAT: printf("float"); break;
            case BOOL : printf("bool");  break;
		}
        printf(", 地址:%d, 宽度:%d\n", p->addr, p->width );
	}
    printf("**************该常量列表共占用%d个字节空间***************\n", ConstList.endaddr - ConstList.beginaddr);
}

/*****************************上面：常数表的定义和相关函数*****************************/





/********************************下面:四元式的定义和函数****************************/

/* 整型加减乘除 */
#define OIntAdd          1001
#define OIntSub          1002
#define OIntMultiply     1003
#define OIntDivide       1004

/* 浮点数加减乘除 */
#define OFloatAdd        1011
#define OFloatSub        1012
#define OFloatMultiply   1013
#define OFloatDivide     1014

/*赋值a=b*/
#define OIntEvaluation   1021
#define OFloatEvaluation 1022
#define OCharEvaluation  1023
#define OBoolEvaluation  1024

/* 无条件goto语句 */
#define OGoto            1031

/* if a op b goto 语句 */
#define OGTGoto          1041
#define OGEGoto          1042
#define OLTGoto          1043
#define OLEGoto          1044
#define OEQGoto          1045
#define ONEQGoto         1046

/*类型转换运算符*/
#define OCharToInt       1051
#define OCharToFloat     1052
#define OIntToFloat      1053
#define OIntToChar       1054
#define OFloatToChar     1055
#define OFloatToInt      1056
#define OCharToBool      1057
#define OIntToBool       1058
#define OFloatToBool     1059
#define OBoolToChar      1060
#define OBoolToInt       1061
#define OBoolToFloat     1062 

/*四元式数据结构*/
struct Quadruple {
    int op; /*运算符*/
    int arg1; /*存放第一个参数的地址，可能是变量、临时变量的地址*/
    int arg2;
    int arg3;/*存放第三个参数的地址，可能是变量、临时变量的地址，还可能是四元式的地址(Goto 的地址参数)*/
    char arg1name[ID_MAX_LEN + 1]; /*本不需要，用于演示时能显示arg1对应的变量或临时变量的名称(若有的话）*/
    char arg2name[ID_MAX_LEN + 1]; /*本不需要，用于演示时能显示arg2对应的变量或临时变量的名称(若有的话）*/
    char arg3name[ID_MAX_LEN + 1]; /*本不需要，用于演示时能显示arg3对应的变量或临时变量的名称(若有的话）*/
};

/*四元式表*/
struct QuadTable {
    int startaddr; /*四元式开始存放的地址,比如100*/
    struct Quadruple * base; /*指向一块内存，用来存放多个四元式，从base[0]开始存放*/
    int size; /*base中可以存放的四元式的个数*/
    int len; /*base[len]是下一个四元式要存放的空间*/

};

struct QuadTable QuadTable; /*只需要一个四元式表*/

void CreateQuadTable(int StartAddr)
{
    QuadTable.startaddr = StartAddr; 
    QuadTable.size = 1000; /* 一开始假设可以存放1000个四元式*/
    QuadTable.base = ( struct Quadruple *)malloc( QuadTable.size * sizeof(struct Quadruple) );
    QuadTable.len = 0;
}

void DestroyQuadTable( void )
{
    QuadTable.startaddr = 0; 
    QuadTable.size = 0;
    if( QuadTable.base != NULL) free(QuadTable.base); 
    QuadTable.len = 0;   
}

/*当Arg1是变量或临时变量时，Arg1Name是该变量的名称,用于演示时使用，其余参数类同 */
int Gen( int Op, int Arg1, int Arg2, int Arg3, char *Arg1Name, char *Arg2Name, char *Arg3Name )
{ struct Quadruple * ptr; 
  int incr = 100;
    if( QuadTable.len >= QuadTable.size ) {
        ptr = realloc( QuadTable.base, QuadTable.size+incr );
        if( ptr==NULL ) return -1;
        QuadTable.base = ptr;
        QuadTable.size += incr;
    }
    ptr = & QuadTable.base[QuadTable.len];
    ptr->op = Op;
    ptr->arg1 = Arg1;
    ptr->arg2 = Arg2;
    ptr->arg3 = Arg3;
    strcpy( ptr->arg1name, Arg1Name);
    strcpy( ptr->arg2name, Arg2Name);
    strcpy( ptr->arg3name, Arg3Name);
    QuadTable.len++;

    return QuadTable.len - 1;
}

/*把四元式所对应的三地址代码写入到文件中*/
void WriteQuadTableToFile( const char * FileName )
{FILE * fp;
struct Quadruple * ptr;
int i,op;
char str[1000],ch;
    fp = fopen( FileName, "w" );
    if( fp==NULL ) return;
    for( i=0, ptr = QuadTable.base; i < QuadTable.len; i++,ptr++ ) {
        fprintf(fp, "%5d:  ", QuadTable.startaddr + i);
        op = ptr->op;
        switch( op ) {
            case OIntAdd        :
            case OIntSub        :
            case OIntMultiply   :
            case OIntDivide     :
            case OFloatAdd      :
            case OFloatSub      :
            case OFloatMultiply :
            case OFloatDivide   : if( op==OIntAdd || op==OFloatAdd) ch = '+';
                                  if( op==OIntSub || op==OFloatSub) ch = '-';
                                  if( op==OIntMultiply || op==OFloatMultiply) ch = '*';
                                  if( op==OIntDivide || op==OFloatDivide) ch = '/';
                                  sprintf(str,"[%d] = [%d] %c [%d]", ptr->arg3, ptr->arg1, ch, ptr->arg2);
                                  break;
            case OIntEvaluation   :
            case OFloatEvaluation :
            case OCharEvaluation  :
            case OBoolEvaluation  : sprintf(str,"[%d] = [%d]", ptr->arg3, ptr->arg1);
                                    break;
            case OGoto            : sprintf(str,"Goto %d", ptr->arg3);
                                    break;
            case OGTGoto  : sprintf(str,"if [%d]>[%d] Goto %d", ptr->arg1, ptr->arg2, ptr->arg3 );  break;
            case OGEGoto  : sprintf(str,"if [%d]>=[%d] Goto %d", ptr->arg1, ptr->arg2, ptr->arg3 ); break;
            case OLTGoto  : sprintf(str,"if [%d]<[%d] Goto %d", ptr->arg1, ptr->arg2, ptr->arg3 );  break;
            case OLEGoto  : sprintf(str,"if [%d]<=[%d] Goto %d", ptr->arg1, ptr->arg2, ptr->arg3 ); break;
            case OEQGoto  : sprintf(str,"if [%d]==[%d] Goto %d", ptr->arg1, ptr->arg2, ptr->arg3 ); break;
            case ONEQGoto : sprintf(str,"if [%d]<>[%d] Goto %d", ptr->arg1, ptr->arg2, ptr->arg3 ); break;

            case OCharToInt   : sprintf( str,"[%d] = (int) [%d]",   ptr->arg3, ptr->arg1 );  break;
            case OCharToFloat : sprintf( str,"[%d] = (float) [%d]", ptr->arg3, ptr->arg1 );  break;
            case OIntToFloat  : sprintf( str,"[%d] = (float) [%d]", ptr->arg3, ptr->arg1 );  break;
            case OIntToChar   : sprintf( str,"[%d] = (char) [%d]",  ptr->arg3, ptr->arg1 );  break;
            case OFloatToChar : sprintf( str,"[%d] = (char) [%d]",  ptr->arg3, ptr->arg1 );  break;
            case OFloatToInt  : sprintf( str,"[%d] = (int) [%d]",   ptr->arg3, ptr->arg1 );  break;

            case OCharToBool   : sprintf( str,"[%d] = (bool) [%d]",  ptr->arg3, ptr->arg1 );  break;
            case OIntToBool    : sprintf( str,"[%d] = (bool) [%d]",  ptr->arg3, ptr->arg1 );  break;
            case OFloatToBool  : sprintf( str,"[%d] = (bool) [%d]",  ptr->arg3, ptr->arg1 );  break;
            case OBoolToChar   : sprintf( str,"[%d] = (char) [%d]",  ptr->arg3, ptr->arg1 );  break;
            case OBoolToInt    : sprintf( str,"[%d] = (int) [%d]",   ptr->arg3, ptr->arg1 );  break;
            case OBoolToFloat  : sprintf( str,"[%d] = (float) [%d]", ptr->arg3, ptr->arg1 );  break;

            default: yyerror("程序错误：出现不认识的运算符！"); strcpy(str, "error: Unknown operator");break;
        }
        fprintf(fp,"%s\n",str);
    }

    fclose(fp);
}

/********************************上面:四元式的定义和函数****************************/



/**************下面:定义句法分析栈中元素的信息，即终结符和非终结符的综合属性****************/

 union ParseStackNodeInfo{
    struct {
	    /*符号名(例如变量名)长度不超过ID_MAX_LEN 个字符*/
        /*如果建立hash表来存储所有的不同名字的标识符的名字，这里就可以使用一个指针指向该标识符的名字，
		  好处是减少分析栈中元素的空间大小，从而节省空间且提高编译效率，*/
        char name[ID_MAX_LEN + 1 ]; 
    }id;  /*标识符:终结符ID的综合属性*/

    struct {
	   char str[ID_MAX_LEN + 1 ]; /*该变量用于存储常数的文本形式，演示的时候用的,实际的编译系统不需要*/	   
       int type; /*用来存放类型名，例如INT*/
	   union ConstVal value; /*常量：终结符CONST的信息*/
	   int width;
	} constval; /*终结符const的综合属性*/

    struct {
        int type; /*用来存放类型名，例如INT*/
    }basic; /*基本数据类型：终结符BASIC的综合属性*/

	struct {
	   char str[ID_MAX_LEN + 1 ]; /*该变量用于存储变量名、临时变量名或常数的文本形式，演示的时候用的,实际的编译系统不需要*/
	   int type;
	   int addr;
	   int width;
	} factor, term, expr, rel, equality, join, bool;/*非终结符factor, term, expr, rel, equality, join, bool的综合属性*/



} ;

#define YYSTYPE union ParseStackNodeInfo 
       

/**************上面:定义句法分析栈中元素的信息，即终结符和非终结符的综合属性****************/
 




#line 467 "y.tab.c"
#define BASIC 257
#define CONST 258
#define ID 259
#define IF 260
#define ELSE 261
#define WHILE 262
#define DO 263
#define BREAK 264
#define RELOP_LT 265
#define RELOP_LE 266
#define RELOP_GT 267
#define RELOP_GE 268
#define RELOP_EQ 269
#define RELOP_NEQ 270
#define OR 271
#define AND 272
#define UMINUS 273
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    1,    2,    5,    3,    3,    6,    7,    4,    4,
    8,    8,    8,    8,    8,    8,    8,   10,   10,   11,
   11,   12,   12,   12,   13,   13,   13,   13,   13,    9,
    9,    9,   14,   14,   14,   15,   15,   15,
};
short yylen[] = {                                         2,
    1,    6,    0,    0,    2,    0,    3,    1,    2,    0,
    4,    5,    7,    5,    7,    2,    1,    3,    1,    3,
    1,    3,    3,    1,    3,    3,    3,    3,    1,    3,
    3,    1,    3,    3,    1,    3,    1,    1,
};
short yydefred[] = {                                      0,
    3,    0,    1,    6,    0,    8,    0,    5,    0,    0,
    0,    0,    0,    0,   17,    0,    9,    0,    0,    0,
    0,    0,   16,    2,    7,   38,   37,    0,    0,    0,
   35,    0,    0,    0,    0,   24,    0,    0,    0,    0,
    0,   11,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   36,    0,    0,   33,   34,
    0,    0,    0,    0,    0,    0,    0,   22,   23,   14,
    0,    0,    0,   13,   15,
};
short yydgoto[] = {                                       2,
   15,    4,    5,    7,   16,    8,    9,   17,   32,   33,
   34,   35,   36,   30,   31,
};
short yysindex[] = {                                   -110,
    0,    0,    0,    0, -229,    0, -108,    0, -220,  -18,
    9,   13, -108,   -4,    0,  -80,    0,   15,  -39,  -39,
  -39, -211,    0,    0,    0,    0,    0,  -39,  -29,  -22,
    0,   14,  -17, -214, -263,    0,   -8,   25,   11,  -39,
  -39,    0,  -39,  -39,  -39,  -39,  -39,  -39,  -39, -108,
  -39,  -39,  -39, -108,  -39,    0,  -22,  -22,    0,    0,
  -11,  -11,  -11,  -11, -214, -195, -263,    0,    0,    0,
    5, -108,   16,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0, -120,    0,  -52,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -41,
    0,  -19,    0,    6,    1,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -33,  -24,    0,    0,
  -14,  -10,   -5,   -1,    7, -114,    3,    0,    0,    0,
    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
   76,    0,    0,    0,    0,    0,    0,   10,   22,    8,
   29,   26,  -15,   21,   28,
};
#define YYTABLESIZE 282
short yytable[] = {                                      32,
   28,   32,   10,   32,   10,   52,   53,   30,   12,   30,
   12,   30,    1,   40,    1,   41,   31,   32,   31,   43,
   31,   29,   22,   50,   44,   30,   25,    6,   37,   42,
   26,   40,   54,   41,   31,   27,   68,   69,   18,   28,
   29,   21,   19,   20,   24,   73,   19,   18,   20,   39,
   38,   56,   21,   40,   23,   41,   40,   51,   41,   66,
   57,   58,   71,   70,   55,   72,   61,   62,   63,   64,
   59,   60,    4,   25,   75,    3,   67,   65,    0,    0,
    0,   74,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   10,   10,
    0,   10,   10,   10,   12,   12,    0,   12,   12,   12,
   10,   11,    0,   12,   13,   14,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   26,   27,
    0,    0,    0,   32,   32,   32,   32,   32,   32,   32,
   32,   30,   30,   30,   30,   30,   30,   30,   30,    0,
   31,   31,   31,   31,   31,   31,   31,   31,    0,   29,
   29,   29,   29,   49,   25,   25,   25,   25,   26,   26,
   26,   26,   49,   27,   27,   27,   27,   28,   28,   28,
   28,   21,   21,   20,   20,   49,   19,   18,   45,   46,
   47,   48,
};
short yycheck[] = {                                      41,
   40,   43,  123,   45,  125,  269,  270,   41,  123,   43,
  125,   45,  123,   43,  123,   45,   41,   59,   43,   42,
   45,   41,   13,   41,   47,   59,   41,  257,   21,   59,
   41,   43,   41,   45,   59,   41,   52,   53,  259,   41,
   19,   41,   61,   41,  125,   41,   41,   41,   40,   28,
  262,   41,   40,   43,   59,   45,   43,  272,   45,   50,
   40,   41,   55,   54,   40,  261,   45,   46,   47,   48,
   43,   44,  125,   59,   59,    0,   51,   49,   -1,   -1,
   -1,   72,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  259,  260,
   -1,  262,  263,  264,  259,  260,   -1,  262,  263,  264,
  259,  260,   -1,  262,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,  259,
   -1,   -1,   -1,  265,  266,  267,  268,  269,  270,  271,
  272,  265,  266,  267,  268,  269,  270,  271,  272,   -1,
  265,  266,  267,  268,  269,  270,  271,  272,   -1,  269,
  270,  271,  272,  271,  269,  270,  271,  272,  269,  270,
  271,  272,  271,  269,  270,  271,  272,  269,  270,  271,
  272,  271,  272,  271,  272,  271,  271,  271,  265,  266,
  267,  268,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 273
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"'!'",0,0,0,0,0,0,"'('","')'","'*'","'+'",0,"'-'",0,"'/'",0,0,0,0,0,0,0,0,0,0,0,
"';'",0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"BASIC","CONST","ID","IF","ELSE","WHILE","DO","BREAK","RELOP_LT",
"RELOP_LE","RELOP_GT","RELOP_GE","RELOP_EQ","RELOP_NEQ","OR","AND","UMINUS",
};
char *yyrule[] = {
"$accept : program",
"program : block",
"block : '{' blockM1 decls stmts blockM2 '}'",
"blockM1 :",
"blockM2 :",
"decls : decls decl",
"decls :",
"decl : type ID ';'",
"type : BASIC",
"stmts : stmts stmt",
"stmts :",
"stmt : ID '=' expr ';'",
"stmt : IF '(' bool ')' stmt",
"stmt : IF '(' bool ')' stmt ELSE stmt",
"stmt : WHILE '(' bool ')' stmt",
"stmt : DO stmt WHILE '(' bool ')' ';'",
"stmt : BREAK ';'",
"stmt : block",
"bool : bool OR join",
"bool : join",
"join : join AND equality",
"join : equality",
"equality : equality RELOP_EQ rel",
"equality : equality RELOP_NEQ rel",
"equality : rel",
"rel : expr RELOP_LT expr",
"rel : expr RELOP_LE expr",
"rel : expr RELOP_GT expr",
"rel : expr RELOP_GE expr",
"rel : expr",
"expr : expr '+' term",
"expr : expr '-' term",
"expr : term",
"term : term '*' factor",
"term : term '/' factor",
"term : factor",
"factor : '(' expr ')'",
"factor : ID",
"factor : CONST",
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
#line 637 "compile.y"
#include "lex.yy.c"

int yyparse();  /*main函数要调用yyparse()函数，但该函数的定义在后面，所以要先声明(才能引用)*/

int main()
{
	char sourcefile[1000],destfile[1000];

	printf("请输入要编译的源程序文件名(回车默认为d:\\gencode\\code.txt)："); gets(sourcefile);
        if( strcmp( sourcefile,"")== 0 ) 
            strcpy( sourcefile, "d:\\gencode\\code.txt");
	printf("请输入存放中间代码的文件名(回车默认为d:\\gencode\\gencode.txt)："); gets(destfile);
        if( strcmp( destfile,"")== 0 ) 
            strcpy( destfile, "d:\\gencode\\gencode.txt");

	BeginCompileOneFile( sourcefile );

    CreateConstList(3000);/*创建常量表,假设从地址3000开始分配空间给常量*/
    /*在C语言编译器中，该符号表用于存放外部变量，函数名等。
      在我们的语法中并不支持外部变量和函数，所以该表没有被用到*/
    TopSymbolList = CreateSymbolList( NULL, 2000 ); /*假设从地址2000开始分配空间给变量*/
    CreateQuadTable(100); /*创建四元式表，假设四元式从地址空间100开始存放*/

    yyparse();

    PrintConstList();
    WriteQuadTableToFile( destfile ); /*把四元式写入到文件destfile中*/

    DestroyQuadTable();
    DestroySymbolList(TopSymbolList);
	DestroyConstList();

    if( CompileFailed == 0 ) 
	    printf("编译成功，生成的四元式在文件%s中。\n", destfile );
	else
	    printf("源文件%s有错误，编译失败。\n", sourcefile );

	EndCompileOneFile();
        
	getchar();
    return 0;
}





   
#line 735 "y.tab.c"
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
#line 490 "compile.y"
{ printf("产生式：program->block\n"); }
break;
case 2:
#line 493 "compile.y"
{ printf("产生式：block->{decls stmts}\n"); }
break;
case 3:
#line 496 "compile.y"
{ TopSymbolList = CreateSymbolList( TopSymbolList, TopSymbolList->endaddr ); }
break;
case 4:
#line 499 "compile.y"
{ SymbolList env;
                                 PrintSymbolList( TopSymbolList); 
                                 env = TopSymbolList->prev;
                                 DestroySymbolList( TopSymbolList ); 
                                 TopSymbolList = env;                 
                              }
break;
case 5:
#line 507 "compile.y"
{ printf("产生式：decls->decls decl\n"); }
break;
case 6:
#line 508 "compile.y"
{ printf("产生式：decls->null\n"); }
break;
case 7:
#line 511 "compile.y"
{ int width;
                                
                                printf("产生式：decl->type ID; ID=%s\n",yyvsp[-1].id.name); 
                                
								switch( yyvsp[-2].basic.type ) {
                                    case CHAR  : width = CHAR_WIDTH;  break;
                                    case INT   : width = INT_WIDTH;   break;
                                    case FLOAT : width = FLOAT_WIDTH; break;
                                    case BOOL  : width = BOOL_WIDTH;  break;
                                    default    : width = -1; break;
                                }
                                AddToSymbolList( TopSymbolList, yyvsp[-1].id.name, yyvsp[-2].basic.type, width );

                              }
break;
case 8:
#line 527 "compile.y"
{ printf("产生式：type->BASIC\n"); yyval.basic.type = yyvsp[0].basic.type; }
break;
case 9:
#line 530 "compile.y"
{printf("产生式：stmts->stmts stmt\n");}
break;
case 10:
#line 531 "compile.y"
{printf("产生式：stmts->null\n");}
break;
case 11:
#line 534 "compile.y"
{ printf("产生式：stmt->id = expr;\n"); }
break;
case 12:
#line 535 "compile.y"
{ printf("产生式：stmt->if (bool) stmt\n");}
break;
case 13:
#line 536 "compile.y"
{ printf("产生式：stmt->if (bool) stmt esle stmt\n"); }
break;
case 14:
#line 537 "compile.y"
{ printf("产生式：stmt->while (bool) stmt\n"); }
break;
case 15:
#line 538 "compile.y"
{ printf("产生式：stmt->do stmt while (bool)\n"); }
break;
case 16:
#line 539 "compile.y"
{ printf("产生式：stmt->break ;\n"); }
break;
case 17:
#line 540 "compile.y"
{ printf("产生式：stmt->block\n"); }
break;
case 18:
#line 543 "compile.y"
{ printf("产生式：bool->bool || join\n"); }
break;
case 19:
#line 544 "compile.y"
{ printf("产生式：bool->join\n"); }
break;
case 20:
#line 547 "compile.y"
{ printf("产生式：join->join && equality\n"); }
break;
case 21:
#line 548 "compile.y"
{ printf("产生式：join->equality\n"); }
break;
case 22:
#line 551 "compile.y"
{ printf("产生式：equality->equality == rel\n"); }
break;
case 23:
#line 552 "compile.y"
{ printf("产生式：equality->equality != rel\n"); }
break;
case 24:
#line 553 "compile.y"
{ printf("产生式：equality->rel\n"); }
break;
case 25:
#line 556 "compile.y"
{ printf("产生式：rel -> expr < expr\n"); }
break;
case 26:
#line 557 "compile.y"
{ printf("产生式：rel -> expr <= expr\n"); }
break;
case 27:
#line 558 "compile.y"
{ printf("产生式：rel -> expr > expr\n"); }
break;
case 28:
#line 559 "compile.y"
{ printf("产生式：rel -> expr >= expr\n"); }
break;
case 29:
#line 560 "compile.y"
{ printf("产生式：rel -> expr\n"); }
break;
case 30:
#line 565 "compile.y"
{ printf("产生式：expr->expr + term\n"); 
                           
						   }
break;
case 31:
#line 569 "compile.y"
{ printf("产生式：expr->expr - term\n"); 
	  
	  
	                      }
break;
case 32:
#line 574 "compile.y"
{ printf("产生式：expr->term\n");
							strcpy( yyval.expr.str, yyvsp[0].term.str );
							yyval.expr.type = yyvsp[0].term.type;
							yyval.expr.addr = yyvsp[0].term.addr;
							yyval.expr.width = yyvsp[0].term.width;	
	  
	                      }
break;
case 33:
#line 583 "compile.y"
{ printf("产生式：term->term*factor\n"); }
break;
case 34:
#line 585 "compile.y"
{ printf("产生式：term->term/factor\n"); }
break;
case 35:
#line 587 "compile.y"
{ printf("产生式：term->factor\n");
							strcpy( yyval.term.str, yyvsp[0].factor.str );
							yyval.term.type = yyvsp[0].factor.type;
							yyval.term.addr = yyvsp[0].factor.addr;
							yyval.term.width = yyvsp[0].factor.width;	
	                      }
break;
case 36:
#line 595 "compile.y"
{ printf("产生式：factor->(expr)\n" );
							strcpy( yyval.factor.str, yyvsp[-1].expr.str );
							yyval.factor.type  = yyvsp[-1].expr.type;
							yyval.factor.addr  = yyvsp[-1].expr.addr;
							yyval.factor.width = yyvsp[-1].expr.width;
                          }
break;
case 37:
#line 602 "compile.y"
{ 
	                        struct SymbolElem * p;
							printf("产生式：factor->id\n"); 
							p = LookUpAllSymbolList( TopSymbolList, yyvsp[0].id.name );
							if( p != NULL ) {
								strcpy( yyval.factor.str, p->name );
								yyval.factor.type  = p->type;
								yyval.factor.addr  = p->addr;
								yyval.factor.width = p->width;
							}							    
							else {
							    yyerror( "变量名没有定义" );
								strcpy( yyval.factor.str, "no_id_defined" ); /*容错处理*/
								yyval.factor.type = INT;
								yyval.factor.addr = -1;
								yyval.factor.width = INT_WIDTH;							    
							}
	                      }
break;
case 38:
#line 621 "compile.y"
{                        
							struct ConstElem * p; 
							    printf("产生式：factor->CONST\n");

								p = LookUpConstList( yyvsp[0].constval.type, yyvsp[0].constval.value, yyvsp[0].constval.width ) ;
								if( p== NULL )
                                    p = AddToConstList( yyvsp[0].constval.str, yyvsp[0].constval.type, yyvsp[0].constval.value, yyvsp[0].constval.width );

								strcpy( yyval.factor.str, yyvsp[0].constval.str );
								yyval.factor.type  = yyvsp[0].constval.type;
								yyval.factor.addr  = p->addr;
								yyval.factor.width = p->width;
                          }
break;
#line 1094 "y.tab.c"
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
