#include <cstdio>

#include <windows.h>

#include <cv.h>
#include <highgui.h>
//#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <math.h>

#define IMGARR(x, y, widthStep) ((y)*(widthStep) + (x)*3)
#define IMGARR1(x, y, widthStep) ((y)*(widthStep) + (x))

#define TRANS2RAD 0.01745329

#define MD_STAY 0
#define MD_MOVE 1
#define MD_SCROLL 2
#define MD_LCLICK 3
#define MD_RCLICK 4
#define MD_DSCROLL 5
#define MD_USCROLL 6
#define MD_PREV 7
#define MD_NEXT 8
#define MD_FUNCON 9
#define MD_TYPE 10
#define MD_MOVETAB 11
#define MD_MOVEWINDOW 12
#define MATCHPOINT 20

#define DEBUG_NEW new(THIS_FILE, __LINE__)


int flame_count = 0;
int func_count = 0;
int dif_distance = 0;
int finger_count = 0;
bool enter_move = false; 
bool enter_scroll = false;
bool enter_drag = false;
bool func_flag = false;
bool sound_flag = true;
bool temp_func_flag;
int start_flag;
int fn_match_cnt = 0;

int lclick_cnt = 0;
int rclick_cnt = 0;
int prev_cnt = 0;
int next_cnt = 0;
int uscr_cnt = 0;
int dscr_cnt = 0;
int move_cnt = 0;
int movetab_cnt = 0;
int movewindow_cnt = 0;
int move_snd_cnt = 0;
int upscr_snd_cnt = 0;
int dnscr_snd_cnt = 0;
int fn_snd_cnt = 0;

int cntDefPx = 0;


CvFont* font = 0;
CvFont f;

CvPoint p = {30,30};
CvPoint p2 = {200,30};
CvPoint fingertip;
CvPoint beforeFingerTip;
char a[10];
int count1 = 0;
CvPoint avgCenter1;
CvPoint avgCenter2;

double alpCenter = 0.6;
double max_area = 0;

POINT screenP;

const int definedValue = 255; 
const int backValue = 0;


void deleteEdge(IplImage* sourceImage, IplImage* edgeImage);
void NoiseReduction(IplImage* img,int range,float rate);
void ch3To1ch(IplImage *image, IplImage *dst_image);
void separateFinger(IplImage *image, CvPoint center);
void copy1chTo3ch(IplImage *image, IplImage *dst_image2);
void getFingerCount(IplImage *image);




//new 함수
CvPoint getCenter(IplImage* oriImg);



IplImage *finger_part=0;

