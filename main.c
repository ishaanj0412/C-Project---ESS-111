// Front-end of the project

#define ZLIB_INTERNAL
#include<stdio.h>
#include<stdlib.h>
#include<zlib.h>
#include<assert.h>
#include<math.h>
#include "BMP_struct.h"
#include "PNG_struct.h"
#include "function.h"


int main()
{
    char file1[30], file2[30];              //name of source and destination file should be 20 characters...
    printf("Enter source name: ");
    scanf("%s",file1);

    printf("Enter destination name: ");
    scanf("%s",file2);
    
    FILE *k;
    FILE *y;
    k=fopen(file1, "rb");
    y=fopen("temp.bin", "wb");                         // temp.bin is an  auxiliary file
    if(k==NULL || y==NULL)                             // Exits if source file doesn't exist or neither of the file is inputted 
    {
        printf("Error in opening file\n");
        exit(1);
    }

    read_header(k);
    printf("\n");
    struct IHDR ih = read_IHDR(k);
    printf("\n");
    read_IDAT(k, y);
    fclose(y);
    printf("\n");
    read_IEND(k);
    fclose(k);

    FILE *x;
    FILE *b;
    x=fopen(file2, "wb");
    b=fopen("temp.bin", "rb");                           // Exits if x is not inputted 
    if(b==NULL || x==NULL)
    {
        printf("Error in opening file\n");
        exit(1);
    }
    write_BMPheader(x,&ih);
    struct BMP_DIB dib=write_DIB(x,&ih);
    write_colorTable(x);
    fseek(x, 1078, SEEK_SET);                          // Pointer to FILE x is moved 1078 bytes ahead.
    int height = get_big_endian((ih.height));
    int width = get_big_endian(ih.width);
    read_pdata(b, height, width, x, &dib);
    fclose(x);
    fclose(b);                                        
    return 0;
}
