/*程序功能：将中缀表达式转换为后缀表达式的翻译器
  程序输入：单个数字(0~9)以及'+'和'-"构成的中缀表达式
            为了简化程序而忽略词法分析功能，所以要求表达式中不能有空格等分隔符。
			例如: 在键盘上输入9-5+2
  原始文法描述：
           expr --> expr + term
                  | expr - term
                  | term
           term --> 0
                  | 1
                  | 2
	              ......
                  | 9

  消除左递归后的语法制导定义：
            产生式                       语义规则
            expr --> term  rest       expr.syn = term.syn || rest.syn

            rest --> + term rest1     rest.syn = term.syn || '+' || rest1.syn
                   | - term rest1     rest.syn = term.syn || '-' || rest1.syn
   		           | 空               rest.syn = ""

            term --> 0                term.syn = "0"
                   | 1                term.syn = "1" 
                   | 2                term.syn = "2"
		           ......
                   | 9                term.syn = "9"
   说明：综合属性的英文翻译为: synthesized attribute
         expr.syn表示expr的一个综合属性为syn，该属性存储expr的后缀表达式串
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
using std::string;

char LookAhead;

void Match(char t)
{
	if( LookAhead==t ) 
		LookAhead = getchar(); //继续往前看后一个字符
    else {
		printf("\n表达式错误：Match函数中需要输入的字符为%c，但是实际输入的是%c\n", t, LookAhead );
		exit(1); //结束程序
	}

}

void Term(string & TermSyn)  
{char t;
	if( LookAhead>='0' && LookAhead<='9' ) {
		t = LookAhead; Match(t); TermSyn = string(1,t); //例如：t='3',则创建串“3”。
	}
    else {
		printf("\n表达式错误：Term函数中需要输入数字字符，但是输入的是%c\n", LookAhead );
		exit(1); //结束程序
	}
}

void Rest(string & RestSyn)  
{string term_syn,rest_syn;
	switch( LookAhead ) {
	    case '+':
			Match('+'); Term(term_syn); Rest(rest_syn); // rest.syn = term.syn || '+' || rest1.syn
			RestSyn = term_syn + "+" + rest_syn;
			break;
		case '-':
			Match('-'); Term(term_syn); Rest(rest_syn); // rest.syn = term.syn || '-' || rest1.syn
			RestSyn = term_syn + "-" + rest_syn;
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
	LookAhead = getchar();

	printf("其后缀表达式为：");
	Expr(expr_syn);
    printf( "%s", expr_syn.c_str() );

	if( LookAhead !='\n' ) {
	    //例如：3+45
		printf("\n输入的表达式错误，错误的字符：%c\n", LookAhead);
		exit(1);
	}

	printf("\n表达式分析成功！\n");

}