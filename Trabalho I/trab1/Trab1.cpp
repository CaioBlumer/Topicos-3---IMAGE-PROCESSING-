#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "IOHeader.h"
#include <cmath>
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
        cout<<"Input arguments error. Input should be <executable file.pgm>"<<endl;
        return 1;
    }

    cout << "-- IMG PROCESSING --"<<endl;
    cout << "1 - Square"<<endl;
    cout << "2 - Sphere"<<endl;
    cout << "3 - k-neighbours (Requires file sphereset.txt)"<<endl;
    cout << "4 - Mahalanobis  (Requires file dataset.txt)"<<endl;
    cin >> c;

    switch (c)
    {
    case 1:
        square(data, W, H, type);
        break;
    case 2:
        sphere(data, W, H, type);
        break;
    case 3:
        kneighbor(data, W, H, type);
        break;
    case 4:
        mahalanobis(data, W, H, type);
        break;
    default:
        cout << "Invalid option" << endl;
        break;
    }
    SavePGM("NewPGM.pgm", data, W, H, type);

    free(data);
    return 0;
}

