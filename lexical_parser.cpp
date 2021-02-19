#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//ȫ�ֱ�������
ifstream fin("code\\program2.txt"); //Դ�����ļ���
ofstream fout("code\\lex.txt"); //�������ļ���
int lexLineNum = 1; //�к�
int lexErrNum = 0; //��������

//������������
char read();
void print(string word, string type);
bool isChar(char ch);
bool isFigure(char ch);
bool isDelimiter(char ch);
bool isOperator(char ch);
bool isWordEnd(char ch);
char solveError(int type, char ch);
char solveError(int type);


//�ʷ�����������
bool LexicalParser()
{
	if (!fin)
	{
		cerr << "Cannot find input file!" << endl;
		return false;
	}

	char ch = read();


	//��ʼʶ��
	while (!fin.eof())
	{
		string rightWord;
		while (isWordEnd(ch) && ch != ';')
		{
			ch = read();
		}

		//ʶ���ʶ��
		if (isChar(ch))
		{
			rightWord.append(1, ch);

			ch = read();
			while (isChar(ch) || isFigure(ch))
			{
				rightWord.append(1, ch);
				ch = read();
			}

			if (isWordEnd(ch) || isDelimiter(ch) || isOperator(ch))
			{
				print(rightWord, "ID");
			}
			else
			{
				ch = solveError(1, ch);
			}
		}
			//ʶ���0���޷�������
		else if (isFigure(ch) && ch != '0')
		{
			rightWord.append(1, ch);

			ch = read();
			while (isFigure(ch))
			{
				rightWord.append(1, ch);
				ch = read();
			}

			print(rightWord, "NUM");
		}
			//ʶ��0
		else if (ch == '0')
		{
			rightWord.append(1, ch);
			print(rightWord, "NUM");
			ch = read();
		}
			//ʶ��ָ���
		else if (isDelimiter(ch))
		{
			rightWord.append(1, ch);
			ch = read();
			print(rightWord, "DEL");
		}
			//ʶ�������
		else if (isOperator(ch))
		{
			if (ch == '+' || ch == '-' || ch == '*')
			{
				rightWord.append(1, ch);
				ch = read();
				print(rightWord, "OP");
			}
			else if (ch == '=' || ch == '<' || ch == '>')
			{
				rightWord.append(1, ch);

				char ch1 = read(); //��ʱ���һ���ַ�����ƥ��
				if (ch1 == '=')
				{
					rightWord.append(1, ch1);
					ch = read();
					print(rightWord, "OP");
				}
				else
				{
					ch = ch1;
					print(rightWord, "OP");
				}
			}
			else
			{
				char ch1 = read();
				if (ch1 == '=')
				{
					rightWord.append(1, ch);
					rightWord.append(1, ch1);

					ch = read();
					print(rightWord, "OP");
				}
				else
				{
					ch = solveError(5);
				}
			}
		}
			//ʶ��ע��
		else if (ch == '/')
		{
			char ch1 = read();

			if (ch1 != '*')
			{
				//�ǳ���
				rightWord.append(1, ch);
				ch = read();
				print(rightWord, "OP");
			}
			else
			{
				//��ע�ͷ�
				rightWord.append(1, ch);
				rightWord.append(1, ch1);

				ch = read();
			a:
				while (ch != '*')
				{
					rightWord.append(1, ch);
					ch = read();

					if (!fin)
					{
						solveError(4);
						goto finish;
					}
				}

				rightWord.append(1, ch);

				ch = read();
				while (ch == '*')
				{
					rightWord.append(1, ch);
					ch = read();

					if (!fin)
					{
						solveError(4);
						goto finish;
					}
				}

				if (ch != '/')
				{
					goto a;
				}
				else
				{
					rightWord.append(1, ch);
				}

				ch = read();
			}
		}
			//�������ܽ��յ��ַ����������
		else
		{
			ch = solveError(2, ch);
		}
	}

finish:
	cout << "Lexical analysis completed." << endl;
	cout << lexErrNum << " lexical errors detected in all." << endl;
	cout << endl;
	fin.close();
	fout.close();

	return true;
}


//ÿ�δ�����������һ���ַ�
char read()
{
	char ch;
	fin.get(ch);

	return ch;
}

//����ָ����ʽ���������ӡ
void print(string word, string type)
{
	fout << lexLineNum << " " << type << " " << word << endl;
}

//�ж��Ƿ��ǵ��ʽ�β
bool isWordEnd(char ch)
{
	if (ch == ' ' || ch == ';')
	{
		return true;
	}
	else if (ch == '\n')
	{
		lexLineNum++;
		return true;
	}
	else
	{
		return false;
	}
}

//��������1����һ�������Ǵ������
char solveError(int type, char ch)
{
	lexErrNum++;

	switch (type)
	{
	case 1:
		cerr << "Lexical wrong occurred in line " << lexLineNum << " : Word has a illegal character " << ch << "."
			<< endl;
		break;
	case 2:
		cerr << "Lexical wrong occurred in line " << lexLineNum << " : " << ch << " cannot be accepted." << endl;
		break;
	}

	char c = read();
	while (isWordEnd(c))
	{
		c = read();
	}

	return c;
}

//��������2����һ�������Ǵ������
char solveError(int type)
{
	lexErrNum++;

	switch (type)
	{
	case 3:
		cerr << "Lexical wrong occurred in line " << lexLineNum << " : 0 cannot be the first figure of a number."
			<< endl;
		break;
	case 4:
		cerr << "Lexical wrong occurred in line " << lexLineNum << " : Comment character doesn't match." << endl;
		break;
	case 5:
		cerr << "Lexical wrong occurred in line " << lexLineNum << " : Wrong operator." << endl;
		break;
	}

	char c = read();
	while (isWordEnd(c))
	{
		c = read();
	}

	return c;
}

//�Ƿ�����ĸ
bool isChar(char ch)
{
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
	{
		return true;
	}
	else
	{
		return 0;
	}
}

//�Ƿ�������
bool isFigure(char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�Ƿ��Ƿָ���
bool isDelimiter(char ch)
{
	if (ch == '(' || ch == ')' || ch == ';' || ch == '{' || ch == '}')
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�Ƿ��������
bool isOperator(char ch)
{
	char op[7] = {'+', '-', '*', '=', '!', '<', '>'};

	for (char i : op)
	{
		if (ch == i)
			return true;
	}

	return false;
}
