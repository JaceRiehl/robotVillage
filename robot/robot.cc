#define PROGRAM_TITLE "Jace Riehl" 

#include <stdlib.h> 
#include <stdio.h>    
#include <string.h>  

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
float Y_Speed = 0.5f;
float Head_Rot = 0; 
float Head_Speed = 0;
//Z value
float Z_Off = 0.0f;




void CallBackRenderScene(void)
{
// char buf[80]; // For our strings.
   glViewport(0,0,800,600);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
  
   //Rotate object and translate it used for when the cube is pushed
   //glTranslatef(0.0f,0.0f,-2.0f);
   //glRotatef(Pyr_Rot,0.0f,0.0f,1.0f);
   //glTranslatef(0.0f,0.0f,2.0f);
   
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
   glPopMatrix();

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

//HEAD

      glBegin(GL_QUADS); 

   //Rotate object and translate it used for when the cube is pushed
   //usleep(100000); 
   
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glBegin(GL_QUADS); 

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
   //glTranslatef(0.0f,0.0f,0.0f);
   //glRotatef(Y_Rot,0.0f,0.0f,1.0f);
   //glTranslatef(0.0f,0.0f,0.0f);
   
  
   glBegin(GL_QUADS); 

   //Rotate object and translate it used for when the cube is pushed
   //usleep(100000); 
   
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glBegin(GL_QUADS); 

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

   glPopMatrix(); 
   glPushMatrix();

   glTranslatef(0.0f,-6.0f,0.0f);
   glRotatef(Y_Rot,0.0f,1.0f,0.0f);
   glTranslatef(0.0f,-6.0f,0.0f);

   glColor3f(0.9,0.0,0.9);
   GLUquadric *quadric;
   quadric = gluNewQuadric();
   glPopMatrix();
   glPushMatrix();
   glTranslatef(0,6,0);
   glRotatef(90,1.0f,0.0f,0.0f);
   glTranslatef(0,0,0); 
   gluCylinder(quadric,0.5,0.5,1.0,30,30);
   

   
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();   
   glTranslatef(6.0f, Window_Height - 14, 0.0f);
   glPopMatrix();
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
   glutSwapBuffers();
   Y_Rot += Y_Speed;
}


void myCBKey(unsigned char key, int x, int y)
{
 switch (key)
   {
      case 'z':
      {
    Z_Off -= 0.50f;
      }
    break;   
   default:
      printf ("KP: No action for %d.\n", key);
      break;
   }
}



void CallBackResizeScene(int Width, int Height)
{
   if (Height == 0)
      Height = 1;
  
       
   gluLookAt(0,4, 10,0,4,0,0,1,0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-16, 16, -12, 12, -20, 20);
   glMatrixMode(GL_MODELVIEW);
   Window_Width  = Width;
   Window_Height = Height;
}


void MyInit(int Width, int Height) 
{
   glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
   glClearDepth(1.0);
   glDepthFunc(GL_LESS); 
   glShadeModel(GL_SMOOTH);
   CallBackResizeScene(Width,Height);
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
   MyInit(Window_Width, Window_Height);
   glutMouseFunc(&mouseClick);
   glEnable(GL_DEPTH_TEST); 
   glutMainLoop();
   
   return 1; 
}
