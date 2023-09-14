// Calculates the absolute value of R1 and stores the result in R0.
// (R0, R1 refer to RAM[0], and RAM[1], respectively.)

// Put your code here.

    @R1          // Set A-register to 1 to access RAM[1] (R1)
    D=M        // Load the value from RAM[1] to D-register

    @POSITIVE  // Jump to POSITIVE label if D >= 0 (D is the value from RAM[1])
    D;JGE

    @NEGATIVE  // If D < 0, we go to NEGATIVE label
    0;JMP

(POSITIVE)
    @R1
    D=M        // Load the value from RAM[1] to D-register again
    @R0
    M=D        // Store the value from D-register to RAM[0] (R0)
    @END
    0;JMP      // Jump to the END label to terminate the program

(NEGATIVE)
    @R1
    D=M        // Load the value from RAM[1] to D-register again
    D=-D       // Calculate the negative of D (which gives the absolute value if D was negative)
    @R0
    M=D        // Store the absolute value from D-register to RAM[0] (R0)

(END)
    @END
    0;JMP
