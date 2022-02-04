/*this file includes two functions which attempt to read the greyscale pixel data from the PNG file and write it into the BMP file after padding.
 
 The PNG pixel data has to be defiltered and only then it can be written into BMP.
 
 There are 5 types of filtering and the function read_pdata defilters each row based on the type of filtering for that row.
 
 The function paeth_predictor is used in defiltering rows which have filter type 4.   */

#include "BMP_struct.h"
#include "PNG_struct.h"
#include "function.h"
#include <stdlib.h>

int paeth_predictor(unsigned char a, unsigned char b, unsigned char c) /*function used in filter type 4*/
{
    /*a, b and c are the prior pixel, the upper pixel and the upper left pixel respectively*/
  unsigned char p = a + b - c;
  if(p > 255)
  {
    p = p - 256; /*checking for overflow, since unsigned chars cannot exceed 255*/
  }
  unsigned char pa = abs(p - a);
  unsigned char pb = abs(p - b);
  unsigned char pc = abs(p - c);
  if(pa <= pb && pa <= pc)
    return a;
  else if(pb <= pc)
    return b;
  else
    return c;
}
void read_pdata(FILE *y,int height,int width, FILE *x,struct BMP_DIB *dib)
{
    unsigned char f_type; /*f_type represents the type of filtering taking place. It takes values from 0-4*/
    unsigned char arr[height*width]; /*array to store pixel data of the PNG file*/
    int i, j, k; 
    unsigned char temp, temp2, temp3, temp4 , temp5; 
    unsigned char array[((*dib).height)*((*dib).width)]; /*array from which the BMP pixel data is written. (*dib).width represents width of final BMP image, whereas width represents the width of the original PNG image*/
    for(i= 0; i < height; i++)
    {
        k = 0; temp = 0;
        for(j=0;j<=width;j++)
        {
            if(j==0)
            {
            	fread(&f_type, 1, 1, y); /*reading the filter type at the start of every row*/
            }
            /*if filter type is 0, the defiltered row is exactly the same as the filtered row*/
            else if(f_type == 0)
            {
              fread(&arr[i*width + j-1], 1, 1, y);
            }
            /*The Sub filter (filter type 1) transmits the difference between each byte and the value of the corresponding byte of the prior pixel. */
            else if(f_type == 1)
            {
              if(k == 0)
              {
                fread(&arr[i*width + j-1], 1, 1, y);
                temp = arr[i*width + j-1];
                k++;
              }
              else
              {
                fread(&temp2, 1, 1, y);
                temp = temp2 + temp; /*to defilter, the pixel is added to the pixel to its left*/
                if(temp > 255)
                {
                  temp = temp - 256;
                }
                arr[i*width + j-1] = temp;
              }
            }
            /*The Up filter (filter type 2)transmits the difference between each byte and the value of the corresponding byte of the upper pixel.*/
            else if(f_type == 2)
            {
              if(i == 0)
              {
                fread(&arr[i*width + j-1], 1, 1, y); /*for the first row, the upper pixels are taken as 0, so the row remains unchanged*/
              }
              else
              {
                fread(&temp2, 1, 1, y);
                temp = temp2 + arr[(i-1)*width + j - 1]; /*to defilter, each byte is added to the byte above it*/
                if(temp > 255)
                {
                  temp = temp - 256;
                }
                arr[i*width + j-1] = temp;
              }
            }
            /*The Average filter uses the average of the two neighboring pixels (left and above) to predict the value of a pixel.*/
            else if(f_type == 3)
            {
              if(i == 0) /*when its the first row, upper bytes are taken to be 0*/
              {
                  if(k == 0)
                  {
                    fread(&arr[i*width + j-1], 1, 1, y);
                    temp = arr[i*width + j-1];
                    k++;
                  }
                  else
                  {
                    fread(&temp2, 1, 1, y);
                    temp = temp2 + temp/2;
                    if(temp > 255)
                    {
                      temp = temp - 256;
                    }
                    arr[i*width + j-1] = temp;
                  }
              }
              else
              {
                if(j == 1) /*when its the first column, prior pixels are taken to be 0*/
                {
                  fread(&temp2, 1, 1, y);
                  temp = temp2 + (arr[(i-1)*width + j-1])/2;
                  if(temp > 255)
                  {
                    temp = temp - 256;
                  }
                  arr[i*width + j-1] = temp;
                }
                else
                {
                  temp = arr[i*width + j-2];
                  temp3 = arr[(i-1)*width + j-1];
                  fread(&temp2, 1, 1, y);
                  arr[i*width + j-1] = temp2 + (temp + temp3)/2; /*to defilter, each byte is added to the average of the prior and the upper byte.*/
                }
              }
            }
            /*The Paeth filter computes a simple linear function of the three neighboring pixels (left, above, upper left), then chooses as predictor the neighboring pixel closest to the computed value*/
            else if(f_type == 4)
            {
              if(i == 0) /*Case when its the first row*/
              {
                  if(k == 0)
                  {
                    fread(&arr[i*width + j-1], 1, 1, y);
                    temp = arr[i*width + j-1];
                    k++;
                  }
                  else
                  {
                    temp = arr[j-2];
                    fread(&temp2, 1, 1, y);
                    temp3 = temp2 + paeth_predictor(temp, 0, 0);
                    if(temp3 > 255)
                    {
                      temp3 = temp3 - 256;
                    }
                    arr[i*width + j-1] = temp3;
                  }
              }
              else if(j == 1) /*case when its the first column*/
              {
                temp = arr[(i-1)*width + j-1];
                fread(&temp2, 1, 1, y);
                temp3 = temp2 + paeth_predictor(0, temp, 0);
                if(temp3 > 255)
                {
                  temp3 = temp3 - 256;
                }
                arr[i*width + j-1] = temp3;
              }
              else
              {
                temp = arr[(i-1)*width + j-1];
                fread(&temp2, 1, 1, y);
                temp3 = arr[(i*width) + j-2];
                temp4 = arr[(i-1)*width + j-2];
                temp5 = temp2 + paeth_predictor(temp3, temp, temp4); /*to defilter add each byte to the paeth predictor function which takes three arguments left pixel, upper pixel and the upper left pixel*/
                if(temp5 > 255)
                {
                  temp5 = temp5 - 256;
                }
                arr[i*width + j-1] = temp5;
              }
            }
        }
    }
    /*Before writing the pixel data into the BMP file, padding must be done. since each row in BMP has to be a multiple of 4,
     if the original png image row size is not a multiple of 4, extra bytes are added at the end of each row in order to make it a multiple of 4. These padding bytes have been taken as 0.*/
    for(i = 0; i < ((*dib).height); i++)
    {
      for(j = 0; j < ((*dib).width); j++)
      {
        if(j < width)
        {
          array[i*((*dib).width) + j] = arr[i*width + j];
        }
        else
        {
          array[i*((*dib).width) + j] = 0; /*adding zeroes to the end of each row*/
        }
      }
    }
    /*BMP image is written from bottom to top and left to right. Hence, the values are being written row by row from bottom to top*/
    for(i = ((*dib).height)-1; i >= 0; i--)
    {
      for(j = 0; j < ((*dib).width); j++)
      {
        fwrite(&array[i*((*dib).width) + j], 1, 1, x);
      }
    }
}
