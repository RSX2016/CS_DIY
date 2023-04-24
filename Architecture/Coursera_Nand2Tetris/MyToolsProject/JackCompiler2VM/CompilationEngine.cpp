#include "CompilationEngine.h"

CompliationEngine::CompliationEngine()
	:mJackTokenizer(0)
{

}

void CompliationEngine::start(std::string fileName, JackTokenizer& jackTokenizer)
{
    mVMFp.open(fileName + ".vm", std::ios_base::out | std::ios_base::trunc);
    if (!mVMFp.is_open()) {
        std::cout << "file [ " << fileName << "] open failed!! \n";
        return;
    }

    mTestXmlFp.open(fileName + "_CompileOut.xml", std::ios_base::out | std::ios_base::trunc);
    if (!mTestXmlFp.is_open()) {
        std::cout << "file [ " << fileName << "] open failed!! \n";
        return;
    }


    mJackTokenizer = &jackTokenizer;
    mJackTokenizer->advance(mTk);
    mSymbolTable.clear();
    compile_class();
}


bool CompliationEngine::compile_class()
{
    // class ->	'Class' className '{' classVarDec* subroutineDec* '}'
    compilationXmlWrite("<class>\n");
    if (mTk.checkKeyWord(Token::eKeyword::eK_CLASS)) {
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
        if (!compile_className(&mClassName)) { return false; }

        if (mTk.checkSymbol('{')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        } else {
            compile_err("class", "{");
            return false;
        }

        while (compile_classVarDec()) {}

        while (compile_subroutineDec()) {}

        if (mTk.checkSymbol('}')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("class", "}");
            return false;
        }
    }
    else {
        compile_err("class", "Class");
        return false;
    }
    compilationXmlWrite("</class>\n");

    return true;
}
bool CompliationEngine::compile_classVarDec()
{
    // classVarDec	->	('static' | 'field') type varName (',' varName)* ';'
    
    SymbolTable::eKind mSymbolKind;
    std::string mSymbolType;
    std::string mSymbolName;

    if (mTk.checkKeyWord(Token::eKeyword::eK_STATIC) || mTk.checkKeyWord(Token::eKeyword::eK_FIELD)) {
        
        mSymbolKind = mTk.checkKeyWord(Token::eKeyword::eK_STATIC) ? SymbolTable::eKind::eK_STATIC : SymbolTable::eKind::eK_FIELD;

        compilationXmlWrite("<classVarDec>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (!compile_type(&mSymbolType)) { return false; }

        if (!compile_varName(&mSymbolName)) { return false; }

        mSymbolTable.define(mSymbolName, mSymbolType, mSymbolKind);

        while (mTk.checkSymbol(',')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
            if (!compile_varName(&mSymbolName)) { return false; }

            mSymbolTable.define(mSymbolName, mSymbolType, mSymbolKind);
        }

        if (mTk.checkSymbol(';')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("classVarDec", ";");
        }
        compilationXmlWrite("</classVarDec>\n");
    }
    else {
        compile_err("classVarDec", "static or field");
        return false;
    }

    
    return true;
}
bool CompliationEngine::compile_type(std::string* id)
{
    if (mTk.checkKeyWord(Token::eKeyword::eK_INT)
        || mTk.checkKeyWord(Token::eKeyword::eK_CHAR)
        || mTk.checkKeyWord(Token::eKeyword::eK_BOOLEAN)) {
        if (id != NULL) { *id = Token::s_eKeywordMapReverse[mTk.mKeywordType]; }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else if (compile_className(id)) {

    }
    else {
        compile_err("type", "int or char or boolean or className");
        return false;
    }

    return true;
}
bool CompliationEngine::compile_subroutineDec()
{
    // subroutineDec	->	('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
    std::string subroutineName;
    int funtionType = 0; // 0->function 1->constructor 2->method
    int paramCounts = 0;
    if (mTk.checkKeyWord(Token::eKeyword::eK_CONSTRUCTOR)
        || mTk.checkKeyWord(Token::eKeyword::eK_FUNCTION)
        || mTk.checkKeyWord(Token::eKeyword::eK_METHOD)) {
        mSymbolTable.startSubroutine();
        if (mTk.checkKeyWord(Token::eKeyword::eK_CONSTRUCTOR)) { funtionType = 1; }
        if (mTk.checkKeyWord(Token::eKeyword::eK_METHOD)) { funtionType = 2; }
        compilationXmlWrite("<subroutineDec>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (mTk.checkKeyWord(Token::eKeyword::eK_VOID)) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else if (compile_type()) {

        }
        else {
            compile_err("subroutineDec", "void | type");
            return false;
        }

        if (!compile_subroutineName(&subroutineName)) { return false; }
        mVMFp << "function " << mClassName << "." << subroutineName << " ";

        if (mTk.checkSymbol('(')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("subroutineDec", "(");
            return false;
        }

        if (!compile_parameterList(&paramCounts)) { return false; };

        if (mTk.checkSymbol(')')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("subroutineDec", ")");
            return false;
        }

        if (!compile_subroutineBody(funtionType)) { return false; }
        compilationXmlWrite("</subroutineDec>\n");
    }
    else {
        compile_err("subroutineDec", "constructor | funtion | method");
        return false;
    }
    
    return true;
}

bool CompliationEngine::compile_parameterList(int *paramCounts)
{
    // parameterList ->	(type varName (',' type varName)* )?
    SymbolTable::eKind mSymbolKind;
    std::string mSymbolType;
    std::string mSymbolName;
    if (paramCounts != 0) { *paramCounts = 0;  }
    compilationXmlWrite("<parameterList>\n");
    if (compile_type(&mSymbolType)) {
        if (!compile_varName(&mSymbolName)) { return false; }

        mSymbolTable.define(mSymbolName, mSymbolType, SymbolTable::eKind::eK_ARG);
        if (paramCounts != 0) { (*paramCounts)++; }
        while (mTk.checkSymbol(',')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);

            if (!compile_type(&mSymbolType)) { return false; }

            if (!compile_varName(&mSymbolName)) { return false; }
            mSymbolTable.define(mSymbolName, mSymbolType, SymbolTable::eKind::eK_ARG);
            if (paramCounts != 0) { (*paramCounts)++; }
        }
    }
    compilationXmlWrite("</parameterList>\n");
    return true;
}

bool CompliationEngine::compile_subroutineBody(int funtionType)
{
    // subroutineBody	->	'{' varDec* statements '}'
    compilationXmlWrite("<subroutineBody>\n");
    int localNums = 0;

    if (mTk.checkSymbol('{')) {
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        while (mTk.checkKeyWord(Token::eKeyword::eK_VAR)) {
            if (!compile_varDec(&localNums)) { return false; }
        }
        mVMFp << localNums << "\n";

        // 0->function 1->constructor 2->method
        if (funtionType == 1) {
            mVMFp << "push constant " << mSymbolTable.varCount(SymbolTable::eKind::eK_FIELD) << "\n";
            mVMFp << "call Memory.alloc 1\n";
            mVMFp << "pop pointer 0\n";
        }
        else if (funtionType == 2) {
            mVMFp << "push argument 0\n";
            mVMFp << "pop pointer 0\n";
        }


        if (!compile_statements()) { return false; }

        if (mTk.checkSymbol('}')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("subroutineBody", "}");
            return false;
        }
    }
    else {
        compile_err("subroutineBody", "{");
        return false;
    }

    compilationXmlWrite("</subroutineBody>\n");
    return true;
}

bool CompliationEngine::compile_varDec(int* varCount)
{
    // varDec -> 'var' type varName (',' varName)* ';'
    SymbolTable::eKind mSymbolKind;
    std::string mSymbolType;
    std::string mSymbolName;
    compilationXmlWrite("<varDec>\n");
    if (mTk.checkKeyWord(Token::eKeyword::eK_VAR)) {
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (!compile_type(&mSymbolType)) { return false; }

        if (!compile_varName(&mSymbolName)) { return false; }

        mSymbolTable.define(mSymbolName, mSymbolType, SymbolTable::eKind::eK_VAR);
        if (varCount != 0) { (*varCount)++; }


        while (mTk.checkSymbol(',')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);

            if (!compile_varName(&mSymbolName)) { return false; }
            mSymbolTable.define(mSymbolName, mSymbolType, SymbolTable::eKind::eK_VAR);
            if (varCount != 0) { (*varCount)++; }
        }

        if (mTk.checkSymbol(';')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("varDec", ";");
            return false;
        }
    }
    else {
        compile_err("varDec", "var");
        return false;
    }
    compilationXmlWrite("</varDec>\n");
    return true;
}
bool CompliationEngine::compile_className(std::string* id)
{
    // className ->	Id
    if (mTk.mTokenType == Token::eTokenType::eTT_ID) {
        if (id != NULL) { *id = mTk.mID; }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else {
        compile_err("className", "id");
        return false;
    }

    return true;
}
bool CompliationEngine::compile_subroutineName(std::string* id)
{
    // subroutineName	->	Id
    if (mTk.mTokenType == Token::eTokenType::eTT_ID) {
        if (id != NULL) { *id = mTk.mID; }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else {
        compile_err("subroutineName", "id");
        return false;
    }

    return true;
}
bool CompliationEngine::compile_varName(std::string* id)
{
    // varName->Id
    if (mTk.mTokenType == Token::eTokenType::eTT_ID) {
        if (id != NULL) { *id = mTk.mID; }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else {
        compile_err("varName", "id");
        return false;
    }

    return true;
}
bool CompliationEngine::compile_statements()
{
    // statements	->	statement*
    compilationXmlWrite("<statements>\n");
    while (compile_statement()) {}
    compilationXmlWrite("</statements>\n");
    return true;
}
bool CompliationEngine::compile_statement()
{
    // statement->letStatement | ifStatement | whileStatement | doStatement | returnStatement

    if (compile_letStatement()) {

    }
    else if (compile_ifStatement()) {

    }
    else if (compile_whileStatement()) {

    }
    else if (compile_doStatement()) {

    }
    else if (compile_returnStatement()) {

    }
    else {
        compile_err("statement", "letStatement | ifStatement | whileStatement | doStatement | returnStatement");
        return false;
    }

    return true;

}
bool CompliationEngine::compile_letStatement()
{
    // letStatement	->	'let' varName ('[' expression ']')? '=' expression ';'
    bool isArray = false;
    std::string varName;
    if (mTk.checkKeyWord(Token::eKeyword::eK_LET)) {
        compilationXmlWrite("<letStatement>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (!compile_varName(&varName)) { return false; }

        if (mTk.checkSymbol('[')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);



            if (!compile_expression()) { return false; }

            switch (mSymbolTable.kindOf(varName))
            {
            case SymbolTable::eKind::eK_ARG:
                mVMFp << "push argument " << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_FIELD:
                mVMFp << "push this " << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_STATIC:
                mVMFp << "push " << mClassName << "." << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_VAR:
                mVMFp << "push local " << mSymbolTable.indexOf(varName) << "\n";
                break;
            default:
                break;
            }

            mVMFp << "add" << "\n";

            if (mTk.checkSymbol(']')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);
            }
            else {
                compile_err("letStatement", "]");
                return false;
            }

            isArray = true;
        }

        if (mTk.checkSymbol('=')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("letStatement", "=");
            return false;
        }

        if (!compile_expression()) { return false; }

        if (mTk.checkSymbol(';')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("letStatement", ";");
            return false;
        }

        if (isArray) {
            mVMFp << "pop temp 0\n";
            mVMFp << "pop pointer 1\n";
            mVMFp << "push temp 0\n";
            mVMFp << "pop that 0\n";
        }
        else {
            switch (mSymbolTable.kindOf(varName))
            {
            case SymbolTable::eKind::eK_ARG:
                mVMFp << "pop argument " << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_FIELD:
                mVMFp << "pop this " << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_STATIC:
                mVMFp << "pop " << mClassName << "." << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_VAR:
                mVMFp << "pop local " << mSymbolTable.indexOf(varName) << "\n";
                break;
            default:
                break;
            }
        }

        


        compilationXmlWrite("</letStatement>\n");
    }
    else {
        return false;
    }
    

    return true;
}
bool CompliationEngine::compile_ifStatement()
{
    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
    int labelIndex;
    if (mTk.checkKeyWord(Token::eKeyword::eK_IF)) {
        labelIndex = mSymbolTable.getNewLableIndex();
        compilationXmlWrite("<ifStatement>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (mTk.checkSymbol('(')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("ifStatement", "(");
            return false;
        }

        if (!compile_expression()) { return false; }

        if (mTk.checkSymbol(')')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("ifStatement", ")");
            return false;
        }

        mVMFp << "not\n";
        mVMFp << "if-goto if_else_" << labelIndex << "\n";


        if (mTk.checkSymbol('{')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("ifStatement", "{");
            return false;
        }

        if (!compile_statements()) { return false; }

        if (mTk.checkSymbol('}')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("ifStatement", "}");
            return false;
        }

        mVMFp << "goto if_end_" << labelIndex << "\n";
        mVMFp << "label if_else_" << labelIndex << "\n";
        if (mTk.checkKeyWord(Token::eKeyword::eK_ELSE)) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);


            if (mTk.checkSymbol('{')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);
            }
            else {
                compile_err("ifStatement", "{");
                return false;
            }

            if (!compile_statements()) { return false; }

            if (mTk.checkSymbol('}')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);
            }
            else {
                compile_err("ifStatement", "}");
                return false;
            }
        }


        mVMFp << "label if_end_" << labelIndex << "\n";
        compilationXmlWrite("</ifStatement>\n");
    }
    else {
        return false;
    }
    
    return true;

}
bool CompliationEngine::compile_whileStatement()
{
    // whileStatement ->	'while' '(' expression ')' '{' statements '}'
    int labelIndex;
    if (mTk.checkKeyWord(Token::eKeyword::eK_WHILE)) {
        compilationXmlWrite("<whileStatement>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        labelIndex = mSymbolTable.getNewLableIndex();

        mVMFp << "label while_start_" << labelIndex << "\n";

        if (mTk.checkSymbol('(')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("whileStatement", "(");
            return false;
        }

        if (!compile_expression()) { return false; }

        if (mTk.checkSymbol(')')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("whileStatement", ")");
            return false;
        }

        mVMFp << "not\n";
        mVMFp << "if-goto while_end_" << labelIndex << "\n";
        

        if (mTk.checkSymbol('{')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("whileStatement", "{");
            return false;
        }

        if (!compile_statements()) { return false; }

        if (mTk.checkSymbol('}')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("whileStatement", "}");
            return false;
        }

        mVMFp << "goto while_start_" << labelIndex << "\n";
        mVMFp << "label while_end_" << labelIndex << "\n";
        compilationXmlWrite("</whileStatement>\n");
    }
    else {
        return false;
    }
    return true;

}
bool CompliationEngine::compile_doStatement()
{
    // doStatement	->	'do' subroutineCall ';'
    
    if (mTk.checkKeyWord(Token::eKeyword::eK_DO)) {
        compilationXmlWrite("<doStatement>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (!compile_subroutineCall()) { return false; }

        if (mTk.checkSymbol(';')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("doStatement", ";");
            return false;
        }

        mVMFp << "pop temp 0\n";

        compilationXmlWrite("</doStatement>\n");
    }
    else {
        return false;
    }
    
    return true;
}
bool CompliationEngine::compile_returnStatement()
{
    // returnStatement	->	'return' (expression)? ';'
    if (mTk.checkKeyWord(Token::eKeyword::eK_RETURN)) {
        compilationXmlWrite("<returnStatement>\n");
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (!mTk.checkSymbol(';')) {
            if (!compile_expression()) { return false; }
        }
        else {
            mVMFp << "push constant 0\n";
        }
        

        if (mTk.checkSymbol(';')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("whileStatement", ";");
            return false;
        }

        mVMFp << "return\n";

        compilationXmlWrite("</returnStatement>\n");
    }
    else {
        return false;
    }

    
    return true;
}
bool CompliationEngine::compile_expression()
{
    // expression->term (op term)*
    std::string opVmCode;
    compilationXmlWrite("<expression>\n");
    if (!compile_term()) { return false; }
    while (compile_op(&opVmCode)) {
        if (!compile_term()) { return false; }
        mVMFp << opVmCode;
    }
    compilationXmlWrite("</expression>\n");
    return true;
}
bool CompliationEngine::compile_term()
{
    // term		->	integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
    std::string vmCode;
    compilationXmlWrite("<term>\n");
    if (mTk.mTokenType == Token::eTokenType::eTT_INT_CONST) {
        compilationXmlWrite();
        mVMFp << "push constant " << mTk.mIntVal << "\n";
        mJackTokenizer->advance(mTk);
    } else if (mTk.mTokenType == Token::eTokenType::eTT_STRING_CONST) {
        mVMFp << "push constant " << mTk.mStringVal.length() << "\n";
        mVMFp << "call String.new 1\n";
        for (int i = 0; i < mTk.mStringVal.length(); ++i) {
            mVMFp << "push constant " << (int)mTk.mStringVal[i] << "\n";
            mVMFp << "call String.appendChar 2\n";
        }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else if (compile_KeywordConstant()) {

    }
    else if (mTk.mTokenType == Token::eTokenType::eTT_ID) {
        Token ahead;
        mJackTokenizer->lookAhead(ahead);
        if (mSymbolTable.kindOf(mTk.mID) == SymbolTable::eKind::eK_NONE) {
            if (!compile_subroutineCall()) { return false; }
        }
        else {
            std::string varName;
            if (!compile_varName(&varName)) { return false; }

            switch (mSymbolTable.kindOf(varName))
            {
            case SymbolTable::eKind::eK_ARG:
                mVMFp << "push argument " << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_FIELD:
                mVMFp << "push this " << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_STATIC:
                mVMFp << "push " << mClassName << "." << mSymbolTable.indexOf(varName) << "\n";
                break;
            case SymbolTable::eKind::eK_VAR:
                mVMFp << "push local " << mSymbolTable.indexOf(varName) << "\n";
                break;
            default:
                break;
            }

            if (mTk.checkSymbol('[')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);

                if (!compile_expression()) { return false; }

                if (mTk.checkSymbol(']')) {
                    compilationXmlWrite();
                    mJackTokenizer->advance(mTk);
                }
                else {
                    compile_err("term", "]");
                    return false;
                }

                mVMFp << "add\n";
                mVMFp << "pop pointer 1\n";
                mVMFp << "push that 0\n";
            }
        }
    }
    else if (mTk.checkSymbol('(')) {
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);

        if (!compile_expression()) { return false; }

        if (mTk.checkSymbol(')')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);
        }
        else {
            compile_err("term", ")");
            return false;
        }
    }
    else if (compile_unaryOp(&vmCode)) {
        if (!compile_term()) { return false; }
        mVMFp << vmCode;
    }
    compilationXmlWrite("</term>\n");
    return true;
}
bool CompliationEngine::compile_subroutineCall()
{
    // subroutineCall	->	subroutineName '(' expressionList ')' | (className  | varName) '.' subroutineName '(' expressionList ')'
    std::string name1, name2;
    int argCount = 0;
    if (compile_subroutineName(&name1) || compile_className(&name1) || compile_varName(&name1)) {

        if (mTk.checkSymbol('(')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);

            // 一个类实例的函数调用自己类的函数，直接将本函数 this作为参数
            mVMFp << "push pointer 0\n";

            if (!mTk.checkSymbol(')')) 
            {
                if (!compile_expressionList(&argCount)) { return false; }
            }
            else {
                compilationXmlWrite("<expressionList>\n");
                compilationXmlWrite("</expressionList>\n");
            }

            if (mTk.checkSymbol(')')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);
            }
            else {
                compile_err("subroutineCall", ")");
                return false;
            }

            mVMFp << "call " << mClassName << "." << name1 << " " << (argCount + 1) << "\n";
        }
        else if (mTk.checkSymbol('.')) {
            compilationXmlWrite();
            mJackTokenizer->advance(mTk);

            if (!compile_subroutineName(&name2)) { return false; }

            if (mTk.checkSymbol('(')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);
            }
            else {
                compile_err("subroutineCall", "(");
                return false;
            }

            if (!mTk.checkSymbol(')')) 
            { 
                if (!compile_expressionList(&argCount)) { return false; }
            }
            else {
                compilationXmlWrite("<expressionList>\n");
                compilationXmlWrite("</expressionList>\n");
            }

            if (mTk.checkSymbol(')')) {
                compilationXmlWrite();
                mJackTokenizer->advance(mTk);
            }
            else {
                compile_err("subroutineCall", ")");
                return false;
            }

            if (mSymbolTable.kindOf(name1) == SymbolTable::eKind::eK_NONE) {
                mVMFp << "call " << name1 << "." << name2 << " " << argCount << "\n";
            }
            else {
                switch (mSymbolTable.kindOf(name1)) {
                case SymbolTable::eKind::eK_STATIC:
                    mVMFp << "push " << mClassName << "." << name1 << "\n";
                    break;
                case SymbolTable::eKind::eK_FIELD:
                    mVMFp << "push this " << mSymbolTable.indexOf(name1) << "\n";
                    break;
                case SymbolTable::eKind::eK_ARG:
                    mVMFp << "push argument " << mSymbolTable.indexOf(name1) << "\n";
                    break;
                case SymbolTable::eKind::eK_VAR:
                    mVMFp << "push local " << mSymbolTable.indexOf(name1) << "\n";
                    break;
                }

                mVMFp << "call " << mSymbolTable.typeOf(name1) << "." << name2 << " " << (argCount + 1) << "\n";
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }

    return true;
}
bool CompliationEngine::compile_expressionList(int* argCount)
{
    // expressionList	->	(expression (',' expression)*)?
    compilationXmlWrite("<expressionList>\n");
    if (!compile_expression()) { return false; }
    if (argCount != 0) { *argCount = 1; }
    while (mTk.checkSymbol(',')) {
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
        if (!compile_expression()) { return false; }
        if (argCount != 0) { (*argCount)++; }
    }
    compilationXmlWrite("</expressionList>\n");
    return true;
}
bool CompliationEngine::compile_op(std::string *vmCode)
{
    static std::map<char, std::string> sOpVMMap = { {'+', "add\n"}, {'-', "sub\n"}, {'*', "call Math.multiply 2\n"}, {'/', "call Math.divide 2\n"}, 
                                                {'&', "and\n"}, {'|', "or\n"}, {'<', "lt\n"}, {'>', "gt\n"}, {'=', "eq\n"}};

    // op ->	’+‘ | ’-’ | '*' | '/' | '&' | '|' | '<' | '>' | '='
    if (mTk.checkSymbol('+') || mTk.checkSymbol('-') || mTk.checkSymbol('*')
        || mTk.checkSymbol('/') || mTk.checkSymbol('&') || mTk.checkSymbol('|')
        || mTk.checkSymbol('<') || mTk.checkSymbol('>') || mTk.checkSymbol('=')) {
        if (vmCode != 0) { *vmCode = sOpVMMap[mTk.mSymbol]; }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else {
        return false;
    }

    return true;
}
bool CompliationEngine::compile_unaryOp(std::string* vmCode)
{
    //unaryOp		->	'-' | '~'
    static std::map<char, std::string> sOpVMMap = { {'-', "neg\n"}, {'~', "not\n"} };
    if (mTk.checkSymbol('-') || mTk.checkSymbol('~')) {
        if (vmCode != 0) { *vmCode = sOpVMMap[mTk.mSymbol]; }
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else {
        return false;
    }
    return true;
}
bool CompliationEngine::compile_KeywordConstant()
{
    // KeywordConstant	->	'true' | 'false' | 'null' | this'

    if (mTk.checkKeyWord(Token::eKeyword::eK_TRUE)) {
        mVMFp << "push constant 1\n";
        mVMFp << "neg\n";
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    } 
    else if (mTk.checkKeyWord(Token::eKeyword::eK_FALSE)) {
        mVMFp << "push constant 0\n";
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else if (mTk.checkKeyWord(Token::eKeyword::eK_NULL)) {
        mVMFp << "push constant 0\n";
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else if (mTk.checkKeyWord(Token::eKeyword::eK_THIS)) {
        mVMFp << "push pointer 0\n";
        compilationXmlWrite();
        mJackTokenizer->advance(mTk);
    }
    else {
        return false;
    }
    return true;
}

void CompliationEngine::compile_err(std::string mode, std::string expectFor)
{
    std::cout << "CompliationEngine " << mode << " Err exceptFor " << expectFor << " !!!!\n";
}

void CompliationEngine::compilationXmlWrite(std::string s)
{
    mTestXmlFp << s;
}


void CompliationEngine::compilationXmlWrite()
{
    switch (mTk.mTokenType)
    {
    case Token::eTokenType::eTT_KEYWORD:
        mTestXmlFp << "<keyword> " << Token::s_eKeywordMapReverse[mTk.mKeywordType] << " </keyword>\n";
        break;
    case Token::eTokenType::eTT_SYMBOL:
        mTestXmlFp << "<symbol> " << mTk.mSymbol << " </symbol>\n";
        break;
    case Token::eTokenType::eTT_ID:
        mTestXmlFp << "<identifier " << " isDefind = ";
        if (mSymbolTable.kindOf(mTk.mID) != SymbolTable::eK_NONE) {
            mTestXmlFp << "1 ";
            switch (mSymbolTable.kindOf(mTk.mID))
            {
            case SymbolTable::eK_STATIC:
                mTestXmlFp << " type = eK_STATIC ";
                break;
            case SymbolTable::eK_FIELD:
                mTestXmlFp << " type = eK_FIELD ";
                break;
            case SymbolTable::eK_ARG:
                mTestXmlFp << " type = eK_ARG ";
                break;
            case SymbolTable::eK_VAR:
                mTestXmlFp << " type = eK_VAR ";
                break;
            default:
                break;
            }
        }
        else {
            mTestXmlFp << "0 ";
        }
        mTestXmlFp << " index = " << mSymbolTable.indexOf(mTk.mID);
        mTestXmlFp << "> " << mTk.mID << " </identifier>\n";
        break;
    case Token::eTokenType::eTT_INT_CONST:
        mTestXmlFp << "<integerConstant> " << mTk.mIntVal << " </integerConstant>\n";
        break;
    case Token::eTokenType::eTT_STRING_CONST:
        mTestXmlFp << "<stringConstant> " << mTk.mStringVal << " </stringConstant>\n";
        break;
    default:
        break;
    }
}