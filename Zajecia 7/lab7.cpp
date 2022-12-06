#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

// g++ `pkg-config --cflags opencv4` lab7.cpp `pkg-config --libs opencv4`
using namespace std;
using namespace cv;
int main(int argc, char **argv){
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;
    cv::Mat frame;
    namedWindow("Flipped", WINDOW_AUTOSIZE);
//    int i = 0;
    while (waitKey(10) != 27) {
        cap >> frame;
        cv::flip(frame,frame,1);
        cv::imshow("Flipped", frame);
//        imwrite("frames/frame" + std::to_string(i++) + ".png", frame);
    }
    return 0;
}