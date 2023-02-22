/*

	how can we incorporate project1 to convert the assembly input into hexcode for storage in
	memory? Is there a way we can exploit ./asm < input.txt > output.txt so that we don't
	have to repurpose that code or rewrite it (although a slight rewrite would be more
	efficient so that we do not "print" but rather just store directly into memory)
		ANSWER: input is in hex so there is no need for this. We get the binary
		data directly.


	the instructions state that the memory should be dumped with memory addresses 0x0000 - 0x000F
	all in one line. But the prof's output only has 8 bytes per line. I thought each memory
	address pointed to a byte? How are memory addresses being counted?

	i am confused on what is the best approach for storing the information in memory.using
	one "element" per each 2 digits of hex code sounds like it will complicate proccessing
	but having one "element" per digit also sounds kind of bad in it's own right.


	how would we process the 3 hex digit constant? if it is 0xA?? or greater it must be
	negative but I think if we just store it in an int it won't actually treat it as
	negative?? would we need to convert that value manually?
	
	

	how to read in hex digits
	
	its not any faster to do bit mask and shifting over div and remainder 
		the div and remainder will just be converted into shift and bitwise & 
		bitwise operations can be easier to understand the intention then div and 
		remainder once your 
	
		
	how to read a 3 hex digit constant in a 4 hex digit instruction? 
		this is a good chunk of the project
		think about the bit shifts 
		get down and dirty with bitwise operators
	
	we increment PC after instruction is completed. When you get a branch instruction the 
	the branch will account for the increment 
	
	
	WARNING: The following structure was explained with bugs on purpose so maybe dont 
	follow this structure but take it with a grain of salt 
		fetch function can be used to fetch the current instruction (while incrementing
		 PC)
		
		decode function breaks up the instruction into its tokens 
		
		execute function executes the instruction 
		
		writeback write any memory that needs to be written 
	
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


#define NUM_REGISTERS 16
#define NUM_MEMORY 2048

void printDump();
void loadInstructions();


//declare registers as global
int16_t registers[NUM_REGISTERS] = {0}; //this should initialize all registers to 0

//declare memory as global
uint8_t memory[NUM_MEMORY] = {0}; //this should initialize all registers to 0

//declare PC
short pc = 0;

int main() {

	loadInstructions();

	//holds 1 or 0
	int zeroFlag = 0;
	while(pc < NUM_MEMORY) {
		//obtain 16 bit instruction
		int instruction = memory[pc] * pow(16, 2) + memory[pc + 1];

		//get opcode
		int opcode = (instruction / (int) pow(16, 3)) % 16;

		//procces ADD operation
		if(opcode == 0) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			registers[dest] = registers[operand1] + registers[operand2];
		}
		//procces SUB operation
		else if(opcode == 1) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			registers[dest] = registers[operand1] - registers[operand2];
		}
		//procces AND  operation
		else if(opcode == 2) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			registers[dest] = registers[operand1] & registers[operand2];
		}
		//procces ORR  operation
		else if(opcode == 3) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			registers[dest] = registers[operand1] | registers[operand2];
		}
		//procces EOR  operation
		else if(opcode == 4) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			registers[dest] = registers[operand1] ^ registers[operand2];
		}
		//procces LSL  operation
		else if(opcode == 5) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			registers[dest] = registers[operand1] << registers[operand2];
		}

		//procces LSR  operation
		else if(opcode == 6) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			//cast to uint16_t so that the value being shifted is considered unsigned meaning that shift will occur logically. Then shift the desired amount and let the compiler recast
			//to a (signed) int16_t
			registers[dest] = ((uint16_t) registers[operand1] >> registers[operand2]);


		}
		//procces ASR  operation
		else if(opcode == 7) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int operand1 = (instruction / 16) % 16;
			int operand2 = instruction % 16;

			// no special processing needed b/c right shift will be performed arithetically b/c registers are declared as signed
			registers[dest] = registers[operand1] >> registers[operand2];
		}
		//process LDR
		else if(opcode == 8) {

			int dest = (instruction / (int)pow(16,2)) % 16;
			int memRegister = (instruction / 16) % 16;		//specifies the register containing the memory address to be loaded

			/*
				For the LDR and STR operations, how many bytes of memory do we load into the destination register? Or do we just load a single byte of memory?
					Answer: load in a word [2 bytes]
			*/

			//get the memory address stored in the register "memRegister"
			int16_t memAddress = registers[memRegister];

			//store the value in memory[memAddress] through memory[Address+1] in the destination register
			registers[dest] = (memory[memAddress] * pow(16, 2) ) + memory[memAddress + 1]; //converts two seperate consecutive bytes into one word

		}
		//process STR
		else if(opcode == 9) {

			int sourceReg = (instruction / (int)pow(16,2)) % 16; //specifies the register holding our source data
			int destReg = (instruction / 16) % 16;		//specifies the register containing the memory address where we will store our data


			int16_t memAddress = registers[destReg];

			memory[memAddress] = (registers[sourceReg] & 0xFF00) >> 8;	//store first byte in "starting" memAddress
			memory[memAddress + 1] = registers[sourceReg] & 0x00FF;	//store second byte in the address after the "starting" address (ie start address + 1)

		}

		//process CMP instruction
		else if(opcode == 10) {

			int reg1  = (instruction / (int)pow(16,2)) % 16;
			int reg2 = (instruction / 16) % 16;		//specifies the register containing the memory address to be loaded

			//assign 1 to zero flag (meaning it is on) if operand1 and operand2 are equal. Assign 0 otherwise.
			zeroFlag = (registers[reg1] == registers[reg2]);

		}
		//Process MOV operation
		else if(opcode == 11){
			int dest = (instruction / (int) pow(16,2)) % 16;

			int constant = instruction & 0xFF; 			//get last 2 hex digits of the instruction
			if(( (instruction / 16) % 16) >= 8)
				constant = constant | 0xFFFFFF00;

			registers[dest] = constant & 0xFFFF;

		}
		//B instruction
		else if(opcode == 12){

			//get constant
			int constant = instruction & 0xFFF;			 //get last 3 hex digits of the instruction
			if(( (instruction / (int) pow(16,2) ) % 16) >= 8 )	//if constant should be interpreted as negative (first hex digit of the 3 is 8 or greater meaning there is a 1 in the sign bit
				constant = constant | 0xFFFFF000;


			pc += constant;

			//break to prevent changing pc further
			continue;
		}
		//BEQ Instruction
		else if(opcode == 13) {

			//get constant
			int constant = instruction & 0xFFF;			 //get last 3 hex digits of the instruction
			if(( (instruction / (int) pow(16,2) ) % 16) >= 8 )	//if constant should be interpreted as negative (first hex digit of the 3 is 8 or greater meaning there is a 1 in the sign bit
				constant = constant | 0xFFFFF000;

			if(zeroFlag) {
				pc += constant;
				continue;
			}
		}
		//END instruction
		else if(opcode == 14) {
			break;
		}



		//Process NOP
			//do literally nothing


		//move PC to next instruction [two bytes fowards]
		pc += 2;

	}
	printDump();

}

void loadInstructions() {


	int count = 0;
	char buffer[10];
	while( (count < NUM_MEMORY) && ( (scanf("%2s", buffer) != EOF ) )) {
		memory[count] = (uint8_t)strtol(buffer, NULL, 16);
		count++;
	}


	//terminate if we ran out of memory
	if(count >= NUM_MEMORY) {
		printf("ERROR: out of memory");
		exit(0);
	}
}


void printDump() {

	//print 16 registers
	for(int count = 0; count < NUM_REGISTERS; count++) {

		printf("register %3d: 0x%.4X \n", count, registers[count] & 0xFFFF);
	}

	//print PC and extra new line to seperate memory dump
	printf("register  PC: 0x%.4X \n\n", pc);


	//print memory
	for(int count = 0; count < NUM_MEMORY; count++) {
		if((count % 16) == 0) {
			//print new line and next lines memory address "header"
			printf("\n0x%.4X:  ", count);
		}

		printf("%.2X", memory[count]);
		if( ((count % 2) == 1))
			printf(" ");
	}

}
