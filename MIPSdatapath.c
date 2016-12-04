/* CDA 3103 
   Data Path Project
   Authors: Zach Muller
   			Danielle Evans
   			Meredith Pyrich
*/
#include "spimcore.h"
#include "limits.h"

#define MEMSIZE (65536 >> 2)


/* instruction fetch */
/* Author: Meredith Pyrich */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{	
	// If the address is not word aligned or if it is trying
    // to access an address beyond the memory, halt.
    if (((PC % 4) != 0) || (PC  > (MEMSIZE << 2))) {
    	printf("instructionFetch Halted\n");
        return 1;
    }
    
    *instruction = Mem[PC >> 2];
    // Return zero, for there was no halt.
    return 0;
}


/* instruction partition */
/* Author: Meredith Pyrich */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	// A copy of the 32-bit instruction to manipulate with shifting.
    unsigned instructionCopy = instruction;

    instructionCopy = instructionCopy >> 26;
    *op = instructionCopy;
    //printf("op = %u\n", *op);

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 27;
    *r1 = instructionCopy;
    //printf("r1 = %u\n", *r1);

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 11;
    instructionCopy = instructionCopy >> 27;
    *r2 = instructionCopy;
    //printf("r2 = %u\n", *r2);

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 27;
    *r3 = instructionCopy;
    //printf("r3 = %u\n", *r3);

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 26;
    instructionCopy = instructionCopy >> 26;
    *funct = instructionCopy;
    //printf("function %u\n", *funct);

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 16;
    instructionCopy = instructionCopy >> 16;
    *offset = instructionCopy;
    //printf("offset = %u\n", *offset);

    instructionCopy = instruction;
    instructionCopy = instructionCopy << 6;
    instructionCopy = instructionCopy >> 6;
    *jsec = instructionCopy;
    //printf("jsec = %u\n", *jsec);
}



/* instruction decode */
/* Author: Meredith Pyrich */
int instruction_decode(unsigned op,struct_controls *controls)
{	
	/* RegDest		0 -> use r2						1 -> use r3 
	   jump         0 -> don't Jump         		1 -> jump
	   branch       0 -> don't Branch       		1 -> branch 
	   MemRead		0 -> don't read from memory 	1 -> read from mem
	   MemtoReg		0 -> dont redirect mem to reg 	1 -> redirect mem to reg
	   ALUOp  		is a 3 - bit bus to ALU control
	   MemWrite		0 -> don't write to mem         1 -> write to mem
	   ALUSrc		0 -> use r2						1 -> sign extend
	   RegWrite 	0 -> don't write to reg         1 -> write to reg 
	*/

    // Set everything to 0 to default.
    controls->RegDst = '0';
    controls->Jump = '0';
    controls->ALUSrc = '0';
    controls->MemtoReg = '0';
    controls->RegWrite = '0';
    controls->MemRead = '0';
    controls->MemWrite = '0';
    controls->Branch = '0';
    controls->ALUOp = '0';

    switch(op) {

    	//R-type
    	case 0:
	    	controls->RegDst = '1';
	    	controls->Jump = '0';
	        controls->ALUSrc = '0';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '7';
    		break;

    	//jump
    	case 2:
    		controls->RegDst = '2';
    		controls->Jump = '1';
	        controls->ALUSrc = '0';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '0';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;

    	//beq
    	case 4:
	    	controls->RegDst = '2';
	    	controls->Jump = '0';
	        controls->ALUSrc = '0';
	        controls->MemtoReg = '2';
	        controls->RegWrite = '0';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '1';
	        controls->ALUOp = '1';
    		break;

    	//addi
    	case 8:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;

    	//slti
    	case 10:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '2';
    		break;

    	//sltui
    	case 11:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '3';
    		break;

    	//lui
    	case 15:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '0';
	        controls->RegWrite = '1';
	        controls->MemRead = '0';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '6';
    		break;

    	//lw
    	case 35:
    		controls->RegDst = '0';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '1';
	        controls->RegWrite = '1';
	        controls->MemRead = '1';
	        controls->MemWrite = '0';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;

    	//sw
    	case 43:
    		controls->RegDst = '2';
    		controls->Jump = '0';
	        controls->ALUSrc = '1';
	        controls->MemtoReg = '2';
	        controls->RegWrite = '0';
	        controls->MemRead = '0';
	        controls->MemWrite = '1';
	        controls->Branch = '0';
	        controls->ALUOp = '0';
    		break;
    }

    // Opcode doesn't equal any of the values here.
    // Illegal instruction error, halt.
    if(op != 0 && op != 2 && op != 4 && op != 8 && op != 10 && op != 11 && op != 15 && op != 35 && op != 43) {
        printf("instructionDecode Halted\n");
        return 1;
    }
    
    return 0;
}

/* Read Register */
/* Author: Danielle Evans */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* Author: Danielle Evans */
void sign_extend(unsigned offset,unsigned *extended_value)
{	
	int val = offset;
    int sign = 0;
   
    if((val >> 15) == 1)
    {
        sign = 1;
    }

    if(sign == 1)
    {
        *extended_value = val | 0xffff0000;
    }

    else{
        *extended_value = val | 0x00000000;
    }
}

