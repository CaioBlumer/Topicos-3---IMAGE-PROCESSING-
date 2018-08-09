#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "IOHeader.h"
using namespace std;
char type[2];
int main(int argc, char** argv)
{

    unsigned int H, W;
    H=W=0;
    int c;
    unsigned char *data = ReadPGM(argv[1], W, H, type);
    if(argc != 2)
    {
        printf("Input arguments error. Input should be <executable file.pgm>\n");
        return 1;
    }

    cout << "-- IMG PROCESSING --"<<endl;
    cout << "1 - Invert color"<<endl;
    cout << "2 - Change color(red)"<<endl;
    cin >> c;
    switch (c)
    {
    case 1 :
        inverted(data, W, H, type);
        break;
    case 2 :
        changecolor(data, W, H, type);
        break;
    default:
        cout << "Invalid option" << endl;
        break;
    }
    SavePGM("NewPGM.pgm", data, W, H, type);

    free(data);
    return 0;
}

