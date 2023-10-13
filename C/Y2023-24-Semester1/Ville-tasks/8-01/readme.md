Write a function that shifts bits to right and masks out unwanted bits. The first parameter of the function is the number to process, the second parameter is number shifts to right, and the last parameter is the number of bits to keep starting from the least significant bit (the rightmost bit). The function returns the result of shift-and-mask operation.

To keep certain number of bits you need to create a mask where the bits that you want to keep are set to one and the rest of the bits are set to zero. Then perform bitwise and between the value and mask. For example to keep four bits from the right the mask is 0xF.