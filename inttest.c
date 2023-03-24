#include <stdio.h>
#include <stdint.h>


int main() {
unsigned char bytes[4];
unsigned long n = 175;

bytes[0] = (n >> 24) & 0xFF;
bytes[1] = (n >> 16) & 0xFF;
bytes[2] = (n >> 8) & 0xFF;
bytes[3] = n & 0xFF;

printf("%x %x %x %x\n", bytes[0], bytes[1], bytes[2], bytes[3]);


int a = 13;
printf("%i\n", (a << 0));
return 0;
}