void main()
{

	IplImage* image = 0;
	IplImage *dst_image = 0;
	IplImage *dst_image2=0;
	IplImage *dst_image3=0;
	IplImage *dst_image4=0;
	IplImage *sobel_image=0;
	IplImage *remove_noise = 0;
	
	IplImage *Y_plane =0;
	IplImage* Cr_plane =0; 
	IplImage* Cb_plane = 0; 
	IplImage *pYCbCrImage =0;

	IplImage *gray_image=0;
	IplImage *vertical_edge=0;
	IplImage *horizontal_edge=0;
	IplImage *tmp_image1 = 0;
	IplImage *tmp_image2 = 0;

	IplImage* Half1=0;
	IplImage* Half2=0;
	IplImage* cap1=0;
	IplImage* cap2=0;

	CvPoint dot1, dot2;
	int speed=0;


	CvCapture* capture = cvCaptureFromCAM(1);

	cvNamedWindow("Hand", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Finger_Part",0);
	cvNamedWindow("check",CV_WINDOW_AUTOSIZE);


	while(1) {
		image = cvQueryFrame( capture );


		cvFlip(image, 0, 1); 
		if(dst_image2==NULL)
			dst_image2 = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); // 1채널 이미지 생성

		if(dst_image4==NULL)
			dst_image4 = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3); // 1채널 이미지 생성

		if(Y_plane==NULL)
			Y_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1); // 1채널 이미지 생성

		if(Cr_plane==NULL)
			Cr_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 ); 

		if(Cb_plane==NULL)
			Cb_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 ); 

		if(pYCbCrImage==NULL)
			pYCbCrImage = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);

		if(dst_image3==NULL)
			dst_image3 = cvCreateImage(cvGetSize(image), 8, 3);

		if(remove_noise==NULL)
			remove_noise = cvCreateImage(cvGetSize(image), 8, 3);

		if(Half2==NULL)
			Half2 = cvCreateImage(cvGetSize(image), 8, 1);

		if(Half1==NULL)
			Half1 = cvCreateImage(cvGetSize(image), 8, 1);

		cvCopy(image,dst_image4);
		cvFlip(dst_image4, 0, 1); // 이미지 좌우 반전

		int widthStep = image->widthStep; 
		
		CvPoint center1;
		CvPoint center2;

		unsigned char* img = (unsigned char*)image->imageData;
		unsigned char* dst_img = (unsigned char*)dst_image2->imageData;
		unsigned char Cb_dt=0, Cr_dt=0;
		
		int width = image->width;
		int height = image->height;
		
		BYTE r, g, b;

		dot1.x=width/2;
		dot1.y=0;
		dot2.x=width/2;
		dot2.y=height;
		
		if(finger_part==NULL)
			finger_part = cvCreateImage(cvSize(width/2, height), 8, 1);
		
		if(gray_image==NULL)
			gray_image = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1);

		if(dst_image==NULL)
			dst_image = cvCreateImage( cvGetSize(gray_image),IPL_DEPTH_32F, gray_image->nChannels );

		if(sobel_image==NULL)
			sobel_image = cvCreateImage( cvGetSize(gray_image),IPL_DEPTH_32F, gray_image->nChannels );

		if(vertical_edge==NULL)
			vertical_edge = cvCreateImage( cvGetSize(gray_image),IPL_DEPTH_32F,1); 
		
		if(horizontal_edge==NULL)
			horizontal_edge = cvCreateImage( cvGetSize(gray_image),IPL_DEPTH_32F,1); 

		cvCvtColor(image, gray_image, CV_BGR2GRAY );


		// init
		int method =2;

		//  cvSobel()
		cvSobel( gray_image, vertical_edge, 1, 0, 3 ); // vertical
		cvSobel( gray_image, horizontal_edge, 0, 1, 3 ); // horizontal
		

		// cvConertScaleAbs()  IPL_DEPTH_32F -> IPL_DEPTH_8U
		if( method == 0 ) cvConvertScaleAbs( vertical_edge, dst_image, 1, 0 );
		else if( method == 1 ) cvConvertScaleAbs( horizontal_edge, dst_image, 1, 0 );
		else if( method == 2 ) 
		{
			if(tmp_image1==NULL)
				tmp_image1 = cvCreateImage( cvGetSize(vertical_edge),IPL_DEPTH_8U,1);

			if(tmp_image2==NULL)
				tmp_image2 = cvCreateImage( cvGetSize(horizontal_edge),IPL_DEPTH_8U,1);

			cvConvertScaleAbs( vertical_edge, tmp_image1, 1, 0 );
			cvConvertScaleAbs( horizontal_edge, tmp_image2, 1, 0 );
			
			cvAdd( tmp_image1, tmp_image2, dst_image, NULL );
			
		}
		

		cvCopy(dst_image,sobel_image);

		cvCvtColor(image,pYCbCrImage,CV_RGB2YCrCb);

		cvCvtPixToPlane(pYCbCrImage,Y_plane,Cr_plane,Cb_plane,0);

		for(int y = 0; y < height; y++)
		{

			for(int x = 0; x < width; x++)
			{
					b = img[IMGARR(x, y, widthStep)]; // Blue
					g = img[IMGARR(x, y, widthStep)+1]; // Green
					r = img[IMGARR(x, y, widthStep)+2]; // Red
					
					if(r < 10) // Filter 1
					{
						img[IMGARR(x, y, widthStep)] = 0;
						img[IMGARR(x, y, widthStep)+1] = 0;
						img[IMGARR(x, y, widthStep)+2] = 0;
						continue;
					}
					
					if(r-5 < g || g+15 < b) // Filter 2
					{
						img[IMGARR(x, y, widthStep)] = 0;
						img[IMGARR(x, y, widthStep)+1] = 0;
						img[IMGARR(x, y, widthStep)+2] = 0;
						continue;
					}
					
					if( (r-g) + 20 < (g-b) ) // Filter 3
					{
						img[IMGARR(x, y, widthStep)] = 0;
						img[IMGARR(x, y, widthStep)+1] = 0;
						img[IMGARR(x, y, widthStep)+2] = 0;
						continue;
					}
					
					img[IMGARR(x, y, widthStep)] = 255; // Blue
					img[IMGARR(x, y, widthStep)+1] = 0;
					img[IMGARR(x, y, widthStep)+2] = 0;
					
				}
			}
			

			for(int y = 0; y < height; y++)
			{
				for(int x = 0; x < width; x++)
				{
					if(img[IMGARR(x, y, widthStep)] == 255 && img[IMGARR(x, y, widthStep)+1] == 0
						&& img[IMGARR(x, y, widthStep)+2] == 0)
						
					{
						Cr_dt = Cr_plane->imageData[IMGARR1(x, y, Cr_plane->widthStep)];
						Cb_dt = Cb_plane->imageData[IMGARR1(x, y, Cb_plane->widthStep)];

						if ((108<=Cb_dt ) && (Cb_dt <= 138) ) 
						{
							img[IMGARR(x, y, widthStep)] = 0;
							img[IMGARR(x, y, widthStep)+1] = 0;
							img[IMGARR(x, y, widthStep)+2] = 0;
							continue;
						}
						else 
						{
							img[IMGARR(x, y, widthStep)] = 255; // Blue 색으로 칠함
							img[IMGARR(x, y, widthStep)+1] = 0;
							img[IMGARR(x, y, widthStep)+2] = 0;
						}
						
						
					}
					
				}
			}
			
			
		cvCopy(image, remove_noise);


		NoiseReduction(remove_noise, 15, 0.5f);
		
		cvZero(dst_image2); 
		ch3To1ch(remove_noise, dst_image2);

		IplConvKernel *element = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_ELLIPSE, NULL); 
		
		cvDilate(dst_image2, dst_image2, element); 
		
		cvErode(dst_image2, dst_image2, element, 1); 
		cvErode(dst_image2, dst_image2, element, 1);
		cvErode(dst_image2, dst_image2, element, 1); 
		
		cvDilate(dst_image2, dst_image2, element);
		
		
		cvFlip(dst_image2, 0, 1); 
		
		deleteEdge(dst_image2, sobel_image);
		
		copy1chTo3ch(dst_image3, dst_image2);
		


		cvZero(Half1);
		cvCopy(dst_image2, Half1);
		cvZero(Half2);
		cvCopy(dst_image2, Half2);

		cvSetImageROI(Half1, cvRect(0,0, width/2, height)); 

		if(cap1==NULL)
			cap1 = cvCreateImage(cvSize(width/2,height), 8, 1);
		
		cvCopy(Half1, cap1);
		center1 =  getCenter(cap1); 
		
		separateFinger(cap1,avgCenter1);

		cvShowImage("check", finger_part); 

		cvDilate(finger_part, finger_part, element); 
		cvDilate(finger_part, finger_part, element);
		cvDilate(finger_part, finger_part, element); 

		getFingerCount(finger_part);
		
		printf("%d    ", finger_count);
		if(avgCenter1.x == 0 && avgCenter1.y == 0)
		{
			avgCenter1 = center1;
		}
		else
		{
			avgCenter1.x = int(center1.x * alpCenter + avgCenter1.x * (1-alpCenter));
			avgCenter1.y = int(center1.y * alpCenter + avgCenter1.y * (1-alpCenter)); 
		}
		
		if(avgCenter1.x>30){
			cvCircle(dst_image3, avgCenter1, 7, CV_RGB(255, 0, 0), 2, 2, 0);
		}

					
		cvSetImageROI(Half2, cvRect(width/2 ,0,width/2, height)); 
		if(cap2==NULL)
			cap2 = cvCreateImage(cvSize(width/2, height), 8, 1);

		cvCopy(Half2, cap2);
		center2 =  getCenter(cap2); 
		
		if(avgCenter2.x == 0 && avgCenter2.y == 0)
		{
			avgCenter2 = center2;
		}
		else
		{
			avgCenter2.x = int(center2.x * alpCenter + avgCenter2.x * (1-alpCenter));
			avgCenter2.y = int(center2.y * alpCenter + avgCenter2.y * (1-alpCenter)); 
		}
		
		avgCenter2.x=avgCenter2.x+width/2-avgCenter1.x;
		if(avgCenter2.x>(width/2)+20){
			cvCircle(dst_image3, avgCenter2, 7, CV_RGB(255, 0, 0), 2, 2, 0);
		}

		if(beforeFingerTip.x == 0 && beforeFingerTip.y == 0)
		{
			beforeFingerTip = fingertip;
		}
		else
		{
			beforeFingerTip.x = int(fingertip.x * alpCenter + beforeFingerTip.x * (1-alpCenter));
			beforeFingerTip.y = int(fingertip.y * alpCenter + beforeFingerTip.y * (1-alpCenter)); 
		}
		
		cvResetImageROI(Half2);
		cvResetImageROI(Half1);

		cvResetImageROI(cap1);
		cvResetImageROI(cap2);

	
		cvLine(dst_image3, avgCenter1, avgCenter2, CV_RGB(255, 100, 0), 8, 8);//중간 선
		
		cvLine(dst_image3, dot1, dot2, CV_RGB(255, 255, 0), 8, 8);	//왼쪽 오른쪽
		cvShowImage("Hand", dst_image3); // 이미지 보이기
		
		cvZero(dst_image3);

		cvZero(finger_part);
		
		cvZero(image);
		copy1chTo3ch(image, dst_image2);
		
		cvZero(dst_image2);
		
		cvShowImage("Finger_Part", dst_image4);  //image 있었음
		
		if( cvWaitKey(10) >= 0 )
			break;

	}

	cvReleaseImage(&dst_image);	
	cvReleaseImage(&dst_image2);
	cvReleaseImage(&dst_image3);
	cvReleaseImage(&dst_image4);
	cvReleaseImage(&sobel_image);
	cvReleaseImage(&remove_noise);
	cvReleaseImage(&Y_plane);
	cvReleaseImage(&Cr_plane);
	cvReleaseImage(&Cb_plane);
	cvReleaseImage(&pYCbCrImage);
	cvReleaseImage(&gray_image);
	cvReleaseImage( &vertical_edge );
	cvReleaseImage( &horizontal_edge );
	cvReleaseImage( &tmp_image1 );
	cvReleaseImage( &tmp_image2 );
	cvReleaseImage( &finger_part );
	cvReleaseImage( &Half1 );
	cvReleaseImage( &Half2 );
	cvReleaseImage( &cap1 );
	cvReleaseImage( &cap2 );

	cvReleaseCapture( &capture );

	cvDestroyWindow( "Hand" );
	cvDestroyWindow( "Finger_Part" );
	cvDestroyWindow( "check" );


}

