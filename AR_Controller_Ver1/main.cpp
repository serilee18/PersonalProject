#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <cstdio>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/video.h>
#include <AR/matrix.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <gl\glut.h>

#include "object.h"

#define COLLIDE_DIST 30000.0
#define DEGINRAD 3.14/180

/* Object Data */
char            *model_name = "Data/object_data11";
ObjectData_T    *object;
int             objectnum;
char		 quote[20][80];
int             xsize, ysize;
int				thresh = 100;
int             count = 0;
double			x,y,z;
double			x1,y11,z1, pre_z;//RotateZ=0;
double			Pos[2]={1,1};			
double			rot1[3][3];
double          trans[3][4];
int				r=100;
int				numb=0;
int				a=0,b=0, c=0;
int				Rotate=0, number=0, scale=0;
double			ScalX=1, scalY=1, scalZ=1; int scalXYZ=0;
double			RotateX=0, RotateY=0, RotateZ=0, pre_RotateX=0,pre_RotateY=0,pre_RotateZ=0,RotatZ=0;; 
int RotatXYZ=0, sum_RotateZ=0;
int				w=0, e=0, t=0, p=0;
double			letter=0;
double			pre_scaleY=0,pre_scaleX=0,pre_scaleZ=0,  pre_sum_scaleX=15,pre_sum_scaleY=15,pre_sum_scaleZ=15;
double			sum_scaleX=0,sum_scaleZ=0,sum_scaleY=0,  scaleX=1,dif_scaleX=0, ZZZ=0, dif_RotateZ=0;
//int				numb=0;
int				ObjCount=0, pre_ObjCount=0;

char			text[32];
int				 pre_obj_id=0;
int             marker_num;

ARMarkerInfo    *marker_info;

/* set up the video format globals */

#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

char           *cparam_name    = "Data/webcam_para.dat";
ARParam         cparam;

static void   init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static int 	draw( ObjectData_T *object, int objectnum );
static int  draw_object( int obj_id, double gl_para[16] );


#include <Vfw.h>

#pragma comment( lib, "opengl32.lib" )								// Search For OpenGL32.lib While Linking ( NEW )
#pragma comment( lib, "glu32.lib" )									// Search For GLu32.lib While Linking    ( NEW )
#pragma comment( lib, "glaux.lib" )									// Search For GLaux.lib While Linking    ( NEW )
#pragma comment( lib, "vfw32.lib" )								// Search For VFW32.lib While Linking


// Text Mapping
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

GLuint tex[1];
unsigned char *buf, *buf1, *buf2;


static char *get_buff( char *buf, int n, FILE *fp );


