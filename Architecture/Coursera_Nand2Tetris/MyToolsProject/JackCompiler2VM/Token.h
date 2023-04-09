#pragma once
#include <string>
#include <map>


class Token
{
public:
	static enum eTokenType
	{
		eTT_KEYWORD,
		eTT_SYMBOL,
		eTT_ID,
		eTT_INT_CONST,
		eTT_STRING_CONST
	};

	static enum eKeyword
	{
		eK_CLASS,
		eK_METHOD,
		eK_INT,
		eK_FUNCTION,
		eK_BOOLEAN,
		eK_CONSTRUCTOR,
		eK_CHAR,
		eK_VOID,
		eK_VAR,
		eK_STATIC,
		eK_FIELD,
		eK_LET,
		eK_DO,
		eK_IF,
		eK_ELSE,
		eK_WHILE,
		eK_RETURN,
		eK_FALSE,
		eK_NULL,
		eK_THIS
	};
public:
	Token();
	static std::map<std::string, Token::eKeyword> s_eKeywordMap;
	static std::map<Token::eKeyword, std::string> s_eKeywordMapReverse;

	eTokenType mTokenType;
	eKeyword mKeywordType;
	char mSymbol;
	std::string mID;
	int mIntVal;
	std::string mStringVal;
};
