#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Token.h"
#include "JackTokenizer.h"

using std::fstream;
class TokenXMLWriter
{
public:
	TokenXMLWriter() { }

    void start(std::string fileName, JackTokenizer &jackTokenizer)
    {
        mFp.open(fileName, std::ios_base::out | std::ios_base::trunc);
        if (!mFp.is_open()) {
            std::cout << "file [ " << fileName << "] open failed!! \n";
            return;
        }

        mFp << "<tokens>\n";

        Token tk;
        while (jackTokenizer.hasMoreTokens()) {
            if (jackTokenizer.advance(tk)) {
                codeWrite(tk);

                switch (tk.mTokenType)
                {
                case Token::eTokenType::eTT_KEYWORD:
                    std::cout << "TOKEN { eTT_KEYWORD val = " << tk.mKeywordType << std::endl;
                    break;
                case Token::eTokenType::eTT_ID:
                    std::cout << "TOKEN { eTT_ID val = " << tk.mID << std::endl;
                    break;
                case Token::eTokenType::eTT_INT_CONST:
                    std::cout << "TOKEN { eTT_INT_CONST val = " << tk.mIntVal << std::endl;
                    break;
                case Token::eTokenType::eTT_STRING_CONST:
                    std::cout << "TOKEN { eTT_STRING_CONST val = " << tk.mStringVal << std::endl;
                    break;
                case Token::eTokenType::eTT_SYMBOL:
                    std::cout << "TOKEN { eTT_SYMBOL val = " << tk.mSymbol << std::endl;
                    break;
                default:
                    break;
                }
            }
        }

        mFp << "</tokens>\n";
        mFp.close();
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

private:
	fstream mFp;
};