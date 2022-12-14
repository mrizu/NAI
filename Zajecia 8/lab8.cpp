#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
//g++ `pkg-config --cflags opencv4` lab8.cpp `pkg-config --libs opencv4`

using namespace std;
using namespace cv;

bool compareContours(vector<Point> &contour1, vector<Point> &contour2) {
    return (abs(contourArea(contour1)) < abs(contourArea(contour2)));
}

int main(int argc, char **argv) {
    VideoCapture camera(0);

    if (!camera.isOpened())
        return -1;

    namedWindow("Settings", WINDOW_AUTOSIZE);

    int iLowH = 170;
    int iHighH = 179;

    int iLowS = 200;
    int iHighS = 255;

    int iLowV = 45;
    int iHighV = 255;

    createTrackbar("LowH", "Settings", &iLowH, 179);
    createTrackbar("HighH", "Settings", &iHighH, 179);

    createTrackbar("LowS", "Settings", &iLowS, 255);
    createTrackbar("HighS", "Settings", &iHighS, 255);

    createTrackbar("LowV", "Settings", &iLowV, 255);
    createTrackbar("HighV", "Settings", &iHighV, 255);

    while (waitKey(1) != 27) {
        Mat src_image;
        camera.read(src_image);
        Mat image_red;
        Mat image_hsv;

        cvtColor(src_image, image_hsv, COLOR_BGR2HSV);
        inRange(image_hsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), image_red);

        Mat k = getStructuringElement(MORPH_ELLIPSE, {10, 10});
        morphologyEx(image_red, image_red, MORPH_CLOSE, k);

        Canny(image_red, image_red, 100, 100);

        auto kern = getStructuringElement(MORPH_ELLIPSE, {30, 30});
        morphologyEx(image_red, image_red, MORPH_CLOSE, kern);

        GaussianBlur(image_red, image_red, Size(5, 5), 0);

        vector<vector<Point>> contours;
        findContours(image_red, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        vector<int> biggestContoursIds;
        vector<vector<Point>> biggestContours;
        sort(contours.begin(), contours.end(), compareContours);

        if (contours.size() > 1){
            biggestContours = {contours[contours.size() - 1], contours[contours.size() - 2]};
            drawContours(src_image, biggestContours, -1, {0, 255, 0});

            Moments m1 = moments(biggestContours[0], false);
            Point p1_1 = {(int) ((m1.m10 / m1.m00) + 10), (int) ((m1.m01 / m1.m00) + 5)};
            Point p1_2 = {(int) ((m1.m10 / m1.m00) + 10), (int) ((m1.m01 / m1.m00) + 8)};

            Moments m2 = moments(biggestContours[1], false);
            Point p2_1 = {(int) ((m2.m10 / m2.m00) + 10), (int) ((m2.m01 / m2.m00) + 5)};
            Point p2_2 = {(int) ((m2.m10 / m2.m00) + 10), (int) ((m2.m01 / m2.m00) + 10)};

            line(src_image, p1_1, p2_1, Scalar(0, 0, 255), 3, LINE_8);
            line(src_image, p1_2, p2_2, Scalar(255,255,0), 2, LINE_8);
        }
        imshow("Red connect", src_image);
    }
}