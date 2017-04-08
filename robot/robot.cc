#define PROGRAM_TITLE "Robot Rampage Reloaded"
#define Z_dist 1.0


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
#include <iostream>
#include <time.h>

// Window IDs, window width and height.
int Window_ID;
int Window_Width = 800;
int Window_Height = 600;

// The cubes rotation 
double Y_Rot   = 0.0f;  
double Y_Speed = -30.0f;
int Robot_X = 0; 
int Robot_Y = -1; 
int Robot_Z = 0; 
double Lookat_X = 0.0;
double Lookat_Y = 0.5;
double Lookat_Z = 1.0;

int robotRotate = 0; 
int headRotate = 0.0;
 
static bool paused = false;
static bool Specs = false;

//Building Variables
float blockSize = 10.0f;
float nextX = 0.0f;
float nextY = 0.0f;
float nextZ = 0.0f;

float startingZ = 0.0f;
float startingX = 0.0f;

float streetSize = 10.0f;

const float step = blockSize + streetSize;

const int numberOfBlocks = 20;

float rectRotSpeed = 0.5f;
float rectRot = 0.0f;
const int numberOfBuildingsPerBlock = 4;
const int maxBuildings = ((numberOfBlocks * numberOfBlocks) * numberOfBuildingsPerBlock);

bool buildingsGenerated = false;
int buildingIndex = 0;

enum BuildingType { RECT, CYL, NONE };

struct BuildingInfo
{
   float xPosition;
   float yPosition;
   float zPosition;

   int xzScalar;
   int yScalar;
   BuildingType type;
};

BuildingInfo buildings[maxBuildings];

static void PrintString(void *font, char *str)
{
   int i,len=strlen(str);

   for(i=0;i < len; i++)
      glutBitmapCharacter(font,*str++);
}

void renderStrokeFontString(
      float x,
      float y,
      float z,
      void *font,
      char *string) {

   char *c;
   glPushMatrix();
   glTranslatef(x, y,z);

   for (c=string; *c != '\0'; c++) {
      glutStrokeCharacter(font, *c);
   }

   glPopMatrix();
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
      Lookat_Z -= 1;
      Robot_Z -= 1; 
   }

   else if((robotRotate) == 90)
   {
      Lookat_X -= 1;
      Robot_X -= 1;
   }

   else if((robotRotate) == 180)
   {
      Lookat_Z += 1;
      Robot_Z += 1; 
   }

   else if((robotRotate) == 270)
   {
      Lookat_X += 1;
      Robot_X += 1;
   }
}

