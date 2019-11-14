#include <iostream>
#include <glut.h>

#pragma comment(lib, "glut32.lib")

using namespace std;

void Display(void);
void Anim(void);
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



void moveInDir(float speed, float DirX, float DirY,float DirZ,float ObX,float ObY,float ObZ) {
	ballPosX = ObX + speed * (DirX - ObX);
	ballPosY = ObY + speed * (DirY - ObY);
	ballPosZ = ObZ + speed * (DirZ - ObZ);
}

int main(int argc, char** argr)
{
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

	
	glutMainLoop();
}

void Display(void)
{
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