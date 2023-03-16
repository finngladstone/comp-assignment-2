#include <stdio.h>
#include <stdlib.h>

/* 
    Each line has 32 bits

*/

int main(int argc, char * argv[]) {
    FILE *myfile;
    myfile = fopen(argv[1], "rb");
    int buffer[64];

    for (int i = 0; i < 64; i++)
        buffer[i] = 0;

    if (myfile == NULL) {
        printf("Error reading file\n");
        exit(1);
    }

    fread(buffer, 1, 64, myfile);

    fclose(myfile);

    for (int i = 0; i < 64; i++) {

        int val = buffer[i];
        int k;

        for (int c = 31; c >= 0; c--) {
            k = val >> c;
            if (k & 1)
                printf("1");
            else  
                printf("0");
        }


        printf(" = %d\n", buffer[i]);
    }

    return 0;
}

