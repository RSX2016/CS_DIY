#include "Token.h"
#include <iostream>

std::map<std::string, Token::eKeyword> Token::s_eKeywordMap = std::map<std::string, Token::eKeyword>();
std::map<Token::eKeyword, std::string> Token::s_eKeywordMapReverse = std::map<Token::eKeyword, std::string>();

Token::Token()
{
	if (Token::s_eKeywordMap.size() == 0) {
		Token::s_eKeywordMap["boolean"] = Token::eKeyword::eK_BOOLEAN;
		Token::s_eKeywordMap["class"] = Token::eKeyword::eK_CLASS;
		Token::s_eKeywordMap["method"] = Token::eKeyword::eK_METHOD;
		Token::s_eKeywordMap["int"] = Token::eKeyword::eK_INT;
		Token::s_eKeywordMap["function"] = Token::eKeyword::eK_FUNCTION;
		Token::s_eKeywordMap["constructor"] = Token::eKeyword::eK_CONSTRUCTOR;
		Token::s_eKeywordMap["char"] = Token::eKeyword::eK_CHAR;
		Token::s_eKeywordMap["void"] = Token::eKeyword::eK_VOID;
		Token::s_eKeywordMap["var"] = Token::eKeyword::eK_VAR;
		Token::s_eKeywordMap["static"] = Token::eKeyword::eK_STATIC;
		Token::s_eKeywordMap["field"] = Token::eKeyword::eK_FIELD;
		Token::s_eKeywordMap["let"] = Token::eKeyword::eK_LET;
		Token::s_eKeywordMap["do"] = Token::eKeyword::eK_DO;
		Token::s_eKeywordMap["if"] = Token::eKeyword::eK_IF;
		Token::s_eKeywordMap["else"] = Token::eKeyword::eK_ELSE;
		Token::s_eKeywordMap["while"] = Token::eKeyword::eK_WHILE;
		Token::s_eKeywordMap["return"] = Token::eKeyword::eK_RETURN;
		Token::s_eKeywordMap["false"] = Token::eKeyword::eK_FALSE;
		Token::s_eKeywordMap["null"] = Token::eKeyword::eK_NULL;
		Token::s_eKeywordMap["this"] = Token::eKeyword::eK_THIS;


		Token::s_eKeywordMapReverse[Token::eKeyword::eK_BOOLEAN] = "boolean";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_CLASS] = "class";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_METHOD] =  "method";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_INT] = "int";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_FUNCTION] =  "function";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_CONSTRUCTOR] = "constructor";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_CHAR] = "char";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_VOID] =  "void";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_VAR] =  "var";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_STATIC] =  "static";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_FIELD] = "field";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_LET] =  "let";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_DO] = "do";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_IF] =  "if";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_ELSE] = "else";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_WHILE] =  "while";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_RETURN] =  "return";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_FALSE] =  "false";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_NULL] =  "null";
		Token::s_eKeywordMapReverse[Token::eKeyword::eK_THIS] =  "this";


	}


}