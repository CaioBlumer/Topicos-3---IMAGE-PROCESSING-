#ifndef IOHEADER_H_INCLUDED
#define IOHEADER_H_INCLUDED
#include <math.h>
#include <iostream>
using namespace std;
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

void square(unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    int ri, rf, gi, gf, bi, bf;

    cout<<"Type the range of RED"<<endl;
    cin>>ri>>rf;
    cout<<"Type the range of GREEN"<<endl;
    cin>>gi>>gf;
    cout<<"Type the range of BLUE"<<endl;
    cin>>bi>>bf;

    for(int i=0; i<W*H*3; i+=3)
    {
        if((data[i]>= ri && data[i] <= rf) && (data[i+1]>= gi && data[i+1] <= gf) && (data[i+2]>= bi && data[i+2] <= bf))
        {
            data[i]   = 255;
            data[i+1] = 255;
            data[i+2] = 255;

        }
    }
}

void sphere(unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    int r,g,b,t;

    cout<<"Type the center of the sphere in RGB and the distance threshold"<<endl;
    cin>>r>>g>>b>>t;

    for(int i=0; i<W*H*3; i+=3)
    {
        if(sqrt( pow((data[i] - r),2) + pow((data[i+1] - g),2) + pow((data[i+2] - b),2)) <= t)
        {
            data[i]   = 255;
            data[i+1] = 255;
            data[i+2] = 255;
        }
    }

}

void kneighbor(unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    int Size, t;
    bool inside;
    unsigned char *pontos; //vector with rgb points and threshold
    FILE* fp2 = fopen("sphereset.txt", "r"); // open the file that contains the center of the spheres and the threshold

    fscanf(fp2, "%d", &Size); // get number of spheres
    pontos = (unsigned char*) calloc(Size*4, sizeof(unsigned int));
    for(int i=0; i<Size*4; i++)
    {
        fscanf(fp2, "%d ", &pontos[i]); //the file should be in r->g->b->t
    }
    for(int i=0; i < H*W*3 ; i+=3)
    {
        inside = false;
        for(int j=0; j<Size*4; j+=4)
        {
            t = pontos[j+3]; // read the treshold of the given sphere
            if(sqrt( pow((data[i] - pontos[j]),2) + pow((data[i+1] - pontos[j+1]),2) + pow((data[i+2] - pontos[j+2]),2)) <= t) //compares the distance with the threshold
            {
                inside = true;
            }
        }
        if(inside == true) //compares the distance with the threshold
        {
            data[i]   = 255;
            data[i+1] = 255;
            data[i+2] = 255;
        }
        else
        {
            data[i]   = 0;
            data[i+1] = 0;
            data[i+2] = 0;
        }
    }
}

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
    //   return false;
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
    double invdet = 1 / det; // inverse of determinant

    double minv[3][3]; // inverse of matrix m
    minv[0][0] = (sigma[1][1] * sigma[2][2] - sigma[2][1] * sigma[1][2]) * invdet;
    minv[0][1] = (sigma[0][2] * sigma[2][1] - sigma[0][1] * sigma[2][2]) * invdet;
    minv[0][2] = (sigma[0][1] * sigma[1][2] - sigma[0][2] * sigma[1][1]) * invdet;
    minv[1][0] = (sigma[1][2] * sigma[2][0] - sigma[1][0] * sigma[2][2]) * invdet;
    minv[1][1] = (sigma[0][0] * sigma[2][2] - sigma[0][2] * sigma[2][0]) * invdet;
    minv[1][2] = (sigma[1][0] * sigma[0][2] - sigma[0][0] * sigma[1][2]) * invdet;
    minv[2][0] = (sigma[1][0] * sigma[2][1] - sigma[2][0] * sigma[1][1]) * invdet;
    minv[2][1] = (sigma[2][0] * sigma[0][1] - sigma[0][0] * sigma[2][1]) * invdet;
    minv[2][2] = (sigma[0][0] * sigma[1][1] - sigma[1][0] * sigma[0][1]) * invdet;

    for(int h =0; h < H*W*3 ; h+=3)
    {
        m[0] = data[h] - rgbM[0];  //variance
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
        if(dist < 1)
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

/*
	I left char* type as a argument even tho it's not used for possible future implementations.
  TODO: Change the return value of functions to bool and return if failed to open files
		Create Header CPP and main files
		Use OpenCV to create the files and view results
*/
#endif // IOHEADER_H_INCLUDED
