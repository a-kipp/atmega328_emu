#include <stdlib.h>
#include <stdint.h>
#include "memory_map.h"


uint8_t *program_memory_ptr;
uint8_t *data_memory_ptr;
uint8_t *eeprom_memory_ptr;
uint16_t program_counter = 0;
