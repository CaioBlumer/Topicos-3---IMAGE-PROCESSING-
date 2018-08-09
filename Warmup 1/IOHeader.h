#ifndef IOHEADER_H_INCLUDED
#define IOHEADER_H_INCLUDED

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

void inverted(unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    unsigned int index;
    if(type[1] == '2')
    {
        for(int i=0; i<H; i++)
            for(int j=0; j<W; j++)
            {
                index = i*W + j;
                data[index] = 255 - data[index];
            }
    }
    if(type[1] == '3')
    {
        for(int i=0; i<H*3; i++)  //verificar H*3
        {
            for(int j=0; j<W; j++)
            {
                for(int k=0; k<3; k++)
                {
                    index = i*W + j + k;
                    data[index] = 255 - data[index];
                }
            }
        }
    }
}

void changecolor(unsigned char *data, unsigned int W, unsigned int H, char* type)
{
    int index;
    if(type[1] == '3')
    {
        for(int i =0; i < H*W*3 ; i+=3)   //percorrendo o vetor de uma forma diferente
        {
            if ((data[i] - data[i+1])>0)
            {
                data[i] = 255;
                data[i+1] = 255;
                data[i+2] = 255;
            }
        }
    }
}


#endif // IOHEADER_H_INCLUDED
