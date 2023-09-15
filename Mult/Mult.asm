// This file is based on part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: Mult.asm

// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Initialize R0 to 0
@R0
M=0

// Check if R1 eq to 0, if so, jump to END
@R1
D=M
@ZR
D;JEQ

// Check if R1 is negative, if so, negate it
@R4
M=0
@R1_NEG
D;JLT
@CHECK_R2_NEG
0;JMP

(R1_NEG)
@R1
M=-M
@R4
M=1

// Check if R2 is negative, if so, negate it
(CHECK_R2_NEG)
@R2
D=M
@ZR
D;JEQ
@R2_NEG
D;JLT
@Check_swap
0;JMP

(R2_NEG)
@R2
M=-M
@R4
D=M
M=D-1
M=-M

(Check_swap)
@R2
D=M
@R1
D=M-D
@SWAP
D;JLT
@MULTIPLY_LOOP
0;JMP

(SWAP)
@R2
D=M
@R3
M=D
@R1
D=M
@R2
M=D
@R3
D=M
@R1
M=D

// Start of the multiplication loop
(MULTIPLY_LOOP)
    // Add abs(R1) to R0
    @R1
    D=M
    @R0
    M=D+M

    // Decrease R2 by 1
    @R2
    M=M-1

    // Load the updated value of R2 into D
    D=M

    // If D is zero, jump to END
    @END
    D;JEQ

    // Otherwise, jump back to the start of the loop
    @MULTIPLY_LOOP
    0;JMP

// End label
(END)
    // Check if we need to negate the result (based on the value in R4)
    @R4
    D=M
    @END_PROGRAM
    D;JEQ
    @NEGATE_RESULT
    0;JMP

(NEGATE_RESULT)
    @R0
    D=M
    M=-D
(ZR)
    // End of program
(END_PROGRAM)
    @END_PROGRAM
    0;JMP