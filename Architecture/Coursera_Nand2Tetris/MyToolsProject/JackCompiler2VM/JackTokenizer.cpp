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

	s_AMEndState.resize(1000); // ��Ԥ����1000��״̬

	std::vector<int> innerVec;
	innerVec.resize(256);
	std::fill_n(innerVec.begin(), 256, -1);

	/*
	�ȴ���
	1����ʼ״̬
	19��symbol״̬
	1�� ���ִ���
	1�� ����ȷ��
	1�� �ַ�����������
	1�� �ַ�������ȷ��
	1�� ��ʶ������
	1�� ��ʶ��ȷ��
	*/
	for (int i = 0; i < 26; ++i) {
		s_analysisAM.push_back(innerVec);
	}

	// ����19��symbol������״̬
	for (int i = 0; i < symbol.size(); ++i) {
		tk = new Token();
		tk->mTokenType = Token::eTokenType::eTT_SYMBOL;
		tk->mSymbol = symbol[i];
		s_AMEndState[i + 1] = tk;
	}
	// �������ֳ���ȷ�� ������״̬
	tk = new Token();
	tk->mTokenType = Token::eTokenType::eTT_INT_CONST;
	s_AMEndState[21] = tk;
	// �����ַ�������ȷ�� ������״̬
	tk = new Token();
	tk->mTokenType = Token::eTokenType::eTT_STRING_CONST;
	s_AMEndState[23] = tk;
	// ������ʶ��ȷ���� ����״̬
	tk = new Token();
	tk->mTokenType = Token::eTokenType::eTT_ID;
	s_AMEndState[25] = tk;

	// ��ʼ״̬��ת������
	for (int i = 0; i < symbol.size(); ++i) { s_analysisAM[0][symbol[i]] = i + 1; }
	for (int i = '0'; i <= '9'; ++i) { s_analysisAM[0][i] = 20; }
	for (int i = 'a'; i <= 'z'; ++i) { s_analysisAM[0][i] = 24; }
	for (int i = 'A'; i <= 'Z'; ++i) { s_analysisAM[0][i] = 24; }
	s_analysisAM[0]['"'] = 22;
	s_analysisAM[0]['_'] = 24;
	s_analysisAM[0]['\n'] = 0;
	s_analysisAM[0]['\t'] = 0;

	// ���ִ���״̬��ת������
	for (int i = 0; i < 256; ++i) { s_analysisAM[20][i] = 21; }
	for (int i = '0'; i <= '9'; ++i) { s_analysisAM[20][i] = 20; }
	// �ַ������������� ת������
	for (int i = 0; i < 256; ++i) { s_analysisAM[22][i] = 22; }
	s_analysisAM[22]['"'] = 23;
	// ��ʶ������״̬�� ת������
	for (int i = 0; i < 256; ++i) { s_analysisAM[24][i] = 25; }
	for (int i = '0'; i <= '9'; ++i) { s_analysisAM[24][i] = 24; }
	for (int i = 'a'; i <= 'z'; ++i) { s_analysisAM[24][i] = 24; }
	for (int i = 'A'; i <= 'Z'; ++i) { s_analysisAM[24][i] = 24; }
	s_analysisAM[24]['_'] = 24;

	// һ���ؼ���״̬�� ��תģ��
	std::vector<int> keywordInnerVecTmp = innerVec;
	for (int i = 0; i < 256; ++i) { keywordInnerVecTmp[i] = 25; }
	// �������� ֱ����ת�� ��ʶ������״̬
	for (int i = '0'; i <= '9'; ++i) { keywordInnerVecTmp[i] = 24; }
	// ������ĸ���»��� Ĭ�������� ��ʶ������״̬
	for (int i = 'a'; i <= 'z'; ++i) { keywordInnerVecTmp[i] = 24; }
	for (int i = 'A'; i <= 'Z'; ++i) { keywordInnerVecTmp[i] = 24; }
	keywordInnerVecTmp['_'] = 24;


	for (int i = 0; i < keyword.size(); i++)
	{
		// ���� ��ͬ����ĸ�� �ؼ����б�
		int firstIn = 1;
		// ��¼һ���µ�����ĸ��Ӧ��״̬��
		int stateIndex = s_analysisAM.size();
		for (int j = 0; j < keyword[i].size(); ++j) {
			// ��������ؼ���
			for (int z = 0; z < keyword[i][j].length(); ++z) {
				// ��������ؼ��ֵ�ÿ���ַ�
				if (firstIn == 1) {
					// ��һ���µ�����ĸ
					firstIn = 0;
					s_analysisAM.push_back(keywordInnerVecTmp);
					// ���³�ʼ״̬ ��Ӧ������ĸ�� ״̬ת��
					s_analysisAM[0][keyword[i][0][0]] = stateIndex;
				}
				else if (z == 0) {
					// ��һ������ĸ�Ѿ������� ��Ӧ��״̬�� Ϊ stateIndex
					continue;
				}
				else if (z == 1) {
					s_analysisAM.push_back(keywordInnerVecTmp);
					// �ǵڶ����ַ� ��Ҫ���¸�������ĸ״̬ ��Ӧ��ǰ��ĸ�� ״̬ת��
					s_analysisAM[stateIndex][keyword[i][j][z]] = s_analysisAM.size() - 1;
				}
				else {
					// �������������ַ�
					s_analysisAM.push_back(keywordInnerVecTmp);
					// ����ǰһ��״̬ ��Ӧ��ǰ��ĸ�� ״̬ת��
					s_analysisAM[s_analysisAM.size() - 2][keyword[i][j][z]] = s_analysisAM.size() - 1;
				}

				if (z == keyword[i][j].length() - 1) {
					// �ǵ�ǰ�ؼ��ֵ����һ����ĸ
					// ����һ����Ӧ�� ȷ��״̬
					s_analysisAM.push_back(keywordInnerVecTmp);
					// ����ǰһ������״̬ ��Ӧ�� �ս�� ��״̬ת��
					for (int i = 0; i < symbol.size(); ++i) {
						s_analysisAM[s_analysisAM.size() - 2][symbol[i]] = s_analysisAM.size() - 1;
					}
					s_analysisAM[s_analysisAM.size() - 2][' '] = s_analysisAM.size() - 1;
					s_analysisAM[s_analysisAM.size() - 2]['\n'] = s_analysisAM.size() - 1;
					s_analysisAM[s_analysisAM.size() - 2]['\t'] = s_analysisAM.size() - 1;
					s_analysisAM[s_analysisAM.size() - 2]['"'] = s_analysisAM.size() - 1;
					// ���� �ս�״̬�б��д������Ӧ�� Token
					tk = new Token();
					tk->mTokenType = Token::eTokenType::eTT_KEYWORD;
					tk->mKeywordType = Token::s_eKeywordMap[keyword[i][j]];
					s_AMEndState[s_analysisAM.size() - 1] = tk;
				}
			}
		}
	}



	//std::vector<char> allChar = { '~', '!', '@', '#', '$', '%' };
	//// ��ӡ �Զ�״̬����
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

bool JackTokenizer::lookAhead(Token& token)
{
	int lineIndex_bk = mLineIndex;
	bool r = advance(token);
	mLineIndex = lineIndex_bk;

	return r;
}

bool JackTokenizer::advance(Token& token)
{
	if (!jumpSpace()) { return false; }
	std::string tmp;
	int curState = 0;
	int isEndState = 0;
	while (isEndState == 0) {
		// std::cout << "          char = " << mLine[mLineIndex] << "   state from [" << curState << "]";
		curState = JackTokenizer::s_analysisAM[curState][mLine[mLineIndex]];
		// std::cout << "to [" << curState << "]";
		if (JackTokenizer::s_AMEndState[curState] != NULL) {
			isEndState = 1;
		}
		else {
			tmp.append(mLine.substr(mLineIndex, 1));
			mLineIndex++;
		}
		// std::cout << "  tmp = " << tmp << std::endl;
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

		// ���� /**����
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

	// std::cout << "new line = \"" << mLine << "\"" << std::endl;
}