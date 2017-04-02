#define PROGRAM_TITLE "Robot Rampage Reloaded"

#include <stdlib.h> 
#include <stdio.h>    
#include <string.h>  
#include <unistd.h> 
#include <math.h>
#include <GL/gl.h>  
#include <GL/glu.h> 
#include <GL/glut.h> 
#include <unistd.h> 
#include <cmath> 

// Window IDs, window width and height.
int Window_ID;
int Window_Width = 800;
int Window_Height = 600;

// The cubes rotation 
double Y_Rot   = 0.0f;  
double Y_Speed = -30.0f;
double Robot_X = 0; 
double Robot_Y = 0; 
double Robot_Z = 0; 
double Lookat_X = 0.0;
double Lookat_Y = 4.0;
double Lookat_Z = 10.0;

int robotRotate = 0; 
int headRotate = 0.0;
 
static bool paused = false;

static void PrintString(void *font, char *str)
{
   int i,len=strlen(str);

   for(i=0;i < len; i++)
      glutBitmapCharacter(font,*str++);
}

void update(int)
{

   glutTimerFunc(10, update, 0);
      glutPostRedisplay();
}

void pushRobot()
{
   if(robotRotate == 0)
   {
      Lookat_Z -= 1.00;
      Robot_Z -= 1.00; 
   }
   else if((robotRotate) == 90)
   {
      Lookat_X -= 1.00;
      Robot_X -= 1.00;
   }
   else if((robotRotate) == 180)
   {
      Lookat_Z += 1.00;
      Robot_Z += 1.00; 
   }
   else if((robotRotate) == 270)
   {
      Lookat_X += 1.00;
      Robot_X += 1.00;
   }
}

void robRot(int r)
{
   robotRotate = (r + robotRotate) % 360; 

}

void adjustHead(int r)
{
   headRotate = (r + headRotate) % 360; 

}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
   int i;
   int triangleAmount = 20; //# of triangles used to draw circle
   
   GLfloat twicePi = 2.0f * M_PI;
   
   glBegin(GL_TRIANGLE_FAN);
      glVertex2f(x, y); // center of circle
      for(i = 0; i <= triangleAmount;i++) { 
         glVertex2f(
            x + (radius * cos(i *  twicePi / triangleAmount)), 
             y + (radius * sin(i * twicePi / triangleAmount))
         );
      }
   glEnd();
}

