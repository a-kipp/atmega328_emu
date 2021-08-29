from itertools import product
import re

top = r"""// Autogenerated file, do not make any changes here. Modification must be made in
// instruction_table_generator.py

#pragma once

#include <stdint.h>
#include "instruction_implementations.h"

void (*jump_table[])(void) = {"""
bottom = r"""}"""


opcode_function_pair = [
    ['0000 0000 0000 0000', 'nop'],
    ['0001 11rd dddd rrrr', 'adc'],
    ['0000 11rd dddd rrrr', 'add'],
    ['1110 KKKK dddd KKKK', 'ldi'],
    ['1011 1AAr rrrr AAAA', 'out'],
    ['0010 01rd dddd rrrr', 'eor'],
    ['1001 0111 KKdd KKKK', 'sbiw'],
    ['1111 01kk kkkk k001', 'brne'],
    ['1001 010d dddd 1010', 'dec'],
    ['1100 kkkk kkkk kkkk', 'rjmp'],
    ['1001 001d dddd 0000', 'sts'],
    ['0001 01rd dddd rrrr', 'cp']
]


function_table = ["unknown"] * 65536


for pair in opcode_function_pair:
    pair[0] = pair[0].replace(" ", "")
    pair[0] = re.sub('[a-zA-Z]', '[0-1]', pair[0])

for i in range(65536):
    for pair in opcode_function_pair:
        if re.match(pair[0], format(i, '016b')):
            function_table[i] = pair[1]

with open('jump_table.h', 'w') as file:
    file.write(top + '\n')
    for function_name in function_table:
        file.write("    " + function_name + ",\n")
    file.write(bottom + '\n')