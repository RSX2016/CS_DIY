#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable()
	:mStaticIndex(0), mFieldIndex(0), mArgIndex(0), mVarIndex(0), mLabelIndex(0)
{

}

void SymbolTable::clear()
{
	mStaticIndex = 0;
	mFieldIndex = 0;
	mArgIndex = 0;
	mVarIndex = 0;
	mSubroutineTable.clear();
	mClassTable.clear();
}

void SymbolTable::startSubroutine()
{
	mArgIndex = 0;
	mVarIndex = 0;
	mSubroutineTable.clear();
}

void SymbolTable::define(std::string name, std::string type, eKind kind)
{
	std::cout << "SymbolTable::define IN name = " << name << std::endl;

	switch (kind) {
	case eK_FIELD:
		mClassTable[name] = symbolInfo(kind, type, mFieldIndex++);
		break;
	case eK_STATIC:
		mClassTable[name] = symbolInfo(kind, type, mStaticIndex++);
		break;
	case eK_ARG:
		mSubroutineTable[name] = symbolInfo(kind, type, mArgIndex++);
		break;
	case eK_VAR:
		mSubroutineTable[name] = symbolInfo(kind, type, mVarIndex++);
		break;
	}
}

int SymbolTable::varCount(eKind kind)
{
	typedef std::map<std::string, symbolInfo>::iterator iterator;

	iterator s, e;
	int result = 0;

	switch (kind) {
	case eK_FIELD:
	case eK_STATIC:
		s = mClassTable.begin();
		e = mClassTable.end();
		break;
	case eK_ARG:
	case eK_VAR:
		s = mSubroutineTable.begin();
		e = mSubroutineTable.end();
		break;
	}

	for (iterator i = s; i != e; ++i) {
		if (i->second.mKind == kind) {
			result++;
		}
	}

	return result;
}

SymbolTable::eKind SymbolTable::kindOf(std::string name)
{
	if (mSubroutineTable.count(name) == 1) {
		return mSubroutineTable[name].mKind;
	}
	else if (mClassTable.count(name) == 1) {
		return mClassTable[name].mKind;
	}

	return eK_NONE;
}

std::string SymbolTable::typeOf(std::string name)
{
	if (mSubroutineTable.count(name) == 1) {
		return mSubroutineTable[name].mType;
	}
	else if (mClassTable.count(name) == 1) {
		return mClassTable[name].mType;
	}

	return "unKnown";
}

int SymbolTable::indexOf(std::string name)
{
	if (mSubroutineTable.count(name) == 1) {
		return mSubroutineTable[name].mIndex;
	}
	else if (mClassTable.count(name) == 1) {
		return mClassTable[name].mIndex;
	}

	return -1;
}