// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Load the address of the first element of the array into A and the value into D
@1
D=M

// Load the value of the first element into R0 (initialize the minimum value)
@0
M=D

// Set A to R1 (the address of the first element in the array)
@1
A=M

// Store the address of the current element in R3
@3
M=A

// Load R2 (the length of the array) into D
@2
D=M

// Store the length of the array in R4
@4
M=D

// Begin loop (label: LOOP_START)
(LOOP_START)

// Check if we have processed all elements (if D=0, jump to END)
@4
D=M
@END
D;JEQ

// Load the address of the current element into A
@3
A=M

// Load the value of the current element into D
D=M

// Load the current minimum value into A
@0
A=M

// Compare the current element with the current minimum value
// If the current element is less than the current minimum value, update the minimum value
D=A-D
@UPDATE_MIN
D;JLT

// Jump to INCREMENT_INDEX
@INCREMENT_INDEX
0;JMP

(UPDATE_MIN)
// Load the address of the current element into A
@3
A=M

// Load the value of the current element into D
D=M

// Update the minimum value in R0
@0
M=D

// Label: INCREMENT_INDEX
(INCREMENT_INDEX)
// Increment the address of the current element (stored in R3)
@3
M=M+1

// Decrement the length of the array (stored in R4)
@4
M=M-1

// Jump back to the start of the loop
@LOOP_START
0;JMP

// Label: END
(END)
