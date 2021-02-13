#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <cstring>
#include <bitset>
#include <unordered_map>
#include "constants.h"

using namespace std;

extern int yylex();
extern int yylineno;
extern char *yytext;

int JUMP_OPCODES[] = {JZER_OPCODE, JNEG_OPCODE, JPOS_OPCODE, JUMP_OPCODE, JNZE_OPCODE};

vector<string> lines;

int curr_lineno = 1;

unordered_map<string, int> labels;

int main(void)
{
    int ntoken;

    ntoken = yylex();
    while (ntoken)
    {
        if (ntoken == LABEL)
        {
            labels[string(yytext).substr(0, strlen(yytext) - 1)] = curr_lineno;
        }
        else if (ntoken == NEWLINE)
            curr_lineno++;
        else
        {
            int opcode = ntoken;
            if (opcode <= CALL_OPCODE || (opcode >= INSP_OPCODE))
            {
                yylex();
                int operand;

                if (find(begin(JUMP_OPCODES), end(JUMP_OPCODES), opcode) != end(JUMP_OPCODES))
                {
                    operand = labels.at(yytext);
                }
                else
                    operand = atoi(yytext);

                lines.push_back(bitset<16>((short)(opcode) | operand).to_string());
            }
            else
                lines.push_back(bitset<16>((short)opcode).to_string());
        }

        ntoken = yylex();
    }

    ofstream output_file("./machine-code");
    ostream_iterator<string> output_iterator(output_file, "\n");
    copy(lines.begin(), lines.end(), output_iterator);

    return 0;
}