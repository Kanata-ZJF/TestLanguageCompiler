#include <cstdio>
#include <string>
#define MAX_CAPACITY 500 //定义符号表容量

//语法分析函数
int program();
int compound_stat();
int statement();
int arithmetic_expression();
int assignment_expression();
int assignment_stat();
int bool_expression();
int term();
int factor();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int declaration_stat();
int declaration_list();
int statement_list();
int compound_stat();

int name_def(char* name); //符号表动作

char token[20], taken[40]; //token保存单词符号，taken保存单词值
FILE *fp, *fout2; //用于指向输入输出文件的指针
int flagout = 0;

//符号表结构
struct
{
	char name[8];
	int address;
} symbolTable[MAX_CAPACITY];

int vartablep = 0;
int labelp = 0;
int datap = 0; //改符号表最多容纳MAX_CAPACITY个记录

//插入符号表动作@name-def
int name_def(char* name)
{
	int es = 0;
	
	if (vartablep >= MAX_CAPACITY)
		return (21);
	
	for (int i = vartablep - 1; i == 0; i--) //查符号表
	{
		if (strcmp(symbolTable[i].name, name) == 0)
		{
			es = 22; //22表示变量重复声明
			break;
		}
	}
	
	if (es > 0)
		return (es);
	
	strcpy_s(symbolTable[vartablep].name, name);
	symbolTable[vartablep].address = datap;
	datap++; //分配一个单元，数据区指针加1
	vartablep++;
	
	return (es);
}

//查询符号表返回地址
int lookup(char* name, int* paddress)
{
	int i, es = 0;
	
	for (i = 0; i < vartablep; i++)
	{
		if (strcmp(symbolTable[i].name, name) == 0)
		{
			*paddress = symbolTable[i].address;
			return (es);
		}
	}
	
	es = 23;
	return (es); //变量没有声明
}

//语法、语义分析及代码生成程序
int TESTparse()
{
	int es = 0;
	fp = fopen("code\\lex.txt", "r");
	if (!fp)
	{
		printf("\n打开%s错误!\n", "lex.txt");
		es = 10;
		return (es);
	}
	/*printf("请输入目标文件名（包含路径）：");
	scanf_s("%s", Codeout, 128);*/
	fout2 = fopen("code\\intermediate_code.txt", "w");
	if (!fout2)
	{
		printf("\n创建错误!\n");
		es = 10;
		return (es);
	}
	if (es == 0)
		es = program();
	printf("==语法、语义分析及代码生成程序结果==\n");
	switch (es)
	{
	case 0:
		printf("语法、语义分析成功并抽象机汇编生成代码!\n");
		break;
	case 10:
		printf("打开文件失败!\n");
		break;
	case 1:
		printf("缺少{!\n");
		break;
	case 2:
		printf("缺少}!\n");
		break;
	case 3:
		printf("缺少标识符!\n");
		break;
	case 4:
		printf("少分号!\n");
		break;
	case 5:
		printf("缺少(!\n");
		break;
	case 6:
		printf("缺少)!\n");
		break;
	case 7:
		printf("缺少操作数!\n");
		break;
	case 21:
		printf("符号表溢出!\n");
		break;
	case 22:
		printf("变量重复定义!\n");
		break;
	case 23:
		printf("变量未声明!\n");
		break;
	}
	fclose(fp);
	fclose(fout2);
	return (es);
}

//program::={<declaration_list><statement_list>}
int program()
{
	int es = 0, i;
	fscanf(fp, "%s %s\n", token, taken);
	printf("%s	:	%s\n", token, taken);
	if (strcmp(token, "{")) //判断是否'{'
	{
		es = 1; //不是{
		return (es);
	}
	fscanf(fp, "%s %s\n", token, taken);
	printf("%s	:	%s\n", token, taken);
	es = declaration_list();
	if (es > 0)
		return (es);
	printf("符号表:\n");
	printf(" 名字 地址\n");
	for (i = 0; i < vartablep; i++)
		printf("   %s   %d\n", symbolTable[i].name, symbolTable[i].address);
	es = statement_list();
	if (es > 0)
		return (es);
	if (strcmp(token, "}")) //判断是否'}'
	{
		es = 2;
		return (es);
	}
	fscanf(fp, "%s %s\n", token, taken);
	printf("%s	:	%s\n", token, taken);
	es = statement_list();
	fprintf(fout2, "    STOP\n"); //产生停止指令
	return (es);
}

int declaration_list()
{
	int es = 0;
	while (strcmp(token, "int") == 0)
	{
		es = declaration_stat();
		if (es > 0)
			return (es);
	}
	return (es);
}

