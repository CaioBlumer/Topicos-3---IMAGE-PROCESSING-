/* convolution.exe image.ppm kernel.txt iterations*/

#include <iostream>
#include "convolution.h"
char type[2];
 int main(int argc, char const *argv[]) {

  unsigned int H, W, IT = (unsigned int)argv[3];
  unsigned int **kernel = ReadK(argv[2]);
  H=W=0;
  int c;
  unsigned char *data = ReadPGM(argv[1], W, H, type);

  if(argc != 3)
  {
      std::cout<<"Input arguments error. Input should be <executable file.pgm>"<<std::endl;
      return 1;
  }

  return 0;
}
