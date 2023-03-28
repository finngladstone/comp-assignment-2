void prints(char * str) {
    while (*str) *((char *) 0x0800) = *(str++);
}

int volatile *const ConsoleWriteSInt = (int *)0x0804;

unsigned int volatile *const ConsoleWriteUInt = (unsigned int *)0x0808;

int volatile *const Halt = (int*)0x080C;

int main() {
    unsigned int value_1 = 123456;
    int value_2 = 123456;

    prints("Console write unsigned... 0x");
    *ConsoleWriteUInt = value_1;
    prints("\n");
    
    prints("Console write signed... ");
    *ConsoleWriteSInt = value_2;
    prints("\n");

    *Halt = 0;
    return 0;

}