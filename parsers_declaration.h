#pragma once
#include <string>

//�ʷ��������ú���
char read();

void print(std::string word, std::string type);

bool isWordEnd(char ch);

char solveError(int type, char ch);

char solveError(int type);

bool isChar(char ch);

bool isFigure(char ch);

bool isDelimiter(char ch);

bool isOperator(char ch);

bool LexicalParser();

int TESTscan();

//�﷨�������ú���
void GetNextSymbol();

void PrintError(std::string str);

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

bool GrammaticalParser();

//����������ú���
int TESTparse();

//�����
int TESTmachine();
