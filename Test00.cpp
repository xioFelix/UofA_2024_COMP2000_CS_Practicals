#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char *argv[]) {
  /* Tokens for:
    static int a ;
   */
  list<Token*> tokens;
  tokens.push_back(new Token("keyword", "class"));
  tokens.push_back(new Token("identifier", "main"));
  tokens.push_back(new Token("symbol", "{"));
  tokens.push_back(new Token("keyword", "function"));
  tokens.push_back(new Token("keyword", "void"));
  tokens.push_back(new Token("identifier", "test"));
  tokens.push_back(new Token("symbol", "("));
  tokens.push_back(new Token("symbol", ")"));
  tokens.push_back(new Token("symbol", "{"));
  tokens.push_back(new Token("symbol", "}"));
  tokens.push_back(new Token("symbol", "}"));

  try {
    CompilerParser parser(tokens);
    ParseTree* result = parser.compileClass();
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
    } catch (ParseException e) {
        cout << "Error Parsing!" << endl;
    }
}