
#define PRINT(x) cout << #x << " => " << x << endl;

#define SQRT2 (double)1.4142135623730950488016

#define PX 412
#define PY 495
#define RV 10 //vision radius
#define RS 5 //step radius
#define RT RV// vision for threshold
#define RN RF * 2 //neighbor radius
#define RF RS // SQRT2  //forbidden radius
#define RM 0 //minimum vision radius
#define STEPS 100
#define DEV 0.37
#define LT 2 // line thiccness for connectable test
#define LD 0.2 //deviation of smoothing if line function for connectable test
#define LS RS*LT // steps for averaging the line function
#define CT 4 // Connectabel threshhold. if the smoothed function goes below this, no new node will be spawned.

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <complex.h>
#include <fftw3.h>

#include "tiffio.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

#include "datafun.h"
#include "datafun.cpp"
#include "mathfun.cpp"
#include "node.h"
#include "node.cpp"




//images are saved in 2 dimensional arrays and the pixels are to be adressed by img[x][y] this has the effect, that any loops acessing all pixels have to loop x first and then y so y is the faster changin index(for performance reasons). but when actually writing image data to a file, doing this will result in x and y being swapped, unless the loop order is swiched(sacreficing some performance).


int main(){
	Mat I2 = imread("../Franzi_CPD_012.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Rect myROI(0,0,1024,884);
	Mat I3 = I2(myROI);
	Size s = I3.size();
	
	fftw_complex* FI1 = fft_2d(I3);
	
	cutouteroval_ft(FI1,0.3,s.width,s.height);
	cutinneroval_ft(FI1,0.00125,s.width,s.height);
	
	Mat I4 = cv_ifft_2d_real(FI1,s.width,s.height);
	
	
	Mat I5(s.height,s.width,CV_64F);
	GaussianBlur(I4,I5,Size(7,7),1);
	
	normalize(I5,I5,1,0,32);
	Scalar meanI5 = mean(I5);
	clamp(I5,meanI5[0] * 0.85,1);
	
	normalize(I5,I5,255,0,32);
	
	
	
	vector<node*> list;
	new node(PX,PY,&list,&I5);
	
	unsigned long long i = 1;
	for (bool buisy = 1; buisy ;){
		buisy = 0;
		unsigned long long end = list.size();
		for (unsigned long long it = 0; it < end; ++it){
			if (!(list[it]->procreated)){
				list[it]->procreate();
				buisy = 1;
			}
		}
		cout << i++ << endl;
		
	}
	
	for (unsigned long long i = 0; i < list.size(); ++i){
		for (unsigned long long j = 0; j < list[i]->connections.size(); ++j){
			line(I5,Point(list[i]->x,list[i]->y),Point(list[i]->connections[j]->x,list[i]->connections[j]->y),255);
			
		}
	}
//	line(I5,Point(PX,PY),Point(499.64,413.84),255);
//	line(I5,Point(PX,PY),Point(2,2),255);
	imwrite("doubt.tif",I3);
	imwrite("doubt2.tif",I5);
	
	return 0;
}
