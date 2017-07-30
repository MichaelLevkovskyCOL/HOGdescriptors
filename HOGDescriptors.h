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



IplImage** calculateIntegralHOG(IplImage* in){ // Function that calculates HOG Integral 
IplImage* img_gray = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U,1); cvCvtColor(in, img_gray, CV_BGR2GRAY); 
cvEqualizeHist(img_gray,img_gray); 
IplImage *xedge= cvCreateImage(cvGetSize(in), IPL_DEPTH_32F,1); 
IplImage *yedge= cvCreateImage(cvGetSize(in), IPL_DEPTH_32F,1); 

/*Creacion mascaras para bordes*/ 
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
CvMat* calculateHOG_window(IplImage** integrals,CvSize bloque, float overlap,int normalization){//Celdas de los descriptores HOG 
int ancho_bloque = bloque.width;
 int alto_bloque = bloque.height;
 int ancho_overlap = overlap * ancho_bloque; 
int alto_overlap = overlap * alto_bloque; 
int nbins = 9; 
int ancho_ventana = (integrals[0]->width) - 1; 
int alto_ventana = (integrals[0]->height) - 1; 
total_properties = (((ancho_ventana - ancho_bloque)/(ancho_bloque - ancho_overlap))+1)* (((alto_ventana - alto_bloque)/(alto_bloque - alto_overlap))+1)*nbins; 
 int num_bloque=0; 
CvMat* features = cvCreateMat(1,total_properties,CV_32FC1);
 for (int i=0; i <= alto_ventana - alto_bloque; i += (alto_bloque - alto_overlap)){ 
for (int j=0; j <= ancho_ventana - ancho_bloque;
 j += (ancho_bloque - ancho_overlap)){ CvRect block = cvRect(j,i,ancho_bloque,alto_bloque); calculateHOG_block(block,num_bloque,features,integrals,normalization); num_bloque++;} } return features; 
cvReleaseMat(&features); } 

void calculateHOG_block(CvRect block, int num_bloque,CvMat* hog_block,IplImage** integrals, int normalization){// Calculo descriptores HOG 
CvMat* aux = cvCreateMat(1,9,CV_32FC1);//matrix de 1 fila y 9 col de 32 bits float con 1 canal for (int i=0; i < 9; i++){ double a = cvGetReal2D(integrals[i],block.y,block.x); 
double b = cvGetReal2D(integrals[i],block.y + block.height, block.x + block.width); 
double c = cvGetReal2D(integrals[i],block.y + block.height, block.x); 
double d = cvGetReal2D(integrals[i],block.y, block.x + block.width); 
cvSetReal2D(aux,0,i,((a + b)-(c + d)));} 
if (normalization != -1){cvNormalize(aux, aux, 1, 0, normalization);} 
for (int j=0; j < 9; j++) {cvSetReal2D(hog_block,0,num_bloque*9+j,cvGetReal2D(aux,0,j));} cvReleaseMat(&aux);}
