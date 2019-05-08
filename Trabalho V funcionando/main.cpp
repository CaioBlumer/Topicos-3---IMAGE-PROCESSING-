#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

#include "shape.h"
#include "triangle.h"

using namespace std;
using namespace cv;

int main(int, char**)
{
    Mat src;
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    VideoWriter out;
    cap >> src;
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');  // select desired codec (must be available at runtime)
    double fps = 25.0;                          // framerate of the created video stream
    string filename = "./live.avi";             // name of the output video file
    out.open(filename, codec, fps, src.size(), true);

    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        std::cout << "ola" << '\n';
        detectShape(cap, out);
        detectTriangle(cap, out);
        imshow("frame", frame);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
