/* convolution.exe image.ppm kernel.txt size iterations*/

#include <iostream>
#include "convolution.h"
char type[2];
int main(int argc, char *argv[])
{
    unsigned int H, W, IT = atoi(argv[4]);
    unsigned char *data = ReadPGM(argv[1], W, H, type);
    int **kernel = ReadK(argv[2], atoi(argv[3]));
    if(kernel == NULL)
        return -2;
    type[1] = '2';

    convolution(data, W, H, type, kernel, IT ,atoi(argv[3]));

    if(type[1]==2)
        SavePGM("newPGM.pgm",data, W, H, type);
    else
        SavePGM("newPPM.ppm",data, W, H, type);

    if(argc != 5)
    {
        std::cout<<"Input arguments error. Input should be <executable file.pgm>"<<std::endl;
        return 1;
    }

    return 0;
}
