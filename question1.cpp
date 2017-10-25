#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    Mat src = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    if( src.empty()){
        cout << "Image: " << argv[1] << " is empty! We shall terminate. " << endl;
        return -1;
    }

    /**
     * Note: Most of the DFT transform code is adapted and or copied from the opencv documentation.
     * The code may be found here: http://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html
     **/
     
    Mat zeroPadded; 
    //Compute the new optimal size for our zero padded Mat.                           
    int m = getOptimalDFTSize( src.rows );
    int n = getOptimalDFTSize( src.cols ); 
    //Now that we have an MxN Mat of optimal size we can use the boarder function to padd it with zeros.
    copyMakeBorder(src, zeroPadded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(zeroPadded), Mat::zeros(zeroPadded.size(), CV_32F)};
    Mat complexNumbers;
    
    // Add to zeroPadded another plane with zeros. This is suppose to be one way that the result will fit in our src Mat.
    merge(planes, 2, complexNumbers);         

    //Find the discrete fourier transform
    dft(complexNumbers, complexNumbers);

    //Split the real and imaginary planes.
    split(complexNumbers, planes);     
         

    //Find the magnitude of our plane[0].        
    magnitude(planes[0], planes[1], planes[0]);
    Mat spectrum = planes[0];
    spectrum += Scalar::all(1);
    
    //We can use log to switch to logrithmic scale.
    log(spectrum, spectrum);

    //Use the trick I learned from the example to cut off the odd row/col of the image.
    spectrum = spectrum(Rect(0, 0, spectrum.cols & -2, spectrum.rows & -2));

    int x = spectrum.cols/2;
    int y = spectrum.rows/2;

    //Rearrange the image to get our spectrum centered.
    Mat topLeft(spectrum, Rect(0, 0, x, y));
    Mat topRight(spectrum, Rect(x, 0, x, y));
    Mat bottomLeft(spectrum, Rect(0, y, x, y));
    Mat bottomRight(spectrum, Rect(x, y, x, y));

    //Swap diagonally across what would normally be the x and y axis.
    Mat temp;
    topRight.copyTo(temp); 
    bottomLeft.copyTo(topRight);
    temp.copyTo(bottomLeft);
    
    topLeft.copyTo(temp);
    bottomRight.copyTo(topLeft);
    temp.copyTo(bottomRight);

    //Normalize the values in the spectrum to range from 0-1.
    normalize(spectrum, spectrum, 0, 1, CV_MINMAX);

    Rect roi(274,275,300,300);
    Mat image_roi = complexNumbers(roi);
    Mat spectrum_roi = spectrum(roi);


    //Replace the noise with zeros to effectively blot out the undesired noise.
    for(int rowNum = 0; rowNum < 12; rowNum++){
	        for(int colNum = 355; colNum < 377 ; colNum++){
			                image_roi.at<float>(rowNum,colNum) = 0.0;
			                spectrum_roi.at<float>(rowNum,colNum) = 0.0;
              }
    }

    for(int rowNum = 160; rowNum < 177; rowNum++){
                for(int colNum = 340; colNum < 365 ; colNum++){
                                        image_roi.at<float>(rowNum,colNum) = 0.0;
                                        spectrum_roi.at<float>(rowNum,colNum) = 0.0;
              }
    }


    for(int rowNum = 77; rowNum < 97; rowNum++){
        for(int colNum = 270; colNum < 295; colNum++){                
                        image_roi.at<float>(rowNum,colNum) = 0.0;
			            spectrum_roi.at<float>(rowNum,colNum) = 0.0;                
        }
    }

    for(int rowNum = 77; rowNum < 97; rowNum++){
        for(int colNum = 443; colNum < 463; colNum++){               
                        image_roi.at<float>(rowNum,colNum) = 0.0;
			            spectrum_roi.at<float>(rowNum,colNum) = 0.0;                
        }
    }    
/*
    for(int rowNum = 115; rowNum < 135; rowNum++){
                for(int colNum = 355; colNum < 377 ; colNum++){
                                        image_roi.at<float>(rowNum,colNum) = 0.0;
                                        spectrum_roi.at<float>(rowNum,colNum) = 0.0;
              }
    }

    for(int rowNum = 35; rowNum < 55; rowNum++){
                for(int colNum = 355; colNum < 377 ; colNum++){
                                        image_roi.at<float>(rowNum,colNum) = 0.0;
                                        spectrum_roi.at<float>(rowNum,colNum) = 0.0;
              }
    }
*/
    //Calculate the inverse discrete fourier transform to get the image back!
    Mat inverseTransform;
    idft(complexNumbers, inverseTransform, DFT_SCALE | DFT_REAL_OUTPUT);
    inverseTransform.convertTo(inverseTransform, CV_8U); 
    
    imshow("Spectrum", spectrum);
    waitKey();
    imshow("IDFT", inverseTransform);
    waitKey();

    return 0;

}
