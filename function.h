// Prototype declarations of all the functions of project

#include <stdio.h>

void zerr(int ret);

int inf(FILE *source, FILE *dest, int CHUNK);

int def(FILE *source, FILE *dest, int level, int CHUNK);

void validate(int val, const char *msg);

void check_header(const char *buf);

int get_big_endian(const unsigned char *buf);

void read_header(FILE *k);

struct IHDR read_IHDR(FILE *k);                     

void read_IDAT(FILE *k, FILE *y);

void read_IEND(FILE *k);

void write_BMPheader(FILE *x,struct IHDR *ih);

struct BMP_DIB write_DIB(FILE *x,struct IHDR *ih);         

void write_colorTable(FILE *x);

int paeth_predictor(unsigned char a, unsigned char b, unsigned char c);

void read_pdata(FILE *y,int height,int width, FILE *x,struct BMP_DIB *dib);
