#include "flood.hpp"

int main(int argc, char** argv)
{
  if (argc != 3)
    return -1;

  Mat input = imread(argv[1]);
  Flood seg(input, atoi(argv[2]));
  Mat result = seg.process();

  namedWindow("Result", 2);
  imshow("Result", result);
  waitKey(0);

  return 0;
}
