%{
#include "constants.h"

%}
%option yylineno

A   [aA]
B   [bB]
C   [cC]
D   [dD]
E   [eE]
F   [fF]
G   [gG]
H   [hH]
I   [iI]
J   [jJ]
K   [kK]
L   [lL]
M   [mM]
N   [nN]
O   [oO]
P   [pP]
Q   [qQ]
R   [rR]
S   [sS]
T   [tT]
U   [uU]
V   [vV]
W   [wW]
X   [xX]
Y   [yY]
Z   [zZ]

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]+
INTEGER [0-9]+

%%
{INTEGER}   return INTEGER;

{L}{O}{D}{D}    {return LODD_OPCODE;}
{S}{T}{O}{D}    {return STOD_OPCODE;}
{A}{D}{D}{D}    {return ADDD_OPCODE;}
{S}{U}{B}{D}    {return SUBD_OPCODE;}
{J}{P}{O}{S}    {return JPOS_OPCODE;}
{J}{Z}{E}{R}    {return JZER_OPCODE;}
{J}{U}{M}{P}    {return JUMP_OPCODE;}
{L}{O}{C}{O}    {return LOCO_OPCODE;}
{L}{O}{D}{L}    {return LODL_OPCODE;}
{S}{T}{O}{L}    {return STOL_OPCODE;}
{A}{D}{D}{L}    {return ADDL_OPCODE;}
{S}{U}{B}{L}    {return SUBL_OPCODE;}
{J}{N}{E}{G}    {return JNEG_OPCODE;}
{J}{N}{Z}{E}    {return JNEZ_OPCODE;}
{C}{A}{L}{L}    {return CALL_OPCODE;}
{P}{S}{H}{I}    {return PSHI_OPCODE;}
{P}{O}{P}{I}    {return POPI_OPCODE;}
{P}{U}{S}{H}    {return PUSH_OPCODE;}
{P}{O}{P}       {return POP_OPCODE;}
{R}{E}{T}{N}    {return RETN_OPCODE;}
{S}{W}{A}{P}    {return SWAP_OPCODE;}
{I}{N}{S}{P}    {return INSP_OPCODE;}
{D}{E}{S}{P}    {return DESP_OPCODE;}

{IDENTIFIER}:   return LABEL;
{IDENTIFIER}    return IDENTIFIER;

\n+  {return NEWLINE;}

[ \f\r\t\v]+    ;
%%

int yywrap(void) { return 1; }