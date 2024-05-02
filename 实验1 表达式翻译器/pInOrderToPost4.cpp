/*程序功能：将中缀表达式转换为后缀表达式的翻译器
  程序输入：常数、变量以及'+'和'-"构成的中缀表达式
            程序使用词法分析功能。
			例如: 在键盘上输入90-abc+23
  原始文法描述：
           expr --> expr + term
                  | expr - term
                  | term
           term --> num | ID

  消除左递归后的语法制导定义：
            产生式                       语义规则
            expr --> term  rest       expr.syn = term.syn || rest.syn

            rest --> + term rest1     rest.syn = term.syn || '+' || rest1.syn
                   | - term rest1     rest.syn = term.syn || '-' || rest1.syn
   		           | 空               rest.syn = ""

            term --> num              term.syn =  tokenval;
                   | ID               term.syn = lexeme; 

  说明：综合属性的英文翻译为: synthesized attribute
         expr.syn表示expr的一个综合属性为syn，该属性存储expr的后缀表达式串
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
using std::string;

#include <ctype.h>

#define TKN_NUM  500
#define TKN_ID   600

int lineno = 1;
int LookAhead; //存放当前的词法单元的类型
int tokenval = 0; char lexeme[1024];

int GetToken()
{
	int t, i;
	while (1) {
		t = getchar();
		if (t == ' ' || t == '\t') 
			;
		//else if (t == '\n')  //因为在该程序中，回车符作为表达式的结束，所以不能把回车符当成空白符
			//lineno++;
		else if (isdigit(t)) {
			tokenval = 0;
			do {
				tokenval = tokenval * 10 + t -'0';
				t = getchar();
			} while (isdigit(t));
			ungetc(t, stdin);
			return TKN_NUM;
		}
        else if( isalpha(t) ) {
            i=0; 
            do {
                lexeme[i++]=t; t = getchar(); 
            }while( isalpha(t) || isdigit(t) );
            lexeme[i]='\0'; ungetc(t, stdin);
            return TKN_ID;
        }
		else {
			tokenval = 0;
			return t;//例如t=’+’，则把’+’的ASCII作为’+’的TokenName。
		}
	}
}

void Match(int t)
{
	if( LookAhead==t ) 
		LookAhead = GetToken(); //继续往前看后一个词法单元
    else {
		printf("\n表达式错误:Match失败。\n"); 
		system("pause");
		exit(1); //结束程序
	}

}

void Term(string & TermSyn)  
{char buf[100];
	if( LookAhead==TKN_NUM ) {
		sprintf(buf,"%d ", tokenval); //该函数将tokenval转化成10进制的字符串，放在buf中。
		TermSyn = buf; Match(LookAhead);
	}
	else if( LookAhead==TKN_ID) {
		sprintf(buf,"%s ",lexeme);
		TermSyn = buf; Match(LookAhead);
	}
    else {
		printf("\n表达式错误:这里需要一个整数或变量。\n" );
		system("pause");
		exit(1); //结束程序
	}
}

void Rest(string & RestSyn)  
{string term_syn,rest_syn;
	switch( LookAhead ) {
	    case '+':
			Match('+'); Term(term_syn); Rest(rest_syn); // rest.syn = term.syn || '+' || rest1.syn
			RestSyn = term_syn + "+ " + rest_syn;
			break;
		case '-':
			Match('-'); Term(term_syn); Rest(rest_syn); // rest.syn = term.syn || '-' || rest1.syn
			RestSyn = term_syn + "- " + rest_syn;
			break;
		default:   // rest --> 空   rest.syn = ""
			RestSyn = "";
			break;
	}
}

void Expr(string & ExprSyn) 
{string term_syn, rest_syn;
    Term(term_syn);
	Rest(rest_syn);
	ExprSyn = term_syn + rest_syn;
}

void main()
{string expr_syn;
	printf("请输入中缀表达式：");
	LookAhead = GetToken();

	printf("其后缀表达式为：");
	Expr(expr_syn);
    printf( "%s", expr_syn.c_str() );

	if( LookAhead !='\n' ) {
		printf("\n输入的表达式错误\n");
		exit(1);
	}

	printf("\n表达式分析成功！\n");
	system("pause");
}