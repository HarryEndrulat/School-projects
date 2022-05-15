/*
 * STUDENT: Harry Endrulat
 */

/*
 * IntegerLab (c) 2018-21 Christopher A. Bohn
 */

/******************************************************************************
 * In this lab, students are required to implement integer arithmetic for
 * unsigned 16-bit integers and signed (twos complement) 16-bit integers using
 * only bitwise and (&), bitwise or (|), bitwise xor (^), bitwise complement
 * (~), and bit shifts (<< and >>). DO NOT USE built-in addition (+),
 * subtraction (-), multiplication (*), division (/), nor modulo (%).
 ******************************************************************************/

#include "alu.h"

/* Adds the two arguments and stores the sum in the return structure's result
 * field.  If the operation overflowed then the overflow flag is set. */
addition_subtraction_result add(uint16_t augend, uint16_t addend) {
	//TODO: consider just calling the 32 bit adder
	addition_subtraction_result addition;
	uint16_t temp = 0;
	uint16_t augendCopy = augend;
	uint16_t addendCopy = addend;
	uint16_t comp1 = 0;
	uint16_t comp2 = 0;
	uint16_t compResult = 0;
	addition.overflow = false;

	while (addendCopy!=0)
	{
		temp = augendCopy&addendCopy;
		augendCopy = augendCopy^addendCopy;
		addendCopy = temp<<1;
	}
	if(is_signed)
	{
		comp1 = augend >> 15;
		comp2 = addend >> 15;
		compResult = augendCopy >> 15;
		if(comp1==comp2 && (compResult!=comp1))
		{
			addition.overflow = true;
		}
		//>>15 times augend and addend if the MSB is the same overflow is possible 
		//>>15 times result if MSB is different from augend and addend, overflow!
	}
	else if(augendCopy<augend)
	{
		addition.overflow = true;
	}

    addition.result = augendCopy;                          
    return addition;
}

thirty_two_add_sub_result thirty_two_bit_adder(uint32_t augend, uint32_t addend) {
	thirty_two_add_sub_result result;
	uint32_t augendCopy = augend;
	uint32_t addendCopy = addend;
	uint32_t temp = 0;
	uint32_t comp1 = 0;
	uint32_t comp2 = 0;
	uint32_t compResult = 0;
	result.overflow = false;

	while (addendCopy != 0)
	{
		temp = augendCopy & addendCopy;
		augendCopy = augendCopy ^ addendCopy;
		addendCopy = temp << 1;
	}
	if (is_signed)
	{
		comp1 = augend >> 31;
		comp2 = addend >> 31;
		compResult = augendCopy >> 31;
		if (comp1 == comp2 && (compResult != comp1))
		{
			result.overflow = true;
		}
	}
	else if (augendCopy < augend)
	{
		result.overflow = true;
	}
	result.result = augendCopy;
	return result;
}

/* Subtracts the second argument from the first, stores the difference in the
 * return structure's result field.  If the operation overflowed then the
 * overflow flag is set. */
addition_subtraction_result subtract(uint16_t menuend, uint16_t subtrahend) {
	//subtraction is just adding the bitwise compliment plus 1
    int16_t augend = (int16_t) menuend;
    int16_t addend = (int16_t) add(~subtrahend, 1).result;
	addition_subtraction_result result = add(augend, addend);
	result.overflow = false;
	//TODO: can this block look cleaner?
	if (is_signed)
	{
		if ((int16_t)result.result>augend)
		{
			result.overflow = true;
		}
		else 
		{
			result.overflow = false;
		}
	}
	else
	{
		if (result.result>menuend)
		{
			result.overflow = true;
		}
		else
		{
			result.overflow = false;
		}
	}
    return result;
}

/* Multiplies the two arguments.  The function stores lowest 16 bits of the
 * product in the return structure's product field and the full 32-bit product
 * in the full_product field.  If the product doesn't fit in the 16-bit
 * product field then the overflow flag is set. */
multiplication_result multiply(uint16_t multiplicand, uint16_t multiplier) {
    multiplication_result multiplication;
	//all ints are a sum of powers of 2 eg: 30 is 2^4+2^3+2^2+2^1
	multiplication.overflow = false;
    if (multiplier==1)
    {
		multiplication.product = multiplicand;
		multiplication.full_product = multiplicand;
		return multiplication;
    }
    if (multiplier == 0)
    {
		multiplication.product = 0;
		multiplication.full_product = 0;
		return multiplication;
    }
	uint32_t multiplicandCopy = multiplicand;
	uint32_t multiplierCopy = multiplier;
	uint32_t result = 0;

    while (multiplierCopy!=0)
    {
	    if(multiplierCopy&1)//as in the book this accopmlishes the same as a bit mask of the multiplier AND checks if LSB is 1
	    {
			result = thirty_two_bit_adder(result,multiplicandCopy).result;
	    }
		multiplicandCopy = multiplicandCopy << 1;//again, left shifting multiplicand and right shifting multiplier as demonstrated in chapter 3
		multiplierCopy = multiplierCopy >> 1;
    }
	multiplication.product = result;
	multiplication.full_product = result;

	if(multiplication.full_product>65535)//because the maximum unsigned int value is known this works. Could also call UINT_MAX
	{
		multiplication.overflow = true;
	}

    return multiplication;
}

/* Divides the first argument by the second.  The function stores the quotient
 * in the return structure's quotient field and the the remainder in the
 * remainder field.  If the divisor is zero then the quotient and remainder
 * fields should be ignored, and the division_by_zero field should be set. */
division_result divide(uint16_t dividend, uint16_t divisor) {
    division_result division;
	//dividend/divisor
	division.division_by_zero = false;
	int n = 512;
	int partial = 0;//z
	uint16_t dividendCopy/*x*/ = dividend;//14
	uint16_t divisorCopy/*y*/ = divisor;//3

    if (divisor == 0)
    {
		division.division_by_zero = true;
		division.quotient = 42;
		division.remainder = 42;
		return division;
    }
    if (dividend == 0)
    {
		division.quotient = 0;
		division.remainder = 0;
		return division;
    }
	//15/2=7.5 15=1111 >> 1 = 111.1
	divisorCopy = divisorCopy<<8;
	while(n>1)//this should be n+1 times
	{
		partial = partial << 1;
		if (dividendCopy>=divisorCopy)
		{
			dividendCopy = subtract(dividendCopy, divisorCopy).result;
			partial = add(partial, 1).result;
		}
		divisorCopy = divisorCopy >> 1;
		n = n >> 1;
		//need to capture quotient and remainder somehow
		//all division returns 0 TODO:find out why and fix
	}
	division.quotient = partial;
	division.remainder = dividendCopy;//these should be the quotient and remainder respectfully
    return division;
}
