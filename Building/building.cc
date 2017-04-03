#define PROGRAM_TITLE "NATHAN TIPPER"
#define DISPLAY_INFO "First OpenGL Program"


#include <stdlib.h>  // Useful for the following includes.
#include <stdio.h>    
#include <string.h>  // For spring operations.

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <time.h>

// Some global variables.
// Window IDs, window width and height.
int Window_ID;
int Window_Width = 800;
int Window_Height = 600;

float blockSize = 5.0f;
float nextX = 0.0f;
float nextY = 0.0f;
float nextZ = 0.0f;

float startingZ = 0.0f;
float startingX = 0.0f;

float streetSize = 10.0f;

const float step = blockSize + streetSize;

const int numberOfBlocks = 4;

float rectRotSpeed = 0.5f;
float rectRot = 0.0f;
const int numberOfBuildingsPerBlock = 4;
const int maxBuildings = (numberOfBlocks * numberOfBlocks) * numberOfBuildingsPerBlock;

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

void init()
{
   startingX = -(((blockSize * (numberOfBlocks/2.0f)) + (streetSize * ((numberOfBlocks / 2.0f) - 1))) + (blockSize / 2));
   startingZ = -(((blockSize * (numberOfBlocks/2.0f)) + (streetSize * ((numberOfBlocks / 2.0f) - 1))) + (blockSize / 2));
   srand(time(NULL));
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
   int i;
   int triangleAmount = 20; //# of triangles used to draw circle
   
   //GLfloat radius = 0.8f; //radius
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
   glColor4f(0.9f, 0.3f, 0.7f, 1.0f);
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
         if(rand() % 10 == 0)
         {
            bInfo.type = NONE;
            continue;
         }

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

void drawBuildings()
{
   for(int i = 0; i < maxBuildings; ++i)
   {
      BuildingInfo info = buildings[i];

      if(info.type == NONE)
         continue;
      
      glTranslatef(info.xPosition, info.yPosition, info.zPosition);

      if(info.type == CYL)
         drawCylindricBuilding(info.xzScalar, info.yScalar);
      else if(info.type == RECT)
         drawRectangularBuilding(info.xzScalar, info.yScalar);

      glPopMatrix();
      glPushMatrix();
   }
}

/////////////////////////////////////////////////////////
// Routine which actually does the drawing             //
/////////////////////////////////////////////////////////
void CallBackRenderScene(void)
{
   glViewport(0, 0, Window_Width, Window_Height);
   // Need to manipulate the ModelView matrix to move our model around.

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
      drawBuildings();

   buildingsGenerated = true;

   glPopMatrix();
   glPushMatrix();
   
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   
   glutSwapBuffers();
}

///////////////////////////////////////////////////////////////
// Callback routine executed whenever the window is resized. //
//////////////////////////////////////////////////////////////
void CallBackResizeScene(int Width, int Height)
{
   // Let's not core dump, no matter what.
   if (Height == 0)
      Height = 1;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //gluPerspective(45.0f, (GLfloat)Width/(GLfloat)Height, 0.1f, 100.0f);
   glOrtho(-16, 16, -12, 12, -40, 40);
   gluLookAt(0,2,10, 0, 0, 0,0,1,0);
   
   glMatrixMode(GL_MODELVIEW);

   Window_Width  = Width;
   Window_Height = Height;
}

////////////////////////////////////////////////////////////
// Callback function called when a normal key is pressed. //
////////////////////////////////////////////////////////////
void CallBackKeyPressed(unsigned char key, int x, int y)
{ 
   
}

void MouseButtonPressed(int button, int state, int x, int y)
{
   if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
   {
      gluLookAt(-10, 20, 5, 0, 0, 0, 0, 1, 0);
   }

   if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
   {
      CallBackResizeScene(Window_Width, Window_Height);
   }

   if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      gluLookAt(-10, -20, 5, 0, 0, 0, 0, 1, 0);
   }

   if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
   {
      CallBackResizeScene(Window_Width, Window_Height);
   }
}



////////////////////////////////////////////////////////
//   Setup your program before passing the control    //
//   to the main OpenGL event loop.                   //
////////////////////////////////////////////////////////
void MyInit(int Width, int Height) 
{
   // Color to clear color buffer to.
   glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

   // Depth to clear depth buffer to; type of test.
   glClearDepth(1.0);
   glDepthFunc(GL_LESS);

   glEnable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);

   // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
   glShadeModel(GL_SMOOTH);

   // Load up the correct perspective matrix; using a callback directly.
   CallBackResizeScene(Width,Height);
}

///////////////////////////////////////////////////
// main() function.                              //
//   There are general steps in making           //
//   an OpenGL application.                      //
//   Inits OpenGL.                               //
//   Calls our own init function                 //
//   then passes control onto OpenGL.            //   
///////////////////////////////////////////////////
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   init();
   // To see OpenGL drawing, take out the GLUT_DOUBLE request.
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

   // The following is for window creation.
   // Set Window size
   glutInitWindowSize(Window_Width, Window_Height);
   // Create and Open a window with its title.
   Window_ID = glutCreateWindow(PROGRAM_TITLE);

   // Register and install the callback function to do the drawing. 
   glutDisplayFunc(&CallBackRenderScene);

   // If there's nothing to do, draw.
   glutIdleFunc(&CallBackRenderScene);

   // It's a good idea to know when our window's resized.
   glutReshapeFunc(&CallBackResizeScene);

   // Register and install the callback function for
   // Some keys and special keys.
   glutKeyboardFunc(&CallBackKeyPressed);
   glutMouseFunc(&MouseButtonPressed);
   
   // OK, OpenGL's ready to go.  Let's call our own init function.
   MyInit(Window_Width, Window_Height);


   // Print out a bit of help dialog.
   printf("\n%s\n\n", PROGRAM_TITLE);
   printf("Print out some helpful information here.\n");
   printf("When you program, you can also use\n");
   printf("printf to do debugging.\n\n");

   // Above functions represents those you will do to set up your
   // application program.
   // Now pass off control to OpenGL.
   glutMainLoop(); 
   // Never returns.
   return 1; 
}