void buildRobot()//**********************************************************************************************************************************************************
{
   glPushMatrix();
   glPopMatrix();

   glTranslatef(Robot_X, Robot_Y, Robot_Z);

   glRotatef(robotRotate, 0,1,0); 

   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
   glTranslatef(Robot_X, Robot_Y, Robot_Z);

   //TRIANGLES 
   glBegin(GL_TRIANGLES); 

   glColor3f( 0.0f, 1.0f, 0.0f);
   
   glVertex3f(0.0f, 2.5f,  1.6f);
   glVertex3f(-1.0f, 1.0f,  1.6f);
   glVertex3f(1.0f,  1.0f,  1.6f); 

   glColor3f( 0.0f, 1.0f, 0.0f);
   
   glVertex3f(0.0f, 1.0f,  1.6f);
   glVertex3f(-1.0f, -0.5f,  1.6f);
   glVertex3f(1.0f,  -0.5f,  1.6f); 
  

   glEnd();

   glPopMatrix();
   glPushMatrix();

   //HEAD
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(headRotate, 0,1,0);

   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
   glTranslatef(Robot_X, Robot_Y, Robot_Z);

   glBegin(GL_QUADS); 


   //Far face.
   glNormal3f(0.0f, 0.0f,-1.0f);
   glColor4f(0.8, 0.3, 0.1, 1.0); 

   glVertex3f(-1.0f, 3.5f, -1.0f);
   glVertex3f(-1.0f,  5.0f, -1.0f);
   glVertex3f( 1.0f,  5.0f, -1.0f);
   glVertex3f( 1.0f, 3.5f, -1.0f);

   //Right face.  
   glNormal3f(1.0f, 0.0f, 0.0f);
   glColor4f(0.5, 0.7, 0.5, 1.0);

   glVertex3f( 1.0f, 3.5f, -1.0f); 
   glVertex3f( 1.0f,  5.0f, -1.0f);
   glVertex3f( 1.0f,  5.0f,  1.0f);
   glVertex3f( 1.0f, 3.5f,  1.0f);

   //Front face; 
   glNormal3f(0.0f, 0.0f, 1.0f);
   glColor3f( 0.9f, 0.0f, 0.0f);
   
    glVertex3f(-1.0f, 3.5f,  1.0f);
    glVertex3f( 1.0f, 3.5f,  1.0f);
    glVertex3f( 1.0f,  5.0f,  1.0f); 
    glVertex3f(-1.0f,  5.0f,  1.0f);


   //Left Face; 
   glNormal3f(-1.0f, 0.0f, 0.0f);  
   glColor3f(1.0, 1.0, 1.0);
   
   glVertex3f(-1.0f, 3.5f, -1.0f); 
   glVertex3f(-1.0f, 3.5f,  1.0f);
   glVertex3f(-1.0f,  5.0f,  1.0f);
   glVertex3f(-1.0f,  5.0f, -1.0f);

   //Bottom
   glNormal3f(0.0f, -1.0f, 0.0f);  
   glColor3f(0.9, 0.9, 0.1);
   
   glVertex3f(-1.0f, 3.5f, -1.0f);   
   glVertex3f(-1.0f, 3.5f,  1.0f);
   glVertex3f(1.0f,  3.5f,  1.0f);
   glVertex3f(1.0f,  3.5f, -1.0f);

   //Top
   glNormal3f(0.0f, 1.0f, 0.0f);
   glColor4f(0.2f, 0.6f, 0.2f, 0.0f);
   
   glVertex3f(-1.0f, 5.0f,  -1.0f);
   glVertex3f( 1.0f, 5.0f,  -1.0f);
   glVertex3f( 1.0f,  5.0f,  1.0f); 
   glVertex3f(-1.0f,  5.0f,  1.0f);
   
   glEnd();

   //front face square
   glPopMatrix(); 
   glPushMatrix();

   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(robotRotate, 0,1,0); 


   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
   glBegin(GL_QUADS); 

   //Far face.
   glNormal3f(0.0f, 0.0f,-1.0f);
   glColor3f(0.8, 0.8, 0.1); 

   glVertex3f(-1.0f, -0.5f, -1.6f);
   glVertex3f(-1.0f,  2.5f, -1.6f);
   glVertex3f( 1.0f,  2.5f, -1.6f);
   glVertex3f( 1.0f, -0.5f, -1.6f);

   
   
   glEnd();


   glPopMatrix(); 
   glPushMatrix();

   //CUBE
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(robotRotate, 0,1,0); 


   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
   glTranslatef(Robot_X, Robot_Y, Robot_Z);

   glBegin(GL_QUADS); 


   //Far face.
   glNormal3f(0.0f, 0.0f,-1.0f);
   glColor4f(0.8, 0.3, 0.1, 1.0); 

   glVertex3f(-1.5f, -1.0f, -1.5f);
   glVertex3f(-1.5f,  3.0f, -1.5f);
   glVertex3f( 1.5f,  3.0f, -1.5f);
   glVertex3f( 1.5f, -1.0f, -1.5f);

   //Right face.  
   glNormal3f(1.0f, 0.0f, 0.0f);
   glColor4f(0.5, 0.7, 0.5, 1.0);

   glVertex3f( 1.5f, -1.0f, -1.5f); 
   glVertex3f( 1.5f,  3.0f, -1.5f);
   glVertex3f( 1.5f,  3.0f,  1.5f);
   glVertex3f( 1.5f, -1.0f,  1.5f);

   //Front face; 
   glNormal3f(0.0f, 0.0f, 1.0f);
   glColor3f( 0.9f, 0.0f, 0.0f);
   
    glVertex3f(-1.5f, -1.0f,  1.5f);
    glVertex3f( 1.5f, -1.0f,  1.5f);
    glVertex3f( 1.5f,  3.0f,  1.5f); 
    glVertex3f(-1.5f,  3.0f,  1.5f);


   //Left Face; 
   glNormal3f(-1.0f, 0.0f, 0.0f);  
   glColor3f(1.0, 1.0, 1.0);
   
   glVertex3f(-1.5f, -1.0f, -1.5f); 
   glVertex3f(-1.5f, -1.0f,  1.5f);
   glVertex3f(-1.5f,  3.0f,  1.5f);
   glVertex3f(-1.5f,  3.0f, -1.5f);

   //Bottom
   glNormal3f(0.0f, -1.0f, 0.0f);  
   glColor3f(0.9, 0.9, 0.1);
   
   glVertex3f(-1.5f, -1.0f, -1.5f);   
   glVertex3f(-1.5f, -1.0f,  1.5f);
   glVertex3f(1.5f,  -1.0f,  1.5f);
   glVertex3f(1.5f,  -1.0f, -1.5f);

   //Top
   glNormal3f(0.0f, 1.0f, 0.0f);
   glColor4f(0.2f, 0.6f, 0.2f, 0.0f);
   
   glVertex3f(-1.5f, 3.0f,  -1.5f);
   glVertex3f( 1.5f, 3.0f,  -1.5f);
   glVertex3f( 1.5f,  3.0f,  1.5f); 
   glVertex3f(-1.5f,  3.0f,  1.5f);
   
   glEnd();

   //CYLINDERS
   glPopMatrix(); 
   glPushMatrix();

   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(robotRotate, 0,1,0); 


   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
   glTranslatef(Robot_X, Robot_Y, Robot_Z);

   usleep(10000);
   glTranslatef(0.0f,5.5f,0.0f);
   glRotatef(Y_Rot,0.0f,1.0f,0.0f);
   glTranslatef(0.0f,-5.5f,0.0f);

   glTranslatef(0,5.5,0);
   glRotatef(90,1.0f,0.0f,0.0f);
   //glTranslatef(0,0,0); 
   glColor3f(0.9,0.9,0.0);
   GLUquadric *quadric;
   quadric = gluNewQuadric();

   gluCylinder(quadric,0.25,0.25,0.5,90,60);
   
   glPopMatrix();
   glPushMatrix();

   glTranslatef(Robot_X, Robot_Y, Robot_Z);
   glRotatef(robotRotate, 0,1,0); 
   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
   glTranslatef(Robot_X, Robot_Y, Robot_Z);

   usleep(10000);
   glTranslatef(0.0f,6.0f, 0.0f);
   glRotatef(Y_Rot,0.0f,1.0f,0.0f);
   glTranslatef(0.0f,-6.0f, 0.0f);

   glTranslatef(0,6.0,0);
   glRotatef(90,1.0f,0.0f,0.0f);

   glColor3f(0.8,0.0,0.0);
  
   quadric = gluNewQuadric();

   gluCylinder(quadric,0.25,0.25,0.5,90,60);
   

   glPopMatrix();
   glPushMatrix();
   

   //NECK
   glTranslatef(0 + Robot_X,3.5 + Robot_Y,0 + Robot_Z);
   //glRotatef(robotRotate, 0,1,0); 
   glRotatef(90,1.0f,0.0f,0.0f);
   
   glColor3f(0.6,0.0,0.6);
   quadric = gluNewQuadric();
   gluCylinder(quadric,0.75,0.75,0.5,90,60);
   


   //EYES
   glPopMatrix();
   glPushMatrix();
   
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(headRotate, 0,1,0);
   glTranslatef(0,0,-1.1);
  
   drawFilledCircle(-0.45,4.35,0.25); 

   glPopMatrix();
   glPushMatrix();
   
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(headRotate, 0,1,0);
   //glRotatef(robotRotate, 0,1,0); 

   glTranslatef(0,0,-1.1);
  
   drawFilledCircle(0.45,4.35,0.25); 

}//*****************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&********************************************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*******************************&&&&