ObjectData_T *read_ObjData( char *name, int *objectnum )
{
	FILE          *fp;
	ObjectData_T  *object;
	char           buf[256], buf1[256];
	int            i;

	printf("Opening Data File %s\n",name);

	if( (fp=fopen(name, "r")) == NULL ) {
		printf("Can't find the file - quitting \n");
		return(0);
	}

	get_buff(buf, 256, fp);
	if( sscanf(buf, "%d", objectnum) != 1 ) {fclose(fp); return(0);}

	printf("About to load %d Models\n",*objectnum);

	object = (ObjectData_T *)malloc( sizeof(ObjectData_T) * *objectnum );
	if( object == NULL ) return(0);

	for( i = 0; i < *objectnum; i++ ) {
		object[i].visible = 0;        
		
		get_buff(buf, 256, fp);
		if( sscanf(buf, "%s", object[i].name) != 1 ) {
			fclose(fp); free(object); return(0);
		}

		printf("Read in No.%d \n", i+1);

		get_buff(buf, 256, fp);
		if( sscanf(buf, "%s", buf1) != 1 ) {
			fclose(fp); free(object); return(0);}

			if( (object[i].id = arLoadPatt(buf1)) < 0 )
				{fclose(fp); free(object); return(0);}

			get_buff(buf, 256, fp);
			if( sscanf(buf, "%lf", &object[i].marker_width) != 1 ) {
				fclose(fp); free(object); return(0);
			}

			get_buff(buf, 256, fp);
			if( sscanf(buf, "%lf %lf", &object[i].marker_center[0], &object[i].marker_center[1]) != 2 ) {
				fclose(fp); free(object); return(0);
			}

		}

		fclose(fp);

		return( object );
	}

	static char *get_buff( char *buf, int n, FILE *fp )
	{
		char *ret;

		for(;;) {
			ret = fgets( buf, n, fp );
			if( ret == NULL ) return(NULL);
			if( buf[0] != '\n' && buf[0] != '#' ) return(ret);
		}
	}


	AUX_RGBImageRec *LoadBMPFile(char *filename)
	{
		FILE *hFile = NULL;
		if(!filename) return NULL;

		hFile = fopen(filename, "r");
		if(hFile) {
			fclose(hFile);
			return auxDIBImageLoad(filename);
		}

		return NULL;
	}


	void initTexture()
	{
		GLfloat ambientLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
		GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
		GLfloat lightPos[] = { -100.0f, 130.0f, 150.0f, 1.0f };
		GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	////////// New Codes ///////////////////////////////////////////////
		AUX_RGBImageRec *texRec[1];
//	memset(texRec, 0, sizeof(void *)*1);

		if( (texRec[0]=LoadBMPFile("sponse2.bmp")) )	
		{
			int i, j, k;

			buf = (unsigned char *)malloc(sizeof(unsigned char) * texRec[0]->sizeX * texRec[0]->sizeY * 4);

			glGenTextures(1, &tex[0]);

			for(i = 0; i < 1; i++) 
			{    
				for(k=0, j=0; k < texRec[i]->sizeX * texRec[i]->sizeY * 3; k+=3, j+=4)
				{
					buf[j] = texRec[i]->data[k];
					buf[j+1] = texRec[i]->data[k+1];
					buf[j+2] = texRec[i]->data[k+2];

					if( (buf[j] == 255) && (buf[j+1] == 0) && (buf[j+2] == 0) )
						buf[j+3] = 0;
					else
						buf[j+3] = 255;
				}

				glBindTexture(GL_TEXTURE_2D, tex[i]);    
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_NOTEQUAL, 0);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0,    
					GL_RGB, GL_UNSIGNED_BYTE, texRec[0]->data);   
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

			}  
		} 

	/*
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	////////////////////////////////////////////////////////////////////

 	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
 	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
 	glFrontFace(GL_CCW);
 	glEnable(GL_LIGHTING);
 */
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glEnable(GL_LIGHT0);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
		glMateriali(GL_FRONT, GL_SHININESS, 10);

		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		

		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}

int main(int argc, char **argv)
{
	//initialize applications
	glutInit(&argc, argv);
	init();
	
	arVideoCapStart();

	//start the main event loop
	argMainLoop( NULL, keyEvent, mainLoop );

	return 0;
}

static void   keyEvent( unsigned char key, int x, int y)   
{
    /* quit if the ESC key is pressed */
	if( key == 0x1b ) {
		printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
		cleanup();
		exit(0);
	}
}

/* main loop */
static void mainLoop(void)
{
	ARUint8         *dataPtr;

	int             i,j,k;

    /* grab a video frame */
	if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
		arUtilSleep(2);
		return;
	}
	
	if( count == 0 ) arUtilTimerReset();  
	count++;

	/*draw the video*/
	argDrawMode2D();
	argDispImage( dataPtr, 0,0 );

	/* detect the markers in the video frame */ 
	if(arDetectMarker(dataPtr, thresh, 
		&marker_info, &marker_num) < 0 ) {
		cleanup(); 
	exit(0);
}

ObjCount=0;
	/* check for known patterns */
