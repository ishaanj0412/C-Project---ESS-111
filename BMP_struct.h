// Declaration of all the structures of BMP file 

struct BMP_header                        // Structure declaration of main header of BMP file
{
    char sign[2];
    unsigned int size;
    int garbage;
    int offset;
};

struct BMP_DIB                           //Structure declaration of BITMAPINFOHEADER (40 bytes) of BMP file
{
    unsigned int size;
    int width;
    int height;
    unsigned short int colorplanes;
    unsigned short int bitsperpixel;
    unsigned int compression;
    unsigned int image_size;
    int xperM;
    int yperM;
    unsigned int num_colors;
    unsigned int imp_colors;
};

struct colorTable                        //Structure declaration of color table
{ 
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
    unsigned char reserved;
};