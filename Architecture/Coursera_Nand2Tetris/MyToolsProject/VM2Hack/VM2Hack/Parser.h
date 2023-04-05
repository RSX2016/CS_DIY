#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Command.h"

using std::fstream;

class Parser
{
public:
	Parser(std::string filename);
	bool hasMoreCommands();
	bool advance(Command& cmd);

private:
	fstream mFp;
};
