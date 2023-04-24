#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Token.h"
#include "JackTokenizer.h"
#include "SymbolTable.h"
#include <string>


class CompliationEngine {
public:
	CompliationEngine();
	void start(std::string fileName, JackTokenizer& jackTokenizer);

private:
	bool compile_class();
	bool compile_classVarDec();
	bool compile_type(std::string *id = NULL);
	bool compile_subroutineDec();
	bool compile_parameterList(int* paramCounts = 0);
	bool compile_subroutineBody(int funtionType = 0);
	bool compile_varDec(int *varCount = 0);
	bool compile_className(std::string* id = NULL);
	bool compile_subroutineName(std::string* id = NULL);
	bool compile_varName(std::string* id = NULL);
	bool compile_statements();
	bool compile_statement();
	bool compile_letStatement();
	bool compile_ifStatement();
	bool compile_whileStatement();
	bool compile_doStatement();
	bool compile_returnStatement();
	bool compile_expression();
	bool compile_term();
	bool compile_subroutineCall();
	bool compile_expressionList(int *argCount = 0);
	bool compile_op(std::string* vmCode = 0);
	bool compile_unaryOp(std::string* vmCode = 0);
	bool compile_KeywordConstant();

	void compile_err(std::string mode, std::string expectFor);
	void compilationXmlWrite();
	void compilationXmlWrite(std::string s);

private:

	Token mTk;
	JackTokenizer *mJackTokenizer;
	fstream mVMFp;
	fstream mTestXmlFp;

	std::string mClassName;
	SymbolTable mSymbolTable;
};