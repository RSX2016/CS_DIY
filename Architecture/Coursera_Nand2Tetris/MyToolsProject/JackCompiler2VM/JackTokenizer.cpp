#include "JackTokenizer.h"


//void JackTokenizer::initAM()
//{
//	std::cout << "len == " << JackTokenizer::s_analysizeAM.size() << std::endl;
//}

std::vector<std::vector<int>> JackTokenizer::s_analysisAM = std::vector<std::vector<int>>();
std::vector<Token*> JackTokenizer::s_AMEndState = std::vector<Token*>();

void JackTokenizer::initAM()
{
	if (s_analysisAM.size() != 0) return;
	std::vector<char> symbol = { '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~' };
	std::vector<std::vector<std::string>> keyword;
	keyword.push_back({ "boolean" });
	keyword.push_back({ "class", "constructor", "char" });
	keyword.push_back({ "do" });
	keyword.push_back({ "else" });
	keyword.push_back({ "false", "field", "function" });
	keyword.push_back({ "int", "if" });
	keyword.push_back({ "let" });
	keyword.push_back({ "method" });
	keyword.push_back({ "null" });
	keyword.push_back({ "return" });
	keyword.push_back({ "static" });
	keyword.push_back({ "true", "this" });
	keyword.push_back({ "var", "void" });
	keyword.push_back({ "while" });
	Token* tk = NULL;

	s_AMEndState.resize(1000); // 先预定有1000个状态

	std::vector<int> innerVec;
	innerVec.resize(256);
	std::fill_n(innerVec.begin(), 256, -1);

	/*
	先创建
	1个起始状态
	19个symbol状态
	1个 数字待定
	1个 数字确定
	1个 字符串常量待定
	1个 字符串常量确定
	1个 标识符待定
	1个 标识符确定
	*/
	for (int i = 0; i < 26; ++i) {
		s_analysisAM.push_back(innerVec);
	}

	// 创建19个symbol的最终状态
	for (int i = 0; i < symbol.size(); ++i) {
		tk = new Token();
		tk->mTokenType = Token::eTokenType::eTT_SYMBOL;
		tk->mSymbol = symbol[i];
		s_AMEndState[i + 1] = tk;
	}
	// 创建数字常量确定 的最终状态
	tk = new Token();
	tk->mTokenType = Token::eTokenType::eTT_INT_CONST;
	s_AMEndState[21] = tk;
	// 创建字符串常量确定 的最终状态
	tk = new Token();
	tk->mTokenType = Token::eTokenType::eTT_STRING_CONST;
	s_AMEndState[23] = tk;
	// 创建标识符确定的 最终状态
	tk = new Token();
	tk->mTokenType = Token::eTokenType::eTT_ID;
	s_AMEndState[25] = tk;

	// 起始状态的转换设置
	for (int i = 0; i < symbol.size(); ++i) { s_analysisAM[0][symbol[i]] = i + 1; }
	for (int i = '0'; i <= '9'; ++i) { s_analysisAM[0][i] = 20; }
	for (int i = 'a'; i <= 'z'; ++i) { s_analysisAM[0][i] = 24; }
	for (int i = 'A'; i <= 'Z'; ++i) { s_analysisAM[0][i] = 24; }
	s_analysisAM[0]['"'] = 22;
	s_analysisAM[0]['_'] = 24;
	s_analysisAM[0]['\n'] = 0;
	s_analysisAM[0]['\t'] = 0;

	// 数字待定状态的转换设置
	for (int i = 0; i < 256; ++i) { s_analysisAM[20][i] = 21; }
	for (int i = '0'; i <= '9'; ++i) { s_analysisAM[20][i] = 20; }
	// 字符串常量待定的 转换设置
	for (int i = 0; i < 256; ++i) { s_analysisAM[22][i] = 22; }
	s_analysisAM[22]['"'] = 23;
	// 标识符待定状态的 转换设置
	for (int i = 0; i < 256; ++i) { s_analysisAM[24][i] = 25; }
	for (int i = '0'; i <= '9'; ++i) { s_analysisAM[24][i] = 24; }
	for (int i = 'a'; i <= 'z'; ++i) { s_analysisAM[24][i] = 24; }
	for (int i = 'A'; i <= 'Z'; ++i) { s_analysisAM[24][i] = 24; }
	s_analysisAM[24]['_'] = 24;

	// 一个关键字状态的 跳转模板
	std::vector<int> keywordInnerVecTmp = innerVec;
	for (int i = 0; i < 256; ++i) { keywordInnerVecTmp[i] = 25; }
	// 遇到数字 直接跳转到 标识符待定状态
	for (int i = '0'; i <= '9'; ++i) { keywordInnerVecTmp[i] = 24; }
	// 遇到字母或下划线 默认先跳到 标识符待定状态
	for (int i = 'a'; i <= 'z'; ++i) { keywordInnerVecTmp[i] = 24; }
	for (int i = 'A'; i <= 'Z'; ++i) { keywordInnerVecTmp[i] = 24; }
	keywordInnerVecTmp['_'] = 24;


	for (int i = 0; i < keyword.size(); i++)
	{
		// 进入 相同首字母的 关键字列表
		int firstIn = 1;
		// 记录一个新的首字母对应的状态号
		int stateIndex = s_analysisAM.size();
		for (int j = 0; j < keyword[i].size(); ++j) {
			// 处理单个关键字
			for (int z = 0; z < keyword[i][j].length(); ++z) {
				// 处理单个关键字的每个字符
				if (firstIn == 1) {
					// 是一个新的首字母
					firstIn = 0;
					s_analysisAM.push_back(keywordInnerVecTmp);
					// 更新初始状态 对应此首字母的 状态转换
					s_analysisAM[0][keyword[i][0][0]] = stateIndex;
				}
				else if (z == 0) {
					// 第一个首字母已经创建了 相应的状态号 为 stateIndex
					continue;
				}
				else if (z == 1) {
					s_analysisAM.push_back(keywordInnerVecTmp);
					// 是第二个字符 需要更新该类首字母状态 对应当前字母的 状态转换
					s_analysisAM[stateIndex][keyword[i][j][z]] = s_analysisAM.size() - 1;
				}
				else {
					// 第三个或后面的字符
					s_analysisAM.push_back(keywordInnerVecTmp);
					// 更新前一个状态 对应当前字母的 状态转换
					s_analysisAM[s_analysisAM.size() - 2][keyword[i][j][z]] = s_analysisAM.size() - 1;
				}

				if (z == keyword[i][j].length() - 1) {
					// 是当前关键字的最后一个字母
					// 创建一个对应的 确定状态
					s_analysisAM.push_back(keywordInnerVecTmp);
					// 更新前一个待定状态 对应的 终结符 的状态转换
					for (int i = 0; i < symbol.size(); ++i) {
						s_analysisAM[s_analysisAM.size() - 2][symbol[i]] = s_analysisAM.size() - 1;
					}
					s_analysisAM[s_analysisAM.size() - 2][' '] = s_analysisAM.size() - 1;
					s_analysisAM[s_analysisAM.size() - 2]['\n'] = s_analysisAM.size() - 1;
					s_analysisAM[s_analysisAM.size() - 2]['\t'] = s_analysisAM.size() - 1;
					s_analysisAM[s_analysisAM.size() - 2]['"'] = s_analysisAM.size() - 1;
					// 并在 终结状态列表中创建相对应的 Token
					tk = new Token();
					tk->mTokenType = Token::eTokenType::eTT_KEYWORD;
					tk->mKeywordType = Token::s_eKeywordMap[keyword[i][j]];
					s_AMEndState[s_analysisAM.size() - 1] = tk;
				}
			}
		}
	}



	//std::vector<char> allChar = { '~', '!', '@', '#', '$', '%' };
	//// 打印 自动状态机表
	//for (int i = '1'; i <= '~'; i++) {
	//	printf("%c  ", i);
	//}
	//printf("\n");
}


