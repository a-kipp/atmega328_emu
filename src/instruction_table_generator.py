from itertools import product
import re

top = r"""#include <stdint.h>
#include "instruction_implementations.h

void (*instruction_table[])(uint16_t) = {"""
bottom = r"""}"""


function_table = ["unknown"] * 65536

opcode_function_pair = []
opcode_function_pair.append(['0000000000000000', 'nop'])
opcode_function_pair.append(['000111rdddddrrrr', 'adc'])
opcode_function_pair.append(['000011rdddddrrrr', 'add'])
opcode_function_pair.append(['001001dddddddddd', 'clr'])
opcode_function_pair.append(['1110KKKKddddKKKK', 'ldi'])
opcode_function_pair.append(['10111AArrrrrAAAA', 'out'])
opcode_function_pair.append(['001001rdddddrrrr', 'eor'])
opcode_function_pair.append(['10010111KKddKKKK', 'sbiw'])
opcode_function_pair.append(['111101kkkkkkk001', 'brne'])
opcode_function_pair.append(['1001010ddddd1010', 'dec'])
opcode_function_pair.append(['1100kkkkkkkkkkkk', 'rjmp'])

for pair in opcode_function_pair:
    pair[0] = re.sub('[a-zA-Z]', '[0-1]', pair[0])

for i in range(65536):
    for pair in opcode_function_pair:
        if re.match(pair[0], format(i, '016b')):
            function_table[i] = pair[1]

with open('instruction_table.h', 'w') as file:
    file.write(top + '\n')
    for function_name in function_table:
        file.write("    " + function_name + ",\n")
    file.write(bottom + '\n')