int declaration_stat()
{
	int es = 0;
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "ID"))
		return (es = 3); //不是标识符
	es = name_def(taken); //插入符号表
	if (es > 0)
		return (es);
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, ";"))
		return (es == 4);
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	return (es);
}

int statement_list()
{
	int es = 0;
	while (strcmp(token, "}"))
	{
		es = statement();
		if (es > 0)
			return (es);
	}
	return (es);
}

int statement()
{
	int es = 0;
	if (es == 0 && strcmp(token, "if") == 0)
		es = if_stat(); //if语句
	if (es == 0 && strcmp(token, "while") == 0)
		es = while_stat(); //while语句
	if (es == 0 && strcmp(token, "for") == 0)
		es = for_stat(); //for语句
	if (es == 0 && strcmp(token, "read") == 0)
		es = read_stat(); //read语句
	if (es == 0 && strcmp(token, "write") == 0)
		es = write_stat(); //write语句
	if (es == 0 && strcmp(token, "{") == 0)
		es = compound_stat(); //复合语句
	if (es == 0 && (strcmp(token, "ID") == 0)) //赋值语句
		es = assignment_stat();
	return (es);
}

int if_stat()
{
	int es = 0, label1, label2; //if
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "("))
		return (es = 5); //少左括号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = bool_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ")"))
		return (es = 6); //少右括号
	label1 = labelp++; //用label1记住条件为假时要转向的标号
	fprintf(fout2, "      BRF LABEL %d\n", label1); //输出假转移指令
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = statement();
	if (es > 0)
		return (es);
	label2 = labelp++; //用label2记住要转向的标号
	fprintf(fout2, "      BR LABEL %d\n", label2); //输出无条件转移指令
	fprintf(fout2, "LABEL%d:\n", label1); //设置label1要标记的符号
	if (strcmp(token, "else") == 0) //else部分处理
	{
		fscanf(fp, "%s %s\n", &token, &taken);
		printf("%s %s\n", token, taken);
		es = statement();
		if (es > 0)
			return (es);
	}
	fprintf(fout2, "LABEL%d:\n", label2); //设置label2记住的标号
	return (es);
}

int while_stat()
{
	int es = 0, label1, label2;
	label1 = labelp++;
	fprintf(fout2, "LABEL%d:\n", label1); //设置label1标号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "("))
		return (es = 5); //少左括号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = bool_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ")"))
		return (es = 6); //少右括号
	label2 = labelp++;
	fprintf(fout2, "      BRF LABEL %d\n", label2); //输出假转移指令
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = statement();
	if (es > 0)
		return (es);
	fprintf(fout2, "      BR LABEL %d\n", label1); //输出无条件转移指令
	fprintf(fout2, "LABEL%d:\n", label2); //设置label2标号
	return (es);
}

int for_stat()
{
	int es = 0, label1, label2, label3, label4;
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "("))
		return (es = 5); //少左括号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = assignment_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ";"))
		return (es = 4); //少分号
	label1 = labelp++;
	fprintf(fout2, "LABEL%d:\n", label1); //设置label1标号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = bool_expression();
	if (es > 0)
		return (es);
	label2 = labelp++;
	fprintf(fout2, "      BRF LABEL%d\n", label2); //输出假条件转移指令
	label3 = labelp++;
	fprintf(fout2, "      BR LABEL%d\n", label3); //输出无条件转移指令
	if (strcmp(token, ";"))
		return (es = 4); //少分号
	label4 = labelp++;
	fprintf(fout2, "LABEL%d:\n", label4); //设置label4标号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = assignment_expression();
	if (es > 0)
		return (es);
	fprintf(fout2, "      POP\n"); //输出出栈指令
	fprintf(fout2, "      BR LABEL%d\n", label1); //输出无条件转移指令
	if (strcmp(token, ")"))
		return (es = 6); //缺少右括号
	fprintf(fout2, "LABEL%d:\n", label3); //设置label3标号
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = statement();
	if (es > 0)
		return (es);
	fprintf(fout2, "      BR LABEL%d\n", label4); //输出无条件转移指令
	fprintf(fout2, "LABEL%d:\n", label2); //设置label2标号
	return (es);
}

int write_stat()
{
	int es = 0;
	fscanf(fp, "%s %s\n", token, &taken);
	int address;
	flagout = 1;
	es = lookup(taken, &address);
	printf("%s %s\n", token, taken);
	es = arithmetic_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ";"))
		return (es = 4); //少分号

	fprintf(fout2, "      OUT %d\n", address);
	flagout = 0;
	fscanf(fp, "%s %s\n", token, &taken);
	printf("%s %s\n", token, taken);
	return (es);
}

