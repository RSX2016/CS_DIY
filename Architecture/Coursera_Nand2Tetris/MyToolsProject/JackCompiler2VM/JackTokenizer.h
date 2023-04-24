#pragma once
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Token.h"

using std::fstream;


class JackTokenizer
{
public:
	static std::vector<std::vector<int>> s_analysisAM;
	static std::vector<Token*> s_AMEndState;
	static void initAM();


public:
	JackTokenizer(std::string filename);
	bool hasMoreTokens();
	bool advance(Token& token);
	bool lookAhead(Token& token);

private:
	void getNewLine();
	bool jumpSpace();

private:
	fstream mFp;
	std::string mLine;
	int mLineIndex;
};
