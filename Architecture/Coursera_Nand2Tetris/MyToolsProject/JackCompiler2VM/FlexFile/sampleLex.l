
%{  
    #include <stdio.h>  
    #include <stdlib.h>   
    int count = 0;  
%}   
  
delim [" "\n\t\r]   
whitespace {delim}+   
/* operator \+|-|\*|\/|:=|>=|<=|#|=|<<|>>|\+\+ */
symbol  \{|\}|\(|\)|\[|\]|\.|,|;|\+|-|\*|\/|&|\||<|>|=|~
/* reservedWord int|include|main|return|using|if|namespace */
keyword     boolean|class|constructor|char|do|else|false|field|function|int|if|let|method|null|return|static|true|this|var|void|while
/* delimiter [,\.;\(\)\"] */
integerConstant ([0-9])+
identfier [A-Za-z]([A-Za-z_]|[0-9])*  
%%   
{symbol} { count++; printf("%d\t(symbol, %s)\n", count, yytext); } 
{integerConstant} {count++;printf("%d\t(integerConstant, %s)\n", count, yytext); } 
{keyword} {count++; printf("%d\t(keyword, %s)\n", count, yytext); }  
\"[^\"]*\" {count++; printf("%d\t(ct,%s)\n", count, yytext); }
{identfier} {count++; printf("%d\t(identfier, %s)\n", count, yytext);}   
{whitespace} { count++; printf("%d\t(whitespace, %s)\n", count, yytext);} 
%%

int main()
{
	yyin = fopen_s("D:\\MyWorkspace\\CS_DIY_GIT\\CS_DIY\\Architecture\\Coursera_Nand2Tetris\\MyHomework\\MyProject10\\ArrayTest\\Test.jack","r");
		yylex();
	fclose(yyin);
}
int yywrap()
{
	return 1;
}
