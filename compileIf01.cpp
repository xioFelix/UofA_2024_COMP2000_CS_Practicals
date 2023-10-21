#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char* argv[]) {
  /* Tokens for:
    if ( skip ) {

    }
   */
  list<Token*> tokens;
  tokens.push_back(new Token("keyword", "if"));
  tokens.push_back(new Token("symbol", "("));
  tokens.push_back(new Token("keyword", "skip"));
  tokens.push_back(new Token("symbol", ")"));
  tokens.push_back(new Token("symbol", "{"));
  tokens.push_back(new Token("symbol", "}"));

  try {
    CompilerParser parser(tokens);
    ParseTree* result = parser.compileIf();
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
  } catch (ParseException e) {
    cout << "Error Parsing!" << endl;
  }
}