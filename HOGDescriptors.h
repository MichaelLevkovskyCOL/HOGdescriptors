…………..Fragment of the code………………………………..
#include "cv.h" 
#include "highgui.h" 
#include "math.h" 
……………………………………………………………………...
CvMat* caracHOG(IplImage* img, int ca, int cb){ // Function that returns HOG descriptors
IplImage* SecondIm = cvCreateImage(cvSize(128,64),img->depth,img- >nChannels); 
cvResize(img,SecondIm,CV_INTER_LINEAR);
integrals = (IplImage**) malloc(9 * sizeof(IplImage*)); 
for (int i = 0; i < 9 ; i++) {
integrals[i] = cvCreateImage(cvSize(SecondIm->width + 1, SecondIm->height + 1), IPL_DEPTH_64F,1);} 
integrals= calculateIntegralHOG(SecondIm); 
CvMat* properties = calculateHOG_window(integrals,cvSize(ca,cb),0,CV_L2); 
for (int i=0; i < total_properties; i++){
double value = cvGetReal2D(properties, 0, i);} 
cvReleaseImage( &SecondIm ); 
for (int i = 0; i <9 ; i++){cvReleaseImage(&integrals[i]);} 
return properties; }



