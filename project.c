#include "spimcore.h"
/*
    CDA3103 Fall 2015

    Ian Lewis
    Kyle Bergman
    Sean Hernandez
*/

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero) {

    // Implement the ALU's functions; i.e. addition, subtraction, less than, greater than, and, or, shifts, and not
    switch (ALUControl) { // A (case) B
        case 000: // Addition
            *ALUresult = A + B;

            if (*ALUresult == 0)
                *Zero = 1;
            else
                *Zero = 0;

            break;
        case 001: // Subtraction
            *ALUresult = A - B;

            if (*ALUresult == 0)
                *Zero = 1;
            else
                *Zero = 0;

            break;
        case 010: // Less than
            if (A < B) {
                *ALUresult = 1;
                *Zero = 0;
            }

            else {
                *ALUresult = 0;
                *Zero = 1;
            }

            break;
        case 011: // Less than with unsigned ints
            if (A < B) {
                *ALUresult = 1;
                *Zero = 0;
            }

            else {
                *ALUresult = 0;
                *Zero = 1;
            }

            break;
        case 100: // And
            *ALUresult = A & B;

            if (*ALUresult == 0)
                *Zero = 1;
            else
                *Zero = 0;

            break;
        case 101: // Or
            *ALUresult = A | B;

            if (*ALUresult == 0)
                *Zero = 1;
            else
                *Zero = 0;

            break;
        case 110: // Left shift by 16 bits
            B <<= 16;
            break;
        case 111: // Not
            *ALUresult = !A;

            if (*ALUresult == 0)
                *Zero = 1;
            else
                *Zero = 0;

            break;

        default:
            break;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
    // In "instruction_fetch" we receive information from "Mem" and transcribe it into "instruction"
    if (PC % 4 != 0)
        return 1;

    else {
        *instruction = Mem[PC >> 2];
        return 0;
    }
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {
    // Here we define the different types of "instructions" that that program will perform
    // Refer to instruction given in the diagram/flow chart
    *op = instruction >> 26;                 // instruction [31-26]
    *r1 = (instruction >> 21) & 0x0000001F;    // instruction [25-21]
    *r2 = (instruction >> 16) & 0x0000001F;    // instruction [20-16]
    *r3 = (instruction >> 11) & 0x0000001F;    // instruction [15-11]
    *funct = instruction & 0x0000003F;       // instruction [5-0]
    *offset = instruction & 0x0000FFFF;      // instruction [15-0]
    *jsec = instruction & 0x03FFFFFF;        // instruction [25-0]
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls) {
    // In this switch statement we initialize the different values that controls give when used

    switch(op) {
        // Cases reflect MIPS commands
        case 0x0: //add, subtract, and, or, slt, sltu
        {
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            return 0;
        }

        case 0x8:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        }

        case 0xA:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        }

        case 0xB:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        }



        case 0x4:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            return 0;
        }

        case 0x23:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
        }

        case 0x2B:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            return 0;
        }

        case 0x2:
        {
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            return 0;
        }

        case 0xF:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
            break;
        }

        default:
            return 1;
    }
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) {
    // Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value) {
    // Value of left-most bit of immediate operand is copied to all bits to the left (into the high-order bits)
    if((offset >> 15) == 1)
        *extended_value = 0xFFFF0000 | offset;

    else
        *extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero) {

    if(ALUOp == 7) // If instruction is an R-type instruction (111 base 2 or 7 base 10)
        switch(funct)
        {
            case 0x20: // add
                ALUOp = 0;
                break;
            case 0x22: // subtract
                ALUOp = 1;
                break;
            case 0x24: // and
                ALUOp = 4;
                break;
            case 0x25: // or
                ALUOp = 5;
                break;
            case 0x27: // NOR
                ALUOp = 7;
                break;
            case 0x2A: // slt
                ALUOp = 2;
                break;
            case 0x2B: // sltu
                ALUOp = 3;
                break;
            default:

                return 1;
                break;
        }

    else if(ALUOp < 0 || ALUOp >7)
        return 1; ; // Halt condition, invalid commands

    if(ALUSrc)
        ALU(data1,extended_value,ALUOp,ALUresult,Zero); // Apply function and output to ALUresult

    else
        ALU(data1,data2,ALUOp,ALUresult,Zero); // Apply function and output to ALUresult

    if(*Zero == 1)
        return 1; // Halt condition
    else
        return 0;

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {
    // Determine memory write operation or memory read operation
    if(MemWrite == 1)
    {
        if((ALUresult % 4) != 0)
            return 1;   //Halt, @ not word aligned

        // Write the value of data2 to the memory location addressed by ALUresult
        Mem[ALUresult >> 2] = data2;    // Memory to be written,
    }

    if(MemRead == 1)
    {
        if((ALUresult % 4) != 0)
            return 1;   //Halt, @ not word aligned

        // Read the content of the memory location addressed by ALUresult to memdata
        *memdata = Mem[ALUresult >> 2];
    }
    return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg) {
// Take the result of ALU operations or memory data and write it to a register
    if(RegWrite == 0)
        return; // Don't write
    if(RegDst == 1)
        r2 = r3;
    if(MemtoReg == 1)
        Reg[r2] = memdata; // Write memdata to Reg[r2]
    else
        Reg[r2] = ALUresult; // Write ALUresult to Reg[r2]
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC) {
    if(Jump == 0)
    {
        if(Branch && Zero == 1)
            *PC = (extended_value * 4) + (*PC + 4); // Update counter to new location

        else
            *PC = *PC + 4; // Update counter one word over
    }
    else if(Jump == 1)
    {
        *PC = ((*PC + 4 & 0xF0000000) | (jsec * 4)); // Jump to new branch
    }

}
