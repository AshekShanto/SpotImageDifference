#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main()
{
  Mat img = imread("../../Resources/sample1.jpg"); 
  namedWindow("image", WINDOW_NORMAL);
  imshow("image", img);
  waitKey(0);
  return 0;
}