void CallBackRenderScene(void)//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
{
   char buf[80]; // For our strings.

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   buildRobot(); 

   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();   
   glLoadIdentity();

   glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
   glColor4f(0.0,0.8,0.5,1.00); 

   sprintf(buf,"%s", "Lookat_X: ");
   glRasterPos2i(2,26);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_X);
   glRasterPos2i(75,26);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Lookat_Y: ");
   glRasterPos2i(2,14);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_Y);
   glRasterPos2i(75,14);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Lookat_Z: ");
   glRasterPos2i(2,2);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_Z);
   glRasterPos2i(75,2);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Robot_X: ");
   glRasterPos2i(2,38);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Robot_X);
   glRasterPos2i(75,38);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Robot_Y: ");
   glRasterPos2i(2,50);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Robot_Y);
   glRasterPos2i(75,50);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Robot_Z: ");
   glRasterPos2i(2,62);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Robot_Z);
   glRasterPos2i(75,62);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "robotRotate: ");
   glRasterPos2i(2,74);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", robotRotate);
   glRasterPos2i(75,74);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "headRotate: ");
   glRasterPos2i(2,86);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", headRotate);
   glRasterPos2i(75,86);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   //Pause Menu
   if(paused)
   {
      sprintf(buf,"%s", "Paused");
      glRasterPos2i(362,576);
      PrintString(GLUT_BITMAP_TIMES_ROMAN_24, buf);

      sprintf(buf,"%s", "Press 'r' again to resume");
      glRasterPos2i(300,558);
      PrintString(GLUT_BITMAP_HELVETICA_18, buf);    
   }

   glTranslatef(6.0f, Window_Height - 14, 0.0f);
   glPopMatrix();
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
   glutSwapBuffers();

   usleep(10000);

   Y_Rot += Y_Speed;
}//((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))






