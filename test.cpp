//
// Created by lsk on 12/7/22.
//
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    VideoCapture videoCapture(0);
    Mat mat;
    cout << videoCapture.read(mat) << endl;
    mat.release();
    return 0;
}