// This file is based on part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: Mult.asm

// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// Initializes R0 to 0
@R0
M=0

// Loads R1 to D register
@R1
D=M

// Loads the address of R2 to A register
@R2
A=M

// Multiplies R1 and R2 (D and A) using a loop and stores the result in R0
(MULTIPLY_LOOP)
@R0
D=M+D  // Adds the content of D to R0, this essentially multiplies R1 and R2 by repeated addition
@R2
M=M-1  // Decreases R2 by 1
@MULTIPLY_LOOP
D;JGT  // If D (which now holds the content of R0) is greater than 0, jump to MULTIPLY_LOOP

(END)
