#include <cstdio>

using namespace std;

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <ml.h>

IplImage* frame, *image, *gray, *image_bgra_ar, *image_bgr_gl, *face_img;
CvMat* dft;
CvCapture* capture = 0;
const char *classifer = "haarcascades\\haarcascade_frontalface_default.xml"; 

CvHaarClassifierCascade* cascade = NULL; 


float win_w, win_h;
int x,y, s;

#include <gl/gl.h>     // The GL Header File
#include <gl/glu.h>
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header

typedef struct {
	int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

glutWindow win;

float g_rotation = 0;
float g_rotation_speed = 2.0f;


float xrot = 0.0f;

void init(){

	printf("init()\n");

	capture = cvCaptureFromCAM(CV_CAP_ANY);
	frame = cvQueryFrame( capture );
	
	printf("- frame\n");
	
	if( frame ) {
		if( !image )
		{
			CvSize s = cvGetSize(frame);
			image = cvCreateImage(s , 8, 3 );
			image->origin = frame->origin;
			image_bgra_ar = cvCreateImage(s,8,4);
			image_bgra_ar->origin = frame->origin;
			
			gray = cvCreateImage(s,8,1);
			gray->origin = frame->origin;
			image_bgr_gl = cvCreateImage(s,8,3);
		}
		win_w = frame->width;
		win_h = frame->height;
	}
	
	printf("- set OpenGL\n");

	   // select projection matrix
	glMatrixMode(GL_PROJECTION);												

	// set the viewport
	glViewport(0, 0, win.width, win.height);									

	// set matrix mode
	glMatrixMode(GL_PROJECTION);												

	// reset projection matrix
	glLoadIdentity();															
	GLfloat aspect = (GLfloat) win.width / win.height;

	// set up a perspective projection matrix
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);		

	// specify which matrix is the current matrix
	glMatrixMode(GL_MODELVIEW);													
	glShadeModel( GL_SMOOTH );

	// specify the clear value for the depth buffer
	glClearDepth( 1.0f );														
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	// specify implementation-specific hints
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
	glShadeModel( GL_SMOOTH );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	glClearColor(0.0, 0.0, 0.0, 1.0);

}

void display ( void )   // Create The Display Function
{
	
	int c;
	bool doMult = false;

	frame = cvQueryFrame( capture );
	if( frame ) {
		cvCopy( frame, image);
		cascade = (CvHaarClassifierCascade*) cvLoad(classifer, 0, 0, 0 ); 
		if(cascade == NULL)
		{
			printf("haarcascade_frontalface_alt2.xml File Error.");
		}

		CvMemStorage* storage = cvCreateMemStorage(0);
		double scale=1.3;
  		//영상준비
		gray = cvCreateImage(cvSize(frame->width, frame->height),8, 1);
		face_img = cvCreateImage(cvSize(cvRound(frame->width/scale),cvRound(frame->height/scale)),8, 1);

  cvCvtColor(frame,gray,CV_BGR2GRAY); // Origin to Gray
  cvResize(gray, face_img,CV_INTER_LINEAR); // Image Resize
  cvEqualizeHist(face_img,face_img); // Histogram

  IplImage* Igray;
  Igray =NULL;
  if(!Igray)
  	Igray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
  cvCvtColor(frame, Igray, CV_BGR2GRAY); // Color to Gray
  int ws = Igray->widthStep;

  //Find Object
  cvClearMemStorage(storage);
  CvSeq* objects = cvHaarDetectObjects(face_img,cascade,storage,1.1,10,CV_HAAR_FIND_BIGGEST_OBJECT,cvSize(30,30));

  //Box Drawing
  for(int i=0;i<(objects ? objects->total:0);i++)
  {
  	CvRect* r=(CvRect*)cvGetSeqElem(objects,i);
  	CvPoint center;
  	int radius;
  	center.x = cvRound((r->x + r->width*0.5)*scale);
  	center.y = cvRound((r->y + r->height*0.5)*scale);
  	x=center.x;
  	y=center.y;
  	radius = cvRound((r->width + r->height)*0.25*scale);
  	s=radius;
  	printf("w=%f,   h=%f\n",win_w, win_h);
  	cvCircle(frame,center,radius,cvScalar(0,255,0),3,8,0);
  }
  cvFlip(frame,frame,1); // Flip


  cvCvtColor(image,gray,CV_RGB2GRAY);
  cvCvtColor(gray,image_bgra_ar,CV_GRAY2BGRA);	
  cvCvtColor(frame,image_bgr_gl,CV_RGB2BGR);
  cvFlip(image_bgr_gl);
}

	////////////////////////////////////////////////////////////////
	// Clear Screen And Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0,win_w, 0.0,win_h);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	 glRasterPos2i(10, 100);    //////////////*****************

	 glDrawPixels(win_w,win_h,GL_RGB,GL_UNSIGNED_BYTE,image_bgr_gl->imageData);
	 glMatrixMode(GL_PROJECTION);
	 glPopMatrix();
	 glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();

	 glEnable(GL_DEPTH_TEST);
	 glDepthFunc(GL_LEQUAL);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();


	 glEnable(GL_LIGHTING);
	 glEnable(GL_LIGHT0);

    ////////////////////////////////////////////////////////





	// Clear Screen and Depth Buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     
	 glLoadIdentity();

	// Define a viewing transformation
	 gluLookAt( 4,2,0, 0,0,0, 0,1,0);					  

	// Push and pop the current matrix stack. 
	// This causes that translations and rotations on this matrix wont influence others.

	 glPushMatrix();										
	 glColor3f(1,0,0);
	 glTranslatef(0,0,-1.5);
	 glScalef((float)s/80,(float)s/80,(float)s/80);

	 if(280<=x && 360>=x){
	 	glRotatef(x,0,1,0);
	 	glRotatef((float)y/2+240,0,0,1);}
	 	else{
	 		glRotatef(0,0,0,1);
	 		glRotatef(x,0,1,0);}
			// Draw the teapot
	 		glutSolidTeapot(1);
	 		glPopMatrix();										  


	 		g_rotation += g_rotation_speed;
			//printf("%f", g_rotation);
	 		glutSwapBuffers();

	 	}

void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
}


int main(int argc,char* argv[])
{
	// set window values
	win.width = 1366;
	win.height = 768;
	win.title = "OpenGL/GLUT Example. Visit http://openglsamples.sf.net ";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);				// set window size
	glutCreateWindow(win.title);					// create Window
	glutDisplayFunc(display);						// register Display Function
	glutIdleFunc( display );						// register Idle Function
	//    glutKeyboardFunc( keyboard );						// register Keyboard Handler
	init();
	glutMainLoop();							// run GLUT mainloop
	
	cvWaitKey();

	cvReleaseCapture(&capture);
	cvReleaseImage(&image);
	cvReleaseImage(&frame);
	cvReleaseImage(&gray);
	cvReleaseImage(&image_bgra_ar);
	cvReleaseImage(&image_bgr_gl);
	cvReleaseImage(&face_img);
	
	cvDestroyAllWindows();


	
	return 0;

}

