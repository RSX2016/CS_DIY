#include "codeWriter.h"

#include <iostream>

using std::fstream;

CodeWriter::CodeWriter(std::string fileName)
	:mEQ_Index(0), mLT_Index(0), mGT_Index(0), mFucEnd_Index(0)
	, mFunctionName("unKnowFunction"), mFileName("unKnowFilename")
{	
	mFp.open(fileName, std::ios_base::out|std::ios_base::trunc);
	if (!mFp.is_open()) {
		std::cout << "file [ " << fileName << "] open failed!! \n";
		return;
	}

	initCmdWrite();
}

void CodeWriter::initCmdWrite()
{
	std::string result("");
	result = result + "@256\n" + "D=A\n" + "@SP\n" + "M=D\n"; // 将SP设置为 RAM[256]
	result = result + "@1\n" + "D=A\n" + "@LCL\n" + "M=D\n"; // 设置LCL=1
	result = result + "@2\n" + "D=A\n" + "@ARG\n" + "M=D\n"; // 设置ARG=1
	result = result + "@3\n" + "D=A\n" + "@THIS\n" + "M=D\n"; // 设置THIS=1
	result = result + "@4\n" + "D=A\n" + "@THAT\n" + "M=D\n"; // 设置THAT=1

	mFp << result;

	Command cmd;
	cmd.mCType = Command::C_CALL;
	cmd.mArg1 = "Sys.init";
	cmd.mArg2 = "0";
	cmdWrite(cmd);
}