void CallBackResizeScene(int Width, int Height)
{
   if (Height == 0)
      Height = 1;
  
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-16, 16, -12, 12, -40, 40);
   gluLookAt(Lookat_X, Lookat_Y, Lookat_Z,Robot_X,Robot_Y,Robot_Z,0,1,0);
   glMatrixMode(GL_MODELVIEW);

   Window_Width  = Width;
   Window_Height = Height;
}


void MyInit(int Width, int Height) 
{
   gluLookAt(5, 5, 5 ,0,0, 0, 0, 1, 0);
   glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
   glClearDepth(1.0);
   glDepthFunc(GL_LESS); 
   glShadeModel(GL_SMOOTH);
   CallBackResizeScene(Width,Height);
}

void myCBKey(unsigned char key, int x, int y)
{
 switch (key)
   {
      case 'q':
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 90)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 180)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 270)
            {
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
         }
      }
      break;   

      case 'a':
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               adjustHead(270);
               robRot(270);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 270)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               adjustHead(270);
               robRot(270);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 180)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 90)
            {
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               adjustHead(270);
               robRot(270);
               CallBackResizeScene(Window_Width, Window_Height);
            }
         }
      }
      break;   

      case 'z':
      {
         if(!paused)
         {
            pushRobot();
            CallBackResizeScene(Window_Width, Window_Height);
         }

      }
      break;   

      case 'r': 
      {
         if(!paused)
         {
            Lookat_X = 0;
            Lookat_Z = 10;
            Robot_X = 0;
            Robot_Z = 0;
            robotRotate = 0;
            headRotate = 0;
            glutPostRedisplay();
            CallBackResizeScene(Window_Width, Window_Height);            
         }
      }
      break;

      case 'p':
         paused = !paused;
      break;
   default:
      printf ("KP: No action for %d.\n", key);
   break;
   }
}

//*******************************************************
// Temporary mouse functions. Selecting building        *
// Will be implemented later                            *
//*******************************************************
void mouseClick(int button, int state, int x, int y)
{
  if(!paused)
  {
     if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
        gluLookAt(5, 5, 5 ,0,0, 0, 0, 1, 0);
     
     if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) 
        CallBackResizeScene(Window_Width, Window_Height);

     if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
         gluLookAt(0,4, -10,0,4,0,0,1,0);
     
     if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
        CallBackResizeScene(Window_Width, Window_Height);

     if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
        gluLookAt(-5,20, -5,0,0,0,0,0,1);
     
     if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
        CallBackResizeScene(Window_Width, Window_Height);   
  }

}

//Actions for when a key is released
void keySpecialUp (int key, int x, int y) 
{  
   switch (key) {
      case GLUT_KEY_F2: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {     
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }    
         }
         break;
      }

      case GLUT_KEY_F3: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {     
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }            
         }
         break;
      }
   }
   glutPostRedisplay();
   CallBackResizeScene(Window_Width, Window_Height);
}

//Actions for when a key is pressed
void CallBackSpecialKeyPressed(int key, int x, int y)
{
   switch (key) {
      case GLUT_KEY_F1:
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {     
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }     
         }  
         break;
      }

      case GLUT_KEY_F2: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {     
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 90)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }
            else if(robotRotate == 270)
            {
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }                       
         }
         break;     
      }

      case GLUT_KEY_F3: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {     
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }
            else if(robotRotate == 90)
            {
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 270)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }            
         }
         break; 
      }

      case GLUT_KEY_F4: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {     
               Lookat_Z = 10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = 10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -10.0 + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -10.0 + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            } 
         }
         break;
      }

      case GLUT_KEY_F5:
      { 
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }           
         }
         break;
      }    

      case GLUT_KEY_F6: 
      { 
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }            
         }
         break;
      }

      case GLUT_KEY_F7: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }            
         }
         break;
      }      

      case GLUT_KEY_F8: 
      {
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = -10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = 10 + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = 10 + Robot_X;
               Lookat_Z = -10 + Robot_Z;
            }            
         }
         break;
      }
   break;
   }
   glutPostRedisplay();
   CallBackResizeScene(Window_Width, Window_Height);
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(Window_Width, Window_Height);
   Window_ID = glutCreateWindow(PROGRAM_TITLE);

   if (!paused)
   {
      glutDisplayFunc(&CallBackRenderScene);
      glutIdleFunc(&CallBackRenderScene);
      glutReshapeFunc(&CallBackResizeScene);
      glutTimerFunc(10, update, 0);
   }


   glutKeyboardFunc(&myCBKey);
   glutSpecialUpFunc(&keySpecialUp);
   glutSpecialFunc(&CallBackSpecialKeyPressed);
   MyInit(Window_Width, Window_Height);
   glutMouseFunc(&mouseClick);
   glEnable(GL_DEPTH_TEST); 
   glutMainLoop();
   
   return 1; 
}
