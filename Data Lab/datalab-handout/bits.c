/*
 * CS:APP Data Lab
 *
 * Annie Kelly
 * SID:101604361
 * Moodle name: shadowfax6894
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */
  
#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */
  
You will provide your solution to the Data Lab by
editing the collection of functions in this source file.
  
INTEGER CODING RULES:
   
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:
   
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;
  
      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }
  
  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
      
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.
  
  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.
  
   
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.
  
EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }
  
  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }
  
FLOATING POINT CODING RULES
  
For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.
  
You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.
  
  
NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function.
     The max operator count is checked by dlc. Note that '=' is not
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
  
/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */
  
  
#endif
/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
  
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
  
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* Define __STDC_IEC_559__ and other similar macros.  */
/* Copyright (C) 2005 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
  
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
  
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    int z = ~(x & y) & (~(~x & ~y));
  return z;
  //lecture slide 27 was very helpful for this!
  //truth table
  //x     y      x^y
  //0     0       0
  //0     1       1
  //1     0       1
  //1     1       0
  //x and y must be different to return 1
}
/*
 * fitsShort - return 1 if x can be represented as a
 *   16-bit, two's complement integer.
 *   Examples: fitsShort(33000) = 0, fitsShort(-32768) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int fitsShort(int x) {
      
    
  int a, z, n;
  n = 16;
    a = 33 + ~n;
    z = !(((x << a)>>a)^x);
    return z;
    //recycled code from fits bits but declared n as 16
}
/*
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
    int z = 1; //now move 1 over 3 bits
    z = (z << 3) + z; //looks like 1001
    z = (z << 6) + z; //1001001001
    z = (z << 12) + z; //1001001001001001001001
    z = (z << 24) + z; //10010010010010| stops here because over 32 bits 01001001001001001001001001001001
      
  return z;
}
/*
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
    return  ( ~0 & ( !n + ( ~0 ) ) ) << ( 32 + ( ~n + 1 ) );
    // !n + ~0 = 0 if n is 0
    // essentially this is -1 being shifted over 32-n times to give the n upper bits padded as 1s and leaving the rest 0
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) { //fits if all values to the left of n-1 are the same as n-1

    
   int a, z;
    a = 33 + ~n; // ~n = -n-1 so a = 32 - n,
    z = !(((x << a)>>a)^x); //makes all high bits 0, and then checks if the new number is still == to x (which would return 0)
    return z;
}
/*
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
    return (!x)|y;
    //if x is true then y has to be true, only (1,0) makes this return 0
}
/*
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int leastBitPos(int x) {
  return x&(~x+1);
  //EXAMPLE: x=4
  //0100
  //1100
  //returns 0100
  //EXAMPLE: x=3
  //0011
  //1101
  //returns 0001
}
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
   int sign = 1 << 31;
   int ub = ~(sign | 0x39); //if an int above 0x39 is added the result will be neg
   int lb = ~0x30; //if an int below 0x30 is added result will also be neg
  
   //add x to upper bound and lower bound, if either becomes negative x is NOT an Ascii digit!
   ub = sign & (ub+x) >> 31;
   lb = sign & (lb+1+x) >> 31;
  
     
   return !(ub | lb); //if either of these is 1 that means it is not an ascii, so we ! it to return a 0 for false
  
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) { //multiplying by two just adds zero to the end
//overflow has occured if x changes signs
    int tmin = 1 << 31; //0x80000000
    int xplusx = x << 1; //multiply x by 2
    int sign = x >> 31; //gives sign bit
    int flo = (x ^ xplusx) >> 31; //if there used to be a 1 in the sign bit and it changed to 0 that means it changed from neg to pos. oflow.
    //flo is one if there is a sign change
    int sat = flo & (sign ^ ~tmin);
    return sat|(~flo & xplusx); //if no overflow then 2*x will be returned
      
}
/*
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3
 */
int rotateLeft(int x, int n) {
  int mask = x >> (32 + (~n + 1)); 
  int logic = ~((~0) << n); //when we right shifted the mask it padded it with 1s not 0s, which is bad
							//so this second mask starts with all ones and shifts left however many times you
							//need to rotate so that you can cancel out all the extra 1s
  int rot = mask & logic;
  x = x << n;
  return (x|rot);
  
}
/*
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  int a = x + (~y+1); // x - y
  int b = (x>>31) & 1; //tells the sign of x
  int c = (y>>31) & 1;//tells the sign of y
  int d = (a>>31) & 1; //gives the sign of x - y
  
  return !(((!b)&c&d) | (b & (!c) & (!d)));
  //neg - neg is ok
  //pos - pos is ok
  //pos - neg or neg - pos may cause overflow
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
  x = ( x >> 16 ) ^ x;
  x = ( x >> 8 ) ^ x;
  x = ( x >> 4 ) ^ x;
  x = ( x >> 2 ) ^ x;
  x = ( x >> 1) ^ x;
  return (x & 1);
  //after performing all of the shifts an even number would give 0&1=0 at the end, for odd it's 1&1=1
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
	//a power of 2 will have a 1 and then all 0s
    int sub1=~0;
    int sign =x>>31; // accounts for negative numbers
    int z = (sub1^sign);
  
  
    return !((x&(x+z))+!x);
}
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) { //not working :(
  int y = ~x;
  y = y | (y >> 16);
  y = y | (y >> 8); //causes all non consecutive 1s to disappear
  y = y | (y >> 4);
  y = y | (y >> 2);
  y = y | (y >> 1);
  y = ~y;
  return (y & 1) + (y >> 1 & 1) + (y >> 2 & 1) + (y >> 3 & 1) + (y >> 4 & 1) + (y >> 5 & 1) + (y >> 6 & 1) + //counts up every set bit
  (y >> 7 & 1) + (y >> 8 & 1) + (y >> 9 & 1) + (y >> 10 & 1) + (y >> 11 & 1) + (y >> 12 & 1) + (y >> 13 & 1) +
  (y >> 14 & 1) + (y >> 15 & 1) + (y >> 16 & 1) + (y >> 17 & 1) + (y >> 18 & 1) + (y >> 19 & 1) + (y >> 20 & 1) +
  (y >> 21 & 1) + (y >> 22 & 1) + (y >> 23 & 1) + (y >> 24 & 1) + (y >> 25 & 1) + (y >> 26 & 1) + (y >> 27 & 1) + (y >> 28 & 1) +
  (y >> 29 & 1) + (y >> 30 & 1) + (y >> 31 & 1);
}
/* Extra credit */
/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
 return 2;
}
/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  return 2;
}
/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  return 2;
}
