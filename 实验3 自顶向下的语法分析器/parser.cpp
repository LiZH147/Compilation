#include "lex.yy.h"

extern "C" void BeginCompileOneFile(const char *filename);
extern "C" void EndCompileOneFile(void);
extern "C" int yylex();
extern "C" int GetLineNo(void);

int lookahead;
void expr();
void stmt();
void stmts();
void Block();
void Program();
void Factory();
void Term();
void Term1();
void Rest();
void bool_op();
void else_op();


void error() {
	printf("在%d行出现句法分析错误! \n", GetLineNo());
	EndCompileOneFile();
	printf("请按回车键退出！");
	getchar();
	exit(1);
}

void match(int TokenID) {
//	printf("匹配%d\n", lookahead);
	if (lookahead == TokenID) {
//        printf("匹配%d\n", lookahead);
		lookahead = yylex();
	} else
		error();
}

void Factory() {
	if (lookahead == NUM_TKN) {
		printf("factor -> NUM\n");
		match(NUM_TKN);
		return;
	} else if (lookahead == ID_TKN) {
		printf("factor -> ID\n");
		match(ID_TKN);
		return;
	} else if (lookahead == '(') {
		printf("factor -> (expr)\n");
		match('(');
		expr();
		match(')');
		return;
	} else {
		printf("\n表达式错误：Term函数中需要输入整数或变量\n");
		error();
	}
}

void Term1() {
	switch (lookahead) {
		case '*':
			printf("* factor term1\n");
			match('*');
			Factory();
			Term1();
			break;
		case '/':
			printf("/ factor term1\n");
			match('/');
			Factory();
			Term1();
			break;
		case '%':
			printf("%% factor term1\n");
			match('%');
			Factory();
			Term1();
			break;
		default:
			break;
	}
}
void Term() {
	printf("term -> factory term1\n");
	Factory();
	Term1();
	return;
}
void Rest() {
	switch (lookahead) {
		case '+':
			printf("+ term rest\n");
			match('+');
			Term();
			Rest(); // rest --> + term {print('+')} rest
			break;
		case '-':
			printf("- term rest\n");
			match('-');
			Term();
			Rest(); // rest --> - term {print('-')} rest
			break;
		default: // rest --> 空
			break;
	}
}
void expr() {
	printf("expr -> term rest\n");
	Term();
	Rest();
	return;
}
void bool_op() {
	if (lookahead == '(' || lookahead == ID_TKN || lookahead == NUM_TKN) {
		printf("bool -> expr relop expr \n");
		expr();
		match(RELOP_TKN);
		expr();
		return;
	}
	error();
}
void else_op() {
	if (lookahead == ELSE_TKN) {
		printf("else_op -> stmt \n");
		match(ELSE_TKN);
		stmt();
		return;
	} else {
		printf("else_op -> empty\n");
		return;
	}
}
void stmt() {
	if (lookahead == BREAK_TKN) {
		printf("stmt →	break ; \n");
		match(BREAK_TKN);
		match(';');
		return;
	} else if (lookahead == DO_TKN) {
		printf("stmt →	do stmt while(bool) ;\n");
		match(DO_TKN);
		stmt();
		match(WHILE_TKN);
		match('(');
		bool_op();
		match(')');
		match(';');
		return;
	} else if (lookahead == WHILE_TKN) {
		printf("stmt →	while(bool) stmt\n");
		match(WHILE_TKN);
		match('(');
		bool_op();
		match(')');
		stmt();
		return;
	}

	else if (lookahead == IF_TKN) {
		printf("if(bool)stmt else_op");
		match(IF_TKN);
		match('(');
		bool_op();
		match(')');
		stmt();
		else_op();
//		stmt();
		return;
	}

	else if (lookahead == ID_TKN) {
		printf("stmt →	id = expr ;\n");
		match(ID_TKN);
		match('=');
		expr();
		match(';');
		return;
	} else if (lookahead == '{') {
		printf("stmt -> block\n");
		Block();
		return;
	} else {
		error();
	}
}

void stmts() {
	/*如果lookahead是First(stmt stmts)中的一个终结符，那么stmts→stmt stmts可用*/
	if (lookahead == ID_TKN || lookahead == IF_TKN ||
	    lookahead == WHILE_TKN || lookahead == DO_TKN ||
	    lookahead == BREAK_TKN || lookahead == '{') {
		printf("stmts 	→　stmt stmts\n");
		stmt();
		stmts();
		return;
	}
	if (lookahead == '}') {
		/*如果lookahead是stmts的一个follow,那么stmts →　empty可用*/
		printf("stmts →　empty \n");
		return;
	}
	/*否则*/
	error();
}

void Block() {
	if (lookahead == '{') {
		printf("block →  { stmts }\n");
		match('{');
		stmts();
		match('}');
	} else
		error();
}

void Program() {
	if (lookahead == '{') {
		printf("program → block\n");
		Block();
	} else
		error();
}

int main() {
	char filename[1000];

	printf("请输入要编译的源程序文件名：");
	gets(filename);
	BeginCompileOneFile(filename);

	// 当flex扫描到文件末尾，yylex函数返回0
	lookahead = yylex();

	/*调用Program()开始句法分析,Program是文法的起始符号*/
	Program();

	if (lookahead == 0)
		printf("\n句法分析成功！\n");
	else
		error();

	EndCompileOneFile();
	printf("请按回车键退出！");
	getchar(); // 如果句法分析失败，就从error()中退出，不在这里退出。

	return 0;
}