#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <ml.h>
#define SIZE 3

void main()
{
 IplImage* frame = NULL;
// CvHaarClassifierCascade* cascade = NULL;
 
 IplImage* gray = NULL;
 IplImage* face_img = NULL;
     const char *classifer = "C:\\Program Files\\opencv\\data\\haarcascades\\haarcascade_frontalface_default.xml"; 

    CvHaarClassifierCascade* cascade = 0; 

     cascade = (CvHaarClassifierCascade*) cvLoad(classifer, 0, 0, 0 ); 

 if(cascade == NULL)
 {
  printf("haarcascade_frontalface_alt2.xml Error");
 }
  
 cvNamedWindow( "frame", CV_WINDOW_AUTOSIZE );
 CvCapture* capture = cvCaptureFromCAM(0);
 int width = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH); 
 int height = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
 
 while(1)
 { 
  frame = cvQueryFrame( capture );
  
  double scale=1.3;
  CvMemStorage* storage = cvCreateMemStorage(0);

  gray = cvCreateImage(cvSize(frame->width, frame->height),8, 1);
  face_img = cvCreateImage(cvSize(cvRound(frame->width/scale),cvRound(frame->height/scale)),8, 1);

  cvCvtColor(frame,gray,CV_BGR2GRAY);//원영상 그래이 스캐일로 변환
  cvResize(gray, face_img,CV_INTER_LINEAR);//이미지 사이즈 조정
  cvEqualizeHist(face_img,face_img);//히스토그램 평탄화 수행

  IplImage* Igray;
  Igray =NULL;
  if(!Igray)
   Igray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
  cvCvtColor(frame, Igray, CV_BGR2GRAY); // 컬러를 흑백으로 변환
  int ws = Igray->widthStep;

  cvClearMemStorage(storage);
  CvSeq* objects = cvHaarDetectObjects(face_img,cascade,storage,1.1,10,CV_HAAR_FIND_BIGGEST_OBJECT,cvSize(30,30));


  for(int i=0;i<(objects ? objects->total:0);i++)
  {
   CvRect* r=(CvRect*)cvGetSeqElem(objects,i);
   CvPoint center;
   int radius;
   center.x = cvRound((r->x + r->width*0.5)*scale);
   center.y = cvRound((r->y + r->height*0.5)*scale);
   radius = cvRound((r->width + r->height)*0.25*scale);
   cvCircle(frame,center,radius,cvScalar(0,255,0),3,8,0);
  }
  cvFlip(frame,frame,1);
  cvShowImage( "frame", frame );
  cvWaitKey(1); 
 }
 cvWaitKey();
 cvReleaseImage(&frame);
 cvReleaseImage(&gray);
 cvReleaseImage(&face_img);
 cvReleaseCapture( &capture );
 cvDestroyAllWindows();
}
