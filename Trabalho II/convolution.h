#ifndef IOHEADER_H_INCLUDED
#define IOHEADER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdio>
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

int** ReadK(const char* filename, int length)
{
    FILE* fp2 = fopen(filename, "r");
    int** kernel;
    if(!fp2)
        return NULL;

    kernel = (int **)malloc(length * sizeof(int*));
    for (size_t i = 0; i < length; i++)
    {
        kernel[i] = (int *)malloc(length * sizeof(int));
    }
    rewind(fp2);
    for(unsigned int i = 0; i < length; i++)
    {
        for (unsigned int j = 0; j < length; j++)
        {
            fscanf(fp2, "%d ", &kernel[i][j]);
        }
    }
    return kernel;
}

void convolution(unsigned char *data, unsigned int w, unsigned int h, char* type, int **kernel, unsigned int it, int msize)
{
    int k = 0;
    int mean = 0;
    unsigned char* out = (unsigned char*) calloc(w*h, sizeof(unsigned int));
    unsigned int index;
    int temp = 0;

    for(int i = 0; i < msize; i++)
    {
        for(int j = 0; j < msize; j++)
        {
            mean += kernel[i][j];
        }
    }
    if(mean ==0)
        mean = 1;
    while(k < it)
    {
            for(int i = (msize-1)/2; i < h-((msize-1)/2); i++)
            {
                for(int j = ((msize-1)/2); j < w-((msize-1)/2); j++)
                {
                    for(int c = 0; c < msize; c++)
                    {
                        for(int l = 0; l < msize; l++)
                        {
                            index = w*(i+c-((msize-1)/2))+ j+l-((msize-1)/2);
                            temp += kernel[c][l] * data[index];
                        }
                    }
                    index = i*w+j;
                    if (temp/mean < 0) out[index] =0;
                    else if (temp/mean > 255) out[index] = 255;
                    else out[index] = temp/mean;
                    temp = 0;
                }
            }
        k++;
        for (int i=0; i<h; i++)
        {
            for(int j=0;j<w;j++)
                data[i*w+j] = out[i*w+j];
        }
    }
    free(out);
}
#endif // IOHEADER_H_INCLUDED
