#include <iostream>
#include <glut.h>
#define GLUT_KEY_ESCAPE 27
#pragma comment(lib, "glut32.lib")

using namespace std;

void Display(void);
void Anim(void);

void reshape(GLsizei width, GLsizei height);
void axis(double length);


int window_height = 600;
int window_width = 1000;
GLfloat  rotateCube = 0.0f;
GLfloat  rotateBall = 0.0f;

//--------------------------------------- init --------------------------------------------
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth(1.0f);                  
	glEnable(GL_DEPTH_TEST);   
	glDepthFunc(GL_LEQUAL);    
	glShadeModel(GL_SMOOTH);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
}
void key(int key, int x, int y);
void spaceKey(unsigned char key, int x, int y);
void spaceKeyUp(unsigned char key, int x, int y);

int window_height = 600;
int window_width = 1000;
float rotAng;
float camera=0;
float add = 0.01;
float PointerX = 0;
float PointerY = 5;
float Power = 0;
int shoot = 0;
float ballPosX = 0;
float ballPosY = 5;
float ballPosZ = 0;
float moveByPower = 0;
#define RADPERDEG 0.0174533
void Arrow(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble D)
{
	double x = x2 - x1;
	double y = y2 - y1;
	double z = z2 - z1;
	double L = sqrt(x * x + y * y + z * z);

	GLUquadricObj* quadObj;

	glPushMatrix();

	glTranslated(x1, y1, z1);

	if ((x != 0.) || (y != 0.)) {
		glRotated(atan2(y, x) / RADPERDEG, 0., 0., 1.);
		glRotated(atan2(sqrt(x * x + y * y), z) / RADPERDEG, 0., 1., 0.);
	}
	else if (z < 0) {
		glRotated(180, 1., 0., 0.);
	}

	glTranslatef(0, 0, L - 4 * D);

	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluCylinder(quadObj, 2 * D, 0.0, 4 * D, 32, 1);
	gluDeleteQuadric(quadObj);

	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluDisk(quadObj, 0.0, 2 * D, 32, 1);
	gluDeleteQuadric(quadObj);

	glTranslatef(0, 0, -L + 4 * D);

	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluCylinder(quadObj, D, D, L - 4 * D, 32, 1);
	gluDeleteQuadric(quadObj);

	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricNormals(quadObj, GLU_SMOOTH);
	gluDisk(quadObj, 0.0, D, 32, 1);
	gluDeleteQuadric(quadObj);

	glPopMatrix();

}
void drawAxes(GLdouble length)
{
	glColor3f(1.0, 0.0, 0.0); // red x
	glPushMatrix();
	glTranslatef(-0.5f*length, 0, 0);
	Arrow(0, 0, 0, 2 * length, 0, 0, 0.1);
	glPopMatrix();

	glColor3f(0.0, 1.0, 0.0); // green y
	glPushMatrix();
	glTranslatef(0, -0.5f*length, 0);
	Arrow(0, 0, 0, 0, 2 * length, 0, 0.1);
	glPopMatrix();

	glColor3f(0.0, 0.0, 1.0); // blue z
	glPushMatrix();
	glTranslatef(0, 0, -0.5f*length);
	Arrow(0, 0, 0, 0, 0, 2 * length, 0.1);
	glPopMatrix();
}
//--------------------------------------- Model --------------------------------------------

//--------------------------------------- View --------------------------------------------

//translate[3], rotate[4], scale[3], color[3]
void DrawCube(double translate[], double rotate[], double scale[], double color[]) {
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
	glScalef(scale[0], scale[1], scale[2]);
	glColor3f(color[0], color[1], color[2]);
	glutSolidCube(1.0);
	glColor3f(0,0,0);
	glutWireCube(1.0);
	glColor3f(0,0,0);
	glPopMatrix();
}
//translate[3], radius, color[3]
void DrawBall(double translate[],double radius, double color[]) {
	
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	glColor3f(color[0], color[1], color[2]);
	glutSolidSphere(0.25, 10, 10);
	glPopMatrix();

}
//dimensions[3], translate[4], color[3]
void DrawInteractiveWall(int dimensions[], double translate[],double color[]) {
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	double scale[3]={ 1, 0.1, 1 };
	for (double x = 0; x < dimensions[0]; x++) {
		for (double y = 0; y < dimensions[1]; y++) {
			for (double z = 0; z < dimensions[2]; z++) {
				glPushMatrix();
				//DrawCube(translate, rotate, scale, color);
				DrawCube(new double[3]{ x*scale[0], y*scale[1], z*scale[2] }, new double[4]{ 0, 0, 1, 0 },scale, new double[3]{1,0,0});				
				glPopMatrix();
			}
		}
	}
	glPopMatrix();
}

