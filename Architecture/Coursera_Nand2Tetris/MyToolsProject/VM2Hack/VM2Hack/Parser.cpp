#include "Parser.h"
#include <vector>
#include <sstream>

Parser::Parser(std::string filename)
{
	mFp.open(filename, std::ios_base::in);
	if (!mFp.is_open()) {
		std::cout << "file [ " << filename << "] open failed!! \n";
		return;
	}

}


bool Parser::hasMoreCommands()
{
	return !mFp.eof();
}


bool Parser::advance(Command& cmd)
{
	std::string line;
	char buffer[1024];

	mFp.getline(buffer, 1024);
	line = buffer;
	std::cout << line << " num = " << line.length() << std::endl;

	std::istringstream ss(line);
	std::vector<std::string> words;
	std::string word;
	while (ss >> word)
	{
		words.push_back(word);
	}

	if (words.size() == 0) { return false; }

	if (words[0].find("//") == 0) { return false; }

	if (words[0] == "push") {
		cmd.mCType = cmd.C_PUSH;
		cmd.mArg1 = words[1];
		cmd.mArg2 = words[2];
		return true;
	}
	else if (words[0] == "pop") {
		cmd.mCType = cmd.C_POP;
		cmd.mArg1 = words[1];
		cmd.mArg2 = words[2];
		return true;
	}
	else if (words[0] == "add" || words[0] == "sub" || words[0] == "neg"
				|| words[0] == "eq" || words[0] == "gt" || words[0] == "lt"
				|| words[0] == "and" || words[0] == "or" || words[0] == "not") {
		cmd.mCType = Command::C_ARITHMETIC;
		cmd.mArg1 = words[0];
		return true;
	}
	else if (words[0] == "label") {
		cmd.mCType = Command::C_LABEL;
		cmd.mArg1 = words[1];
		return true;
	}
	else if (words[0] == "goto") {
		cmd.mCType = Command::C_GOTO;
		cmd.mArg1 = words[1];
		return true;
	}
	else if (words[0] == "if-goto") {
		cmd.mCType = Command::C_IF;
		cmd.mArg1 = words[1];
		return true;
	}
	else if (words[0] == "function") {
		cmd.mCType = Command::C_FUNCTION;
		cmd.mArg1 = words[1];
		cmd.mArg2 = words[2];
		return true;
	}
	else if (words[0] == "return") {
		cmd.mCType = Command::C_RETURN;
		return true;
	}
	else if (words[0] == "call") {
		cmd.mCType = Command::C_CALL;
		cmd.mArg1 = words[1];
		cmd.mArg2 = words[2];
		return true;
	}


	return false;
}