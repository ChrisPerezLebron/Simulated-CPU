#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>



int main() {


	uint8_t memory[4] = {0xD3, 0xA1};



	int16_t result = ( memory[0] * pow(16, 2) ) + memory[1];



	printf("%x", result & 0xFFFF);
















/*
	char buffer[10];

	scanf("%2s", buffer);
	unsigned int x = strtol(buffer, NULL, 16);

	printf("%X\n", x);

	unsigned char xChar = (unsigned char) x;

	printf("%X\n", xChar);
*/













/*
	int instruction = 0xFB05;

	int dest = (instruction / (int) pow(16,2)) % 16;
	int operand1 = (instruction / 16) % 16;
	int operand2 = instruction % 16;

	printf("dest: %X\n", dest);
	printf("op1: %X\n", operand1);
	printf("op2: %X\n", operand2);

*/



/*

	int instruction = 0x5695;

	int constant = instruction & 0xFF;
	if(( (instruction / 16) % 16 ) >= 8)
		constant = constant | 0xFFFFFF00;

	printf("%d", constant);


	printf("size of int16_t%ld", sizeof(int16_t));

*/



/*

	int16_t num = 0x8120; // 1000 0001 0010 0000

	int16_t shift = 2;

	int16_t result = ((uint16_t) num >> shift);

	printf("result: %x", result & 0xFFFF);

*/


}
