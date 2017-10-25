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
  
  namedWindow("RGB", CV_WINDOW_AUTOSIZE);
  imshow("RGB", RGB);
  waitKey(0);
         
  //Make an empty Mat of same specs as the original. 
  Mat HSI(src.rows, src.cols, src.type());

  float r, g, b, hue, saturation, intensity;

  for(int rowNum = 0; rowNum < src.rows; rowNum++){
      for(int colNum = 0; colNum < src.cols; colNum++){
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
  
    //Normalize the I component to 0-1 and perform the specified function for project.
    for(int rowNum = 0; rowNum < HSI.rows; rowNum++){
            for(int colNum = 0; colNum < HSI.cols; colNum++){
                    HSI.at<Vec3b>(rowNum,colNum)[2] /= 255;
                    HSI.at<Vec3b>(rowNum,colNum)[2] += (( 1 - HSI.at<Vec3b>(rowNum,colNum)[2] ) / 2 );
                    HSI.at<Vec3b>(rowNum,colNum)[2] *= 255;
                    if( HSI.at<Vec3b>(rowNum,colNum)[2] > 255)
                        HSI.at<Vec3b>(rowNum,colNum)[2] = 255;
            }
    }
      
  //Time to convert the HSI back to RGB!
  float newR, newG, newB;
  float newHue, newSaturation, newIntensity;
  
  for(int rowNum = 0; rowNum < HSI.rows; rowNum++){
            for(int colNum = 0; colNum < HSI.cols; colNum++){
                   newHue = HSI.at<Vec3b>(rowNum,colNum)[0];
                   newSaturation = HSI.at<Vec3b>(rowNum,colNum)[1];
                   newIntensity = HSI.at<Vec3b>(rowNum,colNum)[2];
                   
        
                   if(newHue > 0 && newHue < 120 || newHue == 0){
                        newR = newIntensity * (1 +  (newSaturation * (cos(newHue)/cos(60 - newHue))));
                        newB = newIntensity * (1 - newSaturation);
                        newG = 3 * newIntensity - (newR + newB);
                   }
                   else if(newHue > 120 && newHue < 240 || newHue == 120){
                        newHue = newHue - 120;
                        newR = newIntensity * (1 - newSaturation);
                        newG = newIntensity * (1 +  (newSaturation * (cos(newHue)/cos(60 - newHue))));
                        newB = 3 * newIntensity - (newR + newG);
                   }
                   else if(newHue > 240 && newHue < 360 || newHue == 240){
                        newHue = newHue - 240;
                        newR = 3 * newIntensity - (newG + newB);
                        newG = newIntensity * (1 - newSaturation);
                        newB = newIntensity * (1 +  (newSaturation * (cos(newHue)/cos(60 - newHue))));
                   }
                   
                   RGB.at<Vec3b>(rowNum,colNum)[0] = newB;
                   RGB.at<Vec3b>(rowNum,colNum)[1] = newG;
                   RGB.at<Vec3b>(rowNum,colNum)[2] = newR;
            }
  }
    
  namedWindow("RGB", CV_WINDOW_AUTOSIZE);
  imshow("RGB", RGB);
  waitKey(0);
  
  imwrite("RGB.jpg", RGB);
  
  return 0;
}

