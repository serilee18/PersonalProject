#include <cv.h>
#include <highgui.h>
#include <stdlib.h>

void swap(char *n1, char *n2)
{
	char temp = *n1;
	*n1 = *n2;
	*n2 = temp;  
}
void swap1(int *n1, int *n2)
{
	int temp1 = *n1;
	*n1 = *n2;
	*n2 = temp1;  
}

void main()
{
	int n,m,p=0,q=0,t=0,z;
	int j,k=0;
	int i=1;
	IplImage* cap;
	IplImage* comp;

	int nContour;

	int threshold = 128; // Set Threshold
	IplImage* image = 0;
	IplImage* dstImage = 0;
	IplImage* dstImage1 = 0;
	IplImage* gray = 0;
	IplImage* C=0;
	IplImage* temp=0;
	CvCapture* capture = cvCaptureFromCAM(1); // Connect Camera


	cvNamedWindow( "T9-output", CV_WINDOW_AUTOSIZE ); // Result Window

	CvPoint offset=cvPoint(0,0);

	double min, max;
	CvPoint left_top;


	char path[100] = "c:\\num\\";

	char imageFilename[100]; 

	struct imgSt{
		char name[20];
		char file[100];
		int recX;
		int recY;
	};

	struct imgSt img[100] = {
		{ "0", "num0.bmp"},
		{ "1", "num1.bmp"},
		{ "2", "num2.bmp"},
		{ "3", "num3.bmp"},
		{ "4", "num4.bmp"},
		{ "5", "num5.bmp"},
		{ "6", "num6.bmp"},
		{ "7", "num7.bmp"},
		{ "8", "num8.bmp"},
		{ "9", "num9.bmp"},
	};

	struct imgSt oder[100] = {};

	while(1) {
		cvGrabFrame( capture );
		image = cvRetrieveFrame( capture );
		if(!dstImage){
			gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // Gray
			dstImage = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // Gray 
			dstImage1 = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1); // Gray  
		}      
		cvCvtColor(image, gray, CV_RGB2GRAY); // Color to Gray

		if( cvWaitKey(10) >= 0 )
			break;        



	cvThreshold(gray, dstImage, 100, 255, CV_THRESH_BINARY_INV); 
	dstImage->origin = image->origin; // Refine

	cvSmooth(dstImage,dstImage,CV_GAUSSIAN,5,5,0,0); //make it good
	cvCanny(dstImage, dstImage, 50, 100, 3);
	cvCopy(dstImage, dstImage1);


	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq *first_contour = 0;

	int header_size=sizeof(CvContour);
	int mode=CV_RETR_EXTERNAL;
	int method=CV_CHAIN_APPROX_SIMPLE;

	nContour = cvFindContours(dstImage, storage, &first_contour, header_size, mode, method, offset );
	printf("nContour=%d\n", nContour);
	k=0;
	p=0;
	q=0;
	for( ; first_contour != 0 ; first_contour = first_contour->h_next)
	{
		CvRect rect = cvBoundingRect(first_contour, 0);

		oder[k].recX=rect.x;
		oder[k].recY=rect.y;

	if(rect.width > (image->width/5) || rect.height > (image->height/5)) // Remove Too Big Image 
		continue;
	if(rect.width < (image->width/30) || rect.height < (image->height/20)) // Remove Too Small Image
		continue;


	cvRectangle(image, cvPoint(rect.x,rect.y), cvPoint( rect.x+rect.width,rect.y+rect.height), CV_RGB( 255, 0, 0), 2, 8, 0);
	p++;
	cap = cvCreateImage(cvSize(rect.width,rect.height), IPL_DEPTH_8U, 1);
	cvZero(cap);//

	cvSetImageROI(dstImage1, cvRect(rect.x,rect.y, rect.width, rect.height)); //ROI

	cvCopy(dstImage1, cap);

	for (j=0 ; j<10; ++j)
	{
		sprintf(imageFilename,"%s%s",path,img[j].file);
		comp = cvLoadImage(imageFilename,0);


		temp = cvCreateImage( cvGetSize(comp), IPL_DEPTH_8U, 1 );

		cvThreshold(comp,comp, 50,  255, CV_THRESH_BINARY);		// Image to Binary Image, Image reversal
		cvSmooth(comp,comp,CV_GAUSSIAN,5,5,0,0); // Gaussian Smooth
		cvCanny(comp, comp, 50, 100, 3);
		cvResize(cap, temp, CV_INTER_LINEAR);


		C = cvCreateImage( cvSize(temp->width - comp->width+1, temp->height - comp->height+1 ), IPL_DEPTH_32F, 1 );	

		cvMatchTemplate(temp, comp, C,CV_TM_CCOEFF_NORMED);	// Find Correlation coefficient.  CV_TM_CCORR_NORMED, CV_TM_CCOEFF_NORMED

		cvMinMaxLoc(C, &min, &max, NULL, &left_top,0);	// Max Correlation Coefficient

		printf("%f\n", max);
		if(max > 0.2)
		{
			cvRectangle(image,cvPoint(rect.x,rect.y), cvPoint( rect.x+rect.width,rect.y+rect.height), CV_RGB(0,255,0),5,0,0);
			printf("Detected Number is %s\n",img[j].name);//img[i]
			//	strcpy(oder[k].name,img[j].name);
			strcpy(oder[k].name,img[j].name);
			k++	;
			q++;
			break;
		}

		z=rect.height;
	}
	cvShowImage( "T9-output", image ); 
	printf("\n");		 
	cvResetImageROI(cap);
	cvResetImageROI(dstImage1);

}

	if(q>3){
		if(q==p)
			break;
			cvReleaseMemStorage(&storage);
	}
	printf("Template Number Print");
	for(k=0;k<q;k++)
		printf("%s  Y=%d   ", oder[k].name,oder[k].recY);

	printf("\n");
	/* 
	printf("\n Reverse Print");

	for(n=q-1;n>=0;n--)	
	printf("Number %s Y=%d ", oder[n].name,oder[n].recY);
	*/

	for(n=0;n<q;n++){
		for(m=0;m<q;m++){
			if(oder[n].recX < oder[m].recX){
				swap(oder[n].name, oder[m].name);
				swap1(&oder[n].recX, &oder[m].recX);
				swap1(&oder[n].recY, &oder[m].recY);
			}
		}
	}


	for(k=0;k<q;k++)
		t=t+oder[k].recY;

	t=t/q;

	printf("%d", z);
	printf("\n Number is \n");
	i=q-1;
	if(oder[0].recY-(z/5) <= oder[i].recY && oder[0].recY+(z/5) >= oder[i].recY){
		printf("\n 1st \n");
		for(n=0;n<q;n++)
			printf("%s  ", oder[n].name);
	}


	else{
		printf("\n 2nd \n");
		for(k=0;k<q;k++){
			if(t>oder[k].recY){	
				printf("%s ", oder[k].name);}
		}  
		for(k=0;k<q;k++){
			if(t<oder[k].recY)
				printf("%s ", oder[k].name);
		} 

	system("Pause") ;

	cvResetImageROI(cap);
	cvResetImageROI(dstImage1);

	cvReleaseImage( &gray );
	cvReleaseImage( &dstImage);
	cvReleaseImage( &cap);
	cvReleaseImage( &comp);
	cvReleaseImage( &image);
	cvReleaseImage( &dstImage1);
	cvReleaseImage( &C);
	cvReleaseImage( &temp);

	cvReleaseCapture( &capture );

	cvDestroyWindow( "T9-output" );
} 
