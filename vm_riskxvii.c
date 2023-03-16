

/* Parse binary file*/

// int main(int argc, char *argv[]) {
//     FILE * myfile = fopen(argv[1], "r");

//     if (myfile == NULL) {
//         printf("Error: File %s could not be found\n", argv[1]);
//         exit(0);
//     }

//     char ch;
//     while ((ch = fgetc(myfile)) != EOF)
//         putchar(ch);

//     fclose(myfile);

//     return 0;

//     FILE * fpointer;
//     fpointer = fopen(argv[1], "rb");

//     if (fpointer == NULL) {
//         printf("Error: File %s could not be found\n", argv[1]);
//         exit(0);
//     }

//     char ch;
//     while ((ch = fgetc(fpointer)) != EOF)
//         putchar(ch);

//     fclose(fpointer);

//     return 0; 
// }

// #include <stdio.h>

// int main(int argc, char** argv)
// {
//     if (argc < 2)
//         return 1; /* TODO: better error handling */

//     FILE* f = fopen(argv[1], "rb");

//     /* TODO: check f is not NULL */

//     /* Read one byte */  
//     int c;
//     while ((c = fgetc(f)) != EOF) {
//         printf("%x", (unsigned) c);
//     }

//     // int first = fgetc(f);

//     // if (first != EOF)
//     //     printf("first byte = %x\n", (unsigned)first);

//     /* TODO else read failed, empty file?? */

//     fclose(f);

//     return 0;
// }

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
        printf("%d\n", buffer[i]);
    }

    return 0;
}

