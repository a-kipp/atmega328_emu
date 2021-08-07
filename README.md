**Handling Pointers

There are two different types of pointers in this program,
there is the ususal C-pointer, that points somwehere into the
host-systems memory and there is a second type of pointer, that points
to an adress in the emulated microcontrollers memory.
To prevent confusion, C-pointer variables carry the suffix "_ptr"
and pointers in the guest-system the "_addr" suffix. Guest system pointers are
always of type "uint16_t" and correspond to an address in the memory map.
The the stack pointer and the program counter are examples for this type of pointer.
the value of the address is acceses by adding the address to the startpoint
of the emulatet memoryspace in the host system, this results in an "*uint_8"
that must be than castet properly.
Example of dereferencing accessing the value of an address:
    *(uint8_t*)(data_memory_ptr + rd_addr)