void pullRobot()
{
   if(robotRotate == 0)
   {
      Lookat_Z += 1;
      Robot_Z += 1; 
   }

   else if((robotRotate) == 90)
   {
      Lookat_X += 1;
      Robot_X += 1;
   }

   else if((robotRotate) == 180)
   {
      Lookat_Z -= 1;
      Robot_Z -= 1; 
   }

   else if((robotRotate) == 270)
   {
      Lookat_X -= 1;
      Robot_X -= 1;
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

void init()
{
   startingX = -(((blockSize * (numberOfBlocks/1.0f)) + (streetSize * ((numberOfBlocks / 4.0f) - 1))) + (blockSize / 1.0f));
   startingZ = -(((blockSize * (numberOfBlocks/1.0f)) + (streetSize * ((numberOfBlocks / 4.0f) - 1))) + (blockSize / 1.0f));
   srand(time(NULL));
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

void drawCylindricBuilding(int xzScalar, int yScalar)
{
   GLUquadric* building;
   building = gluNewQuadric();

   glTranslatef(0.0f, yScalar, 0.0f);
   glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
   
   glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
   gluCylinder(building, 1 * xzScalar, 1 * xzScalar, 1 * yScalar, 90, 60);

   glColor4f(0.9f, 0.9f, 0.0f, 1.0f);
   drawFilledCircle(0.0f, 0.0f, xzScalar * 1);
}

void drawRectangularBuilding(int xzScalar, int yScalar)
{
   // BOTTOM FACE
   glBegin(GL_QUADS);

   glNormal3f(0.0f, -1.0f, 0.0f);
   glColor4f(0.9f, 0.0f, 0.5f, 1.0f);
   glVertex3f(-0.5f * xzScalar, 0.0f, -0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 0.0f, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 0.0f, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 0.0f, -0.5f * xzScalar);
   
   glEnd();

   // LEFT FACE
   glBegin(GL_QUADS);

   glNormal3f(-1.0f, 0.0f, 0.0f);
   glColor4f(0.0f, 0.9f, 0.0f, 1.0f);
   glVertex3f(-0.5f * xzScalar, 1.0f * yScalar, -0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 0.0f, -0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 0.0f, 0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 1.0f * yScalar, 0.5f * xzScalar);
   
   glEnd();

   // FRONT FACE
   
   glBegin(GL_QUADS);

   glNormal3f(0.0f, 0.0f, 1.0f);
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glVertex3f(-0.5f * xzScalar, 1.0f * yScalar, 0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 0.0f, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 0.0f, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 1.0f * yScalar, 0.5f * xzScalar);
   
   glEnd();

   // RIGHT FACE

   glBegin(GL_QUADS);

   glNormal3f(1.0f, 0.0f, 0.0f);
   glColor4f(0.0f, 0.0f, 0.9f, 1.0f);
   glVertex3f(0.5f * xzScalar, 1.0f * yScalar, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 0.0f, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 0.0f, -0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 1.0f * yScalar, -0.5f * xzScalar);

   glEnd();

   // BACK FACE

   glBegin(GL_QUADS);

   glNormal3f(0.0f, 0.0f, -1.0f);
   glColor4f(0.9f, 0.9f, 0.0f, 1.0f);
   glVertex3f(0.5f * xzScalar, 1.0f * yScalar, -0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 0.0f, -0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 0.0f, -0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 1.0f * yScalar, -0.5f * xzScalar);
   
   glEnd();

   // TOP FACE

   glBegin(GL_QUADS);

   glNormal3f(0.0f, 1.0f, 0.0f);
   glColor4f(0.1, 0.7, 0.9, 1.0f);
   glVertex3f(-0.5f * xzScalar, 1.0f * yScalar, -0.5f * xzScalar);
   glVertex3f(-0.5f * xzScalar, 1.0f * yScalar, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 1.0f * yScalar, 0.5f * xzScalar);
   glVertex3f(0.5f * xzScalar, 1.0f * yScalar, -0.5f * xzScalar);

   glEnd();
}

void drawBlock()
{
   glBegin(GL_POLYGON);

   glNormal3f(0.0f, 1.0f, 0.0f);
   glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
   glVertex3f(0.0f - (blockSize / 2), 0.0f, 0.0f - (blockSize / 2));
   glVertex3f(0.0f - (blockSize / 2), 0.0f, 0.0f + (blockSize / 2));
   glVertex3f(0.0f + (blockSize / 2), 0.0f, 0.0f + (blockSize / 2));
   glVertex3f(0.0f + (blockSize / 2), 0.0f, 0.0f - (blockSize / 2));
   
   glEnd();
}

void drawAndPositionBlock()
{
   glTranslatef(nextX, nextY, nextZ);
   drawBlock();
   
   glPopMatrix();
   glPushMatrix();
}

void randomlyGenerateBuildings()
{
   float xQuadrantValue = nextX - (blockSize/4);
   float zQuadrantValue = nextZ - (blockSize/4);
   BuildingInfo bInfo;

   for(int i = 0; i < 2; ++i)
   {
      for(int j = 0; j < 2; ++j)
      {

         int maxBaseSize = blockSize / 4;
         int minBaseSize = maxBaseSize - 2;
         if(minBaseSize < 1)
            minBaseSize = 1;

         int maxHeight = 6;
         int minHeight = maxHeight - 3;

         int randomBase = rand() % maxBaseSize + minBaseSize;
         int randomHeight = rand() % maxHeight + minHeight;
         glTranslatef(xQuadrantValue, nextY, zQuadrantValue);
         if(rand() % 2 == 0)
         {
            drawRectangularBuilding(randomBase, randomHeight);
            bInfo.type = RECT;
         }
         else
         {
            drawCylindricBuilding(randomBase, randomHeight);
            bInfo.type = CYL;
         }

         bInfo.xPosition = xQuadrantValue;
         bInfo.yPosition = nextY;
         bInfo.zPosition = zQuadrantValue;

         bInfo.xzScalar = randomBase;
         bInfo.yScalar = randomHeight;

         glPopMatrix();
         glPushMatrix();

         xQuadrantValue += (blockSize / 2);

         if(buildingIndex < maxBuildings)
            buildings[buildingIndex++] = bInfo;
      }

      zQuadrantValue += (blockSize / 2);
      xQuadrantValue = nextX - (blockSize/4);
   }
}

void drawBuildings(GLenum mode)
{

   for(int i = 0; i < maxBuildings; i++)
   {
      BuildingInfo info = buildings[i];

      glTranslatef(info.xPosition, info.yPosition, info.zPosition);  

      if(mode == GL_SELECT)
         glLoadName(i);
      if(info.type == CYL)
         drawCylindricBuilding(info.xzScalar, info.yScalar);
      else if(info.type == RECT)
         drawRectangularBuilding(info.xzScalar, info.yScalar);
      
      glPopMatrix();
      glPushMatrix();
   }
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

   usleep(100000);
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

  
   glTranslatef(0.0f,6.0f, 0.0f);
    usleep(10000);
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

   glTranslatef(0,0,-1.1);
  
   drawFilledCircle(0.45,4.35,0.25); 

}//*****************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&********************************************&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*******************************&&&&

//function to show data about robot's location in space
void showSpecs()
{
   char buf[80]; // For our strings.

   sprintf(buf,"%s", "Lookat_X: ");
   glRasterPos2i(300,26);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_X);
   glRasterPos2i(375,26);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Lookat_Y: ");
   glRasterPos2i(300,14);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_Y);
   glRasterPos2i(375,14);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Lookat_Z: ");
   glRasterPos2i(300,2);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%f", Lookat_Z);
   glRasterPos2i(375,2);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Robot_X: ");
   glRasterPos2i(300,38);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", Robot_X);
   glRasterPos2i(375,38);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Robot_Y: ");
   glRasterPos2i(300,50);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", Robot_Y);
   glRasterPos2i(375,50);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "Robot_Z: ");
   glRasterPos2i(300,62);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", Robot_Z);
   glRasterPos2i(375,62);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "robotRotate: ");
   glRasterPos2i(300,74);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", robotRotate);
   glRasterPos2i(375,74);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%s", "headRotate: ");
   glRasterPos2i(300,86);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

   sprintf(buf,"%i", headRotate);
   glRasterPos2i(375,86);
   PrintString(GLUT_BITMAP_HELVETICA_12, buf);

}

