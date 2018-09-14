#ifndef IOHEADER_H_INCLUDED
#define IOHEADER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
unsigned char* ReadPGM(const char* filename, unsigned int &W, unsigned int &H, char* type)
{
    FILE* fp = fopen(filename, "r");
    unsigned int maxVal=0;
    unsigned char* data;

    if(!fp)
        return NULL;
    fscanf(fp, "%c", &type[0]);
    fscanf(fp, "%c", &type[1]);
    if(type[0] == 'P' && type[1] == '2')
    {
        fscanf(fp, "%d %d %d", &W, &H, &maxVal);
        data = (unsigned char*) calloc(W*H, sizeof(unsigned int));

        for(int i=0; i<W*H; i++)
        {
            fscanf(fp, "%d ", &data[i]);
        }
    }
    if(type[0] == 'P' && type[1] == '3')
    {
        fscanf(fp, "%d %d %d", &W, &H, &maxVal);
        data = (unsigned char*) calloc(W*H*3, sizeof(unsigned int));

        for(int i=0; i<W*H*3; i++)
        {
            fscanf(fp, "%d ", &data[i]);
        }
    }
    fclose(fp);
    return data;
}

bool SavePGM(const char* filename, unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    FILE* fp = fopen(filename, "w");
    if(!fp)
        return false;
    if(type[1] == '2')
    {
        fprintf(fp, "P2\n%d %d 255\n", W, H);
        for(int i=0; i<W*H; i++)
        {
            fprintf(fp, "%d ", data[i]);
        }
    }
    if(type[1] == '3')
    {
        fprintf(fp, "P3\n%d %d 255\n", W, H);
        for(int i=0; i<W*H*3; i++)
        {
            fprintf(fp, "%d ", data[i]);
        }
    }
    fclose(fp);
    return data;
}

unsigned int** ReadK(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  long length;
  unsigned int** kernel;
  if(!fp)
      return NULL;

  fseek(fp, 0, SEEK_END);
  length = ftell(fp);

  length = sqrt(length); //continuar codigo

  kernel = (unsigned int **)malloc( length * sizeof(unsigned int*));
  for (size_t i = 0; i < length; i++) {
    kernel[i] = (unsigned int *)malloc(length * sizeof(unsigned int));
  }

  for (size_t i = 0; i < length; i++)
  {
    for (size_t j = 0; j < length; j++)
    {
      fscanf(fp, "%d ", &kernel[i][j]);
    }
  }

  for (size_t i = 0; i < length; i++)
  {
    for (size_t j = 0; j < length; j++)
    {
      printf("%d ",kernel[i][j]);
    }
      printf("\n");
  }

}
#endif // IOHEADER_H_INCLUDED