int read_stat()
{
	int es = 0, address;
	fscanf(fp, "%s %s\n", token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "ID"))
		return (es = 3); //少标识符
	es = lookup(taken, &address);
	if (es > 0)
		return (es);
	fprintf(fout2, "      IN  \n"); //输出指令
	fprintf(fout2, "      STO  %d\n", address); //输出STO指令
	fprintf(fout2, "      POP  \n");
	fscanf(fp, "%s %s\n", token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, ";"))
		return (es = 4); //少分号
	fscanf(fp, "%s %s\n", token, &taken);
	printf("%s %s\n", token, taken);
	return (es);
}

int compound_stat()
{
	int es = 0;
	fscanf(fp, "%s %s\n", token, &taken);
	printf("%s %s\n", token, taken);
	es = statement_list();
	return (es);
}

int assignment_expression()
{
	int es = 0, address;
	if (strcmp(token, "ID"))
		return es = 9;
	es = lookup(taken, &address);
	if (es > 0)
		return es;
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "="))
		return es = 10;
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = arithmetic_expression();
	if (es > 0)
		return es;
	fprintf(fout2, "STO %d\n", address);
	fprintf(fout2, " POP\n");
	return (es);
}

int assignment_stat()
{
	int es = 0;
	while (strcmp(token, "ID") == 0)
	{
		es = assignment_expression();
		if (es > 0)
			return (es);
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
	}

	return (es);
}

int bool_expression()
{
	int es = 0;
	es = arithmetic_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ">") == 0 || strcmp(token, ">=") == 0 || strcmp(token, "<") == 0 || strcmp(token, "<=") == 0 ||
		strcmp(token, "==") == 0 || strcmp(token, "!=") == 0)
	{
		char token2[20];
		strcpy_s(token2, token); //保存运算符
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
		es = arithmetic_expression();
		if (es > 0)
			return (es);
		if (strcmp(token2, ">") == 0)
			fprintf(fout2, "      GT\n");
		if (strcmp(token2, ">=") == 0)
			fprintf(fout2, "      GE\n");
		if (strcmp(token2, "<") == 0)
			fprintf(fout2, "      LES\n");
		if (strcmp(token2, "<=") == 0)
			fprintf(fout2, "      LE\n");
		if (strcmp(token2, "==") == 0)
			fprintf(fout2, "      EQ\n");
		if (strcmp(token2, "!=") == 0)
			fprintf(fout2, "      NOTEQ\n");
	}
	return (es);
}

int arithmetic_expression()
{
	int es = 0;
	es = term();
	if (es > 0)
		return (es);
	while (strcmp(token, "+") == 0 || strcmp(token, "-") == 0)
	{
		char token2[20];
		strcpy_s(token2, token); //保存运算符
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
		es = term();
		if (es > 0)
			return (es);
		if (strcmp(token2, "+") == 0)
			fprintf(fout2, "      ADD\n");
		if (strcmp(token2, "-") == 0)
			fprintf(fout2, "      SUB\n");
	}
	return (es);
}

int term()
{
	int es = 0;
	es = factor();
	if (es > 0)
		return (es);
	while (strcmp(token, "*") == 0 || strcmp(token, "/") == 0)
	{
		char token2[20];
		strcpy_s(token2, token);
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
		es = factor();
		if (es > 0)
			return (es);
		if (strcmp(token2, "*") == 0)
			fprintf(fout2, "      MULT\n");
		if (strcmp(token2, "/") == 0)
			fprintf(fout2, "      DIV\n");
	}
	return (es);
}

int factor()
{
	int es = 0;
	if (strcmp(token, "(") == 0)
	{
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
		es = arithmetic_expression();
		if (es > 0)
			return (es);
		if (strcmp(token, ")"))
			return (es = 6); //缺少右括号
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
	}
	else
	{
		if (strcmp(token, "ID") == 0)
		{
			int address;
			es = lookup(taken, &address); //查符号表，获取变量地址
			if (es > 0)
				return (es);
			if (flagout == 0)
				fprintf(fout2, "      LOAD %d\n", address);
			//fprintf(fout2, "      LOAD %d\n", address);
			fscanf(fp, "%s %s\n", token, &taken);
			printf("%s %s\n", token, taken);
			return (es);
		}
		if (strcmp(token, "NUM") == 0)
		{
			fprintf(fout2, "      LOADI %s\n", taken);
			fscanf(fp, "%s %s\n", token, &taken);
			printf("%s %s\n", token, taken);
			return (es);
		}
		else
		{
			es = 7; //缺少操作数
			return (es);
		}
	}
	return (es);
}
