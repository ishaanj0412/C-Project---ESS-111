// Reading contents of PNG image file 

#include "BMP_struct.h"
#include "PNG_struct.h"
#include "function.h"
#include <stdlib.h>


void validate(int val, const char *msg)      // Print appropriate msg where the file is read wrong
{
    if(!val) {
        fprintf(stderr, "Invalid file: %s\n", msg);
        exit(1);
    }
}

void check_header(const char *buf)              // Validate if the file is PNG or not
{
    validate((unsigned char)buf[0] == 0x89, "header byte 1");      
    validate((unsigned char)buf[1] == 0x50, "header byte 2");
    validate((unsigned char)buf[2] == 0x4E, "header byte 3");
    validate((unsigned char)buf[3] == 0x47, "header byte 4");
    validate((unsigned char)buf[4] == 0x0D, "header byte 5");
    validate((unsigned char)buf[5] == 0x0A, "header byte 6");
    validate((unsigned char)buf[6] == 0x1A, "header byte 7");
    validate((unsigned char)buf[7] == 0x0A, "header byte 8");
}

int get_big_endian(const unsigned char *buf)                 
{
    return ((unsigned char)buf[0] << 24) |                // ORing binary equivalent of 4-bytes character
           ((unsigned char)buf[1] << 16) |
           ((unsigned char)buf[2] << 8)  |
            (unsigned char)buf[3];
}

void read_header(FILE *k)                                
{
    struct png_header sign;
    fread(&sign, sizeof(struct png_header), 1, k);
    check_header(sign.sign);
    printf("%c %c %c\n", sign.sign[1], sign.sign[2], sign.sign[3]);
    printf("Size of header= %ld\n", sizeof(struct png_header));
}


struct IHDR read_IHDR(FILE *k)                                
{   
    struct IHDR x;
    fread(&x, sizeof(struct IHDR), 1, k);
    printf("%c %c %c %c\n", x.IHDR[0], x.IHDR[1], x.IHDR[2], x.IHDR[3]);
    printf("Size of struct= %ld\n", sizeof(struct IHDR));
    printf("Chunk length= %d\n", get_big_endian(x.chunk_length));
    printf("Width = %d\n", get_big_endian(x.width));
    printf("Height = %d\n", get_big_endian(x.height));
    printf("Depth= %u\n", x.depth[0]);
    printf("Color type: %u\n", x.color_type[0]);
    printf("Compression Type: %u\n", x.compression_type[0]);
    printf("Filter method: %u\n",x.filter_method[0]);                        
    printf("Interlace method: %u\n", x.interlace_method);
    return x;
}

void read_IDAT(FILE *k, FILE *y)
{
    struct IDAT idat;
    fread(&idat, 8, 1, k);         // Reading length and chunk type (4, 4 bytes)
    printf("%c %c %c %c\n", idat.IDAT[0], idat.IDAT[1], idat.IDAT[2], idat.IDAT[3]);
    printf("Size of struct= %ld\n", sizeof(struct IDAT));
    printf("Chunk length= %d\n", get_big_endian(idat.chunk_length));
    inf(k, y, get_big_endian(idat.chunk_length));
    fread(&idat, 4, 1, k);         // Reading chunk data (4 bytes) 
}

void read_IEND(FILE *k)
{
    struct IEND iend;
    fread(&iend, sizeof(struct IEND), 1, k);             // Reading whole IEND chunk
    printf("%c %c %c %c\n", iend.IEND[0], iend.IEND[1], iend.IEND[2], iend.IEND[3]);
    printf("Size of Struct= %ld\n", sizeof(struct IEND));
    printf("Chunk length= %d\n", get_big_endian(iend.chunk_length));
}