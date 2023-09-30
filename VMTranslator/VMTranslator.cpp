#include <string>

#include "VMTranslator.h"

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
string VMTranslator::vm_push(string segment, int index) {
    string asmCode;
    if (segment == "constant") {
      asmCode = "@" + to_string(index) + "\n"  // set A to index
                + "D=A\n"                      // D = A
                + "@SP\n" + "A=M\n"            // go to the top of the stack
                + "M=D\n"                      // set the top of the stack to D
                + "@SP\n" + "M=M+1\n";         // increment the stack pointer
    } else if (segment == "local") {
      // Handle local segment push
      asmCode = "@" + to_string(index) + "\n" + "D=A\n" + "@LCL\n" + "A=D+M\n" +
                "D=M\n" + "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
    }  // Add additional else if statements for other segments
    return asmCode;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int index) {
    string asmCode;
    if (segment == "local") {
      // Handle local segment pop
      asmCode = "@SP\n" + "M=M-1\n" + "A=M\n" + "D=M\n" + "@" +
                to_string(index) + "\n" + "D=D+A\n" + "@LCL\n" + "A=D+M\n" +
                "M=D\n";
    }  // Add additional else if statements for other segments
    return asmCode;
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
    return "@SP\n" + string("A=M-1\n") + "M=-M\n";  // negate the top value of the stack
}

int VMTranslator::label_count = 0;
/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq() {
    string label = "EQUAL" + to_string(label_count++);
    string asm_code = "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "D=M-D\n" +
                      "@TRUE" + label + "\n" + "D;JEQ\n" + "@SP\n" + "A=M-1\n" +
                      "M=0\n" + "@END" + label + "\n" + "0;JMP\n" + "(TRUE" +
                      label + ")\n" + "@SP\n" + "A=M-1\n" + "M=-1\n" + "(END" +
                      label + ")\n";
    return asm_code;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt(){
    string label = "EQUAL" + to_string(label_count++);
    string asm_code = "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "D=M-D\n" +
                      "@TRUE" + label + "\n" + "D;JGT\n" + "@SP\n" + "A=M-1\n" +
                      "M=0\n" + "@END" + label + "\n" + "0;JMP\n" + "(TRUE" +
                      label + ")\n" + "@SP\n" + "A=M-1\n" + "M=-1\n" + "(END" +
                      label + ")\n";
    return asm_code;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt(){
    string label = "EQUAL" + to_string(label_count++);
    string asm_code = "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "D=M-D\n" +
                      "@TRUE" + label + "\n" + "D;JLT\n" + "@SP\n" + "A=M-1\n" +
                      "M=0\n" + "@END" + label + "\n" + "0;JMP\n" + "(TRUE" +
                      label + ")\n" + "@SP\n" + "A=M-1\n" + "M=-1\n" + "(END" +
                      label + ")\n";
    return asm_code;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and(){
    return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "M=D&M\n";
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or(){
    return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "A=A-1\n" + "M=D|M\n";
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not() { return "@SP\n" + string("A=M-1\n") + "M=!M\n";
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string labelName) {
    return "(" + labelName + ")\n";
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string labelName) {
    return "@" + labelName + "\n" + "0;JMP\n";
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string labelName) {
    return "@SP\n" + "AM=M-1\n" + "D=M\n" + "@" + labelName + "\n" + "D;JNE\n";
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string functionName, int numLocals) {
    string asmCode =
        "(" + functionName + ")\n";  // Label for the function entry
    for (int i = 0; i < numLocals; i++) {
      asmCode += "@SP\n"  // Initialize local variables to 0
                 + "A=M\n" + "M=0\n" + "@SP\n" + "M=M+1\n";
    }
    return asmCode;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string functionName, int numArgs) {
    string returnLabel = "RETURN_LABEL" + to_string(label_count++);
    string asmCode = "@" + returnLabel + "\n" + "D=A\n" + "@SP\n" + "A=M\n" +
                     "M=D\n"  // push return address
                     + "@SP\n" + "M=M+1\n" + "@LCL\n" + "D=M\n" + "@SP\n" +
                     "A=M\n" + "M=D\n"  // push LCL
                     + "@SP\n" + "M=M+1\n" + "@ARG\n" + "D=M\n" + "@SP\n" +
                     "A=M\n" + "M=D\n"  // push ARG
                     + "@SP\n" + "M=M+1\n" + "@THIS\n" + "D=M\n" + "@SP\n" +
                     "A=M\n" + "M=D\n"  // push THIS
                     + "@SP\n" + "M=M+1\n" + "@THAT\n" + "D=M\n" + "@SP\n" +
                     "A=M\n" + "M=D\n"  // push THAT
                     + "@SP\n" + "M=M+1\n" + "@SP\n" + "D=M\n" + "@" +
                     to_string(numArgs + 5) + "\n" + "D=D-A\n" + "@ARG\n" +
                     "M=D\n"                                   // ARG = SP-n-5
                     + "@SP\n" + "D=M\n" + "@LCL\n" + "M=D\n"  // LCL = SP
                     + "@" + functionName + "\n" + "0;JMP\n"   // goto f
                     + "(" + returnLabel + ")\n";              // (return-label)
    return asmCode;
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() {
    string asmCode = "@LCL\n" + "D=M\n" + "@R13\n"              // FRAME is R13
                     + "M=D\n"                                  // FRAME=LCL
                     + "@5\n" + "A=D-A\n" + "D=M\n" + "@R14\n"  // RET is R14
                     + "M=D\n"  // RET=*(FRAME-5)
                     + "@SP\n" + "AM=M-1\n" + "D=M\n" + "@ARG\n" + "A=M\n" +
                     "M=D\n"                                     // *ARG=pop()
                     + "@ARG\n" + "D=M+1\n" + "@SP\n" + "M=D\n"  // SP=ARG+1
                     + "@R13\n"                                  // FRAME is R13
                     + "AM=M-1\n" + "D=M\n" + "@THAT\n" +
                     "M=D\n"  // THAT=*(FRAME-1)
                     + "@R13\n" + "AM=M-1\n" + "D=M\n" + "@THIS\n" +
                     "M=D\n"  // THIS=*(FRAME-2)
                     + "@R13\n" + "AM=M-1\n" + "D=M\n" + "@ARG\n" +
                     "M=D\n"  // ARG=*(FRAME-3)
                     + "@R13\n" + "AM=M-1\n" + "D=M\n" + "@LCL\n" +
                     "M=D\n"                 // LCL=*(FRAME-4)
                     + "@R14\n"              // RET is R14
                     + "A=M\n" + "0;JMP\n";  // goto RET
    return asmCode;
}
