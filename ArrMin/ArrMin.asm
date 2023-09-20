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

// Initialize R0 to the largest possible positive value
@32767
D=A
@R0
M=D

// Initialize counter (R2) to the length of the array
@R2
D=M
@COUNTER
M=D

// Initialize the address register (A) to point to the first element of the array
@R1
D=M
@ARRAY_BASE
M=D

(LOOP_START)
// Check if we have processed all elements (if COUNTER=0, jump to END)
@COUNTER
D=M
@END
D;JEQ

// Load the value of the current element into D
@ARRAY_BASE
A=M
D=M

// Check if the current element is negative
@NEGATIVE_CHECK
D;JLT

(POSITIVE_CHECK)
// Compare the current element with the minimum value stored in R0 (for positive numbers)
@R0
D=M-D
@UPDATE_MIN
D;JGT

// No update needed, jump back to the start of the loop
@DECREMENT_COUNTER
0;JMP

(NEGATIVE_CHECK)
// Handle negative numbers separately
@32767
D=!A
@ARRAY_BASE
A=M
D=M-D
@MIN
D;JEQ
@ARRAY_BASE
A=M
D=M
@R0
D=D-M
@NEGATIVE_UPDATE
D;JLT
@R0
D=M
@32767
D=D-A
@NEGATIVE_UPDATE
D;JLE
@DECREMENT_COUNTER
0;JMP


// Jump back to the positive check if the value in R0 is positive
@POSITIVE_CHECK
0;JMP

(NEGATIVE_UPDATE)
// Update the minimum value in R0 for negative numbers
@ARRAY_BASE
A=M
D=M
@R0
M=D
@DECREMENT_COUNTER
0;JMP

(UPDATE_MIN)
// Update the minimum value in R0 for positive numbers
@ARRAY_BASE
A=M
D=M
@R0
M=D

(DECREMENT_COUNTER)
// Decrement the counter
@COUNTER
M=M-1

// Increment the array base address to point to the next element
@ARRAY_BASE
M=M+1

// Jump back to the start of the loop
@LOOP_START
0;JMP

(MIN)
@32767
D=!A
@R0
M=D
// Jump to the end of the program
@END
0;JMP

(END)
// End of the program
@END
0;JMP
