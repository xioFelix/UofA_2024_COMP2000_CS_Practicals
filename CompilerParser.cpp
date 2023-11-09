#include "CompilerParser.h"
#include <iostream>
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
  ParseTree* program = this->compileClass();
  return program;
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
  // // std::cout << "Entering compileSubroutine\n";

  ParseTree* subroutineTree = new ParseTree("subroutine", "");

  if (have("keyword", "constructor")) {
    // // std::cout << "Parsing constructor\n";
    subroutineTree->addChild(mustBe("keyword", "constructor"));
  } else if (have("keyword", "function")) {
    // // std::cout << "Parsing function\n";
    subroutineTree->addChild(mustBe("keyword", "function"));
  } else if (have("keyword", "method")) {
    // // std::cout << "Parsing method\n";
    subroutineTree->addChild(mustBe("keyword", "method"));
  } else {
    throw ParseException();
  }

  if (have("keyword", "void") || have("keyword", "int") ||
      have("keyword", "char") || have("keyword", "boolean")) {
    // // std::cout << "Parsed return type: " << current()->getValue() << "\n";
    subroutineTree->addChild(current());
    next();
  } else if (have("identifier", current()->getValue())) {
    // // std::cout << "Parsed return type: " << current()->getValue() << "\n";
    subroutineTree->addChild(current());
    next();
  } else {
    throw ParseException();
  }

  // // std::cout << "Parsed subroutine name: " << current()->getValue() << "\n";
  subroutineTree->addChild(mustBe("identifier", current()->getValue()));

  // // std::cout << "Parsed opening parenthesis for parameters\n";
  subroutineTree->addChild(mustBe("symbol", "("));

  subroutineTree->addChild(compileParameterList());

  // // std::cout << "Parsed closing parenthesis for parameters\n";
  subroutineTree->addChild(mustBe("symbol", ")"));

  subroutineTree->addChild(compileSubroutineBody());

  // // std::cout << "Exiting compileSubroutine\n";

  return subroutineTree;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
  ParseTree* parameterListTree = new ParseTree("parameterList", "");

  // // std::cout << "Entering compileParameterList\n";

  bool expectType = true;

  // Check if the current token is a closing parenthesis
  if (have("symbol", ")")) {
    // // std::cout << "Exiting compileParameterList with no parameters\n";
    return parameterListTree;
  }

  while (tokensIterator != tokensList.end()) {
    // // std::cout << "Current token in compileParameterList: "
              // << current()->getValue() << "\n";

    if (expectType) {
      if (have("keyword", "int") || have("keyword", "char") ||
          have("keyword", "boolean") ||
          have("identifier", current()->getValue())) {
        // // std::cout << "Parsed type: " << current()->getValue() << "\n";

        parameterListTree->addChild(current());
        next();
        expectType = false;
      } else {
        throw ParseException();
      }
    } else {
      if (have("identifier", current()->getValue())) {
        // // std::cout << "Parsed identifier: " << current()->getValue() << "\n";

        parameterListTree->addChild(current());
        next();

        if (tokensIterator != tokensList.end() && have("symbol", ",")) {
          parameterListTree->addChild(current());
          next();
          expectType = true;
        } else {
          break;
        }
      } else {
        throw ParseException();
      }
    }
  }

  // // std::cout << "Exiting compileParameterList with token: "
            // << current()->getValue() << "\n";

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

  // Check if the next token is not "}" before processing statements
  if (!have("symbol", "}")) {
    subroutineBodyTree->addChild(compileStatements());
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

  // // std::cout << "Entering compileStatements\n";

  // Process statements until we encounter a character that doesn't start a
  // valid statement
  while (true) {
    // // std::cout << "Current token in compileStatements: " << current()->getValue()
              // << "\n";

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
      // // std::cout << "No valid statement found in compileStatements\n";
      break;
    }
  }

  // // std::cout << "Exiting compileStatements\n";

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
    letTree->addChild(compileExpression());
    letTree->addChild(mustBe("symbol", "]"));
  }

  // Followed by an '=' symbol
  letTree->addChild(mustBe("symbol", "="));

  // Followed by an expression
  letTree->addChild(compileExpression());

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

  // // std::cout << "Entering compileIf\n";
  // // std::cout << "Current token in compileIf: " << current()->getValue() << "\n";

  // 'if' keyword
  if (!have("keyword", "if")) {
    // // std::cout << "Expected 'if' keyword\n";
    throw ParseException();
  }
  ifTree->addChild(mustBe("keyword", "if"));

  // '(' symbol
  if (!have("symbol", "(")) {
    // // std::cout << "Expected '(' symbol\n";
    throw ParseException();
  }
  ifTree->addChild(mustBe("symbol", "("));

  // Expression
  // // std::cout << "Parsing expression in if condition\n";
  ifTree->addChild(compileExpression());

  // ')' symbol
  if (!have("symbol", ")")) {
    // // std::cout << "Expected ')' symbol\n";
    throw ParseException();
  }
  ifTree->addChild(mustBe("symbol", ")"));

  // '{' symbol
  if (!have("symbol", "{")) {
    // // std::cout << "Expected '{' symbol\n";
    throw ParseException();
  }
  ifTree->addChild(mustBe("symbol", "{"));

  // Statements
  // // std::cout << "Parsing statements in if body\n";
  ifTree->addChild(compileStatements());

  // '}' symbol
  if (!have("symbol", "}")) {
    // // std::cout << "Expected '}' symbol\n";
    throw ParseException();
  }
  ifTree->addChild(mustBe("symbol", "}"));

  // Optional 'else' clause
  if (have("keyword", "else")) {
    ifTree->addChild(mustBe("keyword", "else"));

    // '{' symbol for the 'else' clause
    if (!have("symbol", "{")) {
      // // std::cout << "Expected '{' symbol in else clause\n";
      throw ParseException();
    }
    ifTree->addChild(mustBe("symbol", "{"));

    // Statements for the 'else' clause
    // // std::cout << "Parsing statements in else body\n";
    ifTree->addChild(compileStatements());

    // '}' symbol for the 'else' clause
    if (!have("symbol", "}")) {
      // // std::cout << "Expected '}' symbol in else clause\n";
      throw ParseException();
    }
    ifTree->addChild(mustBe("symbol", "}"));
  }

  // // std::cout << "Exiting compileIf\n";

  return ifTree;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
  // Create a new parse tree for the while statement
  ParseTree* whileTree = new ParseTree("whileStatement", "");

  // 'while' keyword
  if (!have("keyword", "while")) {
    throw ParseException();
  }
  whileTree->addChild(mustBe("keyword", "while"));

  // '(' symbol
  if (!have("symbol", "(")) {
    throw ParseException();
  }
  whileTree->addChild(mustBe("symbol", "("));

  // Expression
  whileTree->addChild(compileExpression());

  // ')' symbol
  if (!have("symbol", ")")) {
    throw ParseException();
  }
  whileTree->addChild(mustBe("symbol", ")"));

  // '{' symbol
  if (!have("symbol", "{")) {
    throw ParseException();
  }
  whileTree->addChild(mustBe("symbol", "{"));

  // Statements
  whileTree->addChild(compileStatements());

  // '}' symbol
  if (!have("symbol", "}")) {
    throw ParseException();
  }
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
  doTree->addChild(compileExpression());

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
    returnTree->addChild(compileExpression());
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
  if (tokensIterator == tokensList.end()) {
    throw std::runtime_error("No more tokens to parse.");
  }

  ParseTree* expressionTree = new ParseTree("expression", "");

  // Check for 'skip' keyword which seems to be a special case in your language.
  if (have("keyword", "skip")) {
    expressionTree->addChild(new ParseTree("keyword", current()->getValue()));
    next();  // Consumes the 'skip' keyword token.
    return expressionTree;
  }

  // Start parsing the first term.
  ParseTree* currentTerm = compileTerm();
  expressionTree->addChild(currentTerm);

  // Check for the presence of any operators after the first term.
  while (have("symbol", "+") || have("symbol", "-") || have("symbol", "*") ||
         have("symbol", "/") || have("symbol", "&") || have("symbol", "|") ||
         have("symbol", ">") || have("symbol", "<") || have("symbol", "=") ||
         have("symbol", "~")) {
    expressionTree->addChild(new ParseTree("operator", current()->getValue()));
    next();  // Consumes the operator token.
    ParseTree* nextTerm = compileTerm();
    expressionTree->addChild(nextTerm);
  }

  // Now check for '.' which indicates a method call or member access
  if (have("symbol", ".")) {
    next();  // Consume the '.' token

    if (current()->getType() == "identifier") {
      std::string methodName = current()->getValue();
      next();  // Consume the identifier token

      // Expecting '(' after a method name indicates a method call
      if (have("symbol", "(")) {
        next();  // Consume the '(' token

        ParseTree* functionCallNode = new ParseTree("functionCall", methodName);
        ParseTree* argumentListNode = new ParseTree("argumentList", "");

        // Compile arguments until a closing ')' is found
        while (!have("symbol", ")")) {
          argumentListNode->addChild(compileExpression());
          if (have("symbol", ",")) {
            next();  // Consumes the ',' token between arguments
          }
        }
        next();  // Consume the ')' token at the end of the argument list

        functionCallNode->addChild(argumentListNode);
        expressionTree->addChild(
            functionCallNode);  // Add the functionCall to the expression tree
                                // correctly
      }
    }
  }

  return expressionTree;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
  if (tokensIterator == tokensList.end()) {
    throw ParseException();  // No tokens left to process
  }

  ParseTree* termTree = new ParseTree("term", "");

  // Handle integer constants
  if (have("integerConstant", current()->getValue())) {
    termTree->addChild(new ParseTree("integerConstant", current()->getValue()));
    next();
  }
  // Handle string constants
  else if (have("stringConstant", current()->getValue())) {
    termTree->addChild(new ParseTree("stringConstant", current()->getValue()));
    next();
  }
  // Handle keywords true, false, null, this
  else if (have("keyword", current()->getValue()) &&
           (current()->getValue() == "true" ||
            current()->getValue() == "false" ||
            current()->getValue() == "null" ||
            current()->getValue() == "this")) {
    termTree->addChild(new ParseTree("keyword", current()->getValue()));
    next();
  }
  // Handle identifiers and potentially method calls or array indexing
  else if (have("identifier", current()->getValue())) {
    termTree->addChild(new ParseTree("identifier", current()->getValue()));
    next();
    // Handle array indexing
    if (have("symbol", "[")) {
      next();  // Consume the '['
      termTree->addChild(new ParseTree("symbol", "["));
      termTree->addChild(compileExpression());
      if (!have("symbol", "]")) {
        throw ParseException();  // Expected closing bracket
      }
      termTree->addChild(new ParseTree("symbol", "]"));
      next();  // Consume the ']'
    }
    // Handle method calls
    else if (have("symbol", "(")) {
      next();  // Consume the '('
      termTree->addChild(new ParseTree("symbol", "("));
      termTree->addChild(compileExpressionList());
      if (!have("symbol", ")")) {
        throw ParseException();  // Expected closing parenthesis
      }
      termTree->addChild(new ParseTree("symbol", ")"));
      next();  // Consume the ')'
    }
  }
  // Handle sub-expression
  else if (have("symbol", "(")) {
    next();  // Consume the '('
    termTree->addChild(new ParseTree("symbol", "("));
    termTree->addChild(compileExpression());
    if (!have("symbol", ")")) {
      throw ParseException();  // Expected closing parenthesis
    }
    termTree->addChild(new ParseTree("symbol", ")"));
    next();  // Consume the ')'
  }
  // Handle unary operations
  else if (have("symbol", "-") || have("symbol", "~")) {
    termTree->addChild(new ParseTree("unaryOp", current()->getValue()));
    next();
    termTree->addChild(compileTerm());
  } else {
    throw ParseException();  // Unexpected token
  }

  return termTree;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
  ParseTree* expressionListTree = new ParseTree("expressionList", "");

  // Compile the first expression
  expressionListTree->addChild(compileExpression());

  // Compile subsequent expressions if they are separated by commas
  while (have("symbol", ",")) {
    next();  // Consume the comma
    expressionListTree->addChild(compileExpression());
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
  throw ParseException();
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
 * If so, advance to the next token, returning the current token, otherwise
 * throw a ParseException.
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
