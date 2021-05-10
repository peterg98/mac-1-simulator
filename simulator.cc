#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <bitset>
#include "constants.h"

using namespace std;

/** Masks used to separate the opcode and operand in the instruction register. 
 * Note that X_MASK is used for instructions that use 4 bits for opcode and 12 bits for operand,
 * and Y_MASK is used for instructions INSP and DESP which use 8 bits for opcode and 8 for operand.
*/
const unsigned short X_MASK = (1 << 12) - 1;
const unsigned short Y_MASK = (1 << 8) - 1;

// The code is loaded directly into the code segment after the stack and heap memory which has a size of 4095 bytes.
const unsigned short CODE_SEGMENT_OFFSET = 4095;

/** Instead of incrementing the program counter, these instructions modify the program counter 
 * directly by jumping to it. 
*/
const unsigned short NON_PC_INCR[] = {JPOS_OPCODE, JZER_OPCODE, JUMP_OPCODE, JNEG_OPCODE, JNZE_OPCODE, CALL_OPCODE, SWAP_OPCODE};

struct instruction
{
    unsigned short opcode, operand;
};

class State
{
    vector<short> memory = vector<short>(CODE_SEGMENT_OFFSET);
    short accumulator = 0;
    unsigned short stack_pointer = CODE_SEGMENT_OFFSET;
    unsigned short program_counter = CODE_SEGMENT_OFFSET;
    unsigned short memory_address = 0;
    short memory_buffer = 0;
    unsigned short instruction_register = 0;

public:
    State(vector<short>);
    unsigned short fetch_instruction();
    instruction decode_instruction(unsigned short);
    void execute_instruction(instruction);
    bool _end();

private:
    void print_state()
    {
        printf("ac<-%d, sp<-%u, pc<-%u, mar<-%u, mbr<-%u, ir<-%s\n", accumulator, stack_pointer,
               program_counter, memory_address, memory_buffer, bitset<16>(instruction_register).to_string().c_str());
    }

    short memory_set(unsigned short addr, short value)
    {
        MAR_set(addr);
        MBR_set(value);
        return memory[memory_address] = memory_buffer;
    }

    short memory_get(unsigned short addr)
    {
        MAR_set(addr);
        return MBR_set(memory[memory_address]);
    }

    unsigned short MAR_set(unsigned short addr)
    {
        printf("Setting MAR to %u... ", addr);
        memory_address = addr;
        print_state();

        return addr;
    }
    short MBR_set(short value)
    {
        printf("Setting MBR to %d... ", value);
        memory_buffer = value;
        print_state();

        return value;
    }
    unsigned short PC_set(unsigned short value)
    {
        if (value == memory.size())
        {
            printf("End of program. ");
            program_counter = value;
        }
        else
        {
            printf("Setting PC to %u... ", value);
            program_counter = value;
        }
        print_state();

        return value;
    }
    unsigned short SP_set(unsigned short value)
    {
        printf("Setting SP to %u... ", value);
        stack_pointer = value;
        print_state();

        return value;
    }
    short AC_set(short value)
    {
        printf("Setting AC to %d... ", value);
        accumulator = value;
        print_state();

        return value;
    }
    short load(short x, short offset = 0)
    {
        return AC_set(memory_get(x + offset));
    }

    short store(short x, short offset = 0)
    {
        return memory_set(x + offset, accumulator);
    }

    short add(short x, short offset = 0)
    {
        return AC_set(accumulator + memory_get(x + offset));
    }

    short subtract(short x, short offset = 0)
    {
        return AC_set(accumulator - memory_get(x + offset));
    }

    short jump_positive(short x)
    {
        return accumulator >= 0 ? PC_set(CODE_SEGMENT_OFFSET + x - 1) : PC_set(program_counter + 1);
    }

    short jump_zero(short x)
    {
        return accumulator == 0 ? PC_set(CODE_SEGMENT_OFFSET + x - 1) : PC_set(program_counter + 1);
    }

    short jump_negative(short x)
    {
        return accumulator < 0 ? PC_set(CODE_SEGMENT_OFFSET + x - 1) : PC_set(program_counter + 1);
    }

    short jump_nonzero(short x)
    {
        return accumulator != 0 ? PC_set(CODE_SEGMENT_OFFSET + x - 1) : PC_set(program_counter + 1);
    }

    short jump(short x)
    {
        return PC_set(CODE_SEGMENT_OFFSET + x - 1);
    }

    short load_constant(short x)
    {
        return AC_set(x);
    }

    short call_procedure(short x)
    {
        SP_set(stack_pointer - 1);
        memory_set(stack_pointer, program_counter);
        return PC_set(CODE_SEGMENT_OFFSET + x - 1);
    }

