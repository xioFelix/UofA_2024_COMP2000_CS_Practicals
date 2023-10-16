#include <iostream>
#include <list>
#include <vector>

#include "CompilerParser.h"
#include "Token.h"

using namespace std;

struct TestCase {
  list<Token*> tokens;
  string expectedTreeStructure;
  string exceptionMessage;  // Empty if no exception is expected
};

int main(int argc, char* argv[]) {
  vector<TestCase> testCases;

  // Test Case for compileProgram with class declaration
  TestCase testCase1;
  testCase1.tokens = {new Token("keyword", "class"),
                      new Token("identifier", "Main"), new Token("symbol", "{"),
                      new Token("symbol", "}")};
  testCase1.expectedTreeStructure = "Your expected tree structure for class";
  testCases.push_back(testCase1);

  // Test Case for compileProgram without class declaration
  TestCase testCase2;
  testCase2.tokens = {new Token("keyword", "static"),
                      new Token("keyword", "int"), new Token("identifier", "a"),
                      new Token("symbol", ";")};
  testCase2.exceptionMessage =
      "ParseError (the program doesn't begin with a class)";
  testCases.push_back(testCase2);

  // Test Case for compileClass with class variable declaration
  TestCase testCase3;
  testCase3.tokens = {
      new Token("keyword", "class"), new Token("identifier", "Main"),
      new Token("symbol", "{"),      new Token("keyword", "static"),
      new Token("keyword", "int"),   new Token("identifier", "a"),
      new Token("symbol", ";"),      new Token("symbol", "}")};
  testCase3.expectedTreeStructure =
      "Your expected tree structure for class with classVarDec";
  testCases.push_back(testCase3);

  // Test Case for compileClassVarDec
  TestCase testCase4;
  testCase4.tokens = {new Token("keyword", "static"),
                      new Token("keyword", "int"), new Token("identifier", "a"),
                      new Token("symbol", ";")};
  testCase4.expectedTreeStructure =
      "Your expected tree structure for classVarDec";
  testCases.push_back(testCase4);

  // ... Add more test cases as needed ...

  for (const auto& testCase : testCases) {
    try {
      CompilerParser parser(testCase.tokens);
      ParseTree* result = parser.compileProgram();
      if (result != NULL) {
        if (result->tostring() == testCase.expectedTreeStructure) {
          cout << "Test passed!" << endl;
        } else {
          cout << "Test failed!" << endl;
          cout << "Expected: " << testCase.expectedTreeStructure << endl;
          cout << "Got: " << result->tostring() << endl;
        }
      }
    } catch (ParseException& e) {
      if (string(e.what()) == testCase.exceptionMessage) {
        cout << "Test passed with expected exception: " << e.what() << endl;
      } else {
        cout << "Test failed! Expected exception: " << testCase.exceptionMessage
             << ", but got: " << e.what() << endl;
      }
    }
  }

  return 0;
}
