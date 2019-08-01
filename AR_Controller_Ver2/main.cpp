#ifdef _WIN32
#  include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/video.h>
#include <AR/matrix.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "0403.h"

#define COLLIDE_DIST 30000.0
#define DEGINRAD 3.14/180

/* Object Data */
char            *model_name = "Data/object_data11";
ObjectData_T    *object;
int             objectnum;

int             xsize, ysize;
int				thresh = 100;
int             count = 0, Mcount=0;
double			x,y,z;
double			RotateZ=0;
double			pre_RotateZ=0;
double			x1=0,y11=0,z1=0, pre_z=0;
double			DirectSpeedX=0, DirectSpeedY=0, Q=0;

ARMarkerInfo    *marker_info;


double			rot1[3][3];
double          trans[3][4];
int				r=100;
int				speed=0;
int				a=0,b=0;
int				ObjCount=0;

/* set up the video format globals */

HDC g_HDC;

float angle = 0.0f;
float legAngle[2] = {0.0f, 0.0f};
float armAngle[2] = {0.0f, 0.0f};

float MovSpeed=0;

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
static int draw( ObjectData_T *object, int objectnum );
static int  draw_object( int obj_id, double gl_para[16] );


GLuint tex[8];
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
	AUX_RGBImageRec *texRec[8];
//	memset(texRec, 0, sizeof(void *)*1);

	if( (texRec[0]=LoadBMPFile("break.bmp")) &&
		(texRec[1]=LoadBMPFile("acc.bmp")) &&
		(texRec[2]=LoadBMPFile("number0.bmp")) &&
		(texRec[3]=LoadBMPFile("number1.bmp")) &&
		(texRec[4]=LoadBMPFile("number2.bmp")) &&
		(texRec[5]=LoadBMPFile("number3.bmp")) &&
		(texRec[6]=LoadBMPFile("number4.bmp")) &&
		(texRec[7]=LoadBMPFile("pipipi.bmp")))	
	{
		int i, j, k;
		
		buf = (unsigned char *)malloc(sizeof(unsigned char) * texRec[0]->sizeX * texRec[0]->sizeY * 4);

		glGenTextures(8, &tex[0]);

		for(i = 0; i < 8; i++) 
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texRec[i]->sizeX, texRec[i]->sizeY, 0,    
						GL_RGBA, GL_UNSIGNED_BYTE, buf);   
		}  
	} 

		
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	////////////////////////////////////////////////////////////////////

 	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
 	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
 	glFrontFace(GL_CCW);
 	glEnable(GL_LIGHTING);
 
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

static void mainLoop(void)
{
    ARUint8         *dataPtr;
    int             marker_num;
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

	/* check for known patterns */
    for( i = 0; i < objectnum; i++ ) 
	{
		k = -1;
		for( j = 0; j < marker_num; j++ ) 
		{
	        if( object[i].id == marker_info[j].id) 
			{
				if( k == -1 ) k = j;
		        else /* make sure you have the best pattern (highest confidence factor) */
					if( marker_info[k].cf < marker_info[j].cf ) k = j;
			
			
			}
		}
		if( k == -1 ) {
			//a=0;
			object[i].visible = 0;
			continue;
		}
	arGetTransMatCont(&marker_info[k], object[i].trans,
                          object[i].marker_center, object[i].marker_width,
                          object[i].trans);
       
    object[i].visible = 1;
		
	if(object[i].id == 2){
		double rot[3][3];
        for (int q= 0; q < 3; q++) {
        	for (int j= 0; j < 3; j++) {
				rot[q][j] = object[i].trans[q][j];
	        }	
	    }

		arGetAngle  (rot, &x1, &y11, &z1); 
		
		x=x1;
		y=y11;
		z=z1;
		z = floor(z*100)/100; 

		if(pre_z+0.01 >= z && pre_z-0.01 <= z)
			z=pre_z;

		pre_z=z;

		RotateZ=(z+3.15)*57;
		}
	
	
	
	}
	arVideoCapNext();

	/* draw the AR graphics */
    draw( object, objectnum );

	/*swap the graphics buffers*/
	argSwapBuffers();

//	speed++;
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




