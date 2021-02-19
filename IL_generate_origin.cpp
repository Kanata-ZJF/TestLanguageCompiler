#include <cstdio>
#include <string>
#define MAX_CAPACITY 500 //������ű�����

//�﷨��������
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

int name_def(char* name); //���ű���

char token[20], taken[40]; //token���浥�ʷ��ţ�taken���浥��ֵ
FILE *fp, *fout2; //����ָ����������ļ���ָ��
int flagout = 0;

//���ű�ṹ
struct
{
	char name[8];
	int address;
} symbolTable[MAX_CAPACITY];

int vartablep = 0;
int labelp = 0;
int datap = 0; //�ķ��ű��������MAX_CAPACITY����¼

//������ű���@name-def
int name_def(char* name)
{
	int es = 0;
	
	if (vartablep >= MAX_CAPACITY)
		return (21);
	
	for (int i = vartablep - 1; i == 0; i--) //����ű�
	{
		if (strcmp(symbolTable[i].name, name) == 0)
		{
			es = 22; //22��ʾ�����ظ�����
			break;
		}
	}
	
	if (es > 0)
		return (es);
	
	strcpy_s(symbolTable[vartablep].name, name);
	symbolTable[vartablep].address = datap;
	datap++; //����һ����Ԫ��������ָ���1
	vartablep++;
	
	return (es);
}

//��ѯ���ű��ص�ַ
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
	return (es); //����û������
}

//�﷨������������������ɳ���
int TESTparse()
{
	int es = 0;
	fp = fopen("code\\lex.txt", "r");
	if (!fp)
	{
		printf("\n��%s����!\n", "lex.txt");
		es = 10;
		return (es);
	}
	/*printf("������Ŀ���ļ���������·������");
	scanf_s("%s", Codeout, 128);*/
	fout2 = fopen("code\\intermediate_code.txt", "w");
	if (!fout2)
	{
		printf("\n��������!\n");
		es = 10;
		return (es);
	}
	if (es == 0)
		es = program();
	printf("==�﷨������������������ɳ�����==\n");
	switch (es)
	{
	case 0:
		printf("�﷨����������ɹ��������������ɴ���!\n");
		break;
	case 10:
		printf("���ļ�ʧ��!\n");
		break;
	case 1:
		printf("ȱ��{!\n");
		break;
	case 2:
		printf("ȱ��}!\n");
		break;
	case 3:
		printf("ȱ�ٱ�ʶ��!\n");
		break;
	case 4:
		printf("�ٷֺ�!\n");
		break;
	case 5:
		printf("ȱ��(!\n");
		break;
	case 6:
		printf("ȱ��)!\n");
		break;
	case 7:
		printf("ȱ�ٲ�����!\n");
		break;
	case 21:
		printf("���ű����!\n");
		break;
	case 22:
		printf("�����ظ�����!\n");
		break;
	case 23:
		printf("����δ����!\n");
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
	if (strcmp(token, "{")) //�ж��Ƿ�'{'
	{
		es = 1; //����{
		return (es);
	}
	fscanf(fp, "%s %s\n", token, taken);
	printf("%s	:	%s\n", token, taken);
	es = declaration_list();
	if (es > 0)
		return (es);
	printf("���ű�:\n");
	printf(" ���� ��ַ\n");
	for (i = 0; i < vartablep; i++)
		printf("   %s   %d\n", symbolTable[i].name, symbolTable[i].address);
	es = statement_list();
	if (es > 0)
		return (es);
	if (strcmp(token, "}")) //�ж��Ƿ�'}'
	{
		es = 2;
		return (es);
	}
	fscanf(fp, "%s %s\n", token, taken);
	printf("%s	:	%s\n", token, taken);
	es = statement_list();
	fprintf(fout2, "    STOP\n"); //����ָֹͣ��
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
		return (es = 3); //���Ǳ�ʶ��
	es = name_def(taken); //������ű�
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
		es = if_stat(); //if���
	if (es == 0 && strcmp(token, "while") == 0)
		es = while_stat(); //while���
	if (es == 0 && strcmp(token, "for") == 0)
		es = for_stat(); //for���
	if (es == 0 && strcmp(token, "read") == 0)
		es = read_stat(); //read���
	if (es == 0 && strcmp(token, "write") == 0)
		es = write_stat(); //write���
	if (es == 0 && strcmp(token, "{") == 0)
		es = compound_stat(); //�������
	if (es == 0 && (strcmp(token, "ID") == 0)) //��ֵ���
		es = assignment_stat();
	return (es);
}

int if_stat()
{
	int es = 0, label1, label2; //if
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "("))
		return (es = 5); //��������
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = bool_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ")"))
		return (es = 6); //��������
	label1 = labelp++; //��label1��ס����Ϊ��ʱҪת��ı��
	fprintf(fout2, "      BRF LABEL %d\n", label1); //�����ת��ָ��
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = statement();
	if (es > 0)
		return (es);
	label2 = labelp++; //��label2��סҪת��ı��
	fprintf(fout2, "      BR LABEL %d\n", label2); //���������ת��ָ��
	fprintf(fout2, "LABEL%d:\n", label1); //����label1Ҫ��ǵķ���
	if (strcmp(token, "else") == 0) //else���ִ���
	{
		fscanf(fp, "%s %s\n", &token, &taken);
		printf("%s %s\n", token, taken);
		es = statement();
		if (es > 0)
			return (es);
	}
	fprintf(fout2, "LABEL%d:\n", label2); //����label2��ס�ı��
	return (es);
}