    void push(bool indirect = false)
    {
        SP_set(stack_pointer - 1);

        indirect ? memory[stack_pointer] =
                       memory_set(stack_pointer, memory_get(accumulator))
                 : memory_set(stack_pointer, accumulator);
    }

    void pop(bool indirect = false)
    {

        indirect ? memory[accumulator] =
                       memory_set(accumulator, memory_get(stack_pointer))
                 : AC_set(memory_get(stack_pointer));
        SP_set(stack_pointer + 1);
    }

    void _return()
    {
        PC_set(memory_get(stack_pointer));
        SP_set(stack_pointer + 1);
    }

    void swap_sp_pc()
    {
        short temp = stack_pointer;
        SP_set(program_counter);
        PC_set(temp);
    }

    short increment_sp(short y)
    {
        return SP_set(stack_pointer + y);
    }

    short decrement_sp(short y)
    {
        return SP_set(stack_pointer - y);
    }
};

State::State(vector<short> code)
{
    memory.insert(memory.end(), code.begin(), code.end());
}

unsigned short State::fetch_instruction()
{
    printf("Fetching instruction at address %u... ", program_counter);
    print_state();
    return instruction_register = memory_get(program_counter);
}

instruction State::decode_instruction(unsigned short n)
{
    printf("Decoding instruction in IR... ");
    instruction instr;

    if (n >= INSP_OPCODE)
    {
        instr.opcode = n & ~Y_MASK;
        instr.operand = n & Y_MASK;
    }
    else if (n >= PSHI_OPCODE)
    {
        instr.opcode = n;
        instr.operand = 0;
    }
    else
    {
        instr.opcode = n & ~X_MASK;
        instr.operand = n & X_MASK;
    }
    print_state();
    return instr;
}

void State::execute_instruction(instruction instr)
{
    printf("Executing instruction in IR... ");
    print_state();

    switch (instr.opcode)
    {
    case 0:
        load(instr.operand);
        break;
    case STOD_OPCODE:
        store(instr.operand);
        break;
    case ADDD_OPCODE:
        add(instr.operand);
        break;
    case SUBD_OPCODE:
        subtract(instr.operand);
        break;
    case JPOS_OPCODE:
        jump_positive(instr.operand);
        break;
    case JZER_OPCODE:
        jump_zero(instr.operand);
        break;
    case JUMP_OPCODE:
        jump(instr.operand);
        break;
    case LOCO_OPCODE:
        load_constant(instr.operand);
        break;
    case LODL_OPCODE:
        load(instr.operand, stack_pointer);
        break;
    case STOL_OPCODE:
        store(instr.operand, stack_pointer);
        break;
    case ADDL_OPCODE:
        add(instr.operand, stack_pointer);
        break;
    case SUBL_OPCODE:
        subtract(instr.operand, stack_pointer);
        break;
    case JNEG_OPCODE:
        jump_negative(instr.operand);
        break;
    case JNZE_OPCODE:
        jump_nonzero(instr.operand);
        break;
    case CALL_OPCODE:
        call_procedure(instr.operand);
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
        increment_sp(instr.operand);
        break;
    case DESP_OPCODE:
        decrement_sp(instr.operand);
        break;
    default:
        cout << "Unknown instruction: " << instr.operand << endl;
    }

    if (find(begin(NON_PC_INCR), end(NON_PC_INCR), instr.opcode) == end(NON_PC_INCR))
    {
        PC_set(program_counter + 1);
    }

    printf("\n");
}

bool State::_end() { return program_counter == memory.size(); }

/** Simulates the program until end of program, while printing out the contents of the registers
 * at each instruction and at each memory fetch.
 */
int simulate(State state)
{
    while (!state._end())
    {
        unsigned short n = state.fetch_instruction();
        instruction instr = state.decode_instruction(n);
        state.execute_instruction(instr);
    }

    return 0;
}

int main(int argc, char **argv)
{
    ifstream src(argv[1]);

    int fd;

    // Specify the 'file' argument to redirect output to a file rather than printing to stdout.
    if (argc > 2 && strcmp(argv[2], "file") == 0) {
        char buf[128];
        sprintf(buf, "%s-output.txt", argv[1]);
        fd = open(buf, O_WRONLY | O_CREAT | O_TRUNC, 0777);

        if (dup2(fd, 1) == -1)
        {
            perror("dup failed");
        }
    }

    string line;
    vector<short> code;

    while (getline(src, line))
    {
        code.push_back((short)stoi(line, nullptr, 2));
    }

    State state = State(code);

    simulate(state);

    close(fd);
}