//function to show a pause screen with p is pressed
void pauseSplash()
{
   char buf[80];

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   sprintf(buf,"%s", "Paused");
   glRasterPos2i(362,312);
   PrintString(GLUT_BITMAP_TIMES_ROMAN_24, buf);

   sprintf(buf,"%s", "Press 'p' again to resume");
   glRasterPos2i(300,288);
   PrintString(GLUT_BITMAP_HELVETICA_18, buf);    
}

void CallBackRenderScene(void)//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
{
   char buf[80]; // For our strings.

   nextZ = startingZ;
   nextX = startingX;

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glPopMatrix();
   glPushMatrix();

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   for(int i = 0; i < numberOfBlocks; ++i)
   {
      for(int j = 0; j < numberOfBlocks; ++j)
      {
    drawAndPositionBlock();
         if(!buildingsGenerated)
            randomlyGenerateBuildings();
         nextZ += step;
      }
      
      nextX += step;
      nextZ = startingZ;
   }

   if(buildingsGenerated)
   {
      drawBuildings(GL_RENDER);
      glFlush();
   }
   

   buildingsGenerated = true;

   buildRobot(); 

   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();   
   glLoadIdentity();

   glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
   glColor4f(0.0,0.8,0.5,1.00); 

   //show translate, rotate and lookat data. toggle with 's'
   if(Specs)
      showSpecs();

   //Pause Menu. toggle with 'p'
   if(paused)
      pauseSplash();


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
   //glViewport(0, 0, Window_Width, Window_Height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-12, 12, -9, 9, -12, 12);
   gluLookAt(Lookat_X, Lookat_Y, Lookat_Z,Robot_X,Robot_Y,Robot_Z,0,1,0);
   glMatrixMode(GL_MODELVIEW);

   Window_Width  = Width;
   Window_Height = Height;
}


