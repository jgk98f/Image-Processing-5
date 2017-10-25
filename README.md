# Image-Processing-5
Image operations involving discrete fourier transforms, HSI (Hue, Saturation, Intensity) color model, and other color model transformations.

----------------------------------------------------------------------------
README
----------------------------------------------------------------------------
Author: Jason Klamert
Programs: question[1-4]
Date: 12/01/2016
Professor Bhatia
Digital Image Processing
----------------------------------------------------------------------------
Summary:
----------------------------------------------------------------------------

The following programs correspond to the questions of project number four and
are named accordingly.

Question 1 takes one image as an arguement grayscale and produces the discrete
fourier transform. Then we zero out the frequencies that are causing the problems
(the noise). Then we convert the discrete fourier trasform back using the
inverse discrete fourier transform. This is the regular image with less noise.

Question 2 takes one image as an arguement that is color and converts this color
image to HSI. Then we modify the intensity channel, which has been normalized to
0-1 scale, and convert back to RGB. The image is then saved.

Question 3 takes one image as an arguement. This finds the image statistics for
the average value of each channel in RGB and HSI.

Question 4 takes one image as an arguement and uses two trackbars to specify
saturation and intensity. The image is converted to HSI and we assign all
pixels the newly entered saturation and intensity. We do not touch the hue!
Bonus: I converted back into RGB after the HSI editing to see the newly
transformed colors.

----------------------------------------------------------------------------
Directions:
----------------------------------------------------------------------------

Makefile:
	make -f MakeFile

clean:
	make -f MakeFile clean

run:
	question1 imageName
	question2 imageName
	question3 imageName
	question4 imageName


