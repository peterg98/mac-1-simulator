#include <math.h>

#define INSTR_MASK (int)pow(2, 17) - 1

#define LODD_OPCODE 1 << 17
#define STOD_OPCODE 1 << 12
#define ADDD_OPCODE 2 << 12
#define SUBD_OPCODE 3 << 12
#define JPOS_OPCODE 4 << 12
#define JZER_OPCODE 5 << 12
#define JUMP_OPCODE 6 << 12
#define LOCO_OPCODE 7 << 12
#define LODL_OPCODE 8 << 12
#define STOL_OPCODE 9 << 12
#define ADDL_OPCODE 10 << 12
#define SUBL_OPCODE 11 << 12
#define JNEG_OPCODE 12 << 12
#define JNEZ_OPCODE 13 << 12
#define CALL_OPCODE 14 << 12
#define PSHI_OPCODE 15 << 12
#define POPI_OPCODE (15 << 12) + (1 << 9)
#define PUSH_OPCODE (15 << 12) + (2 << 9)
#define POP_OPCODE (15 << 12) + (3 << 9)
#define RETN_OPCODE (15 << 12) + (4 << 9)
#define SWAP_OPCODE (15 << 12) + (5 << 9)
#define INSP_OPCODE (15 << 12) + (6 << 9)
#define DESP_OPCODE (15 << 12) + (7 << 9)

#define LABEL 30
#define NEWLINE 31
#define INTEGER 32
#define IDENTIFIER 33