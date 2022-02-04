// This file contains the writing part of BMP data.

#include "BMP_struct.h"
#include "PNG_struct.h"
#include "function.h"


void write_BMPheader(FILE *x,struct IHDR *ih)                     
{
    struct BMP_header header;
    header.sign[0]=0x42;          // B
    header.sign[1]=0x4D;          // M
    // defining height and width here
    int height = get_big_endian(ih->height);
    int width;
    if((get_big_endian(ih->width))%4==0)  
    {
        width=(get_big_endian(ih->width));        
    }
    else
    {
        width=(((get_big_endian(ih->width))/4)+1)*4;
    }

    // 1078 --> 256x4 [8-bit grayscale --> 2^8 shades of gray x 4(size of color table)] + 40 (size of DIB_header) + 14 (size of BMP_header)
    // Total size of header--> 1078 + total size of image pixels (height*width)
    header.size=1078 + height*width;
    header.garbage=0;
    header.offset=1078;

    // Struct allocate 4 bytes to each data type(even for unsigned char) in BMP_header for efficiency purpose 
    fwrite(&header.sign, 2, 1, x);   
    fwrite(&header.size, 12, 1, x);
}


struct BMP_DIB write_DIB(FILE *x,struct IHDR *ih)          
{
    struct BMP_DIB m;
    m.size=40;
    if((get_big_endian(ih->width))%4==0)        // no need of padding as width is multiple of 4
    {
        m.width=(get_big_endian((ih->width)));        
    }
    else
    {
        m.width=(((get_big_endian((ih->width))/4)+1)*4);           // padding is done to make width multiple of 4
    }
    m.height=get_big_endian((ih->height));
    m.colorplanes=1;
    m.bitsperpixel=8;
    m.compression=0;
    m.image_size=0;
    m.xperM=0;                   
    m.yperM=0;
    m.num_colors=256;
    m.imp_colors=0;
    fwrite(&m, sizeof(struct BMP_DIB), 1, x);
    return m;
}

void write_colorTable(FILE *x)
{
    struct colorTable ct;
    for(int i=0; i<256; i++)                                 // range of integer value of RGB color is 0 - 255
    {
        char k=i;
        ct.Blue = ct.Green = ct.Red=i;        
        ct.reserved = 0;                                    // This member is reserved and must be zero because any color is composed of RGB (24-bits (not a power of 2)) 
        fwrite(&ct, 4, 1, x);                               // thats's why 4 bytes RGB (32-bits)
    }
}