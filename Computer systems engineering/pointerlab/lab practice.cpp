// lab practice.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>

 int* add(int a, int b) {
	int result = a + b;
	return &result;
	
}

 int* multiply(int p, int q) {
	 int result[1];
	 result[0] = p * q;
	 return result;
	
}

 int main() {
	 int* sum = add(4, 5);
	 printf(" sum = %d \n ", *sum);
	 int* product = multiply(2, 3);
	 printf(" product = %d \n ", *product);
	 printf(" sum - product = %d \n ", *sum - *product);
	 return 0;
	
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
