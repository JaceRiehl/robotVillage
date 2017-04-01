#define PROGRAM_TITLE "Jace Riehl" 

#include <stdlib.h> 
#include <stdio.h>    
#include <string.h>  
#include <unistd.h> 
#include <math.h>
#include <GL/gl.h>  
#include <GL/glu.h> 
#include <GL/glut.h> 
#include<unistd.h> 
#include<cmath> 

// Window IDs, window width and height.
int Window_ID;
int Window_Width = 800;
int Window_Height = 600;

// The cubes rotation 
float Y_Rot   = 0.0f;  
float Y_Speed = -30.0f;
float Head_Rot = 0; 

//Z value
float Z_Off = 0.0f;
float Robot_X = 0; 
float Robot_Y = 0; 
float Robot_Z = 0; 
float Lookat_X = 0.0f;
float Lookat_Y = 4.0f;
float Lookat_Z = 10.0f;
int robotRotate = 0; 
bool translateOrigin = false; 

static void PrintString(void *font, char *str)
{
   int i,len=strlen(str);

   for(i=0;i < len; i++)
      glutBitmapCharacter(font,*str++);
}

void newLookAt(float x, float y, float z)
{
   Lookat_X = x;
   Lookat_Y = y;
   Lookat_Z = z;
}

void pushRobot()
{
   if(robotRotate == 0)
      Robot_Z -= 0.05f; 
   else if((robotRotate) == 90)
      Robot_X -= 0.05f;
   else if((robotRotate) == 180)
      Robot_Z += 0.05f; 
   else if((robotRotate) == 270)
      Robot_X += 0.05f;
}

void robRot(int r)
{
   robotRotate = (r + robotRotate) % 360; 

}

void adjustHead(float r)
{
   Head_Rot = robotRotate + r;
}

void adjustLookat()
{
   
   if((robotRotate) == 90)
        gluLookAt(5,4, 10,0,0,0,0,1,0);
   else if((robotRotate) == 180)
      gluLookAt(0,4 , -10,0,0,0,0,1,0);
   else if((robotRotate) == 270)
      gluLookAt(0,4, 10 ,0,0,0,0,1,0);
   else 
      gluLookAt(-5,4 , 10,0,0,0,0,1,0);
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

   if(translateOrigin == true)
      glTranslatef(0,0,0);

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

   if(translateOrigin == true)
   glTranslatef(0,0,0);
   
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(Head_Rot, 0,1,0);

   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
     glTranslatef(Robot_X, Robot_Y, Robot_Z);
   //glRotatef(robotRotate, 0,1,0); 

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

   if(translateOrigin == true)
      glTranslatef(0,0,0);


   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(robotRotate, 0,1,0); 


   glTranslatef(-Robot_X, -Robot_Y, -Robot_Z);
     glTranslatef(Robot_X, Robot_Y, Robot_Z);
   glBegin(GL_QUADS); 

   //Rotate object and translate it used for when the cube is pushed
   //usleep(100000); 
   
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glBegin(GL_QUADS); 

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
   if(translateOrigin == true)
      glTranslatef(0,0,0);


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
   //YELLOW ON HEAD
   if(translateOrigin == true)
      glTranslatef(0,0,0);

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


   //RED ON HEAD
   if(translateOrigin == true)
      glTranslatef(0,0,0);


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

   if(translateOrigin == true)
      glTranslatef(0,0,0);

   glTranslatef(0 + Robot_X,3.5 + Robot_Y,0 + Robot_Z);
   //glRotatef(robotRotate, 0,1,0); 
   glRotatef(90,1.0f,0.0f,0.0f);
   
   glColor3f(0.6,0.0,0.6);
   quadric = gluNewQuadric();
   gluCylinder(quadric,0.75,0.75,0.5,90,60);
   


   //EYES
   glPopMatrix();
   glPushMatrix();

   if(translateOrigin == true)
      glTranslatef(0,0,0);
   
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(Head_Rot, 0,1,0);
   glTranslatef(0,0,-1.1);
  
   drawFilledCircle(-0.45,4.35,0.25); 

   glPopMatrix();
   glPushMatrix();
   
   glTranslatef(Robot_X, Robot_Y, Robot_Z);
  
   glRotatef(Head_Rot, 0,1,0);
   //glRotatef(robotRotate, 0,1,0); 

   glTranslatef(0,0,-1.1);
  
   drawFilledCircle(0.45,4.35,0.25); 

}//*****************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&********************************************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*******************************&&&&




