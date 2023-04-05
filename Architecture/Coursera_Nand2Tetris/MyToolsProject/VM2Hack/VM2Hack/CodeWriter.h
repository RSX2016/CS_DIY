#pragma once
#include <string>
#include <fstream>
#include "Command.h"

using std::fstream;

class CodeWriter
{
public:
	CodeWriter(std::string fileName);

	void testWrite(std::string txt) {
		mFp << txt;
	}

	void setFileName(std::string fileName) { mFileName = fileName; }
	void cmdWrite(Command& cmd);

	void initCmdWrite();

	void finish();
private:
	fstream mFp;
	std::string mFileName;
	std::string mFunctionName;
	int		mEQ_Index;
	int		mLT_Index;
	int		mGT_Index;
	int     mFucEnd_Index;
};