/* ALU operations */
/* Author: Zach Muller */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{	
	/* check for HALT condition */
	//An illegal instruction appears
	if((ALUOp != '0') && (ALUOp != '1') && (ALUOp != '2') && (ALUOp != '3') && (ALUOp != '6') && (ALUOp != '7') && (ALUOp != '8')) {
		printf("ALU Op Halted\n");
		return 1;
	}

	//R-type
	if(ALUOp == '7') {
		switch(funct) {
			//add
			case 32:
				ALUOp = '0';
				break;
			 
			//subtract
			case 34:
				ALUOp = '1';
				break;

			//AND
			case 36:
				ALUOp = '4';
				break;

			//OR
			case 37:
				ALUOp = '5';
				break;

			//slt signed
			case 42:
				ALUOp = '2';
				break;

			//slt unsigned
			case 43:
				ALUOp = '3';
				break;

			//NOT
			case 12:
				ALUOp = '7';
				break;

			//Shift left B by 16
			case 63:	
				ALUOp = '6';
				break;
		}
	}

	/* use data 2 or extended_value? (determined by ALU source control signal) */
	//Use extended value (immediate)
	if(ALUSrc == '1') {
		ALU(data1,extended_value,ALUOp,ALUresult,Zero);
	}
	//Use r2
	else {
		ALU(data1,data2,ALUOp,ALUresult,Zero);	
	}

	return 0;
}

/* ALU */
/* Author: Zach Muller */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{	
	switch(ALUControl) {
		//Add
		case '0':
		{	
			*ALUresult = A + B;
			//printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//Subtract
		case '1':
		{
			*ALUresult = A - B;
			//printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//SLT signed
		case '2':
		{	
			*ALUresult = 0;
			if((int)A < (int)B) {
				*ALUresult = 1;
			}
			//printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//SLT unsigned
		case '3':
		{
			*ALUresult = 0;
			if(A < B) {
				*ALUresult = 1;
			}
			//printf("ALU result = %u\n", *ALUresult);
			break;
		}
		//AND
		case '4':
		{	
			*ALUresult = A & B;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//OR
		case '5':
		{	
			*ALUresult = A | B;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//shift LEFT B by 16 bits
		case '6':
		{	
			*ALUresult = B << 16;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
		//NOT A
		case '7':
		{	
			*ALUresult = ~A;
			//printf("ALUresult = %u\n", *ALUresult);
			break;
		}
	}

	/* If result is 0, Zero = 1,
	Otherwise Zero = 0 */
	*Zero = '0';
	if(*ALUresult == 0) {
		*Zero = '1';
	}
}

/* Read / Write Memory */
/* Author: Danielle Evans */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{	
	/* Most false positives for halting */
	 if(MemRead == '1')
    {	
    	printf("Reading from memory...\n");

    	//Make sure result is word-alligned
    	if(ALUresult % 4 == 0) {
        	*memdata = Mem[ALUresult >> 2];
    	}
    	else {
	    	printf("rw_mem Halted\n");

	    	//We have an improper address
	    	return 1;
	    }
    }

    if(MemWrite == '1')
    {	
    	printf("Writing to memory...\n");

    	//Make sure result is word-alligned
    	if(ALUresult % 4 == 0) {
        	Mem[ALUresult >> 2] = data2;
    	}
    	else {
	    	printf("rw_mem Halted\n");

	    	//We have an improper address
	    	return 1;
	    }
    }

	return 0;
}


/* Write Register */
/* Author: Danielle Evans */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{	
	//write value from memory to register r2
	if(RegWrite == '1' && MemtoReg == '1' && RegDst == '0')
    {	
    	//printf("write value from memory to register r2...\n");
        Reg[r2] = memdata;

    }
    //Write value from memory to register r3
    else if(RegWrite == '1' && MemtoReg == '1' && RegDst == '1')
    {	
    	//printf("Write value from memory to register r3...\n");
        Reg[r3] = memdata;

    }
    //Write ALU result to register r2
    else if(RegWrite == '1' && MemtoReg == '0' && RegDst == '0')
    {	
    	//printf("Write ALU result to register r2...\n");
        Reg[r2] = ALUresult;

    }
    //Write ALU result to register r3
    else if(RegWrite == '1' && MemtoReg == '0' && RegDst == '1')
    {	
    	//printf("Write ALU result to register r3...\n");
        Reg[r3] = ALUresult;
    }
}

/* PC update */
/* Author: Zach Muller */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{	
	/* If instruction is a branch, calculate target address */
	if(Branch == '1' && Zero == '1') {
		*PC = (*PC + 4) + (4 * extended_value);
	}
	else if(Jump == '1') {
		//Jump to instruction at address
		*PC = jsec << 2;
	}
	/* PC+4 */
	else {
		*PC += 4;
	}
}