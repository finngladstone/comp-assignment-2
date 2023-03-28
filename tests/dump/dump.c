void prints(char * str) {
    while (*str) *((char *) 0x0800) = *(str++);
}

int volatile *const DumpPC = (int *)0x0820;

int volatile *const DumpRegisterBanks = (int *)0x0824;


int main() {
    prints("Dumping PC...  ");
    *DumpPC = 0;
    prints("\n");

    prints("Dumping register banks... \n");
    *DumpRegisterBanks = 0;

    return 0;
}