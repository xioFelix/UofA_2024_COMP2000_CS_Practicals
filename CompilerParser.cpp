#include "CompilerParser.h"


/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
  // Initialize the token list and set the current token iterator
  tokensList = tokens;
  tokensIterator = tokensList.begin();
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
  // Create a new parse tree for the program
  ParseTree* programTree = new ParseTree("program", "");

  // Ensure the program starts with a "class" keyword
  if (!have("keyword", "class")) {
    throw ParseException();
  }

  // Loop through and process each class in the file
  do {
    programTree->addChild(compileClass());
  } while (have("keyword", "class"));

  if (tokensIterator != tokensList.end()) {
    throw ParseException();  // If there are remaining tokens after processing
                             // all classes, it's a parsing error.
  }

  return programTree;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
  // Create a new parse tree for the class
  ParseTree* classTree = new ParseTree("class", "");

  // A class should start with the keyword 'class'
  if (!have("keyword", "class")) {
    throw ParseException();
  }
  classTree->addChild(mustBe("keyword", "class"));

  // Followed by the class name (an identifier)
  classTree->addChild(mustBe("identifier", current()->getValue()));

  // Followed by a left curly brace
  classTree->addChild(mustBe("symbol", "{"));

  // Process class variable declarations and/or subroutine declarations
  while (!have("symbol", "}")) {
    if (have("keyword", "static") || have("keyword", "field")) {
      classTree->addChild(compileClassVarDec());
    } else if (have("keyword", "constructor") || have("keyword", "function") ||
               have("keyword", "method")) {
      classTree->addChild(compileSubroutine());
    } else {
      throw ParseException();
    }
  }

  // Followed by a right curly brace
  classTree->addChild(mustBe("symbol", "}"));

  return classTree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
  // Create a new parse tree for the class variable declaration
  ParseTree* classVarDecTree = new ParseTree("classVarDec", "");

  // A class variable declaration should start with either 'static' or 'field'
  if (have("keyword", "static")) {
    classVarDecTree->addChild(mustBe("keyword", "static"));
  } else if (have("keyword", "field")) {
    classVarDecTree->addChild(mustBe("keyword", "field"));
  } else {
    throw ParseException();
  }

  // Followed by the type of the variable
  if (have("keyword", "int") || have("keyword", "char") ||
      have("keyword", "boolean")) {
    classVarDecTree->addChild(current());
    next();
  } else if (have("identifier", current()->getValue())) {
    classVarDecTree->addChild(current());
    next();
  } else {
    throw ParseException();
  }

  // Followed by one or more variable names separated by commas
  while (true) {
    if (have("identifier", current()->getValue())) {
      classVarDecTree->addChild(current());
      next();

      if (have("symbol", ",")) {
        classVarDecTree->addChild(mustBe("symbol", ","));
      } else if (have("symbol", ";")) {
        classVarDecTree->addChild(mustBe("symbol", ";"));
        break;
      } else {
        throw ParseException();
      }
    } else {
      throw ParseException();
    }
  }

  return classVarDecTree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
  // Create a new parse tree for the subroutine
  ParseTree* subroutineTree = new ParseTree("subroutine", "");

  // A subroutine should start with 'constructor', 'function', or 'method'
  if (have("keyword", "constructor")) {
    subroutineTree->addChild(mustBe("keyword", "constructor"));
  } else if (have("keyword", "function")) {
    subroutineTree->addChild(mustBe("keyword", "function"));
  } else if (have("keyword", "method")) {
    subroutineTree->addChild(mustBe("keyword", "method"));
  } else {
    throw ParseException();
  }

  // Followed by the return type of the subroutine
  if (have("keyword", "void") || have("keyword", "int") ||
      have("keyword", "char") || have("keyword", "boolean")) {
    subroutineTree->addChild(current());
    next();
  } else if (have("identifier", current()->getValue())) {
    subroutineTree->addChild(current());
    next();
  } else {
    throw ParseException();
  }

  // Followed by the subroutine name
  subroutineTree->addChild(mustBe("identifier", current()->getValue()));

  // Followed by an opening parenthesis
  subroutineTree->addChild(mustBe("symbol", "("));

  // Handle parameters inside the parenthesis
  subroutineTree->addChild(compileParameterList());

  // Followed by a closing parenthesis
  subroutineTree->addChild(mustBe("symbol", ")"));

  // Handle the subroutine body
  subroutineTree->addChild(compileSubroutineBody());

  return subroutineTree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
  // Create a new parse tree for the parameter list
  ParseTree* parameterListTree = new ParseTree("parameterList", "");

  // Check if the parameter list is empty
  if (have("symbol", ")")) {
    return parameterListTree;
  }

  // Process parameters until we encounter a closing parenthesis
  while (!have("symbol", ")")) {
    // Get the type of the parameter
    if (have("keyword", "int") || have("keyword", "char") ||
        have("keyword", "boolean")) {
      parameterListTree->addChild(current());
      next();
    } else if (have("identifier", current()->getValue())) {
      parameterListTree->addChild(current());
      next();
    } else {
      throw ParseException();
    }

    // Get the name of the parameter
    parameterListTree->addChild(mustBe("identifier", current()->getValue()));

    // If there's a comma, we expect another parameter, so we add the comma and
    // continue
    if (have("symbol", ",")) {
      parameterListTree->addChild(mustBe("symbol", ","));
    }
  }

  return parameterListTree;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
  // Create a new parse tree for the subroutine body
  ParseTree* subroutineBodyTree = new ParseTree("subroutineBody", "");

  // The subroutine body should start with a left curly brace
  subroutineBodyTree->addChild(mustBe("symbol", "{"));

  // Process zero or more variable declarations
  while (have("keyword", "var")) {
    subroutineBodyTree->addChild(compileVarDec());
  }

  // Process statements until we encounter a closing curly brace
  while (!have("symbol", "}")) {
    if (have("keyword", "let") || have("keyword", "if") ||
        have("keyword", "while") || have("keyword", "do") ||
        have("keyword", "return")) {
      subroutineBodyTree->addChild(compileStatements());
    } else {
      throw ParseException();
    }
  }

  // The subroutine body should end with a right curly brace
  subroutineBodyTree->addChild(mustBe("symbol", "}"));

  return subroutineBodyTree;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
  // Create a new parse tree for the variable declaration
  ParseTree* varDecTree = new ParseTree("varDec", "");

  // A variable declaration should start with the keyword 'var'
  varDecTree->addChild(mustBe("keyword", "var"));

  // Followed by the type of the variable
  if (have("keyword", "int") || have("keyword", "char") ||
      have("keyword", "boolean")) {
    varDecTree->addChild(current());
    next();
  } else if (have("identifier", current()->getValue())) {
    varDecTree->addChild(current());
    next();
  } else {
    throw ParseException();
  }

  // Followed by one or more variable names separated by commas
  while (true) {
    if (have("identifier", current()->getValue())) {
      varDecTree->addChild(current());
      next();

      if (have("symbol", ",")) {
        varDecTree->addChild(mustBe("symbol", ","));
      } else if (have("symbol", ";")) {
        varDecTree->addChild(mustBe("symbol", ";"));
        break;
      } else {
        throw ParseException();
      }
    } else {
      throw ParseException();
    }
  }

  return varDecTree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
  // Create a new parse tree for the series of statements
  ParseTree* statementsTree = new ParseTree("statements", "");

  // Process statements until we encounter a character that doesn't start a
  // valid statement
  while (true) {
    if (have("keyword", "let")) {
      statementsTree->addChild(compileLet());
    } else if (have("keyword", "if")) {
      statementsTree->addChild(compileIf());
    } else if (have("keyword", "while")) {
      statementsTree->addChild(compileWhile());
    } else if (have("keyword", "do")) {
      statementsTree->addChild(compileDo());
    } else if (have("keyword", "return")) {
      statementsTree->addChild(compileReturn());
    } else {
      break;
    }
  }

  return statementsTree;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
  // Create a new parse tree for the let statement
  ParseTree* letTree = new ParseTree("letStatement", "");

  // A let statement should start with the keyword 'let'
  letTree->addChild(mustBe("keyword", "let"));

  // Followed by a variable name
  letTree->addChild(mustBe("identifier", current()->getValue()));

  // Check for an array index (i.e., '[' followed by an expression followed by
  // ']')
  if (have("symbol", "[")) {
    letTree->addChild(mustBe("symbol", "["));
    letTree->addChild(
        compileExpression());
    letTree->addChild(mustBe("symbol", "]"));
  }

  // Followed by an '=' symbol
  letTree->addChild(mustBe("symbol", "="));

  // Followed by an expression
  letTree->addChild(
      compileExpression());

  // The let statement should end with a semicolon
  letTree->addChild(mustBe("symbol", ";"));

  return letTree;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
  // Create a new parse tree for the if statement
  ParseTree* ifTree = new ParseTree("ifStatement", "");

  // An if statement should start with the keyword 'if'
  ifTree->addChild(mustBe("keyword", "if"));

  // Followed by a left parenthesis
  ifTree->addChild(mustBe("symbol", "("));

  // Followed by an expression
  ifTree->addChild(
      compileExpression());

  // Followed by a right parenthesis
  ifTree->addChild(mustBe("symbol", ")"));

  // Followed by a left curly brace
  ifTree->addChild(mustBe("symbol", "{"));

  // Followed by statements
  ifTree->addChild(compileStatements());

  // Followed by a right curly brace
  ifTree->addChild(mustBe("symbol", "}"));

  // Check for an optional else clause
  if (have("keyword", "else")) {
    ifTree->addChild(mustBe("keyword", "else"));
    ifTree->addChild(mustBe("symbol", "{"));
    ifTree->addChild(compileStatements());
    ifTree->addChild(mustBe("symbol", "}"));
  }

  return ifTree;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
  // Create a new parse tree for the while statement
  ParseTree* whileTree = new ParseTree("whileStatement", "");

  // A while statement should start with the keyword 'while'
  whileTree->addChild(mustBe("keyword", "while"));

  // Followed by a left parenthesis
  whileTree->addChild(mustBe("symbol", "("));

  // Followed by an expression
  whileTree->addChild(
      compileExpression());

  // Followed by a right parenthesis
  whileTree->addChild(mustBe("symbol", ")"));

  // Followed by a left curly brace
  whileTree->addChild(mustBe("symbol", "{"));

  // Followed by statements
  whileTree->addChild(compileStatements());

  // Followed by a right curly brace
  whileTree->addChild(mustBe("symbol", "}"));

  return whileTree;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
  // Create a new parse tree for the do statement
  ParseTree* doTree = new ParseTree("doStatement", "");

  // A do statement should start with the keyword 'do'
  doTree->addChild(mustBe("keyword", "do"));

  // Followed by an expression
  doTree->addChild(
      compileExpression());

  // The do statement should end with a semicolon
  doTree->addChild(mustBe("symbol", ";"));

  return doTree;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
  // Create a new parse tree for the return statement
  ParseTree* returnTree = new ParseTree("returnStatement", "");

  // A return statement should start with the keyword 'return'
  returnTree->addChild(mustBe("keyword", "return"));

  // Optionally followed by an expression
  if (!have("symbol", ";")) {  // If the next token is not a semicolon, then we
                               // expect an expression
    returnTree->addChild(
        compileExpression());
  }

  // The return statement should end with a semicolon
  returnTree->addChild(mustBe("symbol", ";"));

  return returnTree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
  // Create a new parse tree for the expression
  ParseTree* expressionTree = new ParseTree("expression", "");

  // An expression should start with a term
  expressionTree->addChild(compileTerm());

  // Local lambda function to check if a string is an operator in Jack language
  auto isOperator = [](std::string value) {
    return value == "+" || value == "-" || value == "*" || value == "/" ||
           value == "&" || value == "|" || value == "<" || value == ">" ||
           value == "=" || value == "~";
  };

  // Check for an optional operator followed by another term
  while (isOperator(current()->getValue())) {
    expressionTree->addChild(mustBe(
        "symbol", current()->getValue()));    // Add the operator to the tree
    expressionTree->addChild(compileTerm());  // Add the next term
  }

  return expressionTree;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
  // Create a new parse tree for the term
  ParseTree* termTree = new ParseTree("term", "");

  Token* currentToken = current();

  // Check the type of the current token to determine the type of the term
  if (currentToken->getType() == "integerConstant") {
    termTree->addChild(mustBe("integerConstant", currentToken->getValue()));
  } else if (currentToken->getType() == "stringConstant") {
    termTree->addChild(mustBe("stringConstant", currentToken->getValue()));
  } else if (currentToken->getType() == "keyword" &&
             (currentToken->getValue() == "true" ||
              currentToken->getValue() == "false" ||
              currentToken->getValue() == "null" ||
              currentToken->getValue() == "this")) {
    termTree->addChild(mustBe("keyword", currentToken->getValue()));
  } else if (currentToken->getType() == "identifier") {
    termTree->addChild(mustBe("identifier", currentToken->getValue()));

    // If the next token is '[', it's an array access
    if (have("symbol", "[")) {
      termTree->addChild(mustBe("symbol", "["));
      termTree->addChild(compileExpression());
      termTree->addChild(mustBe("symbol", "]"));
    } else if (currentToken->getType() == "identifier") {
      // Save the first identifier (it could be className, varName, or
      // subroutineName)
      termTree->addChild(mustBe("identifier", currentToken->getValue()));

      if (have("symbol", "[")) {  // Array access
        termTree->addChild(mustBe("symbol", "["));
        termTree->addChild(compileExpression());
        termTree->addChild(mustBe("symbol", "]"));
      } else if (have("symbol", "{")) {
        termTree->addChild(mustBe("symbol", "{"));
        termTree->addChild(compileExpression());
        termTree->addChild(mustBe("symbol", "}"));
      } else if (have("symbol", "(")) {  // Subroutine call in form:
                                         // subroutineName(expressionList)
        termTree->addChild(mustBe("symbol", "("));
        termTree->addChild(compileExpressionList());
        termTree->addChild(mustBe("symbol", ")"));
      } else if (
          have("symbol",
               ".")) {  // Subroutine call in form:
                        // className/varName.subroutineName(expressionList)
        termTree->addChild(mustBe("symbol", "."));
        termTree->addChild(mustBe("identifier", current()->getValue()));
        termTree->addChild(mustBe("symbol", "("));
        termTree->addChild(compileExpressionList());
        termTree->addChild(mustBe("symbol", ")"));
      }
    }
  } else if (currentToken->getType() == "symbol" &&
             currentToken->getValue() == "(") {
    termTree->addChild(mustBe("symbol", "("));
    termTree->addChild(compileExpression());
    termTree->addChild(mustBe("symbol", ")"));
  } else if (currentToken->getType() == "symbol" &&
             (currentToken->getValue() == "-" ||
              currentToken->getValue() == "~")) {
    termTree->addChild(mustBe("symbol", currentToken->getValue()));
    termTree->addChild(compileTerm());
  } else {
    throw ParseException();
  }

  return termTree;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
  // Create a new parse tree for the expression list
  ParseTree* expressionListTree = new ParseTree("expressionList", "");

  // If the next token is not a closing parenthesis, we expect at least one
  // expression
  if (!have("symbol", ")")) {
    expressionListTree->addChild(compileExpression());

    // While the next token is a comma, we expect more expressions
    while (have("symbol", ",")) {
      expressionListTree->addChild(mustBe("symbol", ","));
      expressionListTree->addChild(compileExpression());
    }
  }

  return expressionListTree;
}

/**
 * Advance to the next token
 */
void CompilerParser::next() {
  if (tokensIterator != tokensList.end()) {
    ++tokensIterator;
  }
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current() {
  if (tokensIterator != tokensList.end()) {
    return *tokensIterator;
  }
  return nullptr;
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
  if (tokensIterator != tokensList.end()) {
    Token* currentToken = *tokensIterator;
    if (currentToken->getType() == expectedType &&
        currentToken->getValue() == expectedValue) {
      return true;
    }
  }
  return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType,
                              std::string expectedValue) {
  if (have(expectedType, expectedValue)) {
    Token* currentToken = *tokensIterator;
    next();
    return currentToken;
  } else {
    throw ParseException();
  }
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}