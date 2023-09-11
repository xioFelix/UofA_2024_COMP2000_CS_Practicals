// Sorts the array of length R2 whose first element is at RAM[R1] in ascending order in place. Sets R0 to True (-1) when complete.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
// Initialize R0 to 0 (false) to indicate that the sorting is not yet completed
@R0
M=0

// Load the address of the first element of the array into A register
@R1
D=M
@R3
M=D

// Load the length of the array into D register
@R2
D=M

// Initialize the outer loop counter to D register
@R4
M=D

// Label for the outer loop (OUTER_LOOP_START)
(OUTER_LOOP_START)

// Check if the outer loop counter is 0, if yes, jump to END
@R4
D=M
@END
D;JEQ

// Initialize the inner loop counter to D-1
@R4
D=M
@R5
M=D
M=M-1

// Label for the inner loop (INNER_LOOP_START)
(INNER_LOOP_START)

// Check if the inner loop counter is 0, if yes, jump to OUTER_LOOP_END
@R5
D=M
@OUTER_LOOP_END
D;JEQ

// Load the address of the current element into A register
@R3
A=M

// Load the value of the current element into D register
D=M

// Load the value of the next element into A register
@R3
A=M
A=A+1

// Subtract the value of the current element from the next element and store the result in D register
D=M-D

// If the result is negative, jump to SWAP
@SWAP
D;JLT

// Jump to NO_SWAP
@NO_SWAP
0;JMP

// Label for the swap logic (SWAP)
(SWAP)
// Swap the current and next elements
@R3
A=M
D=M
@R6
M=D
@R3
A=M
A=A+1
D=M
@R3
A=M
M=D
@R6
D=M
@R3
A=M
A=A+1
M=D

// Label for no swap logic (NO_SWAP)
(NO_SWAP)
// Increment the address of the current element
@R3
M=M+1

// Decrement the inner loop counter
@R5
M=M-1

// Jump back to the start of the inner loop
@INNER_LOOP_START
0;JMP

// Label for the end of the outer loop (OUTER_LOOP_END)
(OUTER_LOOP_END)
// Reset the address of the current element to the start of the array
@R1
D=M
@R3
M=D

// Decrement the outer loop counter
@R4
M=M-1

// Jump back to the start of the outer loop
@OUTER_LOOP_START
0;JMP

// Label for the end of the program (END)
(END)
// Set R0 to -1 (true) to indicate that the sorting is completed
@R0
M=-1
