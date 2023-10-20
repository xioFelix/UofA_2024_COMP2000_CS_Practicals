#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char* argv[]) {
  /* Tokens for:
    function void myFunc ( int a ) {
        var int a ;
        let a = 1 ;
    }
   */
  list<Token*> tokens;
  tokens.push_back(new Token("keyword", "function"));
  tokens.push_back(new Token("keyword", "void"));
  tokens.push_back(new Token("identifier", "myFunc"));
  tokens.push_back(new Token("symbol", "("));
  tokens.push_back(new Token("keyword", "int"));
  tokens.push_back(new Token("identifier", "a"));
  tokens.push_back(new Token("symbol", ")"));
  tokens.push_back(new Token("symbol", "{"));
  tokens.push_back(new Token("keyword", "var"));
  tokens.push_back(new Token("keyword", "int"));
  tokens.push_back(new Token("identifier", "a"));
  tokens.push_back(new Token("symbol", ";"));
  tokens.push_back(new Token("keyword", "let"));
  tokens.push_back(new Token("identifier", "a"));
  tokens.push_back(new Token("symbol", "="));
  tokens.push_back(new Token("integerConstant", "1"));
  tokens.push_back(new Token("symbol", ";"));
  tokens.push_back(new Token("symbol", "}"));

  try {
    CompilerParser parser(tokens);
    ParseTree* result = parser.compileSubroutine();
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
  } catch (ParseException e) {
    cout << "Error Parsing!" << endl;
  }
}