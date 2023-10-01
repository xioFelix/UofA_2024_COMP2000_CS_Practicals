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
string VMTranslator::vm_push(string segment, int offset) {
    string asm_code;
    if (segment == "constant") {  // Handling constant values
      asm_code = "@" + to_string(offset) + "\n" +
                 "D=A\n";  // load constant value into D register
    } else if (segment == "pointer") {  // Handling pointer values
      int newoffset = 3 + offset;
      asm_code = "@" + to_string(newoffset) + "\n" + "D=M\n";
    } else if (segment == "temp") {  // Handling temp values
      int newoffset = 5 + offset;
      asm_code = "@" + to_string(newoffset) + "\n" + "D=M\n";
    } else if (segment == "static") {  // Handling static values
      int newoffset = 16 + offset;
      asm_code = "@" + to_string(newoffset) + "\n" + "D=M\n";
    } else {
      // Handle other segments like local, argument, this, that, etc.
      string segment_base;
      if (segment == "local")
        segment_base = "LCL";
      else if (segment == "argument")
        segment_base = "ARG";
      else if (segment == "this")
        segment_base = "THIS";
      else if (segment == "that")
        segment_base = "THAT";
      asm_code = "@" + to_string(offset) + "\n" + "D=A\n" + "@" + segment_base +
                 "\n" + "A=D+M\n" + "D=M\n";  // load value into D register
    }
    asm_code += "@SP\n" + string("AM=M+1\n") + "A=A-1\n" + "M=D\n";  // push value onto stack
    return asm_code;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset) {
    string asm_code, segment_base;
    if (segment == "local")
      segment_base = "LCL";
    else if (segment == "argument")
      segment_base = "ARG";
    // ... handle other segments

    asm_code = "@" + segment_base + "\n" + "D=M\n" + "@" + to_string(offset) +
               "\n" + "D=D+A\n" +      // compute the effective address
               "@R13\n" + "M=D\n" +    // store the effective address in R13
               "@SP\n" + "AM=M-1\n" +  // decrement stack pointer and load top
                                       // of stack into D register
               "D=M\n" + "@R13\n" + "A=M\n" +
               "M=D\n";  // store D register value at the effective address
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
string VMTranslator::vm_label(string label) { return "(" + label + ")\n"; }

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label){
    return "@" + label + "\n" + "0;JMP\n";
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label){
    return "@SP\n" + string("AM=M-1\n") + "D=M\n" + "@" + label + "\n" + "D;JNE\n";
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars){
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

        "@SP\n" + "D=M\n" + "@5\n" + "D=D-A\n" + "@" + to_string(numArgs) +
        "\n" + "D=D-A\n" + "@ARG\n" + "M=D\n" +  // reposition ARG

        "@SP\n" + "D=M\n" + "@LCL\n" + "M=D\n" +  // reposition LCL

        "@" + function_name + "\n" + "0;JMP\n" +  // goto f

        "(" + returnLabel + ")\n";  // (return-label)
    return asm_code;
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() {
    string asm_code = "@LCL\n" + string("D=M\n") + "@R13\n" + "M=D\n" +  // FRAME = LCL

                      "@5\n" + "A=D-A\n" + "D=M\n" + "@R14\n" +
                      "M=D\n" +  // RET = *(FRAME-5)

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
