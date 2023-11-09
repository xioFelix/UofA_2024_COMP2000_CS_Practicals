#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char* argv[]) {
  /* Tokens for:
   1 , a - b
  */

  list<Token*> tokens;
  tokens.push_back(new Token("integerConstant", "1"));
  tokens.push_back(new Token("symbol", ","));
  tokens.push_back(new Token("identifier", "a"));
  tokens.push_back(new Token("symbol", "-"));
  tokens.push_back(new Token("identifier", "b"));

  try {
    CompilerParser parser(tokens);
    ParseTree* result =
        parser.compileExpressionList();  // Changed to compileExpressionList
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
  } catch (ParseException& e) {  // Catch by reference to avoid object slicing
    cout << "Error Parsing!" << endl;
  }
}
