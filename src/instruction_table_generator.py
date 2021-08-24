from itertools import product
import re

top = r"""#include <stdint.h>
#include "instruction_implementations.h

void (*instruction_table[])(uint16_t) = {"""
bottom = r"""}"""


function_table = ["unknown"] * 65536

opcode_function_pair = []
opcode_function_pair.append(('0000000000000000', 'nop'))
opcode_function_pair.append(('000000000000000q', 'noi'))

for pair in opcode_function_pair:
    print(re.sub('[a-zA-Z]', '[0-1]', pair[0]))

for i in range(65536):
    print (i)
    for pair in opcode_function_pair:
        if re.match(pair[0], combination):
            function_table[int(combination)] = pair[1]

with open('instruction_table.h', 'w') as file:
    file.write(top + '\n')
    for function_name in function_table:
        file.write("    " + function_name + ",\n")
    file.write(bottom + '\n')