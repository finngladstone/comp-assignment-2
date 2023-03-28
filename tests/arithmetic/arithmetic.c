void prints(char * str) {
    while (*str) *((char *) 0x0800) = *(str++);
}

int volatile *const ConsoleWriteSInt = (int *)0x0804;

int main() {
    int a = 1;
    int b = 2;
    int c = a | b;
    int d = c ^ b;
    int e = a & b;

    e = 4 << 2;
    int f = 40 >> 1;

    a = a + d;
    b = b + c;

    prints("Result of a = ");
    *ConsoleWriteSInt = a;
    prints("\nResult of b = ");
    *ConsoleWriteSInt = b;
    prints("\n");


    return 0;
}