#include "VMTranslator.h"

#include <string>

using namespace std;

/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
  // Your code here
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
  // Your code here
}

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset) {
  string asm_code;
  if (segment == "constant") {  // Handling constant values
    asm_code = "@" + to_string(offset) + "\n" + "D=A\n" + "@SP\n" + "AM=M+1\n" +
               "A=A-1\n" + "M=D\n";  // load constant into D register
  } else if (segment == "static") {  // Handling static values
    int newoffset = 16 + offset;
    asm_code = "@" + to_string(newoffset) + "\n" + "D=M\n" + "@SP\n" +
               "AM=M+1\n" + "A=A-1\n" + "M=D\n";
  } else if (segment == "pointer") {  // Handling static values
    int newoffset = 3 + offset;
    asm_code = "@" + to_string(newoffset) + "\n" + "D=M\n" + "@SP\n" +
               "AM=M+1\n" + "A=A-1\n" + "M=D\n";
  } else if (segment == "temp") {  // Handling static values
    int newoffset = 5 + offset;
    asm_code = "@" + to_string(newoffset) + "\n" + "D=M\n" + "@SP\n" +
               "AM=M+1\n" + "A=A-1\n" + "M=D\n";
  } else {
    // Handle other segments like local, argument, this, that, etc.
    string segment_base;
    if (segment == "local")
      segment_base = "LCL";
    else if (segment == "argument")
      segment_base = "ARG";
    else if (segment == "this")
      segment_base = "THIS";
    else if (segment == "argument")
      segment_base = "that";
    else if (segment == "THAT")
      segment_base = "ARG";
    else if (segment == "argument")
      segment_base = "ARG";
    else if (segment == "argument")
      segment_base = "ARG";
    asm_code = "@" + segment_base + "\n" + "D=M\n" + "@" + to_string(offset) +
               "\n" + "A=D+A\n" +  // compute the effective address
               "D=M\n";            // load value into D register
  }
  asm_code += "@SP\n" + string("A=M\n") +
              "M=D\n" +              // push D register value onto the stack
              "@SP\n" + "AM=M+1\n";  // increment stack pointer
  return asm_code;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int index) {
  string asm_code;

  if (segment == "local") {
    asm_code += "@LCL\n";
    asm_code += "D=M\n";
  } else if (segment == "argument") {
    asm_code += "@ARG\n";
    asm_code += "D=M\n";
  } else if (segment == "this") {
    asm_code += "@THIS\n";
    asm_code += "D=M\n";
  } else if (segment == "that") {
    asm_code += "@THAT\n";
    asm_code += "D=M\n";
  } else if (segment == "static") {
    asm_code += "@" + to_string(16 + index) + "\n";
    asm_code += "D=A\n";  // For static, D will hold the address of the variable
  } else if (segment == "pointer") {
    if (index == 0) {
      // THIS register
      return "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    } else if (index == 1) {
      // THAT register
      return "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    }
  }

  else if (segment == "temp") {
    return "@" + to_string(5 + index) + "\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
  }

  else if (segment != "static") {
    int newindex = 16 + index;
    asm_code += "@" + to_string(newindex) + "\n";
    asm_code += "D=D+A\n";  // For non-static segments, D now holds the
                            // effective address
  }

  asm_code += "@R13\n";  // Use R13 as temporary register to store the
                         // effective address
  asm_code += "M=D\n";

  asm_code += "@SP\n";
  asm_code += "AM=M-1\n";  // Decrement SP and put the address of the top of
                           // the stack in A
  asm_code += "D=M\n";     // D holds the value at the top of the stack

  asm_code += "@R13\n";
  asm_code += "A=M\n";  // A holds the effective address (from R13)
  asm_code += "M=D\n";  // Store the value in D to the effective address

  return asm_code;
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add() {
  return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" +
         "M=D+M\n";  // add top two values of the stack and store the result
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub() {
  return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" +
         "M=M-D\n";  // subtract top value of the stack from the second top
                     // value and store the result
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg() {
  return "@SP\n" + string("A=M-1\n") +
         "M=-M\n";  // negate the top value of the stack
}

int VMTranslator::label_count = 0;
/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq() {
  string label = "EQUAL" + to_string(label_count++);
  string asm_code = "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" +
                    "D=M-D\n" + "@TRUE" + label + "\n" + "D;JEQ\n" + "@SP\n" +
                    "A=M-1\n" + "M=0\n" + "@END" + label + "\n" + "0;JMP\n" +
                    "(TRUE" + label + ")\n" + "@SP\n" + "A=M-1\n" + "M=-1\n" +
                    "(END" + label + ")\n";
  return asm_code;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt() {
  string label = "EQUAL" + to_string(label_count++);
  string asm_code = "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" +
                    "D=M-D\n" + "@TRUE" + label + "\n" + "D;JGT\n" + "@SP\n" +
                    "A=M-1\n" + "M=0\n" + "@END" + label + "\n" + "0;JMP\n" +
                    "(TRUE" + label + ")\n" + "@SP\n" + "A=M-1\n" + "M=-1\n" +
                    "(END" + label + ")\n";
  return asm_code;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt() {
  string label = "EQUAL" + to_string(label_count++);
  string asm_code = "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" +
                    "D=M-D\n" + "@TRUE" + label + "\n" + "D;JLT\n" + "@SP\n" +
                    "A=M-1\n" + "M=0\n" + "@END" + label + "\n" + "0;JMP\n" +
                    "(TRUE" + label + ")\n" + "@SP\n" + "A=M-1\n" + "M=-1\n" +
                    "(END" + label + ")\n";
  return asm_code;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and() {
  return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "M=D&M\n";
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or() {
  return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "M=D|M\n";
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not() { return "@SP\n" + string("A=M-1\n") + "M=!M\n"; }

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label) { return "(" + label + ")\n"; }

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label) {
  return "@" + label + "\n" + "0;JMP\n";
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label) {
  return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "@" + label + "\n" +
         "D;JNE\n";
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars) {
  string asm_code = "(" + function_name + ")\n";
  for (int i = 0; i < n_vars; i++) {
    asm_code += "@SP\n" + string("A=M\n") + "M=0\n" + "@SP\n" + "M=M+1\n";
  }
  return asm_code;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int numArgs) {
  static int callCounter = 0;  // to create unique return labels
  string returnLabel = "RETURN_LABEL" + to_string(callCounter++);

  string asm_code =
      "@" + returnLabel + "\n" + "D=A\n" + "@SP\n" + "A=M\n" +
      "M=D\n" +  // push return address
      "@SP\n" + "M=M+1\n" +

      "@LCL\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" +  // push LCL
      "@SP\n" + "M=M+1\n" +

      "@ARG\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" +  // push ARG
      "@SP\n" + "M=M+1\n" +

      "@THIS\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" +  // push THIS
      "@SP\n" + "M=M+1\n" +

      "@THAT\n" + "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" +  // push THAT
      "@SP\n" + "M=M+1\n" +

      "@SP\n" + "D=M\n" + "@5\n" + "D=D-A\n" + "@" + to_string(numArgs) + "\n" +
      "D=D-A\n" + "@ARG\n" + "M=D\n" +  // reposition ARG

      "@SP\n" + "D=M\n" + "@LCL\n" + "M=D\n" +  // reposition LCL

      "@" + function_name + "\n" + "0;JMP\n" +  // goto f

      "(" + returnLabel + ")\n";  // (return-label)
  return asm_code;
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() {
  string asm_code =
      "@LCL\n" + string("D=M\n") + "@R13\n" + "M=D\n" +  // FRAME = LCL

      "@5\n" + "A=D-A\n" + "D=M\n" + "@R14\n" + "M=D\n" +  // RET = *(FRAME-5)

      "@SP\n" + "A=M-1\n" + "D=M\n" + "@ARG\n" + "A=M\n" +
      "M=D\n" +  // *ARG = pop()

      "@ARG\n" + "D=M+1\n" + "@SP\n" + "M=D\n" +  // SP = ARG+1

      "@R13\n" + "AM=M-1\n" + "D=M\n" + "@THAT\n" +
      "M=D\n" +  // THAT = *(FRAME-1)

      "@R13\n" + "AM=M-1\n" + "D=M\n" + "@THIS\n" +
      "M=D\n" +  // THIS = *(FRAME-2)

      "@R13\n" + "AM=M-1\n" + "D=M\n" + "@ARG\n" +
      "M=D\n" +  // ARG = *(FRAME-3)

      "@R13\n" + "AM=M-1\n" + "D=M\n" + "@LCL\n" +
      "M=D\n" +  // LCL = *(FRAME-4)

      "@R14\n" + "A=M\n" + "0;JMP\n";  // goto RET
  return asm_code;
}
