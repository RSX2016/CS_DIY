#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Token.h"

using std::fstream;
class TokenXMLWriter
{
public:
	TokenXMLWriter(std::string fileName)
	{
		mFp.open(fileName, std::ios_base::out | std::ios_base::trunc);
		if (!mFp.is_open()) {
			std::cout << "file [ " << fileName << "] open failed!! \n";
			return;
		}

		mFp << "<tokens>\n";
	}

	void codeWrite(Token& tk)
	{
        switch (tk.mTokenType)
        {
        case Token::eTokenType::eTT_KEYWORD:
            mFp << "<keyword> " << Token::s_eKeywordMapReverse[tk.mKeywordType] << " </keyword>\n";
            break;
        case Token::eTokenType::eTT_ID:
            mFp << "<identifier> " << tk.mID << " </identifier>\n";
            break;
        case Token::eTokenType::eTT_INT_CONST:
            mFp << "<integerConstant> " << tk.mIntVal << " </integerConstant>\n";
            break;
        case Token::eTokenType::eTT_STRING_CONST:
            mFp << "<stringConstant> " << tk.mStringVal << " </stringConstant>\n";
            break;
        case Token::eTokenType::eTT_SYMBOL:
            mFp << "<symbol> " << tk.mSymbol << " </symbol>\n";
            break;
        default:
            break;
        }
	}

    void end() 
    {
        mFp << "</tokens>\n";
        mFp.close();
    }


private:
	fstream mFp;
};