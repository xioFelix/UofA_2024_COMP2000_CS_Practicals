// Sample Test file for ArrSort.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"

load ArrSort.asm,
output-file ArrSort00.out,
compare-to ArrSort00.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[20]%D2.6.2 RAM[21]%D2.6.2 RAM[22]%D2.6.2 RAM[23]%D2.6.2;

set PC 0,
set RAM[0]  0,  // Set R0
set RAM[1]  40, // Set R1
set RAM[2]  4,  // Set R2
set RAM[40] 4,  // Set Arr[0]
set RAM[41] 8,  // Set Arr[1]
set RAM[42] 6,  // Set Arr[2]
set RAM[43] 2;  // Set Arr[3]
repeat 600 {
  ticktock;    // Run for 600 clock cycles
}
set RAM[1] 40,  // Restore arguments in case program used them
set RAM[2] 4,
output;        // Output to file

