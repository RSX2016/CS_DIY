#pragma once
#include <string>

class Command
{
public:
	static enum CType {
		C_ARITHMETIC,
		C_PUSH,
		C_POP,
		C_LABEL,
		C_GOTO,
		C_IF,
		C_FUNCTION,
		C_RETURN,
		C_CALL
	};

	CType mCType;
	std::string mArg1;
	std::string mArg2;
};
