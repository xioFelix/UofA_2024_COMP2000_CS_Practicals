// Sample Test file for ArrMin.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"

load ArrMin.asm,
output-file ArrMin06.out,
compare-to ArrMin06.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2 RAM[4]%D2.6.2 RAM[5]%D2.6.2 RAM[6]%D2.6.2;

set PC 0,
set RAM[0]  0,  // Set R0
set RAM[1]  3, // Set R1
set RAM[2]  4,  // Set R2
set RAM[3] -1,  // Set Arr[0]
set RAM[4] -2,  // Set Arr[1]
set RAM[5] -3,  // Set Arr[2]
set RAM[6] -4;  // Set Arr[3]
repeat 300 {
  ticktock;    // Run for 300 clock cycles
}
set RAM[1] 3,  // Restore arguments in case program used them
set RAM[2] 4,
output;        // Output to file

