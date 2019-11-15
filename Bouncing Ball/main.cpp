#include <iostream>
#include <glut.h>
#define GLUT_KEY_ESCAPE 27
#pragma comment(lib, "glut32.lib")

using namespace std;

//------------------------------- identifiers -------------------------------
void Display(void);
void Anim(void);
void axis(double length);
void key(int key, int x, int y);
void spaceKey(unsigned char key, int x, int y);
void spaceKeyUp(unsigned char key, int x, int y);
void axis(double length);

//------------------------------- variabes -------------------------------

// ------ dimensions --------
int dimensions[] = { 16,16,16 };
double sphereRadius = 0.5;
// ------ camera --------
double eyeX = dimensions[0] / 2;
double eyeY = (dimensions[1] / 2);
double eyeZ = 50;
// ------ prospective --------
double fovy = 25;
// ----------   Screen Size ---------
int winX = 800;
int winY = 600;
GLfloat  rotateCube = 0.0f;
GLfloat  rotateBall = 0.0f;
int window_height = 600;
int window_width = 1000;
float rotAng;
float camera=0;
float add = 0.01;
float PointerX = eyeX;
float PointerY = eyeY-1;
float Power = 0;
int shoot = 0;
float ballPosX = 0;
float ballPosY = 5;
float ballPosZ = 0;
float moveByPower = 0;
#define RADPERDEG 0.0174533

//--------------------------------------- Model --------------------------------------------

//--------------------------------------- View --------------------------------------------

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

void DrawCube(double translate[], double rotate[], double scale[], double color[]) {
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
	//glScalef(scale[0], scale[1], scale[2]);
	glScalef(1, 0.01, 1);
	glColor3f(color[0], color[1], color[2]);
	glutSolidCube(1.0);
	glColor3f(0, 0, 0);
	//glutWireCube(1.0);
	glPopMatrix();

}

void DrawBall(double radius, double color[]) {
	glPushMatrix();
	glColor3f(color[0], color[1], color[2]);
	glutSolidSphere(radius, 50, 50);
	glPopMatrix();

}

void moveInDir(float speed, float DirX, float DirY,float DirZ,float ObX,float ObY,float ObZ) {
	ballPosX = ObX + speed * (DirX - ObX);
	ballPosY = ObY + speed * (DirY - ObY);
	ballPosZ = ObZ + speed * (DirZ - ObZ);
}

void DrawWall(int rowSize, int colSize, int isVertical) {
	double color1[3] = { 1.000, 0.000, 0.000 };
	double color2[3] = { 1.000, 0.271, 0.000 };
	double color3[3] = { 1.000, 0.843, 0.000 };
	double color4[3] = { 1.000, 0.843, 0.000 };


	for (double i = 0; i < colSize; i++) {
		int modX = (int)(i) % 2;
		for (double j = 0; j < rowSize; j++) {
			double color[3] = { 1.000, 0.000, 0.000 };
			double colorR = i == 0 && j == 0 ? 1 : 0;
			int modY = (int)(j) % 2;
			if (modX == isVertical) {
				if (modY == 0) {
					//offWhite
					color[0] = 0.961;
					color[1] = 0.961;
					color[2] = 0.961;
				}
				else {
					//blue
					color[0] = 0.000;
					color[1] = 0.000;
					color[2] = 0.804;

				}
			}
			else {
				if (modY == 0) {
					//red
					color[0] = 0.863;
					color[1] = 0.078;
					color[2] = 0.235;
				}
				else {
					//green
					color[0] = 0.196;
					color[1] = 0.804;
					color[2] = 0.196;

				}

			}
			DrawCube(new double[3]{ j,0,i }, new double[4]{ 0,0,0,0 }, new double[3]{ 1,1,1 }, color);
		}
	}
}

void DrawColoredWall(int rowSize, int colSize, double color[]) {
	for (double i = 0; i < colSize; i++) {
		for (double j = 0; j < rowSize; j++) {
			double colorR = i == 0 && j == 0 ? 1 : 0;
			DrawCube(new double[3]{ j,0,i }, new double[4]{ 0,0,0,0 }, new double[3]{ 1,1,1 }, color);
		}
	}
}

//--------------------------------------- Main --------------------------------------------

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutSpecialFunc(key);
	glutKeyboardUpFunc(spaceKeyUp);
	glutKeyboardFunc(spaceKey);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
	gluLookAt( eyeX, eyeY, eyeZ, eyeX, eyeY, 0, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	if (shoot == 1) {
		moveByPower += Power;
		moveInDir(moveByPower, PointerX, PointerY, eyeZ - 3 - eyeX / 2, eyeX, eyeY - 1, eyeZ - 3);
		glTranslatef(ballPosX, ballPosY, ballPosZ);
	}
	else {
		glTranslatef(eyeX, eyeY - 1, eyeZ-3);
	}
	glColor3f(0.0f, 1.0f, 1.0f);
	glutSolidSphere(0.3, 20, 20);
	glPopMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	if (shoot == 0)
		Arrow(eyeX , eyeY - 1, eyeZ-3, PointerX, PointerY, eyeZ - 3 - eyeX/2, 0.05);
	/*
	if (shoot == 1) {
		glTranslatef(-ballPosX, 5 - ballPosY, -ballPosZ);
	}

	drawAxes(5);


	glColor3f(1.0, 0.0, 1.0);
	glPushMatrix();
	glScalef(10.0f, 10.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, -20.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidCube(1);
	glPopMatrix();
	*/
	drawAxes(5);
	//------------- draw back wall of the room
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslated(0.5, 0, 0.5);
	DrawColoredWall(dimensions[0], dimensions[2], new double[3]{ 1,1,1 });
	glPopMatrix();

	//------------- draw floor of the room
	int depthDiff = 5;
	glPushMatrix();
	glTranslated(0.5, 0, 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[0], dimensions[2], 0);
	glPopMatrix();

	//------------- draw left wall of the room
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[1], dimensions[0], 1);
	glPopMatrix();
	//------------- draw right wall of the room
	glPushMatrix();
	glTranslated(dimensions[0], 0, 0);
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[1], dimensions[0], 1);
	glPopMatrix();

	//------------- draw the ciel of the room
	glPushMatrix();
	glTranslated(0.5, dimensions[1], 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[0], dimensions[2], 0);
	glPopMatrix();

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


//--------------------------------------- Control --------------------------------------------

void spaceKey(unsigned char key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		break;
	case ' ':
		Power += 0.002;
	}
	
	glutPostRedisplay();
}

void spaceKeyUp(unsigned char key, int x, int y) {
	if (key == ' ')
		shoot = 1;
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

//--------------------------------------- Others --------------------------------------------
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