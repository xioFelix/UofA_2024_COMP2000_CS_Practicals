#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char* argv[]) {
  /* Tokens for:
  ( ( a + ( 1 - c ) ) > 5 ) = true
  */

  list<Token*> tokens;
  tokens.push_back(new Token("symbol", "("));
  tokens.push_back(new Token("symbol", "("));
  tokens.push_back(new Token("identifier", "a"));
  tokens.push_back(new Token("symbol", "+"));
  tokens.push_back(new Token("symbol", "("));
  tokens.push_back(new Token("integerConstant", "1"));
  tokens.push_back(new Token("symbol", "-"));
  tokens.push_back(new Token("identifier", "c"));
  tokens.push_back(new Token("symbol", ")"));
  tokens.push_back(new Token("symbol", ")"));
  tokens.push_back(new Token("symbol", ">"));
  tokens.push_back(new Token("integerConstant", "5"));
  tokens.push_back(new Token("symbol", ")"));
  tokens.push_back(new Token("symbol", "="));
  tokens.push_back(new Token("keyword", "true"));

  try {
    CompilerParser parser(tokens);
    ParseTree* result = parser.compileExpression();
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
  } catch (ParseException e) {
    cout << "Error Parsing!" << endl;
  }
}