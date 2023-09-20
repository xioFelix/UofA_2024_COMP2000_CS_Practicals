// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Load the address of the first element of the array into A and the value into D

//Edge
@2
D=A
@R1
D=M-D
@END
D;JLE

@R2
D=M
@END
D;JLE

@R1
A=M
D=M

// Load the value of the first element into R0 (initialize the value)
@32767
D=A
@R0
M=D

// @32767
// D=D-A
// @MAX
// D;JEQ
@R2
D=M-1
@END
D;JLE

@32767
D=!A
@R0
D=D-M
@MIN
D;JEQ

@32767
D=A
@R0
D=M-D
@MAX
D;JEQ

// Begin loop (label: LOOP_START)
(LOOP_START)
// Update the counter
@R2
M=M-1

// Load R2 (the length of the array) into D
@R2
D=M

// Check if we have processed all elements (if D=0, jump to END)
@END
D;JLT

@32767
D=!A
@TEMP
M=D
// Load R2 (the length of the array) into D
@R2
D=M
@R1
A=M+D
D=M
@TEMP
D=D-M
@MIN
D;JEQ

// Load R2 (the length of the array) into D
@R2
D=M
// Load the value of the current element into D
@R1
A=M+D
D=M

@32767
D=D-A
@MAX
D;JEQ

@R2
D=M

// Load the value of the current element into D
@R1
A=M+D
D=M

// Store the final result in R0
@R0
D=M-D
@UPDATE_MIN
D;JGT

// @TEMP
// M=D
// @32767
// D=!A
// @TEMP
// D=D-M
// @UPDATE_MIN
// D;JGE

// Jump back to the start of the loop
@LOOP_START
0;JMP

(MAX)
// @R2
// D=M-1
// @R1
// A=M+D
// D=M
// @R0
// M=D
@LOOP_START
0;JMP

(MIN)
@32767
D=!A
@R0
M=D
@END
0;JMP

(UPDATE_MIN)
@R2
D=M
@R1
A=M+D
D=M
@R0
M=D

// Jump back to the start of the loop
@LOOP_START
0;JMP

// Label: END
(END)
    @END
    0;JMP

