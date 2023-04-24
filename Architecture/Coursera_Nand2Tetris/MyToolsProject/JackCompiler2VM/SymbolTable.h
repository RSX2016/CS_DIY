#pragma once
#include <string>
#include <map>

class SymbolTable
{
public:
	static enum eKind
	{
		eK_NONE,
		eK_STATIC,
		eK_FIELD,
		eK_ARG,
		eK_VAR
	};

	struct symbolInfo
	{
		symbolInfo() {}

		symbolInfo(eKind k, std::string t, int i)
			:mKind(k), mIndex(i), mType(t)
		{

		}

		eKind mKind;
		int	  mIndex;
		std::string mType;
	};

public:
	SymbolTable();
	void clear();
	void startSubroutine();
	void define(std::string name, std::string type, eKind kind);
	int varCount(eKind kind);
	eKind kindOf(std::string name);
	std::string typeOf(std::string name);
	int indexOf(std::string name);

	int getNewLableIndex() { return mLabelIndex++; }
	
private:
	int mStaticIndex, mFieldIndex, mArgIndex, mVarIndex;
	std::map<std::string, symbolInfo> mClassTable;
	std::map<std::string, symbolInfo> mSubroutineTable;

	int mLabelIndex;
};