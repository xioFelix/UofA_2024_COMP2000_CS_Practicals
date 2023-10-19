#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char *argv[]) {
  /* Tokens for:
    while ( skip ) {

    }
   */
  list<Token*> tokens;
  // tokens.push_back(new Token("keyword", "while"));
  // tokens.push_back(new Token("symbol", "("));
  // tokens.push_back(new Token("keyword", "skip"));
  // tokens.push_back(new Token("symbol", ")"));
  // tokens.push_back(new Token("symbol", "{"));
  // tokens.push_back(new Token("symbol", "}"));

  tokens.push_back(new Token("keyword", "let"));
  tokens.push_back(new Token("identifier", "a"));
  tokens.push_back(new Token("symbol", "="));
  tokens.push_back(new Token("keyword", "skip"));
  tokens.push_back(new Token("symbol", ";"));


  try {
    CompilerParser parser(tokens);
    ParseTree* result = parser.compileProgram();
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
    } catch (ParseException e) {
        cout << "Error Parsing!" << endl;
    }
}