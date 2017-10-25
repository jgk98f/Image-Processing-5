/**
 * Author: Jason Klamert
 * Date: 10/09/2016
 * Description: Program to determine image stats for RGB and HSI color spaces.
 **/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <iostream>
#include <unistd.h>
#define DEBUG false
#define PI 3.14159265
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  if(argc != 2){
          cerr << "Error: program takes an image as an arguement. \nCall like this: \nquestion3 imageName" << endl;
          exit(-1);
  }

  //Load img in color.
  Mat src = imread(argv[1], 1);
  
  if(src.empty())
    cerr << "Error: image is empty or not an image." << endl;
    
    Mat RGB = src.clone();
    cvtColor(RGB, RGB, CV_BGR2RGB);
    float accumR, accumG, accumB, total = 0;
    total = RGB.rows * RGB.cols;
    
    for(int rowNum = 0; rowNum < RGB.rows; rowNum++){
            for(int colNum = 0; colNum < RGB.cols; colNum++){
                    accumR += RGB.at<Vec3b>(rowNum,colNum)[0];
                    accumG += RGB.at<Vec3b>(rowNum,colNum)[1];
                    accumB += RGB.at<Vec3b>(rowNum,colNum)[2];
            }
    }
    
    accumR = accumR / total;
    accumG = accumG / total;
    accumB = accumB / total;
       
  //Make an empty Mat of same specs as the original. 
  Mat HSI(src.rows, src.cols, src.type());

  float r, g, b, hue, saturation, intensity;

  for(int rowNum = 0; rowNum < src.rows; rowNum++)
    {
      for(int colNum = 0; colNum < src.cols; colNum++)
        {
          int minVal = 0;
          b = src.at<Vec3b>(rowNum, colNum)[0];
          g = src.at<Vec3b>(rowNum, colNum)[1];
          r = src.at<Vec3b>(rowNum, colNum)[2];

          intensity = (b + g + r) / 3;

          
          minVal = min(r,min(b,g));

          saturation = 1 - 3*(minVal/(b + g + r));
          
          //Round off the saturation to determine purity. (IE Gray Levels)
          if(saturation < 0.00001)
            {
                  saturation = 0;
            }else if(saturation > 0.99999){
                  saturation = 1;
            }
          //If saturation is not zero, then compute the hue.
          if(saturation != 0)
            {
              hue = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
              hue = acos(hue);

              if(b <= g)
                {
                  hue = hue;
                } else{
                  hue = ((360 * PI) / 180.0) - hue;
                }
            }

          HSI.at<Vec3b>(rowNum, colNum)[0] = (hue * 180) / PI;
          HSI.at<Vec3b>(rowNum, colNum)[1] = saturation * 100;
          HSI.at<Vec3b>(rowNum, colNum)[2] = intensity;
        }
    }
    
    double accumS;
    float accumH, accumI;
    float totalHSI = HSI.rows * HSI.cols;
    
    for(int rowNum = 0; rowNum < HSI.rows; rowNum++){
            for(int colNum = 0; colNum < HSI.cols; colNum++){
                    accumH += HSI.at<Vec3b>(rowNum,colNum)[0];
                    accumS += HSI.at<Vec3b>(rowNum,colNum)[1];
                    accumI += HSI.at<Vec3b>(rowNum,colNum)[2];
            }
    }
    
    accumH = accumH / totalHSI;
    accumS = accumS / totalHSI;
    accumI = accumI / totalHSI;
    
    cout << "RGB Image Stats:" << endl;
    cout << "-----------------" << endl;
    cout << "Average R: " << accumR << endl;
    cout << "Average G: " << accumG << endl;
    cout << "Average B: " << accumB << endl;
    cout << endl;
    cout << "HSI Image Stats: " << endl;
    cout << "-----------------" << endl;
    cout << "Average H: " << accumH  << " degrees" << endl;
    cout << "Average S: " << accumS << endl;
    cout << "Average I: " << accumI << endl;
    cout << "Normalized S: " << accumS/255 << endl;
    cout << "Normalized I: " << accumI/255 << endl;
    cout << "Note: Normalization = 0 - 1 span." << endl;
    cout << endl;
    
  namedWindow("RGB", CV_WINDOW_AUTOSIZE);
  namedWindow("HSI", CV_WINDOW_AUTOSIZE);

  imshow("RGB", RGB);
  imshow("HSI", HSI);

  waitKey(0);
  return 0;
}