void moveInDir(float speed, float DirX, float DirY,float DirZ,float ObX,float ObY,float ObZ) {
	ballPosX = ObX + speed * (DirX - ObX);
	ballPosY = ObY + speed * (DirY - ObY);
	ballPosZ = ObZ + speed * (DirZ - ObZ);
}


void Display(void)
{
	glMatrixMode(GL_PROJECTION); // set the view volume shape
	glLoadIdentity();
	glOrtho(-2.0 * 1024 / 768.0, 2.0 * 1024 / 768.0, -2.0, 2.0, 0.1, 100);
	glMatrixMode(GL_MODELVIEW); // position and aim the camera
	glLoadIdentity();
	gluLookAt(20,10,20, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen  


	//------------- draw floor of the room
	int dimensions[] = { 4,1,4};
	double translate[] = {0,0,0};
	double rotate[] = { 0,0,1,0 };
	double scale[] = { 2,0.5,1 };
	double color[]={1.0,0,0};

	DrawInteractiveWall(dimensions, translate, color);
	
  
  
	glutSwapBuffers();
	glFlush();
}
  //--------------------------------------- Main --------------------------------------------
void main(int argc, char** argv) {
  glutInit(&argc, argr);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutSpecialFunc(key);
	glutKeyboardUpFunc(spaceKeyUp);
	glutKeyboardFunc(spaceKey);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, window_width / window_height, 0.05f, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 10, 15, PointerX, PointerY, -10, 0.0f, 1.0f, 0.0f);
	if (shoot == 1) {
		glTranslatef(-ballPosX,5 - ballPosY, -ballPosZ);
	}

	drawAxes(5);
	
	glPushMatrix();
	
	if (shoot == 1) {
		moveByPower += Power;
		moveInDir(moveByPower, PointerX, PointerY, -10,0,5,0);
		glTranslatef(ballPosX, ballPosY, ballPosZ);
	}
	else {
		glTranslatef(0, 5, 0);
	}
	glColor3f(0.0f, 1.0f, 1.0f);
	glutSolidSphere(1,20,20);
	glPopMatrix();
	if(shoot==0)
		Arrow(0.0f, 5, 0, PointerX, PointerY, -10, 0.1);

	glColor3f(1.0, 0.0, 1.0);
	glPushMatrix();
	glScalef(10.0f, 10.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -20.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidCube(1);
	glPopMatrix();
  
	glutMainLoop();

}

void DisplayOld(void)
{
	glMatrixMode(GL_PROJECTION); // set the view volume shape
	glLoadIdentity();
	glOrtho(-2.0 * 64 / 48.0, 2.0 * 64 / 48.0, -2.0, 2.0, 0.1, 100);
	glMatrixMode(GL_MODELVIEW); // position and aim the camera
	glLoadIdentity();
	gluLookAt(2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT); // clear the screen  

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);

	int dimensions[] = { 10,10,1 };
	double translate[] = { 0,0,10 };
	double color[] = { 1.0,0,0 };

	//DrawInteractiveWall(dimensions, translate, color);



	glutSwapBuffers();
	glFlush();
}

void Anim()
{
	rotAng += 0.01;

	camera += add;
	if ((int)camera == 0) add = 0.01;
	if ((int)camera == 10) add = -0.01;

	glutPostRedisplay();
}	

void key(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		PointerY += 0.1;
		break;
	case GLUT_KEY_DOWN:
		PointerY -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		PointerX -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		PointerX += 0.1;
		break;
	}
	glutPostRedisplay();

}

void spaceKey(unsigned char key, int x, int y) {
	if (key == ' ')
		Power += 0.002;
	
	glutPostRedisplay();
}

void spaceKeyUp(unsigned char key, int x, int y) {
	if (key == ' ')
		shoot = 1;
	glutPostRedisplay();
}

//--------------------------------------- Control --------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	if (key == GLUT_KEY_ESCAPE)
		exit(EXIT_SUCCESS);
}

//-------------------------------------------------------------------------------------------
//--------------------------------------- Others --------------------------------------------
void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void axis(double length) { // draw a z-axis, with cone at end  
	glPushMatrix();

	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, length); // along the z-axis 
	glEnd();

	glTranslated(0, 0, length - 0.2);
	glutWireCone(0.04, 0.2, 12, 9);

	glPopMatrix();
}