void CallBackRenderScene(void)//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
{
   char buf[80]; // For our strings.

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
  
   //Rotate object and translate it used for when the cube is pushed
   //glTranslatef(0.0f,0.0f,-2.0f);
   //glRotatef(Pyr_Rot,0.0f,0.0f,1.0f);
   //glTranslatef(0.0f,0.0f,2.0f);
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   buildRobot(); 

   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();   
   glLoadIdentity();

   glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
   glColor4f(0.6,1.0,0.6,1.00); 

   sprintf(buf,"%f", Lookat_X);
   glRasterPos2i(2,2);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_Y); // Print the string into a buffer
   glRasterPos2i(2,14);                         // Set the coordinate
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);  // Display the string.

   sprintf(buf,"%f", Lookat_Z); // Print the string into a buffer
   glRasterPos2i(2,26);                         // Set the coordinate
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);  // Display the string.

   glTranslatef(6.0f, Window_Height - 14, 0.0f);
   glPopMatrix();
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
   glutSwapBuffers();


   usleep(10000);
   adjustHead(0);


   Y_Rot += Y_Speed;
}//((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))






void CallBackResizeScene(int Width, int Height)
{
   if (Height == 0)
      Height = 1;
  
   //adjustLookat(); 
   
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
         if(robotRotate == 0)
         {
            newLookAt(10.0f,4.0f,0.0f);
            robRot(90);
            CallBackResizeScene(Window_Width, Window_Height);
         }
         else if((robotRotate) == 90)
         {
            newLookAt(0.0f,4.0f,-10.0f);
            robRot(90);
            CallBackResizeScene(Window_Width, Window_Height);
         }
         else if((robotRotate) == 180)
         {
            newLookAt(-10.0f,4.0f,0.0f);
            robRot(90);
            CallBackResizeScene(Window_Width, Window_Height);
         }
         else if((robotRotate) == 270)
         {
            newLookAt(0.0f,4.0f,10.0f);
            robRot(90);
            CallBackResizeScene(Window_Width, Window_Height);
         }
      }
    break;   

          case 'a':
      {
         if(robotRotate == 0)
         {
            newLookAt(-10.0f,4.0f,0.0f);
            robRot(270);
            CallBackResizeScene(Window_Width, Window_Height);
         }
         else if((robotRotate) == 270)
         {
            newLookAt(0.0f,4.0f,-10.0f);
            robRot(270);
            CallBackResizeScene(Window_Width, Window_Height);
         }
         else if((robotRotate) == 180)
         {
            newLookAt(-10.0f,4.0f,0.0f);
            robRot(90);
            CallBackResizeScene(Window_Width, Window_Height);
         }
         else if((robotRotate) == 90)
         {
            newLookAt(0.0f,4.0f,10.0f);
            robRot(270);
            CallBackResizeScene(Window_Width, Window_Height);
         }
      }
    break;   

      case 'z':
      {
         pushRobot();
         CallBackResizeScene(Window_Width, Window_Height);
      }
    break;   

    case 'r': 
      translateOrigin = true;

   break;
   default:
      printf ("KP: No action for %d.\n", key);
      break;
   }
}