void MyInit(int Width, int Height) 
{
   //gluLookAt(0, 0, 0 ,0,0, 0, 0, 1, 0);
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
      case 'Q':
      {
        if(((Robot_X % 20) == 0) && (Robot_Z % 20) == 0)
         {
         if(!paused)
         {
            if(robotRotate == 0)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 90)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 180)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
            else if((robotRotate) == 270)
            {
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               adjustHead(90);
               robRot(90);
               CallBackResizeScene(Window_Width, Window_Height);
            }
         }
         }
      }
      break;   

      case 'a':
      case 'A':
      {
         if(!paused)
         {
            if(((Robot_X % 20) == 0) && (Robot_Z % 20) == 0)
           {
                if(robotRotate == 0)
                  {
                     Lookat_X = -Z_dist + Robot_X;
                     Lookat_Z = Robot_Z;
                     adjustHead(270);
                     robRot(270);
                     CallBackResizeScene(Window_Width, Window_Height);
                  }
               else if((robotRotate) == 270)
                  {
                     Lookat_Z = -Z_dist + Robot_Z;
                     Lookat_X = Robot_X;
                     adjustHead(270);
                     robRot(270);
                     CallBackResizeScene(Window_Width, Window_Height);
                  }
               else if((robotRotate) == 180)
                  {
                     Lookat_X = Z_dist + Robot_X;
                     Lookat_Z = Robot_Z;
                     adjustHead(270);
                     robRot(270);
                     CallBackResizeScene(Window_Width, Window_Height);
                  }
               else if((robotRotate) == 90)
                 {
                     Lookat_Z = Z_dist + Robot_Z;
                     Lookat_X = Robot_X;
                     adjustHead(270);
                     robRot(270);
                     CallBackResizeScene(Window_Width, Window_Height);
                  }
           }
         }
      }
      break;   

      case 'z':
      case 'Z':
      {
         if(!paused)
         {
            pushRobot();
            CallBackResizeScene(Window_Width, Window_Height);
         }

      }
      break;  

      case 'x':
      case 'X':
      {
         if(!paused)
         {
            pullRobot();
            CallBackResizeScene(Window_Width, Window_Height);
         }

      }
      break;  

      case 'r': 
      case 'R':
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

      case 's': 
      case 'S':
      {
         if(!paused)
         {
            Specs = !Specs;            
         }
      }
      break;

      case 'p':
      case 'P':
         paused = !paused;
      break;
   default:
      printf ("KP: No action for %d.\n", key);
   break;
   }
}

void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

   printf ("hits = %d\n", hits);
   ptr = (GLuint *) buffer;
   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) {  
      names = *ptr;
     ptr++;
     if (*ptr < minZ) {
        numberOfNames = names;
        minZ = *ptr;
        ptrNames = ptr+2;
     }
     
     ptr += names+2;
   }

  printf ("The closest hit names are:");
  ptr = ptrNames;
  for (j = 0; j < numberOfNames; j++,ptr++) {
     printf ("%d ", *ptr);
  }
  printf ("\n");
}

//*******************************************************
// Temporary mouse functions. Selecting building        *
// Will be implemented later                            *
//*******************************************************
#define SIZE 512
void mouse(int button, int state, int x, int y)
{
   GLuint selectBuf[SIZE];
   GLint hits;
   GLint viewport[4];

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
   {
      printf("x = %i ", x);
      printf("y = %i\n", y);
      glGetIntegerv(GL_VIEWPORT, viewport);

      glSelectBuffer(SIZE, selectBuf);
      glRenderMode(GL_SELECT);

      glInitNames();
      glPushName(0);
   
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();

      /*  create 5x5 pixel picking region near cursor location */
      gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);
      gluOrtho2D(-5, 5, -5, 5);
      drawBuildings(GL_SELECT);


      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glFlush();

      hits = glRenderMode(GL_RENDER);
      processHits(hits, selectBuf);

      glutPostRedisplay();
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
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -Z_dist + Robot_X;
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
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -Z_dist + Robot_X;
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
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -Z_dist + Robot_X;
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
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }
            else if(robotRotate == 90)
            {
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 270)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
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
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 90)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 270;
            }
            else if(robotRotate == 270)
            {
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
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
               Lookat_Z = Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 0;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Robot_Z;
               headRotate = 90;
            }
            else if(robotRotate == 180)
            {
               Lookat_Z = -Z_dist + Robot_Z;
               Lookat_X = Robot_X;
               headRotate = 180;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -Z_dist + Robot_X;
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
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
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
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
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
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
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
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
            }
            else if(robotRotate == 90)
            {
               Lookat_X = -Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
            }
            else if(robotRotate == 180)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = Z_dist + Robot_Z;
            }
            else if(robotRotate == 270)
            {
               Lookat_X = Z_dist + Robot_X;
               Lookat_Z = -Z_dist + Robot_Z;
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
   init();
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
   glutMouseFunc(mouse);
   glEnable(GL_DEPTH_TEST); 
   glutMainLoop();
   
   return 1; 
}
