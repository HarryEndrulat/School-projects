/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
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

FLOATING POINT CODING RULES

You are allowed to use looping and conditional control.  You are allowed to use
both ints and unsigneds. You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros. (You may use #define to create named constants
     but no other macros.)
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned (you may also use uint32_t).
     This means that you cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc(data lab checker) compiler(described in the handout) to
	  check the legality of your solutions.
	  2. Each function has a maximum number of operators(!~& ^ | +<< >>)
	  that you are allowed to use for your implementation of the function.
	  The max operator count is checked by dlc.Note that '=' is not
	  counted; you may use as many of these as you want without penalty.
	  3. Use the btest test harness to check your functions for correctness.
	  4. Use the BDD checker to formally verify your functions
	  5. The maximum number of ops for each function is given in the
	  header comment for each function.If there are any inconsistencies
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
	   /* Copyright (C) 1991-2015 Free Software Foundation, Inc.
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
			 /* glibc's intent is to support the IEC 559 math functionality, real
				and complex.  If the GCC (4.9 and later) predefined macros
				specifying compiler intent are available, use them to determine
				whether the overall intent is to support these features; otherwise,
				presume an older compiler has intent to support these features and
				define these macros by default.  */
				/* wchar_t uses Unicode 7.0.0.  Version 7.0 of the Unicode Standard is
				   synchronized with ISO/IEC 10646:2012, plus Amendments 1 (published
				   on April, 2013) and 2 (not yet published as of February, 2015).
				   Additionally, it includes the accelerated publication of U+20BD
				   RUBLE SIGN.  Therefore Unicode 7.0.0 is between 10646:2012 and
				   10646:2014, and so we use the date ISO/IEC 10646:2012 Amd.1 was
				   published.  */
				   /* We do not support C11 <threads.h>.  */


#include <stdint.h>
#include <stdio.h>
#define signBitMask 0x80000000//10...
#define absoluteBitMask 0x7FFFFFFF//01...
#define fractionBitMask 0x007fffff//000000000111...
#define nanBitMask 0x7FC00000//011111111100...
#define exponentBitMask 0x7F800000//011111111000... also the definition of infinity
#define oneBitMask 0x00800000//000000001000...
#define overflowBitMask 0x40000000//0100... maybe unnecessary
#define bias 127
/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while, switch
 *   Max ops: 10
 *   Rating: 2
 */
uint32_t float_neg(uint32_t f) {
	  if ((f&nanBitMask)==nanBitMask)
	  {
		  return f;
	  }
 return f^signBitMask;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while, switch
 *   Max ops: 30
 *   Rating: 4
 */
  //int to float
  //all these should be whole numbers so the fraction is 0 
  //must round properly
  //cannot be infinite and is guaranteed to be a number.
uint32_t float_i2f(int i) {

  return 2;
}

/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while, switch
 *   Max ops: 30
 *   Rating: 4
 */
uint32_t float_twice(uint32_t f) {
	if ((f&nanBitMask) == nanBitMask)
	{
		return f;
	}
	uint32_t sign = f&signBitMask;
	uint32_t exponent = f&exponentBitMask;
	uint32_t fraction = f&fractionBitMask;
	uint32_t result = 0;
	if ((exponent&exponentBitMask) == exponentBitMask)//do i need to worry about negative infinity?
	{
		return f;
	}
	if((exponent==0)&&(fraction==0))
	{
		return f;
	}
	if ((exponent==0)&&(fraction>0))
	{
		fraction <<= 1;
		result = (sign | exponent) | fraction;
		return result;
	}
	exponent >>= 23;
	exponent = exponent + 1;
	exponent <<= 23;//could i use 0x008 to reduce operation count?
	result = (sign | exponent) | fraction;
	//shift until the exponent is the bottom 8 bits
	//add 1, left shift back
	//subnormal -- left shift fraction 1

  return result;
}

/*
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while, switch
 *   Max ops: 10
 *   Rating: 2
 */
uint32_t float_abs(uint32_t f) {
	if ((f&nanBitMask) == nanBitMask)
	{
		return f;
	}
  return absoluteBitMask&f;
}

/*
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while, switch
 *   Max ops: 30
 *   Rating: 4
 */
//the float that’s being cast to int can be infinite and can be NaN.
//see photo on phone for explanation
int float_f2i(uint32_t f) {
	if ((f&nanBitMask) == nanBitMask)
	{
		return 0x80000000u;
	}
	uint32_t sign = f & signBitMask;
	uint32_t exponent = f >> 23;
	exponent = exponent & 0xFF;//does this preserve the sign bit?
	uint32_t fraction = f&fractionBitMask;//shift a 1 left a number of times equal to the exponent
	int result = 0;
	if ((exponent&0xFF) == 0xFF)//i've already done part of this above
	{
		return 0x80000000u;
	}
	exponent = exponent - bias;//when f=0x3F800001 exponent = 0
	//subnormal numbers return 0
	//printf("%x\n", exponent);
	if (exponent & signBitMask)
	{
		return 0;
	}
	if (exponent>31)
	{
		return 0x80000000u;
	}

	/*if (exponent>=23)
	{
		fraction <<= (exponent - 23);//this results in an infinite loop on the tests
	}
	else if (exponent<23)
	{
		fraction >>= (23 - exponent);
	}*/

	if (exponent==0&&(fraction < 0x00400000))//this also results in an infinite loop
	{
		//round up to the next whole number
		//printf("here\n");
		//printf("sign: %x\n", sign);
		if (sign)
		{
			return -1;
		}
		return 1;
	}
	/*else
	{
		//round down or truncate
		fraction = 0;
	}*/
	result = fraction;//here fraction is a uint32_t. make it a decimal. when interpreting 0x000001 returns 1 not .000...1
	result = result + (1 << exponent);


	if (sign > 0)
	{
		result = result * -1;
	}
  return result;
}