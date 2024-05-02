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

  消除左递归后的语法制导的翻译方案：
            expr --> term  rest 

            rest --> + term {print('+')} rest
                   | - term {print('-')} rest
   		           | 空

            term --> 0  {print('0')}
                   | 1  {print('1')}
                   | 2  {print('2')}
		              ......
                   | 9  {print('9')} 
*/
#include <stdio.h>
#include <stdlib.h>

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

void Term()
{char t;
	if( LookAhead>='0' && LookAhead<='9' ) {
		t = LookAhead; Match(t); printf("%c",t);
	}
    else {
		printf("\n表达式错误：Term函数中需要输入数字字符，但是输入的是%c\n", LookAhead );
		exit(1); //结束程序
	}
}

void Rest()
{
	switch( LookAhead ) {
	    case '+':
			Match('+'); Term(); putchar('+'); Rest(); // rest --> + term {print('+')} rest
			break;
		case '-':
			Match('-'); Term(); putchar('-'); Rest(); // rest --> - term {print('-')} rest
			break;
		default:   // rest --> 空
			break;
	}
}

void Expr()
{
    Term();
	Rest();
}

int main()
{
	printf("请输入中缀表达式：");
	LookAhead = getchar();

	printf("其后缀表达式为：");
	Expr();
    
	if( LookAhead !='\n' ) {
	    //例如：3+45
		printf("\n输入的表达式错误，错误的字符：%c\n", LookAhead);
		exit(1);
	}

	printf("\n表达式分析成功！\n");
	return 0;
}