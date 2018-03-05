#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void main()
{
    VideoCapture cap;
    cap.open("1.mp4");
    Mat frame;
    while (true)
    {
        cap >> frame;


        imshow("img",frame);
        waitKey(50);
    }
    

    getchar();
}