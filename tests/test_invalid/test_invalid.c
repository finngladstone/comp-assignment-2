void prints(char * str) {
    while (*str) *((char *) 0x0800) = *(str++);
}

int volatile *const InvalidOperation_1 = (int *)0x0900;



int main() {
    prints("Attempting to store in memory that is out of range...\n");
    *InvalidOperation_1 = 1;

}