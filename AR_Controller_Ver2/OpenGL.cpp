void DrawCube(float xPos, float yPos, float zPos)
{
        glPushMatrix();
        glBegin(GL_POLYGON);

                /*      This is the top face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, 0.0f, 0.0f);

                /*      This is the front face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);

                /*      This is the right face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, -1.0f);
                glVertex3f(0.0f, 0.0f, -1.0f);

                /*      This is the left face*/
                glVertex3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);

                /*      This is the bottom face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);

                /*      This is the back face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(0.0f, -1.0f, -1.0f);

        glEnd();
        glPopMatrix();
}

/*      Function:       DrawArm
        Purpose:        This function draws the arm
                                for the robot.
*/

void DrawArm(float xPos, float yPos, float zPos)
{

	glColor3f(1.0f, 0.0f, 0.0f);
                
        glPushMatrix();

                /*      Sets color to red*/
                glTranslatef(xPos, yPos, zPos);
				glColor3f(1.0f, 0.0f, 0.0f);
                
                /*      Creates 1 x 4 x 1 cube*/
                glScalef(1.0f, 4.0f, 1.0f);
                DrawCube(0.0f, 0.0f, 0.0f);

        glPopMatrix();
}

/*      Function:       DrawHead
        Purpose:        This function will create the
                                head for the robot.
*/

void DrawHead(float xPos, float yPos, float zPos)
{
        glPushMatrix();

        glColor3f(1.0f, 1.0f, 1.0f);
        glTranslatef(xPos, yPos, zPos);
		glTranslatef(-0.9, 0, 0);

                /*      Creates 2 x 2 x 2 cube*/
        glScalef(2.0f, 2.0f, 2.0f);
        DrawCube(0.0f, 0.0f, 0.0f);


	glBindTexture(GL_TEXTURE_2D, tex[7]);
 			
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);
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

                /*      Sets color to white*/
 
		glPopMatrix();
		
		glColor3f(1.0f, 1.0f, 1.0f);
 
}

/*      Function:       DrawTorso
        Purpose:        Function will do as suggested
                                and draw a torso for our robot.
*/

void DrawTorso(float xPos, float yPos, float zPos)
{
        glPushMatrix();

                /*      Sets color to blue*/
                glColor3f(0.0f, 0.0f, 1.0f);
                glTranslatef(xPos, yPos, zPos);

                /*      Creates 3 x 5 x 1 cube*/
                glScalef(3.0f, 5.0f, 1.0f);
                DrawCube(0.0f, 0.0f, 0.0f);

        glPopMatrix();
}

/*      Function:       DrawLeg
        Purpose:        Not to sound repetitve, but as suggested
                                this function will draw our robots legs.
*/

void DrawLeg(float xPos, float yPos, float zPos)
{
        glPushMatrix();

                /*      Sets color to yellow*/
                glColor3f(1.0f, 1.0f, 0.0f);
                glTranslatef(xPos, yPos, zPos);

                /*      Creates 1 x 5 x 1 cube*/
                glScalef(1.0f, 5.0f, 1.0f);
                DrawCube(0.0f, 0.0f, 0.0f);

        glPopMatrix();
}

/*      Function:       DrawRobot
        Purpose:        Function to draw our entire robot
*/
/* draw the the AR objects */
static int draw( ObjectData_T *object, int objectnum )
{
    int     i;
    double  gl_para[16];
//  	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	          
	glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
	//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	ObjCount=0;

    /* calculate the viewing parameters - gl_para */
    for( i = 0; i < objectnum; i++ ) {
        
		if( object[i].visible == 0 ){ 
			continue;
		}

        argConvGlpara(object[i].trans, gl_para);
    
		ObjCount++;
	
		draw_object( object[i].id, gl_para);

	//	if(object[0].id != 3 && object[1].id != 3 && object[2].id != 3 && object[3].id != 3){
	//		a=0;
		
	}
		
	if(ObjCount == 4)
			b=0;

		if(ObjCount == 3){
			for(i=0; i<ObjCount;i++){
			if(marker_info[i].id == 1)
				speed=0, a=0;
			if(marker_info[i].id == 0 && b==0)
					a=1;
				}
			}		
	
		
		


	glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
	glDisable(GL_COLOR_MATERIAL);
 
	return(0);
}
