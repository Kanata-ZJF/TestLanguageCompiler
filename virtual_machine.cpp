#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

int TESTmachine()
{
	int es = 0, i, k = 0;
	int codecount; //������
	int stack[1000], stacktop = 0; //������ջ��������ջ��
	char Codein[300] = "code\\target_code.txt";
	char code[1000][20]; //���յ���
	int data[1000]; //��ʾi��ַ������
	int label[100] = {0}; //ÿ����Ԫʽ�ı��
	char lno[4]; //
	FILE* fin; //����ָ����������ļ���ָ��


	if ((fin = fopen(Codein, "r")) == NULL)
	{
		printf("\n ��%s����!\n", Codein);
		es = 10;
		return (es);
	}
	codecount = 0; //�ȳ�ʼ��������Ϊ0
	fscanf(fin, "%s", &code[codecount]); //�����ʶ��뵽code[i]�ַ�������
	while (!feof(fin)) //�����ļ�����
	{
		i = strlen(code[codecount]) - 1;
		if (code[codecount][i] == ':') //�����ַ����顰label��������
		{
			i = i - 5;
			for (int j = 0; j < i; j++)
			{
				lno[j] = code[codecount][j + 5];
			}
			lno[i] = '\0';
			label[atoi(lno)] = codecount; //��label�����סÿ����ŵĵ�ַ lno�Ǳ�ǩ������ ��ÿ�����ʱ��
			printf("label[ %d]= %d \n", atoi(lno), label[atoi(lno)]); //���label[i]��code�ַ������е�λ��
			code[codecount][i + 5] = ':';
			code[codecount][i + 6] = '\0';
			strcat(code[codecount], lno); //�������code[codecount]�����ַ������Ӳ���
			k++; //��ʱ��������ʲô��
		}
		codecount++;
		fscanf(fin, "%s", &code[codecount]); //����������
	}
	fclose(fin);

	for (i = 0; i < codecount; i++)
	{
		// ����
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

	while (i < codecount) //ִ��ÿ��ָ��
	{
		//	printf("code % d %s \n", i,code[i]);
		if (strcmp(code[i], "LOAD") == 0) //LOAD ��D�е����ݼ��ص�������ջ
		{
			i++;
			stack[stacktop] = data[atoi(code[i])];
			stacktop++;
		}
		else if (strcmp(code[i], "LOADI") == 0) //LOADI ��������ѹ�������ջ
		{
			i++;
			stack[stacktop] = atoi(code[i]); //�ַ���תint��ջ����������
			stacktop++;
		}
			//STO D��������ջջ����Ԫ���ݴ���0,��ջ����Ԫ���ݱ��ֲ��� 
		else if (strcmp(code[i], "STO") == 0)
		{
			i++;
			data[atoi(code[i])] = stack[stacktop - 1]; //data[0]='0'
			printf("sto stack %d\n", stack[stacktop - 1]); //�����ջ��Ԫ��
			printf("sto data %d\n", data[atoi(code[i])]); //���data[0]������
		}
			//POP�C����Ԫ���ݳ��C 
		else if (strcmp(code[i], "POP") == 0)
		{
			stacktop--;
		}
			//��00���ΗC����Ԫ��ջ����Ԫ���ݳ�ջ����ӣ�������ջ�� 
		else if (strcmp(code[i], "ADD") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] + stack[stacktop - 1];
			printf("add % d\n", stack[stacktop - 1]);
			stacktop--;
		}
			//SUB ����ջ����Ԫ��ȥջ����Ԫ���ݲ���ջ��������ջ�� 
		else if (strcmp(code[i], "SUB") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] - stack[stacktop - 1];
			stacktop--;
		}
			//MULT ����ջ����Ԫ��ջ����Ԫ���ݳ�ջ����ˣ�������ջ�� 
		else if (strcmp(code[i], "MULT") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] * stack[stacktop - 1];
			printf("mult % d\n", stack[stacktop - 1]);
			stacktop--;
		}
			//DIV ����ջ����Ԫ��ջ����Ԫ���ݳ�ջ�����,������ջ�� 
		else if (strcmp(code[i], "DIV") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] / stack[stacktop - 1];
			stacktop--;
		}
			//BR lab������ת�Ƶ�lab 
		else if (strcmp(code[i], "BR") == 0)
		{
			i++;
			i = atoi(code[i]);
		}
			//BRF lab���ջ����Ԫ�߼�ֵ����ջ,��Ϊ��(0)��ת�Ƶ�lab 
		else if (strcmp(code[i], "BRF") == 0)
		{
			i++;
			if (stack[stacktop - 1] == 0) i = atoi(code[i]);
			stacktop--;
		}
			//EQ��ջ������Ԫ�����ڱȽϲ���ջ,�����������(1��0)����ջ�� 
		else if (strcmp(code[i], "EQ") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] == stack[stacktop - 1];
			stacktop--;
		}
			//NOTEQջ������Ԫ�������ڱȽϲ���ջ�������������(1��0)����ջ�� 
		else if (strcmp(code[i], "NOTEQ") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] != stack[stacktop - 1];
			stacktop--;
		}
			//GT ��ջ������ջ������������ջ����ջ����1,������0 
		else if (strcmp(code[i], "GT") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] > stack[stacktop - 1];
			stacktop--;
		}
			//LES�ΗC��С��ջ������������ջ����ջ����1,������0 
		else if (strcmp(code[i], "LES") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] < stack[stacktop - 1];
			stacktop--;
		}
			//GE��ջ�����ڵ���ջ������������ջ����C����1,������0 
		else if (strcmp(code[i], "GE") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] >= stack[stacktop - 1];
			stacktop--;
		}
			//LE��ջ��С�ڵ��ڗC������������ջ����ջ����1,������0 
		else if (strcmp(code[i], "LE") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] <= stack[stacktop - 1];
			stacktop--;
		}
			//AND��ջ������Ԫ���߼������㲢��ջ�������������(1��0>����ջ�� 
		else if (strcmp(code[i], "AND") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] && stack[stacktop - 1];
			stacktop--;
		}
			//0R��ջ������Ԫ���߼������㲢��ջ�������������(1��0)����ջ�� 
		else if (strcmp(code[i], "OR") == 0)
		{
			stack[stacktop - 2] = stack[stacktop - 2] || stack[stacktop - 1];
			stacktop--;
		}
			//NOT��ջ�����߼�ֵȡ�� 
		else if (strcmp(code[i], "NOT") == 0)
		{
			stack[stacktop - 1] = !stack[stacktop - 1];
		}
			//IN����һ����������,����ջ 
		else if (strcmp(code[i], "IN") == 0)
		{
			printf("���������ݣ�");
			scanf("%d", &stack[stacktop]);
			stacktop++;
		}
			//OUT��ջ����Ԫ���ݳ�ջ������� 
		else if (strcmp(code[i], "OUT") == 0)
		{
			i++;
			printf("������� %d\n", data[atoi(code[i])]);
			stacktop--;
		}
			//STOPִֹͣ��
		else if (strcmp(code[i], "STOP") == 0)
		{
			printf("��������гɹ�\n");
			stack[stacktop] = '\n';
			return es = 0;
		}
		i++;
	}
	return (es);
}
