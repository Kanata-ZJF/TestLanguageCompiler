#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//全局变量声明
ifstream lexFileIn;
int gramLineNum = 0;
string type;
string symbol;

//获取下一个符号
void GetNextSymbol()
{
	if (lexFileIn >> gramLineNum >> type >> symbol)
	{
		cout << gramLineNum << " " << type << " " << symbol << endl;
	}
	else
	{
		type = "";
		symbol = "";
	}
}

//在控制台打印错误
void PrintError(string str)
{
	cout << "Grammatical error detected in line " << gramLineNum << " : " << str << endl;
}

//函数列表
bool program();
bool declaration_list();
bool declaration_stat();
bool statement_list();
bool statement();
bool if_stat();
bool while_stat();
bool for_stat();
bool write_stat();
bool read_stat();
bool compound_stat();
bool assignment_expression();
bool assignment_stat();
bool bool_expression();
bool arithmetic_expression();
bool term();
bool factor();

//语法分析主程序
bool GrammaticalParser()
{
	lexFileIn.open("code\\lex.txt");

	if (!lexFileIn)
	{
		cout << "Cannot find lexical analysis file!" << endl;
		return false;
	}

	const bool status = program();
	if (!status)
	{
		cout << "Grammar analysis terminated in line " << gramLineNum << "." << endl;
		lexFileIn.close();
		return true;
	}

	cout << "Grammar analysis completed!" << endl;
	lexFileIn.close();

	return true;
}

//program ::= {<declaration_list><statement_list>}
bool program()
{
	bool status = true;

	GetNextSymbol();

	if (symbol != "{")
	{
		PrintError("\'{\' is expected but not found. <program>1");
		return false;
	}
	GetNextSymbol();

	status = declaration_list();
	if (!status)
	{
		return false;
	}

	status = statement_list();
	if (!status)
	{
		return false;
	}

	if (symbol != "}")
	{
		PrintError("\'}\' is expected but not found. <program>2");
		return false;
	}

	return true;
}

// <declaration_list> ::= {<declaration_stat>}
bool declaration_list()
{
	bool status = true;
	while (symbol == "int")
	{
		status = declaration_stat();
		if (!status)
			return false;
	}

	return true;
}

// <declaration_stat> ::= int ID;
bool declaration_stat()
{
	GetNextSymbol();

	if (type != "ID")
	{
		PrintError("ID is required but not found. <declaration_stat>1");
		return false;
	}
	GetNextSymbol();

	if (symbol != ";")
	{
		PrintError("\';\' is expected but not found. <declaration_stat>2");
		return false;
	}
	GetNextSymbol();

	return true;
}

// <statement_list> ::= {<statement>}
bool statement_list()
{
	bool status = true;
	while (symbol != "}")
	{
		status = statement();
		if (!status)
			return false;
	}

	return true;
}

// <statement> ::= <if_stat> | <while_stat> | < for_stat> | <compound_stat> | <assignment_stat> | ;
bool statement()
{
	bool status = true;

	if (symbol == "if")
	{
		status = if_stat();
		if (!status)
			return false;
	}
	else if (symbol == "while")
	{
		status = while_stat();
		if (!status)
			return false;
	}
	else if (symbol == "for")
	{
		status = for_stat();
		if (!status)
			return false;
	}
	else if (symbol == "read")
	{
		status = read_stat();
		if (!status)
			return false;
	}
	else if (symbol == "write")
	{
		status = write_stat();
		if (!status)
			return false;
	}
	else if (symbol == "{")
	{
		status = compound_stat();
		if (!status)
			return false;
	}
	else if (type == "ID")
	{
		status = assignment_stat();
		if (!status)
			return false;
	}
	else if (symbol == ";")
	{
		GetNextSymbol();
	}
	else
	{
		PrintError("illegal symbol. <statement>");
		return false;
	}

	return true;
}

// <if_stat> ::= if (<bool_expression>) <statement> [else <statement>]
bool if_stat()
{
	bool status = true;

	GetNextSymbol();

	if (symbol != "(")
	{
		PrintError("\'(\' is expected but not found. <if_stat>1");
		return false;
	}
	GetNextSymbol();

	status = bool_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ")")
	{
		PrintError("\')\' is expected but not found. <if_stat>2");
		return false;
	}
	GetNextSymbol();

	status = statement();
	if (!status)
	{
		return false;
	}

	if (symbol == "else")
	{
		GetNextSymbol();

		status = statement();
		if (!status)
			return false;
	}

	return true;
}

// <while_stat> ::= while (<bool_expression>) <statement>
bool while_stat()
{
	bool status = true;

	GetNextSymbol();

	if (symbol != "(")
	{
		PrintError("\'(\' is expected but not found. <while_stat>1");
		return false;
	}
	GetNextSymbol();

	status = bool_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ")")
	{
		PrintError("\')\' is expected but not found. <while_stat>2");
		return false;
	}
	GetNextSymbol();

	status = statement();
	if (!status)
	{
		return false;
	}

	return true;
}

