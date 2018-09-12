#include <stdio.h>

int int main(int argc, char const *argv[]) {

  unsigned int H, W;
  H=W=0;
  int c;
  unsigned char *data = ReadPGM(argv[1], W, H, type);
  if(argc != 2)
  {
      cout<<"Input arguments error. Input should be <executable file.pgm>"<<endl;
      return 1;
  }



  return 0;
}
