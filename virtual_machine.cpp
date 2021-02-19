#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

int TESTmachine()
{
	int es = 0, i, k = 0;
	int codecount; //单词数
	int stack[1000], stacktop = 0; //操作数栈，操作数栈顶
	char Codein[300] = "code\\target_code.txt";
	char code[1000][20]; //接收单词
	int data[1000]; //表示i地址的数据
	int label[100] = {0}; //每个三元式的标号
	char lno[4]; //
	FILE* fin; //用于指向输入输出文件的指针


	if ((fin = fopen(Codein, "r")) == NULL)
	{
		printf("\n 打开%s错误!\n", Codein);
		es = 10;
		return (es);
	}
	codecount = 0; //先初始化单词数为0
	fscanf(fin, "%s", &code[codecount]); //将单词读入到code[i]字符数组中
	while (!feof(fin)) //程序文件读入
	{
		i = strlen(code[codecount]) - 1;
		if (code[codecount][i] == ':') //查找字符数组“label：”符号
		{
			i = i - 5;
			for (int j = 0; j < i; j++)
			{
				lno[j] = code[codecount][j + 5];
			}
			lno[i] = '\0';
			label[atoi(lno)] = codecount; //用label数组记住每个标号的地址 lno是标签的数字 给每个单词标号
			printf("label[ %d]= %d \n", atoi(lno), label[atoi(lno)]); //输出label[i]在code字符数组中的位置
			code[codecount][i + 5] = ':';
			code[codecount][i + 6] = '\0';
			strcat(code[codecount], lno); //将标号与code[codecount]进行字符串连接操作
			k++; //暂时不晓得有什么用
		}
		codecount++;
		fscanf(fin, "%s", &code[codecount]); //继续读单词
	}
	fclose(fin);

	for (i = 0; i < codecount; i++)
	{
		// 处理
		int l = strlen(code[i]);
		if ((l > 1) && (code[i][1] == 'A'))
		{
			lno[0] = code[i][l - 1];
			lno[1] = '\0';
			_itoa(label[atoi(lno)], code[i], 10);
		}
		printf("%s\n", code[i]);
	}

	i = 0;

	while (i < codecount) //执行每条指令
	{
		//	printf("code % d %s \n", i,code[i]);
		if (strcmp(code[i], "LOAD") == 0) //LOAD 将D中的内容加载到操作数栈
		{
			i++;
			stack[stacktop] = data[atoi(code[i])];
			stacktop++;
		}
		else if (strcmp(code[i], "LOADI") == 0) //LOADI 将常量己压入操作数栈
		{
			i++;
			stack[stacktop] = atoi(code[i]); //字符串转int，栈顶保存数字
			stacktop++;
		}
			//STO D将操作数栈栈顶单元内容存入0,且栈顶单元内容保持不变 
		else if (strcmp(code[i], "STO") == 0)
		{
			i++;
			data[atoi(code[i])] = stack[stacktop - 1]; //data[0]='0'
			printf("sto stack %d\n", stack[stacktop - 1]); //先输出栈顶元素
			printf("sto data %d\n", data[atoi(code[i])]); //输出data[0]的数据
		}
			//POP桟顶单元内容出桟 
		else if (strcmp(code[i], "POP") == 0)
		{
			stacktop--;
		}
			//八00将次桟顶单元与栈顶单元内容出栈并相加，和置于栈顶 
		else if (strcmp(code[i], "ADD") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] + stack[stacktop - 1];
			printf("add % d\n", stack[stacktop - 1]);
			stacktop--;
		}
			//SUB 将次栈顶单元减去栈顶单元内容并出栈，差置于栈顶 
		else if (strcmp(code[i], "SUB") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] - stack[stacktop - 1];
			stacktop--;
		}
			//MULT 将次栈顶单元与栈顶单元内容出栈并相乘，积置于栈顶 
		else if (strcmp(code[i], "MULT") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] * stack[stacktop - 1];
			printf("mult % d\n", stack[stacktop - 1]);
			stacktop--;
		}
			//DIV 将次栈顶单元与栈顶单元内容出栈并相除,商置于栈顶 
		else if (strcmp(code[i], "DIV") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] / stack[stacktop - 1];
			stacktop--;
		}
			//BR lab无条件转移到lab 
		else if (strcmp(code[i], "BR") == 0)
		{
			i++;
			i = atoi(code[i]);
		}
			//BRF lab检查栈顶单元逻辑值并出栈,若为假(0)则转移到lab 
		else if (strcmp(code[i], "BRF") == 0)
		{
			i++;
			if (stack[stacktop - 1] == 0) i = atoi(code[i]);
			stacktop--;
		}
			//EQ将栈顶两单元做等于比较并出栈,并将结果真或假(1或0)置于栈顶 
		else if (strcmp(code[i], "EQ") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] == stack[stacktop - 1];
			stacktop--;
		}
			//NOTEQ栈顶两单元做不等于比较并出栈，并将结果真或假(1或0)置于栈顶 
		else if (strcmp(code[i], "NOTEQ") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] != stack[stacktop - 1];
			stacktop--;
		}
			//GT 次栈顶大于栈顶操作数并出栈，则栈顶置1,否则置0 
		else if (strcmp(code[i], "GT") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] > stack[stacktop - 1];
			stacktop--;
		}
			//LES次桟顶小于栈顶操作数并出栈，则栈顶置1,否则置0 
		else if (strcmp(code[i], "LES") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] < stack[stacktop - 1];
			stacktop--;
		}
			//GE次栈顶大于等手栈顶操作数并出栈，则桟顶置1,否则置0 
		else if (strcmp(code[i], "GE") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] >= stack[stacktop - 1];
			stacktop--;
		}
			//LE次栈顶小于等于桟顶操作数并出栈，则栈顶置1,否则置0 
		else if (strcmp(code[i], "LE") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] <= stack[stacktop - 1];
			stacktop--;
		}
			//AND将栈顶两单元做逻辑与运算并出栈，并将结果真或假(1或0>置于栈顶 
		else if (strcmp(code[i], "AND") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] && stack[stacktop - 1];
			stacktop--;
		}
			//0R将栈顶两单元做逻辑或运算并出栈，并将结果真或假(1或0)置于栈顶 
		else if (strcmp(code[i], "OR") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] || stack[stacktop - 1];
			stacktop--;
		}
			//NOT将栈顶的逻辑值取反 
		else if (strcmp(code[i], "NOT") == 0)
		{
			stack[stacktop - 1] = !stack[stacktop - 1];
		}
			//IN读入一个整型数据,并入栈 
		else if (strcmp(code[i], "IN") == 0)
		{
			printf("请输入数据：");
			scanf("%d", &stack[stacktop]);
			stacktop++;
		}
			//OUT将栈顶单元内容出栈，并输出 
		else if (strcmp(code[i], "OUT") == 0)
		{
			i++;
			printf("程序输出 %d\n", data[atoi(code[i])]);
			stacktop--;
		}
			//STOP停止执行
		else if (strcmp(code[i], "STOP") == 0)
		{
			printf("虚拟机运行成功\n");
			stack[stacktop] = '\n';
			return es = 0;
		}
		i++;
	}
	return (es);
}