JackTokenizer::JackTokenizer(std::string filename)
	:mLine(""), mLineIndex(0)
{
	JackTokenizer::initAM();

	mFp.open(filename, std::ios_base::in);
	if (!mFp.is_open()) {
		std::cout << "file [ " << filename << "] open failed!! \n";
		return;
	}
}

bool JackTokenizer::hasMoreTokens()
{
	if (!jumpSpace()) { return false; }
	return true;
}

bool JackTokenizer::advance(Token& token)
{
	if (!jumpSpace()) { return false; }
	std::string tmp;
	int curState = 0;
	int isEndState = 0;
	while (isEndState == 0) {
		std::cout << "          char = " << mLine[mLineIndex] << "   state from [" << curState << "]";
		curState = JackTokenizer::s_analysisAM[curState][mLine[mLineIndex]];
		std::cout << "to [" << curState << "]";
		if (JackTokenizer::s_AMEndState[curState] != NULL) {
			isEndState = 1;
		}
		else {
			tmp.append(mLine.substr(mLineIndex, 1));
			mLineIndex++;
		}
		std::cout << "  tmp = " << tmp << std::endl;
	}

	token.mTokenType = JackTokenizer::s_AMEndState[curState]->mTokenType;
	switch (token.mTokenType)
	{
	case Token::eTokenType::eTT_ID:
		token.mID = tmp;
		break;
	case Token::eTokenType::eTT_INT_CONST:
		token.mIntVal = std::stoi(tmp);
		break;
	case Token::eTokenType::eTT_STRING_CONST:
		token.mStringVal = tmp.substr(1);
		mLineIndex++;
		break;
	case Token::eTokenType::eTT_KEYWORD:
		token.mKeywordType = JackTokenizer::s_AMEndState[curState]->mKeywordType;
		break;
	case Token::eTokenType::eTT_SYMBOL:
		token.mSymbol = JackTokenizer::s_AMEndState[curState]->mSymbol;
		mLineIndex++;
		break;
	default:
		break;
	}
	return true;
}

bool JackTokenizer::jumpSpace()
{
	if (mLineIndex == mLine.length()) { getNewLine(); }
	if (mLine.length() == 0) { return false; }
	while (mLine[mLineIndex] == ' ' || mLine[mLineIndex] == '\t' || mLine[mLineIndex] == '\n') {
		mLineIndex++;
		if (mLineIndex == mLine.length()) {
			getNewLine();
			if (mLine.length() == 0) { return false; }
		}
	}
	return true;
}

void JackTokenizer::getNewLine()
{
	mLine.clear();
	mLineIndex = 0;

	while (mLine.length() == 0) {
		if (mFp.eof()) { return; }

		std::string line;
		char buffer[2048];

		mFp.getline(buffer, 2048);
		line = buffer;
		// std::cout << line << " num = " << line.length() << std::endl;

		// 过滤 /**部分
		int s = line.find("/*");
		int e = -1;
		if (s != -1) {
			if (s != 0) {
				mLine.append(line.substr(0, s));
			}

			e = line.find("*/");
			while (e == -1) {
				mFp.getline(buffer, 2048);
				line = buffer;
				e = line.find("*/");
			}

			mLine.append(line.substr(e + 2, line.length()));

			s = mLine.find("//");
			if (s != -1) {
				mLine = mLine.substr(0, s);
			}
			continue;
		}

		s = line.find("//");
		if (s != -1) {
			mLine.append(line.substr(0, s));
		}
		else {
			mLine = line;
		}
	}

	std::cout << "new line = \"" << mLine << "\"" << std::endl;
}