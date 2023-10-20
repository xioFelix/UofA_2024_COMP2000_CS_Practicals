#include <iostream>
#include <list>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

int main(int argc, char *argv[]) {
  /* Tokens for:
    int a, char b

    keyword int
    identifier a
    symbol ,
    keyword char
    identifier b
   */
  list<Token*> tokens;
  tokens.push_back(new Token("keyword", "int"));
  tokens.push_back(new Token("identifier", "a"));
  // tokens.push_back(new Token("symbol", ","));
  // tokens.push_back(new Token("keyword", "char"));
  // tokens.push_back(new Token("identifier", "b"));

  try {
    CompilerParser parser(tokens);
    ParseTree* result = parser.compileParameterList();
    if (result != NULL) {
      cout << result->tostring() << endl;
    }
    } catch (ParseException e) {
        cout << "Error Parsing!" << endl;
    }
}