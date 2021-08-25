from itertools import product
import re

top = r"""#pragma once

#include <stdint.h>
#include "instruction_implementations.h"

void (*instruction_table[])(void) = {"""
bottom = r"""}"""


opcode_function_pair = [
    ['0000 0000 0000 0000', 'nop'],
    ['0001 11rd dddd rrrr', 'adc'],
    ['0000 11rd dddd rrrr', 'add'],
    ['0010 01dd dddd dddd', 'clr'],
    ['1110 KKKK dddd KKKK', 'ldi'],
    ['1011 1AAr rrrr AAAA', 'out'],
    ['0010 01rd dddd rrrr', 'eor'],
    ['1001 0111 KKdd KKKK', 'sbiw'],
    ['1111 01kk kkkk k001', 'brne'],
    ['1001 010d dddd 1010', 'dec'],
    ['1100 kkkk kkkk kkkk', 'rjmp'],
]


function_table = ["unknown"] * 65536


for pair in opcode_function_pair:
    pair[0] = pair[0].replace(" ", "")
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