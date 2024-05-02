#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TKN_NUM  500
#define TKN_ID   600

int lookAhead;
int tokenval; // 记录数字的int型值
char lexeme[1024];

//此处是读取token函数，需要读取至终结符，才算读入完成一个token
//记录数字和字符串
int GetToken() {
	int t, i; // 用于临时接收字符和记录字符串数组位置
	while (1) {
		t = getchar();
		if (t == ' ' || t == '\t')  {
//			continue;
		} else if (isdigit(t)) {
			//数字
			tokenval = 0;
			do {
				tokenval = 10 * tokenval + t - '0';
				t = getchar();
			} while (isdigit(t));
			ungetc(t, stdin);
			return TKN_NUM;
		} else if(isalpha(t)){
//			字符串
			i = 0;
			do{
				lexeme[i] = t;
				i++;
				t = getchar();
			}while(isalpha(t) || isdigit(t)); // 注意字符数字混装时一并算入字符串
			lexeme[i] = '\0'; // 注意添加字符串终止符
			ungetc(t, stdin);
			return TKN_ID;
		} else {
//			终结符
			tokenval = 0;
			return t;
		}
		
	}
}
void Term();
void Rest();

void Expr(){
	
}

int main() {
	printf("请输入前缀表达式：");
//	此处读入第一个token，以便进入翻译过程
	lookAhead = GetToken();
	printf("翻译得到的后缀表达式是：");
//	调用翻译的入口函数，在翻译过程中不断读入token
	Expr();
//	处理错误，输入的表达式不以回车结尾
	if (lookAhead != '\n') {
		printf("\n输入的表达式错误.\n");
		exit(1);
	}

	printf("\n表达式分析成功！\n");

	return 0;
}
