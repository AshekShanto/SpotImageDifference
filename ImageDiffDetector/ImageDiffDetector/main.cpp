#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

bool isEven(int inNumber);

int main()
{
  const std::string _imagePath = "../../Resources/sample1.jpg";
  // Load the image
  Mat _image = imread(_imagePath);  
  if (_image.data)
  {
    std::cout << "Successfully loaded " << _imagePath << "\n";    
    
    // convert the image to grayscale
    Mat _grayImage;
    cvtColor(_image, _grayImage, CV_BGR2GRAY);

    const int _width = _grayImage.cols;
    const int _height = _grayImage.rows;

    // calculate left and right image limits
    const int _leftImageStartCol = 0;
    int _leftImageEndCol;
    int _rightImageStartCol;
    const int _rightImageEndCol = _width - 1;

    if (isEven(_width))
    {
      _leftImageEndCol = (_width / 2) -1;
      _rightImageStartCol = _leftImageEndCol + 1;
    } 
    else
    {
      _leftImageEndCol = ((_width-1) / 2) -1;
      _rightImageStartCol = _leftImageEndCol + 2;
    }

    // Now produce the left and right image
    const int _halfWidth = _width / 2;;
    Mat _leftImage = Mat(_grayImage, cv::Range(0, _height - 1), cv::Range(_leftImageStartCol, _leftImageEndCol));
    Mat _rightImage = Mat(_grayImage, cv::Range(0, _height - 1), cv::Range(_rightImageStartCol, _rightImageEndCol));
    imshow("ori", _image);
    imshow("left", _leftImage);
    imshow("right", _rightImage);

    // find difference image
    Mat _diff;
    cv::absdiff(_leftImage, _rightImage, _diff);
    cv::threshold(_diff, _diff, 200, 255, 0);
    imshow("diff", _diff);
    waitKey(0);
  }
  else
  {
    std::cout << "Failed to load " << _imagePath << "\n";
    std::cin.get();
  }
  
  
  return 0;
}

bool isEven(int inNumber)
{
  return (inNumber % 2) == 0;
}