void CodeWriter::cmdWrite(Command& cmd)
{
	std::string result("");

	switch (cmd.mCType)
	{
	case Command::C_PUSH:
		if (cmd.mArg1 == "constant") {
			result = result +  "@" + cmd.mArg2 + "\n";
			result = result + "D=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
		}
		else if (cmd.mArg1 == "local" || cmd.mArg1 == "argument" || cmd.mArg1 == "this" 
				|| cmd.mArg1 == "that" || cmd.mArg1 == "temp" || cmd.mArg1 == "pointer") {
			result = result + "@" + cmd.mArg2 + "\n" + "D=A\n";
			if (cmd.mArg1 == "local")			{ result = result + "@LCL\n" + "A=M+D\n"; }
			else if (cmd.mArg1 == "argument")	{ result = result + "@ARG\n" + "A=M+D\n"; }
			else if (cmd.mArg1 == "this")		{ result = result + "@THIS\n" + "A=M+D\n"; }
			else if (cmd.mArg1 == "that")		{ result = result + "@THAT\n" + "A=M+D\n"; }
			else if (cmd.mArg1 == "temp")		{ result = result + "@5\n" + "A=A+D\n"; }
			else if (cmd.mArg1 == "pointer")	{ result = result + "@3\n" + "A=A+D\n"; }
			result = result  + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		}
		else if (cmd.mArg1 == "static") {
			result = result + "@" + mFileName + "." + cmd.mArg2 + "\n";
			result = result + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		}
		break;
	case Command::C_POP:
		if (cmd.mArg1 == "local" || cmd.mArg1 == "argument" || cmd.mArg1 == "this"
			|| cmd.mArg1 == "that" || cmd.mArg1 == "temp" || cmd.mArg1 == "pointer") {
			result = result + "@" + cmd.mArg2 + "\n" + "D=A\n";
			if (cmd.mArg1 == "local")			{ result = result + "@LCL\n" + "D=M+D\n"; }
			else if (cmd.mArg1 == "argument")	{ result = result + "@ARG\n" + "D=M+D\n"; }
			else if (cmd.mArg1 == "this")		{ result = result + "@THIS\n" + "D=M+D\n"; }
			else if (cmd.mArg1 == "that")		{ result = result + "@THAT\n" + "D=M+D\n"; }
			else if (cmd.mArg1 == "temp")		{ result = result + "@5\n" + "D=A+D\n"; }
			else if (cmd.mArg1 == "pointer")	{ result = result + "@3\n" + "D=A+D\n"; }
			result = result  + "@R15\n" + "M=D\n" + "@SP\n" + "AM=M-1\n" + "D=M\n" + "@R15\n" + "A=M\n" + "M=D\n";
		}
		else if (cmd.mArg1 == "static") {
			result = result + "@SP\n" + "AM=M-1\n" + "D=M\n";
			result = result + "@" + mFileName + "." + cmd.mArg2 + "\n";
			result += "M=D\n";
		}
		break;
	case Command::C_ARITHMETIC:
		if (cmd.mArg1 == "add" || cmd.mArg1 == "sub" || cmd.mArg1 == "and" || cmd.mArg1 == "or") {
			result = result + "@SP\n" + "AM=M-1\n" + "D=M\n" + "A=A-1\n";
			if (cmd.mArg1 == "add") { result += "M=M+D\n"; }
			else if (cmd.mArg1 == "sub") { result += "M=M-D\n"; }
			else if (cmd.mArg1 == "and") { result += "M=M&D\n"; }
			else if (cmd.mArg1 == "or") { result += "M=M|D\n"; }			
		}
		else if (cmd.mArg1 == "eq" || cmd.mArg1 == "lt" || cmd.mArg1 == "gt") {
			result = result + "@SP\n" + "AM=M-1\n" + "D=M\n" + "A=A-1\n" + "D=M-D\n" + "M=0\n";
			if (cmd.mArg1 == "eq") { result += "@eq_" + std::to_string(this->mEQ_Index) + "\n" + "D;JNE\n";	}
			else if (cmd.mArg1 == "lt") { result += "@lt_" + std::to_string(this->mLT_Index) + "\n" + "D;JGE\n"; }
			else if (cmd.mArg1 == "gt") { result += "@gt_" + std::to_string(this->mGT_Index) + "\n" + "D;JLE\n"; }
			result = result + "@SP\n" + "A=M-1\n" + "M=-1\n";
			if (cmd.mArg1 == "eq") { result += "(eq_" + std::to_string(this->mEQ_Index++) + ")\n";}
			else if (cmd.mArg1 == "lt") { result += "(lt_" + std::to_string(this->mLT_Index++) + ")\n"; }
			else if (cmd.mArg1 == "gt") { result += "(gt_" + std::to_string(this->mGT_Index++) + ")\n"; }
		}
		else if (cmd.mArg1 == "not" || cmd.mArg1 == "neg") {
			result = result + "@SP\n" + "A=M-1\n";
			if (cmd.mArg1 == "not") { result += "M=!M\n"; }
			else if (cmd.mArg1 == "neg") { result += "M=-M\n"; }
		}
		break;
	case Command::C_LABEL:
		result = result + "(" + mFunctionName + "$" + cmd.mArg1 + ")\n";
		break;
	case Command::C_GOTO:
		result = result + "@" + mFunctionName + "$" + cmd.mArg1 + "\n" + "0;JMP\n";
		break;
	case Command::C_IF:
		result = result + "@SP\n" + "AM=M-1\n" + "D=M\n";
		result = result + "@" + mFunctionName + "$" + cmd.mArg1 + "\n" + "D;JNE\n";
		break;
	case Command::C_FUNCTION:
	{
		mFunctionName = cmd.mArg1;
		result = result + "(" + cmd.mArg1 + ")\n";
		int count = std::stoi(cmd.mArg2);
		for (int i = 0; i < count; ++i) {
			result = result + "@SP\n" + "A=M\n" + "M=0\n" + "@SP\n" + "M=M+1\n";
		}
	}
		break;
	case Command::C_RETURN:
		result = result + "@LCL\n" + "D=M\n" + "@R15\n" + "M=D\n";
		result = result + "@5\n" + "D=A\n" + "@R15\n" + "A=M-D\n" + "D=M\n" + "@R14\n" + "M=D\n";
		result = result + "@SP\n" + "AM=M-1\n" + "D=M\n" + "@ARG\n" + "A=M\n" + "M=D\n";
		result = result + "@ARG\n" + "D=M+1\n" + "@SP\n" + "M=D\n";
		result = result + "@R15\n" + "A=M-1\n" + "D=M\n" + "@THAT\n" + "M=D\n";
		result = result + "@2\n" + "D=A\n" + "@R15\n" + "A=M-D\n" + "D=M\n" + "@THIS\n" + "M=D\n";
		result = result + "@3\n" + "D=A\n" + "@R15\n" + "A=M-D\n" + "D=M\n" + "@ARG\n" + "M=D\n";
		result = result + "@4\n" + "D=A\n" + "@R15\n" + "A=M-D\n" + "D=M\n" + "@LCL\n" + "M=D\n";
		result = result + "@R14\n" + "A=M\n" + "0;JMP\n";
		break;
	case Command::C_CALL:
	{
		std::string endLabel;
		endLabel = endLabel + "End$" + cmd.mArg1 + "$" + std::to_string(mFucEnd_Index++);
		int arg_offset = std::stoi(cmd.mArg2) + 5;

		result = result + "@" + endLabel + "\n" + "D=A\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		result = result + "@LCL\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		result = result + "@ARG\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		result = result + "@THIS\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		result = result + "@THAT\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
		result = result + "@" + std::to_string(arg_offset) + "\n" + "D=A\n" + "@SP\n" + "D=M-D\n" + "@ARG\n" + "M=D\n";
		result = result + "@SP\n" + "D=M\n" + "@LCL\n" + "M=D\n";
		result = result + "@" + cmd.mArg1 + "\n" + "0;JMP\n";
		result = result + "(" + endLabel + ")\n";
	}

		break;
	default:
		break;
	}

	mFp << result;
}


void CodeWriter::finish()
{
	mFp.close();
}