for( i = 0; i < objectnum; i++ ) 
{
	k = -1;
	for( j = 0; j < marker_num; j++ ) 
	{
		if( object[i].id == marker_info[j].id) 
		{
			/* you've found a pattern */
			//printf("Found pattern: %d ",patt_id);
			//printf("%f   %f\n", object[j].marker_center[0], object[j].marker_center[1]);

			//	printf("%f   %f \n", marker_info[j].pos[0], marker_info[j].pos[1]);
			ObjCount++;

			if( k == -1 ) k = j;
		    else /* make sure you have the best pattern (highest confidence factor) */
			if( marker_info[k].cf < marker_info[j].cf ) k = j;
		}

	}
	if( k == -1 ) {
		object[i].visible = 0;
		continue;
	}

	arGetTransMatCont(&marker_info[k], object[i].trans,
		object[i].marker_center, object[i].marker_width,
		object[i].trans);


	object[i].visible = 1;

	if(object[i].id == 0){
		
		scaleX=marker_info[0].pos[1]/20.0;

		if(pre_scaleX+0.25 >= scaleX && pre_scaleX-0.25 <= scaleX)
			scaleX=pre_scaleX;

		dif_scaleX = scaleX - pre_scaleX;

		if(dif_scaleX>0.8 || dif_scaleX<-0.8)
			dif_scaleX=0;

		pre_scaleX = scaleX;
		
	//	printf("%f   %f    %f \n",scaleX, dif_scaleX, marker_info[0].pos[1]);
	}
	if(object[i].id == 2){
		double rot[3][3];
		for (int q= 0; q < 3; q++) {
			for (int j= 0; j < 3; j++) {
				rot[q][j] = object[i].trans[q][j];
			}}

			arGetAngle  (rot, &x1, &y11, &z1); 

			x=x1;
			y=y11;
			ZZZ=z1;

			if(z!=0)
				ZZZ = floor(ZZZ*100)/100; 

			ZZZ=(ZZZ+3.15)*57;
	
			if(pre_z+3 >= ZZZ && pre_z-3 <= ZZZ)
				ZZZ=pre_z;

			dif_RotateZ=ZZZ-pre_z;

			pre_z=ZZZ;

		}
	}
	
	arVideoCapNext();

	/* draw the AR graphics */
	draw( object, objectnum );

	/*swap the graphics buffers*/
	argSwapBuffers();
}

static void init( void )
{
	ARParam  wparam;

    /* open the video path */
	if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
	if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
	printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
	if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
		printf("Camera parameter load error !!\n");
		exit(0);
	}
	arParamChangeSize( &wparam, xsize, ysize, &cparam );
	arInitCparam( &cparam );
	printf("*** Camera Parameter ***\n");
	arParamDisp( &cparam );

	/* load in the object data - trained markers and associated bitmap files */
	if( (object=read_ObjData(model_name, &objectnum)) == NULL ) exit(0);
	printf("Objectfile num = %d\n", objectnum);

    /* open the graphics window */
	argInit( &cparam, 2.0, 0, 0, 0, 0 );

	initTexture();
}

/* cleanup function called when program exits */
static void cleanup(void)
{
	arVideoCapStop();
	arVideoClose();
	argCleanup();
}

/* draw the the AR objects */
static int draw( ObjectData_T *object, int objectnum )
{
	int     i;
	double  gl_para[16];       
	glClearDepth( 1.0 );
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
    /* calculate the viewing parameters - gl_para */
	for( i = 0; i < objectnum; i++ ) {

		if( object[i].visible == 0 ){ 
			continue;
		}

		argConvGlpara(object[i].trans, gl_para);


		draw_object( object[i].id, gl_para);

	}
	
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable(GL_COLOR_MATERIAL);

	return(0);
}