int while_stat()
{
	int es = 0, label1, label2;
	label1 = labelp++;
	fprintf(fout2, "LABEL%d:\n", label1); //����label1���
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "("))
		return (es = 5); //��������
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = bool_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ")"))
		return (es = 6); //��������
	label2 = labelp++;
	fprintf(fout2, "      BRF LABEL %d\n", label2); //�����ת��ָ��
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = statement();
	if (es > 0)
		return (es);
	fprintf(fout2, "      BR LABEL %d\n", label1); //���������ת��ָ��
	fprintf(fout2, "LABEL%d:\n", label2); //����label2���
	return (es);
}

int for_stat()
{
	int es = 0, label1, label2, label3, label4;
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, "("))
		return (es = 5); //��������
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = assignment_expression();
	if (es > 0)
		return (es);
	if (strcmp(token, ";"))
		return (es = 4); //�ٷֺ�
	label1 = labelp++;
	fprintf(fout2, "LABEL%d:\n", label1); //����label1���
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = bool_expression();
	if (es > 0)
		return (es);
	label2 = labelp++;
	fprintf(fout2, "      BRF LABEL%d\n", label2); //���������ת��ָ��
	label3 = labelp++;
	fprintf(fout2, "      BR LABEL%d\n", label3); //���������ת��ָ��
	if (strcmp(token, ";"))
		return (es = 4); //�ٷֺ�
	label4 = labelp++;
	fprintf(fout2, "LABEL%d:\n", label4); //����label4���
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = assignment_expression();
	if (es > 0)
		return (es);
	fprintf(fout2, "      POP\n"); //�����ջָ��
	fprintf(fout2, "      BR LABEL%d\n", label1); //���������ת��ָ��
	if (strcmp(token, ")"))
		return (es = 6); //ȱ��������
	fprintf(fout2, "LABEL%d:\n", label3); //����label3���
	fscanf(fp, "%s %s\n", &token, &taken);
	printf("%s %s\n", token, taken);
	es = statement();
	if (es > 0)
		return (es);
	fprintf(fout2, "      BR LABEL%d\n", label4); //���������ת��ָ��
	fprintf(fout2, "LABEL%d:\n", label2); //����label2���
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
		return (es = 4); //�ٷֺ�

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
		return (es = 3); //�ٱ�ʶ��
	es = lookup(taken, &address);
	if (es > 0)
		return (es);
	fprintf(fout2, "      IN  \n"); //���ָ��
	fprintf(fout2, "      STO  %d\n", address); //���STOָ��
	fprintf(fout2, "      POP  \n");
	fscanf(fp, "%s %s\n", token, &taken);
	printf("%s %s\n", token, taken);
	if (strcmp(token, ";"))
		return (es = 4); //�ٷֺ�
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
		strcpy_s(token2, token); //���������
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
		strcpy_s(token2, token); //���������
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
			return (es = 6); //ȱ��������
		fscanf(fp, "%s %s\n", token, &taken);
		printf("%s %s\n", token, taken);
	}
	else
	{
		if (strcmp(token, "ID") == 0)
		{
			int address;
			es = lookup(taken, &address); //����ű���ȡ������ַ
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
			es = 7; //ȱ�ٲ�����
			return (es);
		}
	}
	return (es);
}