// <for_stat> ::= for (<assignment_expression> ; <bool_expression> ; <assignment_expression> ) <statement>
bool for_stat()
{
	bool status = true;

	GetNextSymbol();

	if (symbol != "(")
	{
		PrintError("\'(\' is expected but not found. <for_stat>1");
		return false;
	}
	GetNextSymbol();

	status = assignment_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ";")
	{
		PrintError("\';\' is expected but not found. <for_stat>2");
		return false;
	}
	GetNextSymbol();

	status = bool_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ";")
	{
		PrintError("\';\' is expected but not found. <for_stat>3");
		return false;
	}
	GetNextSymbol();

	status = assignment_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ")")
	{
		PrintError("\')\' is expected but not found. <for_stat>4");
		return false;
	}
	GetNextSymbol();

	status = statement();
	if (!status)
	{
		return false;
	}

	return true;
}

// <write_stat> ::= write <arithmetic_expression>
bool write_stat()
{
	bool status = true;

	GetNextSymbol();

	status = arithmetic_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ";")
	{
		PrintError("\';\' is expected but not found. <write_stat>");
		return false;
	}
	GetNextSymbol();

	return true;
}

// <read_stat> ::= read ID;
bool read_stat()
{
	bool status = true;

	GetNextSymbol();

	if (type != "ID")
	{
		PrintError("ID is required but not found. <read_stat>1");
		return false;
	}
	GetNextSymbol();

	if (symbol != ";")
	{
		PrintError("\';\' is expected but not found. <read_stat>2");
		return false;
	}
	GetNextSymbol();

	return true;
}

// <compound_stat>  ::= { <statement_list> }
bool compound_stat()
{
	bool status = true;

	GetNextSymbol();

	status = statement_list();
	if (!status)
	{
		return false;
	}

	if (symbol != "}")
	{
		PrintError("\'}\' is expected but not found. <compound_stat>");
	}
	GetNextSymbol();

	return true;
}

// <assignment_expression> ::= ID = <arithmetic_expression>
bool assignment_expression()
{
	bool status = true;

	if (type != "ID")
	{
		PrintError("ID is required but not found. <assignment_expression>1");
		return false;
	}
	GetNextSymbol();

	if (symbol != "=")
	{
		PrintError("\'=\' is expected but not found. <assignment_expression>2");
		return false;
	}
	GetNextSymbol();

	status = arithmetic_expression();
	if (!status)
	{
		return false;
	}

	return true;
}

// <assignment_stat> ::= <assignment_expression>;
bool assignment_stat()
{
	bool status = true;

	status = assignment_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ";")
	{
		PrintError("\';\' is expected but not found. <assignment_stat>");
		return false;
	}
	GetNextSymbol();

	return true;
}

// <bool_expression> ::= <arithmetic_expression> ( > | < | >= | <= | == | !=) <arithmetic_expression>>
bool bool_expression()
{
	bool status = true;

	status = arithmetic_expression();
	if (!status)
	{
		return false;
	}

	if (symbol != ">" && symbol != "<" && symbol != ">=" && symbol != "<=" && symbol != "==" && symbol != "!=")
	{
		PrintError("Compared operator is required but not found. <bool_expression>");
		return false;
	}
	GetNextSymbol();

	status = arithmetic_expression();
	if (!status)
	{
		return false;
	}

	return true;
}

// <arithmetic_expression> ::= <term> {(+ | -) <term>}
bool arithmetic_expression()
{
	bool status = true;

	status = term();
	if (!status)
	{
		return false;
	}

	while (symbol == "+" || symbol == "-")
	{
		GetNextSymbol();

		status = term();
		if (!status)
		{
			return false;
		}
	}

	return true;
}

// <term> ::= <factor> { (* | /) <factor>}
bool term()
{
	bool status = true;

	status = factor();
	if (!status)
	{
		return false;
	}

	while (symbol == "*" || symbol == "/")
	{
		GetNextSymbol();

		status = factor();
		if (!status)
		{
			return false;
		}
	}

	return true;
}

// <factor> ::= (<arithmetic_expression>) ID | NUM
bool factor()
{
	bool status = true;

	if (symbol == "(")
	{
		GetNextSymbol();

		status = arithmetic_expression();
		if (!status)
		{
			return false;
		}

		if (symbol != ")")
		{
			PrintError("\')\' is expected but not found. <factor>1");
			return false;
		}
		GetNextSymbol();
	}
	else if (type == "ID" || type == "NUM")
	{
		GetNextSymbol();
	}
	else
	{
		PrintError("operand is required but not found. <factor>2");
		return false;
	}

	return true;
}