/* draw the user object */
static int  draw_object( int obj_id, double gl_para[16])
{


	GLfloat   mat_ambient[]				= {0.0, 0.0, 1.0, 1.0};
	GLfloat   mat_ambient_collide[]     = {1.0, 0.0, 0.0, 1.0};
	GLfloat   mat_flash[]				= {0.0, 0.0, 1.0, 1.0};
	GLfloat   mat_flash_collide[]       = {1.0, 0.0, 0.0, 1.0};
	GLfloat   mat_flash_shiny[] = {50.0};
	GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
	GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
	GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

	argDrawMode3D();
	argDraw3dCamera( 0, 0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( gl_para );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);


	glEnable(GL_TEXTURE_2D);
	
	glColor3f (1.0, 1.0, 1.0); 
	
	//printf("%d \n",  objectnum);

	if(ObjCount == 3){
		a=0,b=0,c=0,w=0;
		sum_scaleX=1;
			//dif_scaleX=0;
		t++;
		if(t<=2){
			e=0;}	
		}	
		else if(ObjCount == 2 ){
			w++, t=0;
			if(w<=2)
				e=0;
			if(w>2){	
				if(((obj_id == 0 &&  pre_obj_id == 2) || (obj_id == 2 &&  pre_obj_id == 0)) && b==0){
					numb=1;}

					else if(((obj_id == 0 &&  pre_obj_id == 1) || (obj_id == 1 &&  pre_obj_id == 0)) && a==0){
						RotatXYZ=1, dif_scaleX=0;}

						else if(((obj_id == 2 &&  pre_obj_id == 1) || (obj_id == 1 &&  pre_obj_id == 2)) && c==0){
							scalXYZ=1;}

							pre_scaleX = scaleX;
						}
					}
					if(RotatXYZ==1 && a==0){
						Rotate++;
						RotatXYZ=0;
						a=1;
						dif_scaleX=0;
					}

					if(Rotate==3)
						Rotate=0;

					if(scalXYZ==1 && c==0){
						scale++;
						scalXYZ=0;
						c=1;
						e=0;
					}				

					if(scale==4)
						scale=0;


					if(numb==1 && b==0 ){
						number++;
						numb=0;
						b=1;
					}

					if(number==3)
						number=0;

					pre_obj_id = obj_id;

	if(obj_id == 0)	//For Scale Change
	{
		switch (scale){

			case 0:
			sprintf(text, "XYZ \n");
			break;
			case 1:
			sprintf(text, "X \n");
			break;
			case 2:
			sprintf(text, "Y \n");
			break;		
			case 3:
			sprintf(text, "Z \n");
			break;
			default:
			break;
		}
		glColor3f(0,0,1);
		glRasterPos3f( -25 , -32 , -30);
		for(int i = 0; text[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		
		if(scaleX > pre_scaleX-1 || scaleX < pre_scaleX+1)
			scaleX=pre_scaleX;

		sprintf(text, " Scale : %.0f", scaleX);
		for(int i = 0; text[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);

		pre_ObjCount = ObjCount;

		glColor3f(1,1,1);

	}
	else if(obj_id == 1)
	{
		switch (Rotate){
			case 0:
			RotateX=RotateX+dif_RotateZ;
			break;

			case 1:
			RotateY=RotateY+dif_RotateZ;
			break;

			case 2:
			RotatZ=RotatZ+dif_RotateZ;
			break;

			default :
			break;
		}		
		glRotated(RotateX,-1,0,0);
		glRotated(RotateY,0,0-1,0);
		glRotated(RotatZ,0,0,-1);

		e++;


		switch (scale){

			case 0:
			if(e<2)
				dif_scaleX=0;
			pre_sum_scaleX=pre_sum_scaleX+dif_scaleX;
			pre_sum_scaleY=pre_sum_scaleY+dif_scaleX;
			pre_sum_scaleZ=pre_sum_scaleZ+dif_scaleX;
			break;

			case 1:
			if(e<2)
				dif_scaleX=0;
			pre_sum_scaleX=pre_sum_scaleX+dif_scaleX;

			break;

			case 2:
			if(e<2)
				dif_scaleX=0;
			pre_sum_scaleY=pre_sum_scaleY+dif_scaleX;

			break;	

			case 3:
			if(e<2)
				dif_scaleX=0;
			pre_sum_scaleZ=pre_sum_scaleZ+dif_scaleX;

			break;

			default:
			break;
			
		}
		glScalef(pre_sum_scaleX, pre_sum_scaleY, pre_sum_scaleZ);
		glScalef(0.2, 0.2, 0.2);
		printf("%f   %f   %f   %f %d \n",pre_sum_scaleX,pre_sum_scaleY, pre_sum_scaleZ, dif_scaleX );
		glColor3f(0,1,0);

		glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 120.0f); 
		glEnd();
		
		glBegin(GL_LINES);
		glVertex2f(0.0f, 120.0f);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		
		glBegin(GL_LINES);
		glVertex2f(120.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		
		glBegin(GL_LINE_LOOP);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,-120.0f);
		glEnd();

		if(number==1)
		{
			glPushMatrix();										
			glColor3f(0,0,1);
			glScalef(5.8f,5.8f,5.8f);

			glRotatef(10,0,1,0);
			glRotatef(90,0,1,0);

			glutSolidSphere(5.0f, 20, 20); 
			glPopMatrix();		

			glColor3f(1,1,1);
		}

		else if(number==0)
		{
			glPushMatrix();										
			glColor3f(1,0,0);
			glTranslatef(0,0,-1.3);
			glScalef(0.6f,0.6f,0.6f);

			glRotatef(270,1,0,0);

		// Draw the teapot
			glutSolidTeapot(50);
		 //glutSolidSphere(1.0f, 10, 0); 
			glPopMatrix();	

			glColor3f(1,1,1);

		}
		else if(number==2)
		{
			glScalef(20.0, 20.0, 20.0);

			glBindTexture(GL_TEXTURE_2D, tex[0]);	
			glColor3f(1,1,1);	
			glBegin(GL_QUADS);
		// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);
	//	glNormal3f( 0.0f,-1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f( 1.0f, -1.0f, -1.0f);
			glVertex3f( 1.0f, -1.0f,  1.0f);
			glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glVertex3f( 1.0f, -1.0f, -1.0f);
			glVertex3f( 1.0f,  1.0f, -1.0f);
			glVertex3f( 1.0f,  1.0f,  1.0f);
			glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f,  1.0f);
			glVertex3f(-1.0f,  1.0f,  1.0f);
			glVertex3f(-1.0f,  1.0f, -1.0f);
			glEnd();

			glColor3f(1,1,1);
			glPopMatrix();	



		}

		
		pre_ObjCount = ObjCount;

		glColor3f(1,1,1);
	}



	else if(obj_id == 2)	// Rotation
	{

		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

		glColor3f (1.0, 1.0, 1.0);
		
		switch (Rotate){
			case 0:
			sprintf(text, "X \n");
			break;
			case 1:
			sprintf(text, "Y \n");
			break;
			case 2:
			sprintf(text, "Z \n");
			break;		
			default:
			break;
		}
		glColor3f(0,1,1);
		glRasterPos3f( -25 , -35 , -30);
		for(int i = 0; text[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		
		sprintf(text, " Rotate : %.0f", RotateZ);
		glColor3f(0,1,1);
		for(int i = 0; text[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
		
		glColor3f (1.0, 1.0, 1.0);
		glPushMatrix();
		glColor3f (0.0, 0.0, 1.0);
		glLineWidth(50);
		glTranslatef(0.0,0.0,0.0);
		glBegin(GL_LINE_LOOP);
		for (int i=0; i<360; i++){
			float degInRad=i*DEGINRAD;
			//glVertex3f(cos(degInRad)*r, 0,sin(degInRad)*r);
			glVertex3f(cos(degInRad)*r, sin(degInRad)*r,0);
		}
		glEnd();
		glPopMatrix();

		glPushMatrix();
		
         // Draw the line
		glColor3f (1.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 120.0f); 
		glEnd();
		glPopMatrix();
		///////////////////////line

//		glPushMatrix();

		glColor3f (0.0, 0.0, 1.0); 
		gluOrtho2D(-2.0,-2.0,2.0,2.0);
		glScalef(0.5f, 0.5f, 1);
		glMatrixMode(GL_MODELVIEW);

		glRotated(RotateZ,0,0,-1);

//		glPopMatrix();

		for(int i=0;i<60;i++)
		{
			glPushMatrix();
			glRotatef(i*6,0,0,1);
			glTranslatef(200,0,0);
			glColor3f(0.5,1,0); 
			
			if((i%5 == 0) && (i!= 45)){
				glScalef(1.5f, 1.0f, 1.0f);
				glColor3f(0,0,1); 
				glBegin(GL_TRIANGLES);
				glVertex2f(-20.,0.0);
				glVertex2f(0.0,3.5);
				glVertex2f(0.0,-3.5);

			}
			if(i == 35){
				glColor3f(0,1,1);
				glScalef(4.0f, 1.0f, 1.0f);
				glBegin(GL_TRIANGLES);
				glVertex2f(-20.,0.0);
				glVertex2f(0.0,3.5);
				glVertex2f(0.0,-3.5);
			}
			else{
			}

			glEnd();
			glPopMatrix();
			glColor3f (1.0, 1.0, 1.0);
		}

		pre_ObjCount = ObjCount;

	}

	else
	{
		glPushMatrix();										
		glColor3f(0,1,0);
		glTranslatef(0,0,+20);
		glScalef(10.8f,10.8f,10.8f);

		glRotatef(10,0,1,0);
		glRotatef(90,0,1,0);

		glutSolidSphere(5.0f, 20, 20); 
		glPopMatrix();		


	}

	glDisable(GL_TEXTURE_2D);
	argDrawMode2D();

	return 0;
}


