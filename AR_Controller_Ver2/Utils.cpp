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

  // glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	




   if(a==1 && b==0){
		speed++;
		a=0;
		b=1;
		}



   /*
	argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );
	*/
	glEnable(GL_TEXTURE_2D);
	
	glColor3f (1.0, 1.0, 1.0); 
	
	////////////////////////////////////////////////////////////마커 ID:0
	if(obj_id == 0)
	{
		glScalef(40.0, 30.0, 3.0);	//x,y,z

		//glTranslatef(0.0, -10.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
 			
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
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
	}


	////////////////////////////////////////////////////////////마커 ID:1
	if(obj_id == 1)
	{
				
		glPushMatrix();
		
		glScalef(20.0f, 20.0f, 20.0f);	//크기조정
		glTranslatef(0,4,0);			//숫자 위치 조정
		
		if(speed == 0){
		glBindTexture(GL_TEXTURE_2D, tex[2]);
 		MovSpeed=0;
		armAngle[0]=0;
		armAngle[1]=0;
		legAngle[0]=0;
		legAngle[1]=0;
		Q=0;
		}

		else if(speed == 1){
		glBindTexture(GL_TEXTURE_2D, tex[3]);
		MovSpeed=1.0;
		Q=1;
		}		
		
		else if(speed == 2){
		glBindTexture(GL_TEXTURE_2D, tex[4]);
		MovSpeed=3.0;
		Q=2;
		}

		else if(speed == 3){
		glBindTexture(GL_TEXTURE_2D, tex[5]);
		MovSpeed=5.0;
		Q=3;
		}

		else {
		glBindTexture(GL_TEXTURE_2D, tex[6]);
		MovSpeed=7.0;
		Q=4;
		}
			
		//printf("%d  ", speed);

			glBegin(GL_QUADS);
		//	glTranslatef(0,0,30);

	// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
			
			
			
		glEnd();
	glPopMatrix();
//////////////


	//glPushMatrix();

	glScalef(25.0, 50.0, 3.0);	//x,y,z

		//glTranslatef(0.0, -10.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
 			
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
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glEnd();
	}

	////////////////////////////////////////////////////////////마커 ID:2
	if(obj_id == 2)
	{
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
		////////////////////////////////////////원?ⓒª

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

		glRotated(RotateZ,0,0,-1);		//회￠￢전u
		 
//		glPopMatrix();

		for(int i=0;i<60;i++)
			{
		glPushMatrix();
		glRotatef(i*6,0,0,1);
		glTranslatef(200,0,0);
		glColor3f(0,0,1); 
			
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
		glColor3f(0,0,1);
	   glBegin(GL_TRIANGLES);
	   glVertex2f(-20.,0.0);
		glVertex2f(0.0,3.5);
		glVertex2f(0.0,-3.5);
		}
		glEnd();
		glPopMatrix();
		glColor3f (1.0, 1.0, 1.0);

	   
	}
	//glColor3f (1.0, 1.0, 1.0);

	}

	////////////////////////////////////////////////////////////마커 ID:3
	
	if(obj_id == 3)
	{
		DirectSpeedX = DirectSpeedX + -(Q * (cos(RotateZ*DEGINRAD)));
		DirectSpeedY = DirectSpeedY + (Q * (sin(RotateZ*DEGINRAD)));		
		
		
		glPushMatrix();

		glTranslatef(DirectSpeedY, DirectSpeedX, 0);	//x,y,z
		glRotated(RotateZ+180,0,0,1);		//회전
		glTranslatef(-DirectSpeedY, -DirectSpeedX, 0);	//x,y,z

		glBegin(GL_POLYGON);
		glEnd();


		glTranslatef(0.0, 0.0, 10.0);
		glTranslatef(DirectSpeedY, DirectSpeedX, 0);	//x,y,z
		printf("Y = %lf"   "X = %lf\n", Q * fabs(cos(RotateZ*DEGINRAD)), Q * fabs(sin(RotateZ*DEGINRAD)));
		
		/*      Variables for state of robots legs. True
                means the leg is forward, and False means
                the leg is back. The same applies to the
                robots arm states.
        */

		static bool leg1 = true;
        static bool leg2 = false;
        static bool arm1 = true;
        static bool arm2 = false;

		glRotatef(90, 1.0f, 0.0f, 0.0f);
        glScalef(8.0f, 8.0f, 8.0f);
		        


        glPushMatrix();

                /*      This will draw our robot at the
                        desired coordinates.
                */
//                glTranslatef(xPos, yPos, zPos);

                /*      These three lines will draw the
                        various components of our robot.
                */
                DrawHead(1.0f, 2.0f, 0.0f);
                DrawTorso(1.5f, 0.0f, 0.0f);
		glPushMatrix();


                /*      If the arm is moving forward we will increase
                        the angle; otherwise, we will decrease the
                        angle.
                */
			    if (arm1)
                {
                        armAngle[0] = armAngle[0] + MovSpeed;
                }
                else
                {
                        armAngle[0] = armAngle[0] - MovSpeed;
                }

                /*      Once the leg has reached its max angle
                        in one direction, we want it to reverse
                        and change direction.
                */
                if (armAngle[0] >= 15.0f)
                {
                        arm1 = false;
                }
				else if (armAngle[0] <= -15.0f)
                {
                        arm1 = true;
                }



                /*      Here we are going to move the arm away
                        from the torso and rotate. This will
                        create a walking effect.
                */
                glTranslatef(0.0f, -0.5f, 0.0f);
                //glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
                glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
                
				DrawArm(2.5f, 0.0f, -0.5f);

        glPopMatrix();

        glPushMatrix();


                /*      If the arm is moving forward we will increase
                        the angle, otherwise we will decrease the
                        angle
                */
                if (arm2)
                {
                        armAngle[1] = armAngle[1] + MovSpeed;
                }
                else
                {
                        armAngle[1] = armAngle[1] - MovSpeed;
                }

				if (armAngle[1] >= 15.0f)
                {
                        arm2 = false;
                }
				else if (armAngle[1] <= -15.0f)
                {
                        arm2 = true;
                }

                /*      Here we are going to move the arm away
                        from the torso and rotate. This will
                        create a walking effect.
                */
                glTranslatef(0.0f, -0.5f, 0.0f);
                glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
                DrawArm(-1.5f, 0.0f, -0.5f);

        glPopMatrix();

        /*      Now its time to rotate the legs relative to the
                robots position in the world, this is the first
                leg, ie the right one.
        */
        glPushMatrix();

                /*      If the leg is moving forward we will increase
                        the angle; otherwise, we will decrease the
                        angle.
                */
                if (leg1)
                {
                        legAngle[0] = legAngle[0] + MovSpeed;
                }
                else
                {
                        legAngle[0] = legAngle[0] - MovSpeed;
                }

                /*      Once the leg has reached its max angle
                        in one direction, we want it to reverse
                        and change direction.
                */
                if (legAngle[0] >= 15.0f)
                {
                        leg1 = false;
                }
                else if (legAngle[0] <= -15.0f)
                {
                        leg1 = true;
                }


                /*      Here we are going to move the leg away
                        from the torso and rotate. This will
                        create a walking effect.
                */
                glTranslatef(0.0f, -0.5f, 0.0f);
                glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);


                /*      Time to draw the leg.
                */
                DrawLeg(-0.5f, -5.0f, -0.5f);

        glPopMatrix();

        /*      Same as above, for the left leg.
        */
        glPushMatrix();

                /*      If the leg is moving forward we will increase
                        the angle, otherwise we will decrease the
                        angle
                */
                if (leg2)
                {
                        legAngle[1] = legAngle[1] + MovSpeed;
                }
                else
                {
                        legAngle[1] = legAngle[1] - MovSpeed;
                }

                /*      Once the leg has reached its max angle
                        in one direction, we want it to reverse
                        and change direction.
                */
                if (legAngle[1] >= 15.0f)
                {
                        leg2 = false;
                }
                else if (legAngle[1] <= -15.0f)
                {
                        leg2 = true;
                }

                /*      Here we are going to move the leg away
                        from the torso and rotate. This will
                        create a walking effect.
                */
                glTranslatef(0.0f, -0.5f, 0.0f);
                glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
                DrawLeg(1.5f, -5.0f, -0.5f);

        glPopMatrix();
        glPopMatrix();



		angle = angle + MovSpeed;

        /*      Reset after we have completed a circle
        */
        if (angle >= 360.0f)
        {
                angle = MovSpeed;
        }

        glPushMatrix();
 	 
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -30.0f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        glPopMatrix();
	   
		//glPushMatrix();
		glRotated(RotateZ+180,0,0,1);		// Rotation
        //glPopMatrix();



		glFlush();
		SwapBuffers(g_HDC);

		glEnd();
		glColor3f (1.0, 1.0, 1.0);

	}
	glDisable(GL_TEXTURE_2D);
    argDrawMode2D();

    return 0;
}


