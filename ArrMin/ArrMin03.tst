// Sample Test file for ArrMin.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"

load ArrMin.asm,
output-file ArrMin03.out,
compare-to ArrMin03.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[60]%D2.6.2 RAM[61]%D2.6.2 RAM[62]%D2.6.2 RAM[63]%D2.6.2 RAM[64]%D2.6.2 RAM[65]%D2.6.2 RAM[66]%D2.6.2 RAM[67]%D2.6.2;

set PC 0,
set RAM[0]  0,  // Set R0
set RAM[1]  60, // Set R1
set RAM[2]  8,  // Set R2
set RAM[60] 32767,  // Set Arr[0]
set RAM[61] 32760,  // Set Arr[1]
set RAM[62] 32761,  // Set Arr[2]
set RAM[63] -32768;  // Set Arr[3]
set RAM[64] 32763,  // Set Arr[4]
set RAM[65] 32764,  // Set Arr[5]
set RAM[66] 32765,  // Set Arr[6]
set RAM[67] 32766;  // Set Arr[7]
repeat 200 {
  ticktock;    // Run for 200 clock cycles
}
set RAM[1] 60,  // Restore arguments in case program used them
set RAM[2] 8,
output;        // Output to file

