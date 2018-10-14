#include<iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

char letter[5] = {'a', 'b', 'c', 'd', 'e'};

int main(int argc, char const *argv[])
{
  char buffer[25];
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if we succeeded
      return -1;

  for(;;)
  {
      Mat frame;
      cap >> frame; // get a new frame from camera
      flip(frame,frame,1);
      cvtColor( frame, frame, CV_BGR2GRAY );
      GaussianBlur( frame, frame, Size(11, 11), 2, 2 );
      // threshold(frame, frame, 80, 255, 0);
      vector<Vec3f> circles;
      HoughCircles( frame, circles, CV_HOUGH_GRADIENT, 1, frame.rows/8, 100, 50, 0, 0 );

      for( size_t i = 0; i < circles.size(); i++ )
      {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
      }

      if(circles.size() > 2)
      {
        for (size_t i = 0; i < circles.size()-1; i++)
        {
          Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          Point center2(cvRound(circles[i+1][0]), cvRound(circles[i+1][1]));
          line(frame, center, center2, Scalar(0,0,0), 1, LINE_8, 0);
        }
      }

      imshow("edges", frame);
      if(waitKey(10) >= 0) break;
  }
  waitKey(0);

  return 0;
}
