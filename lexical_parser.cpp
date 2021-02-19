#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//全局变量声明
ifstream fin("code\\program2.txt"); //源代码文件流
ofstream fout("code\\lex.txt"); //输出结果文件流
int lexLineNum = 1; //行号
int lexErrNum = 0; //错误数量

//辅助函数声明
char read();
void print(string word, string type);
bool isChar(char ch);
bool isFigure(char ch);
bool isDelimiter(char ch);
bool isOperator(char ch);
bool isWordEnd(char ch);
char solveError(int type, char ch);
char solveError(int type);


//词法分析主函数
bool LexicalParser()
{
	if (!fin)
	{
		cerr << "Cannot find input file!" << endl;
		return false;
	}

	char ch = read();


	//开始识别
	while (!fin.eof())
	{
		string rightWord;
		while (isWordEnd(ch) && ch != ';')
		{
			ch = read();
		}

		//识别标识符
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
			//识别非0的无符号整数
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
			//识别0
		else if (ch == '0')
		{
			rightWord.append(1, ch);
			print(rightWord, "NUM");
			ch = read();
		}
			//识别分隔符
		else if (isDelimiter(ch))
		{
			rightWord.append(1, ch);
			ch = read();
			print(rightWord, "DEL");
		}
			//识别运算符
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

				char ch1 = read(); //此时多读一个字符用于匹配
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
			//识别注释
		else if (ch == '/')
		{
			char ch1 = read();

			if (ch1 != '*')
			{
				//是除号
				rightWord.append(1, ch);
				ch = read();
				print(rightWord, "OP");
			}
			else
			{
				//是注释符
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
			//遇到不能接收的字符，处理错误
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


//每次从输入流读入一个字符
char read()
{
	char ch;
	fin.get(ch);

	return ch;
}

//按照指定格式向输出流打印
void print(string word, string type)
{
	fout << lexLineNum << " " << type << " " << word << endl;
}

//判断是否是单词结尾
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

//错误处理函数1，第一个参数是错误代码
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

//错误处理函数2，第一个参数是错误代码
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

//是否是字母
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

//是否是数字
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

//是否是分隔符
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

//是否是运算符
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
