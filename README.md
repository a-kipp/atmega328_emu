### Handling Pointers

There are two different types of pointers in this program,
there is the ususal C-pointer, that points somwehere into the
host-systems memory and there is a second type of pointer, that points
to an adress in the emulated microcontrollers memory.
To prevent confusion, C-pointer variables carry the suffix "_ptr"
and pointers in the guest-system the "_addr" suffix. Guest system pointers are
always of type "uint16_t" and correspond to an address in the memory map.
The the stack pointer and the program counter are examples for this type of pointer.
The pointer can be dereferenced by adding the pointer to the pointer of the
emulatet memory in the host system. This results in an "*uint_8" pointer
Example of dereferencing accessing the value of an address:
    `*(*uint8_t*)(data_memory_ptr + rd_addr)`
In the file "memory.h" functions for reading and writing to the memory are provided.
