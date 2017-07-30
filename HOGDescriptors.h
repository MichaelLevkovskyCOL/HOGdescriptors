…………..Fragment of the code………………………………..
//Functions to calculate HOG descriptors to track and draw a person trajectory
//Michael Levkovsky Mateus
//2012
// 
//After previously load the video, segment the video, using BoundingBox to enclose the person, loaded person characteristics, calculate error, and determine distance. The
//The main functions to calculate the HOG descriptors are shown below
 
//Libraries used// 
#include "cv.h" 
#include "highgui.h" 
#include "math.h" 
 
//**FUNCTIONS-ALGORITH FRAGMENT**//
//____________________________________________________________________
CvMat* caracHOG(IplImage* img, int ca, int cb);
   IplImage** calculateIntegralHOG(IplImage* in);
    CvMat* calculateHOG_window(IplImage** integrals,CvSize bloque, floatoverlap,int normalization);
      void calculateHOG_block(CvRect block, int num_bloque,CvMat*hog_block,IplImage** integrals, int normalization);
      IplImage** integrals;
        float mask[3]={1.0,0.0,-1.0};
         int total_caracteristicas, width, height, ca, cb,  aux;
          float b=0, minaux=0;
            double *min_val, *max_val;
             IplImage *img;

//____________________________________________________________________
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


//____________________________________________________________________
IplImage** calculateIntegralHOG(IplImage* in){ // Function that calculates HOG Integral 
   IplImage* img_gray = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U,1); cvCvtColor(in, img_gray, CV_BGR2GRAY); 
    cvEqualizeHist(img_gray,img_gray); 
      IplImage *xedge= cvCreateImage(cvGetSize(in), IPL_DEPTH_32F,1); 
        IplImage *yedge= cvCreateImage(cvGetSize(in), IPL_DEPTH_32F,1); 

/*Creation of Border Masks*/ 
CvMat* maskV = cvCreateMat(3,1,CV_32FC1); 
CvMat* maskH = cvCreateMat(1,3,CV_32FC1); 
cvSetData(maskV,mask,maskV->step); 
cvSetData(maskH,mask,maskH->step);
cvFilter2D(img_gray,xedge,maskH);
cvFilter2D(img_gray,yedge,maskV); 

cvReleaseImage(&img_gray); IplImage** bins = (IplImage**) malloc(9 * sizeof(IplImage*));
for (int i = 0; i < 9 ; i++) {bins[i] = cvCreateImage(cvGetSize(in), IPL_DEPTH_32F,1); cvSetZero(bins[i]);} 
int x, y; 
float temp_gradient, temp_magnitude; 
for (y = 0; y height; y++) { float* ptr1 = (float*) (xedge->imageData + y * (xedge->widthStep)); float* ptr2 = (float*) (yedge->imageData + y * (yedge->widthStep)); float** ptrs = (float**) malloc(9 * sizeof(float*));
for (int i = 0; i < 9 ;i++){ptrs[i] = (float*) (bins[i]->imageData + y * (bins[i]->widthStep));} 
for (x = 0; x width; x++) { if (ptr1[x] == 0){temp_gradient = ((atan(ptr2[x] / (ptr1[x] + 0.00001))) * (180/ CV_PI)) + 90;} 
else{temp_gradient = ((atan(ptr2[x] / ptr1[x])) * (180 / CV_PI)) + 90;} temp_magnitude = sqrt((ptr1[x] * ptr1[x]) + (ptr2[x] * ptr2[x]));
if (temp_gradient <= 20) {ptrs[0][x] = temp_magnitude;} 
else if (temp_gradient <= 40) {ptrs[1][x] = temp_magnitude;} 
else if (temp_gradient <= 60) {ptrs[2][x] = temp_magnitude;} 
else if (temp_gradient <= 80) {ptrs[3][x] = temp_magnitude;} 
else if (temp_gradient <= 100) {ptrs[4][x] = temp_magnitude;} 
else if (temp_gradient <= 120) {ptrs[5][x] = temp_magnitude;} 
else if (temp_gradient <= 140) {ptrs[6][x] = temp_magnitude;} 
else if (temp_gradient <= 160) {ptrs[7][x] = temp_magnitude;} else {ptrs[8][x] = temp_magnitude;} } } for (int i = 0; i <9 ; i++){cvIntegral(bins[i], integrals[i]);} 
for (int i = 0; i <9 ; i++){cvReleaseImage(&bins[i]);} 
cvReleaseImage(&xedge); 
cvReleaseImage(&yedge); 
cvReleaseMat(&maskV); 
cvReleaseMat(&maskH); 
return (integrals); } 

//____________________________________________________________________
CvMat* calculateHOG_window(IplImage** integrals,CvSize blq, float overlap,int normalization){//HOG descriptors cells
int width_blq = blq.width;
int height_blq = blq.height;
int width_overlap = overlap * width_blq; 
int height_overlap = overlap * height_blq; 
int nbins = 9; 
int width_vent = (integrals[0]->width) - 1; 
int height_vent = (integrals[0]->height) - 1; 
total_properties = (((width_vent - width_blq)/(width_blq - width_overlap))+1)* (((height_vent - height_blq)/(height_blq - height_overlap))+1)*nbins; 
int num_blq=0; 
    CvMat* features = cvCreateMat(1,total_properties,CV_32FC1);
      for (int i=0; i <= height_vent - height_blq; i += (height_blq - height_overlap)){ 
         for (int j=0; j <= width_vent - width_blq;
          j += (width_blq - width_overlap)){ CvRect block = cvRect(j,i,width_blq,height_blq); calculateHOG_block(block,num_blq,features,integrals,normalization); num_blq++;} } return features; } 

//____________________________________________________________________
void calculateHOG_block(CvRect block, int num_bloque,CvMat* hog_block,IplImage** integrals, int normalization){// HOG descriptors calculation
 CvMat* aux = cvCreateMat(1,9,CV_32FC1);// 1 x 9 Mtraix  32 bits float 1CH
  for (int i=0; i < 9; i++){ 
   double a = cvGetReal2D(integrals[i],block.y,block.x); 
   double b = cvGetReal2D(integrals[i],block.y + block.height, block.x + block.width); 
   double c = cvGetReal2D(integrals[i],block.y + block.height, block.x); 
   double d = cvGetReal2D(integrals[i],block.y, block.x + block.width); 
cvSetReal2D(aux,0,i,((a + b)-(c + d)));} 
if (normalization != -1){cvNormalize(aux, aux, 1, 0, normalization);} 
for (int j=0; j < 9; j++) {cvSetReal2D(hog_block,0,num_bloque*9+j,cvGetReal2D(aux,0,j));}}
