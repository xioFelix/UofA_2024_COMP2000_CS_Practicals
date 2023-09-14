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

// Load R1 into D register
@R1
D=M
@ZR
D;JEQ

// Check if R1 is negative, if so, negate it and store the negated value in R3, also set R4 to 1 to indicate a sign change
@R4
M=0
@R3
M=D
@R1_NEG
D;JLT
@CHECK_R2_NEG
0;JMP

(R1_NEG)
@R3
M=-M
@R4
M=1

// Check if R2 is negative, if so, negate it and toggle R4 to indicate a sign change
(CHECK_R2_NEG)
@R2
D=M
@ZR
D;JEQ
@R2_NEG
D;JLT
@MULTIPLY_LOOP
0;JMP

(R2_NEG)
@R2
M=-M
@R4
D=M
M=D-1
M=-M

// Start of the multiplication loop
(MULTIPLY_LOOP)
    // Add R3 (which holds the value of R1) to R0
    @R3
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
(END_PROGRAM)
    @END_PROGRAM
    0;JMP