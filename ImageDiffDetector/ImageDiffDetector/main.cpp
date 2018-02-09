#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;

// returns true if given number is even
bool isEven(int inNumber);

// cuts given image into two equal parts along width and returns them as left and right image
void getLeftRightPartialImages(const Mat& inSourceImg, Mat& outLeftImg, Mat& outRightImg, int& outOffset);

// a procedure to detect difference between to images
Mat getImgDiffProc1(const Mat& inImage1, const Mat& inImage2);

// Returns a copy of the orginal image along with all the detected diffs drawnn as rectangles
Mat getImageWithDiffDrawn(const Mat& inOriginalImage, const Mat& inDiffImage, int leftRightImageOffset);

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

    // Now produce the left and right image    
    Mat _leftImage, _rightImage;
    int _offset;
    getLeftRightPartialImages(_grayImage, _leftImage, _rightImage, _offset);

    // find difference image
    Mat _diff = getImgDiffProc1(_leftImage, _rightImage);   
    imshow("diff image", _diff);

    // Draw the detected changes in an copy of the original image
    Mat _imgWithDiffsDrawn = getImageWithDiffDrawn(_image, _diff, _offset);

    // The application key event logic. On Q key press, it toggles showing the detected differences.
    // For pressing esc, the application exits. For all other keys, it does nothing.
    std::cout << "Press Q to toggle showing detected differences and press esc to exit.\n";

    bool _showDiffs = true;
    const std::string _windowTitle = "Spot Image Differences";
    bool _exitApp = false;
        
    while (!_exitApp)
    {
      Mat _imgToShow = _showDiffs ? _imgWithDiffsDrawn : _image;
      imshow(_windowTitle, _imgToShow);      
      
      bool _appNeedsUpdate = false;
      while (!_appNeedsUpdate)
      {
        const char _key = (char)waitKey(0);
        if (_key == 'q')
        {
          _showDiffs = !_showDiffs;
          destroyWindow(_windowTitle);
          _appNeedsUpdate = true;
        }
        else if(_key == 27) // 27, the escape key
        {
          _exitApp = true;
          break;
        }
      }      
    }        
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

void getLeftRightPartialImages(const Mat& inSourceImg, Mat& outLeftImg, Mat& outRightImg, int& outOffset)
{
  const int _width = inSourceImg.cols;
  const int _height = inSourceImg.rows;

  // calculate left and right image limits
  const int _leftImageStartCol = 0;
  int _leftImageEndCol;
  int _rightImageStartCol;
  const int _rightImageEndCol = _width - 1;

  if (isEven(_width))
  {
    _leftImageEndCol = (_width / 2) - 1;
    _rightImageStartCol = _leftImageEndCol + 1;
  }
  else
  {
    _leftImageEndCol = ((_width - 1) / 2) - 1;
    _rightImageStartCol = _leftImageEndCol + 2;
  }

  // Now produce the left and right image
  const int _halfWidth = _width / 2;;
  outLeftImg = Mat(inSourceImg, cv::Range(0, _height - 1), cv::Range(_leftImageStartCol, _leftImageEndCol));
  outRightImg = Mat(inSourceImg, cv::Range(0, _height - 1), cv::Range(_rightImageStartCol, _rightImageEndCol));
  outOffset = _rightImageStartCol;
}

Mat getImgDiffProc1(const Mat& inImage1, const Mat& inImage2)
{
  Mat _diff;
  cv::absdiff(inImage1, inImage2, _diff);
  cv::threshold(_diff, _diff, 200, 255, 0);
  return _diff;
}

Mat getImageWithDiffDrawn(const Mat& inOriginalImage, const Mat& inDiffImage, int leftRightImageOffset)
{
  Mat _imageWithDiffDrawn;
  inOriginalImage.copyTo(_imageWithDiffDrawn);
  std::vector<std::vector<Point>> _contours;
  findContours(inDiffImage, _contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

  for (int i = 0; i< _contours.size(); ++i)
  {
    Rect r = boundingRect(_contours[i]);
    r.x += leftRightImageOffset;
    rectangle(_imageWithDiffDrawn, r, Scalar(0, 0, 255), 2);
  }
  return _imageWithDiffDrawn;
}