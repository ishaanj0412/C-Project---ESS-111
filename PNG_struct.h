// Declaration of all the structures of PNG file 

struct png_header
{
    unsigned char sign[8];
};


struct IHDR
{
    unsigned char chunk_length[4];
    char IHDR[4];
    unsigned char width[4];
    unsigned char height[4];
    unsigned char depth[1];
    unsigned char color_type[1];
    unsigned char compression_type[1];
    unsigned char filter_method[1];
    unsigned char interlace_method;
    unsigned char CRC[4];
};

struct IDAT
{
    unsigned char chunk_length[4];
    char IDAT[4];
    unsigned char CRC[4];
};

struct IEND
{
    unsigned char chunk_length[4];
    char IEND[4];
    unsigned char CRC[4];
};