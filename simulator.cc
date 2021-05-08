#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include "constants.h"

using namespace std;

const unsigned short X_MASK = (1 << 12) - 1;
const unsigned short Y_MASK = (1 << 8) - 1;

vector<unsigned short> lines;

short memory[4095];

short accumulator = 0;
unsigned short stack_pointer = 4095;
unsigned short program_counter = 0;
unsigned short memory_address = 0;
unsigned short memory_buffer = 0;
unsigned short instruction_register = 0;

struct instruction
{
    unsigned short opcode, value;
};

const unsigned short NON_PC_INCR[] = {JPOS_OPCODE, JZER_OPCODE, JUMP_OPCODE, JNEG_OPCODE, JNZE_OPCODE, CALL_OPCODE, SWAP_OPCODE};

/* Decouples a binary instruction into a struct with their opcode and operand */
instruction parse_instruction(unsigned short n)
{
    instruction instr;

    if (n >= INSP_OPCODE)
    {
        instr.opcode = n & ~Y_MASK;
        instr.value = n & Y_MASK;
    }
    else if (n >= PSHI_OPCODE)
    {
        instr.opcode = n;
        instr.value = 0;
    }
    else
    {
        instr.opcode = n & ~X_MASK;
        instr.value = n & X_MASK;
    }
    return instr;
}

short load(short x, short offset = 0)
{
    return accumulator = memory[x + offset];
}

short store(short x, short offset = 0)
{
    return memory[x + offset] = accumulator;
}

short add(short x, short offset = 0)
{
    return accumulator += memory[x + offset];
}

short subtract(short x, short offset = 0)
{
    return accumulator -= memory[x + offset];
}

short jump_positive(short x)
{
    return accumulator >= 0 ? program_counter = x - 1 : program_counter += 1;
}

short jump_zero(short x)
{
    return accumulator == 0 ? program_counter = x - 1 : program_counter += 1;
}

short jump_negative(short x)
{
    return accumulator < 0 ? program_counter = x - 1 : program_counter += 1;
}

short jump_nonzero(short x)
{
    return accumulator != 0 ? program_counter = x - 1 : program_counter += 1;
}

short jump(short x)
{
    return program_counter = x - 1;
}

short load_constant(short x)
{
    return accumulator = x;
}

short call_procedure(short x)
{
    stack_pointer -= 1;
    memory[stack_pointer] = program_counter;
    return program_counter = x - 1;
}

void push(bool indirect = false)
{
    stack_pointer -= 1;
    indirect ? memory[stack_pointer] = memory[accumulator] : memory[stack_pointer] = accumulator;
}

void pop(bool indirect = false)
{
    indirect ? memory[accumulator] = memory[stack_pointer] : accumulator = memory[stack_pointer];
    stack_pointer += 1;
}

void _return()
{
    program_counter = memory[stack_pointer];
    stack_pointer += 1;
}

void swap_sp_pc()
{
    short temp = stack_pointer;
    stack_pointer = program_counter;
    program_counter = temp;
}

short increment_sp(short x)
{
    return stack_pointer += x;
}

short decrement_sp(short x)
{
    return stack_pointer -= x;
}

/* Simulates the program line by line, while printing out the contents of the registers. */

int simulate()
{
    while (program_counter < lines.size())
    {
        memory_address = program_counter;
        memory_buffer = lines.at(program_counter);
        instruction_register = memory_buffer;

        instruction instr = parse_instruction(instruction_register);

        switch (instr.opcode)
        {
        case 0:
            load(instr.value);
            break;
        case STOD_OPCODE:
            store(instr.value);
            break;
        case ADDD_OPCODE:
            add(instr.value);
            break;
        case SUBD_OPCODE:
            subtract(instr.value);
            break;
        case JPOS_OPCODE:
            jump_positive(instr.value);
            break;
        case JZER_OPCODE:
            jump_zero(instr.value);
            break;
        case JUMP_OPCODE:
            jump(instr.value);
            break;
        case LOCO_OPCODE:
            load_constant(instr.value);
            break;
        case LODL_OPCODE:
            load(instr.value, stack_pointer);
            break;
        case STOL_OPCODE:
            store(instr.value, stack_pointer);
            break;
        case ADDL_OPCODE:
            add(instr.value, stack_pointer);
            break;
        case SUBL_OPCODE:
            subtract(instr.value, stack_pointer);
            break;
        case JNEG_OPCODE:
            jump_negative(instr.value);
            break;
        case JNZE_OPCODE:
            jump_nonzero(instr.value);
            break;
        case CALL_OPCODE:
            call_procedure(instr.value);
            break;
        case PSHI_OPCODE:
            push(true);
            break;
        case POPI_OPCODE:
            pop(true);
            break;
        case PUSH_OPCODE:
            push(false);
            break;
        case POP_OPCODE:
            pop(false);
            break;
        case RETN_OPCODE:
            _return();
            break;
        case SWAP_OPCODE:
            swap_sp_pc();
            break;
        case INSP_OPCODE:
            increment_sp(instr.value);
            break;
        case DESP_OPCODE:
            decrement_sp(instr.value);
            break;
        default:
            cout << "Unknown instruction: " << instr.value << endl;
        }

        if (find(begin(NON_PC_INCR), end(NON_PC_INCR), instr.opcode) == end(NON_PC_INCR))
        {
            program_counter++;
        }

        cout << "ac<-" << accumulator << ", sp<-" << stack_pointer << ", pc<-" << program_counter 
            << ", mar<-" << memory_address << ", mbr<-" << memory_buffer << ", ir<-" << 
            bitset<16>(instruction_register).to_string() << endl;
    }

    return 0;
}

int main(int argc, char **argv)
{
    ifstream src(argv[1]);

    string line;
    while (getline(src, line))
    {
        lines.push_back((unsigned short)stoi(line, nullptr, 2));
    }

    simulate();
}