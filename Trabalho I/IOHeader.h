#ifndef IOHEADER_H_INCLUDED
#define IOHEADER_H_INCLUDED
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

//double covariance() implementar

void mahalanobis(unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    int index; // red mean, green mean, blue mean
    double rgbM [3] = {0};
    int Size; //size of the samples vector
    unsigned char *amostra; //samples vector
    double sigma[3][3] = {0};
    double m[3] = {0};
    double temp[3] = {0};
    double dist = 0;

    FILE* fp2 = fopen("dataset.txt", "r"); // open the dataset

    //if(!fp)
    //   return NULL;
    fscanf(fp2, "%d", &Size); // get size of data
    amostra = (unsigned char*) calloc(Size*3, sizeof(unsigned int));
    for(int i=0; i<Size*3; i++)
    {
        fscanf(fp2, "%d ", &amostra[i]); //the file should be in r->g->b
    }
    fclose(fp2);
    for(int i =0; i < Size*3; i+=3)
    {
        rgbM[0] += amostra[i];
        rgbM[1] += amostra[i+1];
        rgbM[2] += amostra[i+2];
    }
    rgbM[0] = rgbM[0]/Size; //mean value
    rgbM[1] = rgbM[1]/Size;
    rgbM[2] = rgbM[2]/Size;
    for(int i=0; i<Size*3; i+=3)
    {
        sigma[0][0] += (amostra[i] - rgbM[0]) * (amostra[i] - rgbM[0]);
        sigma[1][1] += (amostra[i+1] - rgbM[1]) * (amostra[i+1] - rgbM[1]);
        sigma[2][2] += (amostra[i+2] - rgbM[2]) * (amostra[i+2] - rgbM[2]);
        sigma[0][1] += (amostra[i] - rgbM[0]) * (amostra[i+1] - rgbM[1]);
        sigma[0][2] += (amostra[i] - rgbM[0]) * (amostra[i+2] - rgbM[2]);
        sigma[1][2] += (amostra[i+1] - rgbM[1]) * (amostra[i+2] - rgbM[2]);
    }
    sigma[1][0] = sigma[0][1];
    sigma[2][1] = sigma[1][2];
    sigma[2][0] = sigma[0][2]; //Covarience matrix(sigma) created through a dumb method

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            sigma[i][j] = sigma[i][j]/(Size-1);
        }
    }
// computes the inverse of a matrix m
    double det = sigma[0][0] * (sigma[1][1] * sigma[2][2] - sigma[2][1] * sigma[1][2]) -
                 sigma[0][1] * (sigma[1][0] * sigma[2][2] - sigma[1][2] * sigma[2][0]) +
                 sigma[0][2] * (sigma[1][0] * sigma[2][1] - sigma[1][1] * sigma[2][0]);
    double invdet = 1 / det; //inverso do determinante

    double minv[3][3]; // inverse of matrix m
    minv[0][0] = (sigma[1][1] * sigma[2][2] - sigma[2][1] * sigma[1][2]) * invdet;
    minv[0][1] = (sigma[0][2] * sigma[2][1] - sigma[0][1] * sigma[2][2]) * invdet;
    minv[0][2] = (sigma[0][1] * sigma[1][2] - sigma[0][2] * sigma[1][1]) * invdet;
    minv[1][0] = (sigma[1][2] * sigma[2][0] - sigma[1][0] * sigma[2][2]) * invdet; //inverte a matriz
    minv[1][1] = (sigma[0][0] * sigma[2][2] - sigma[0][2] * sigma[2][0]) * invdet;
    minv[1][2] = (sigma[1][0] * sigma[0][2] - sigma[0][0] * sigma[1][2]) * invdet;
    minv[2][0] = (sigma[1][0] * sigma[2][1] - sigma[2][0] * sigma[1][1]) * invdet;
    minv[2][1] = (sigma[2][0] * sigma[0][1] - sigma[0][0] * sigma[2][1]) * invdet;
    minv[2][2] = (sigma[0][0] * sigma[1][1] - sigma[1][0] * sigma[0][1]) * invdet;

    for(int h =0; h < H*W*3 ; h+=3)
    {
        m[0] = data[h] - rgbM[0];
        m[1] = data[h+1] - rgbM[1];
        m[2] = data[h+2] - rgbM[2];

        for(int i = 0; i<3; i++)
        {
            for(int j = 0; j<3; j++)
            {
                temp[i] += m[i] * minv[i][j];
            }

        }
        for(int l = 0; l<3; l++)
        {
                dist += temp[l] * m[l];
        }
        if(dist < 6)
        {
            data[h] = 255;
            data[h+1] = 255;
            data[h+2] = 255;
        }
        dist = 0;
        temp[0] = 0;
        temp[1] = 0;
        temp[2] = 0;

    }

}


#endif // IOHEADER_H_INCLUDED