void mouseClick(int button, int state, int x, int y)
{
 if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
   //gluLookAt(0,4, -10,0,4,0,0,1,0);
     gluLookAt(5, 5, 5 ,0,0, 0, 0, 1, 0);
  
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) 
     CallBackResizeScene(Window_Width, Window_Height);

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
      gluLookAt(0,4, -10,0,4,0,0,1,0);
     //gluLookAt(5, 5, 5 ,0,0, 0, 0, 1, 0);
  
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
     CallBackResizeScene(Window_Width, Window_Height);

    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
      gluLookAt(-5,20, -5,0,0,0,0,0,1);
     //gluLookAt(5, 5, 5 ,0,0, 0, 0, 1, 0);
  
  if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
     CallBackResizeScene(Window_Width, Window_Height);
}

void CallBackSpecialKeyPressed(int key, int x, int y)
{
   switch (key) {
   case GLUT_KEY_F1:
      adjustHead(0);
      break;
   case GLUT_KEY_F2: 
      adjustHead(270);
      break;

   case GLUT_KEY_F3: 
      adjustHead(90);
      break;

   case GLUT_KEY_F4: 
     CallBackResizeScene(Window_Width, Window_Height);
      break;

   case GLUT_KEY_F5: 
   if(robotRotate == 90)
      gluLookAt(3 + Robot_X, 0 - Robot_Y, 3 + Robot_Z ,Robot_X, Robot_Y, Robot_Z, 0, 1, 0);
   else if(robotRotate == 180)
      gluLookAt(3 - Robot_X, 0 - Robot_Y, -3 + Robot_Z ,Robot_X, Robot_Y, Robot_Z, 0, 1, 0);
   else if(robotRotate == 270)
      gluLookAt(-3 - Robot_X, 0 - Robot_Y, -3 + Robot_Z ,Robot_X, Robot_Y, Robot_Z, 0, 1, 0);
   else 
       gluLookAt(-3 + Robot_X, 0 - Robot_Y, 3 - Robot_Z ,Robot_X, Robot_Y, Robot_Z, 0, 1, 0);

      break;



   case GLUT_KEY_F6: 
    if(robotRotate == 90)
      gluLookAt(3 - Robot_X, 0, -3 + Robot_Z ,Robot_X,Robot_Y, Robot_Z, 0, 1, 0);
   else  if(robotRotate == 180)
      gluLookAt(-3 - Robot_X, 0, -3 + Robot_Z ,Robot_X,Robot_Y, Robot_Z, 0, 1, 0);
   else  if(robotRotate == 270)
      gluLookAt(-3 - Robot_X, 0, 3 + Robot_Z ,Robot_X,Robot_Y, Robot_Z, 0, 1, 0);
   else
      gluLookAt(3 - Robot_X, 0, 3 + Robot_Z ,Robot_X,Robot_Y, Robot_Z, 0, 1, 0);
      break;



   case GLUT_KEY_F7: 
      gluLookAt(5 - Robot_X, 5, -5 + Robot_Z ,Robot_X,Robot_Y, Robot_Z, 0, 1, 0);
      break;

   case GLUT_KEY_F8:
      gluLookAt(-5 - Robot_X, 5, -5 + Robot_Z ,Robot_X,Robot_Y, Robot_Z, 0, 1, 0);
      break;

      break;
   default:
      printf ("SKP: No action for %d.\n", key);
      break;
    }
}


int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(Window_Width, Window_Height);
   Window_ID = glutCreateWindow(PROGRAM_TITLE);
   glutDisplayFunc(&CallBackRenderScene);
   glutIdleFunc(&CallBackRenderScene);
   glutReshapeFunc(&CallBackResizeScene);
   glutKeyboardFunc(&myCBKey);
   glutSpecialFunc(&CallBackSpecialKeyPressed);
   MyInit(Window_Width, Window_Height);
   glutMouseFunc(&mouseClick);
   glEnable(GL_DEPTH_TEST); 
   glutMainLoop();
   
   return 1; 
}
