// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Load the address of the first element of the array into A and the value into D
@R1
D=M
A=D
D=M

// Load the value of the first element into R0 (initialize the minimum value)
@R0
M=D

// Load R2 (the length of the array) into D
@R2
D=M

// Initialize a loop counter to 1 (as we have already processed the first element)
@R4
M=1

// Begin loop (label: LOOP_START)
(LOOP_START)

// Check if we have processed all elements (if D=0, jump to END)
@R4
D=M
@R2
D=D-M
@END
D;JEQ

// Load the value of the current element into D
@R1
A=M
D=M

// Store the current element value in R5
@R5
M=D

// Load the current minimum value into D
@R0
D=M

// Compare the current element with the current minimum value
// If the current element is less than the current minimum value, update the minimum value
@R5
D=D-M
@UPDATE_MIN
D;JGT

// Jump to INCREMENT_INDEX
@INCREMENT_INDEX
0;JMP

(UPDATE_MIN)
// Load the value of the current element from R5 into D
@R5
D=M

// Update the minimum value in R0
@R0
M=D

// Label: INCREMENT_INDEX
(INCREMENT_INDEX)
// Increment the address of A register
@R1
M=M+1

// Increment the loop counter
@R4
M=M+1

// Jump back to the start of the loop
@LOOP_START
0;JMP

// Label: END
(END)
    @END
    0;JMP

