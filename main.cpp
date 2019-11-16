#include <iostream>
#include <glut.h>
#define GLUT_KEY_ESCAPE 27
#pragma comment(lib, "glut32.lib")
#define RADPERDEG 0.0174533

using namespace std;

//------------------------------- identifiers -------------------------------
void Display(void);
void axis(double length);
void Anim(void);
void key(int key, int x, int y);
void spaceKey(unsigned char key, int x, int y);
void spaceKeyUp(unsigned char key, int x, int y);


//------------------------------- variabes -------------------------------
// ----------   Screen Size ---------
int winX = 800;
int winY = 600;

// ------ dimensions --------
int dimensions[] = { 7,7,7};
double sphereRadius = 0.3;

// ------ camera --------
double eyeX = dimensions[0] / 2;
double eyeY = (int)(dimensions[1] / 2) + 1;
double eyeZ = dimensions[2] * 5;
double centerX = eyeX;
double centerY = eyeY;
double centerZ = eyeZ / 2;

// ------ prospective --------
double fovy = 25;

// ----------   Game model ---------
int playerScore = 0;
GLboolean ballHit = false;
int hitVertical = 1;
double sphereX = dimensions[0] / 2;
double sphereY = dimensions[1] / 2;
double sphereZ = dimensions[2]*2;

//------------------- islam -------------------
GLfloat  rotateCube = 0.0f;
GLfloat  rotateBall = 0.0f;
int window_height = 600;
int window_width = 1000;
float rotAng;
float camera = 0;
float add = 0.01;
float PointerX = eyeX;
float PointerY = eyeY - 1;
float PointerZ = 0;
float Power = 0;
int shoot = 0;
float ballPosX = eyeX;
float ballPosY = eyeY - 1;
float ballPosZ = eyeZ - 5;
float moveByPower = 0;
int starts = 1;

float start[3];
float dir[3];
int camerMode = 0;
//--------------------------------------- init --------------------------------------------
void initGL() {
}
//--------------------------------------- Model --------------------------------------------
void updateScore(double x, double y, double z) {
	//------------- horizontal x,z
	//------------- vertical   y,z
	int modX;
	int modY;
	if (hitVertical == 1) {
		modX = (int)(y) % 2;
		modY = (int)(z) % 2;
	}
	else {
		modX = (int)(x) % 2;
		modY = (int)(z) % 2;
	}

	if (modX == hitVertical) {
		if (modY == 0) {
			//offWhite
			playerScore += 5;
		}
		else {
			//blue
			playerScore -= 5;
		}
	}
	else {
		if (modY == 0) {
			//red
			playerScore += 10;
		}
		else {
			//green
			playerScore -= 10;
		}

	}
}
void isSphereHit() {
	//------------- horizontal x,z
	//------------- vertical   y,z
	if (sphereY == 0 || sphereY == dimensions[1]) {
		hitVertical = 0;
		ballHit = true;
	}
	else {
		if (sphereX == 0 || sphereX == dimensions[0]) {
			hitVertical = 1;
			ballHit = true;
		}
	}
	if (ballHit) {
		updateScore(sphereX, sphereY, sphereZ);
		// reflect the ball so we need a direction vector
	}
}
void moveInDir(float speed, float Dir[], float Ob[]) {
	ballPosX = Ob[0] + speed * Dir[0];
	ballPosY = Ob[1] + speed * Dir[1];
	ballPosZ = Ob[2] + speed * Dir[2];
}
float* Reflect(float V[], float N[])
{
	float Dot = V[0] * N[0] + V[1] * N[1] + V[2] * N[2];
	float o[3];
	o[0] = V[0] - 2.f * Dot * N[0];
	o[1] = V[1] - 2.f * Dot * N[1];
	o[2] = V[2] - 2.f * Dot * N[2];
	return o;
}

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
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0); // red x
	//glTranslatef(-0.5f * length, 0, 0);
	Arrow(0, 0, 0, 2 * length, 0, 0, 0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0); // green y
	//glTranslatef(0, -0.5f * length, 0);
	Arrow(0, 0, 0, 0, 2 * length, 0, 0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0); // blue z
	//glTranslatef(0, 0, -0.5f * length);
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
	glutSolidSphere(radius, 20, 20);
	glPopMatrix();

}
void DrawWall(int rowSize, int colSize, int isVertical) {
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

void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60, window_width / window_height, 0.05f, 100);
	gluPerspective(fovy, winX * 1.0 / winY, 0.001f, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (camerMode == 1) {
		gluLookAt(eyeX, eyeY, ballPosZ + 15, eyeX, eyeY, 0, 0.0f, 1.0f, 0.0f);
		//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	}
	else {
		gluLookAt(eyeX, eyeY, ballPosZ + 15, ballPosX, ballPosY, 0, 0.0f, 1.0f, 0.0f);
		//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	}

	
	

	

	//------------- draw back wall of the room
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslated(0.5, 0, 0.5);
	DrawColoredWall(dimensions[0], dimensions[2], new double[3]{ 0,0,0 });
	glPopMatrix();

	//------------- draw floor of the room
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
	DrawWall(dimensions[1], dimensions[2], 1);
	glPopMatrix();
	//------------- draw right wall of the room
	glPushMatrix();
	glTranslated(dimensions[0], 0, 0);
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[1], dimensions[2], 1);
	glPopMatrix();

	//------------- draw the ciel of the room
	glPushMatrix();
	glTranslated(0.5, dimensions[1], 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[0], dimensions[2], 0);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.5f);
	if (shoot == 0) {
		//Arrow(eyeX, eyeY - 1, eyeZ - 5, PointerX, PointerY, eyeZ - 5 - eyeX / 2, 0.05);
		//Arrow(sphereX, sphereY - 1, sphereZ - 5, PointerX, PointerY, eyeZ - 5 - eyeX / 2, 0.05);
		Arrow(sphereX, sphereY, sphereZ, PointerX, PointerY, PointerZ, 0.05);
	}
	drawAxes(1);
	glPopMatrix();
	//------------- draw the ball
	
	glPushMatrix();
	if (shoot == 1) {
		moveByPower = Power;
		start[0] = ballPosX;
		start[1] = ballPosY;
		start[2] = ballPosZ;
		if (starts == 1) {
			dir[0] = PointerX - start[0];
			dir[1] = PointerY - start[1];
			dir[2] = -eyeX / 2;
			starts = 0;
		}

		if (ballPosX >= dimensions[0]) {
			float* dir1 = Reflect(dir, new float[3]{ 1,0,0 });
			dir[0] = dir1[0];
			dir[1] = dir1[1];
			dir[2] = dir1[2];
		}

		if (ballPosX <= 0) {
			float* dir1 = Reflect(dir, new float[3]{ -1,0,0 });
			dir[0] = dir1[0];
			dir[1] = dir1[1];
			dir[2] = dir1[2];
		}
		if (ballPosY >= dimensions[1]) {
			float* dir1 = Reflect(dir, new float[3]{ 0,-1,0 });
			dir[0] = dir1[0];
			dir[1] = dir1[1];
			dir[2] = dir1[2];
		}
		if (ballPosY <= 0) {
			float* dir1 = Reflect(dir, new float[3]{ 0,1,0 });
			dir[0] = dir1[0];
			dir[1] = dir1[1];
			dir[2] = dir1[2];
		}

		moveInDir(moveByPower, dir, start);
		glTranslatef(ballPosX, ballPosY, ballPosZ);

	}
	else {
		glTranslatef(eyeX, eyeY - 1, eyeZ - 5);
	}
	//glPopMatrix();
	
	//glPushMatrix();
	//glTranslated(sphereX, sphereY, sphereZ);
	DrawBall(sphereRadius, new double[3]{ 0.0f, 1.0f, 1.0f });
	glPopMatrix();


	glutSwapBuffers();
	glFlush();
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
		if (PointerY < dimensions[0])
			PointerY += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (PointerY > 0)
			PointerY -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		if (PointerX > 0)
			PointerX -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		if (PointerX < dimensions[1])
			PointerX += 0.1;
		break;
	}
	glutPostRedisplay();

}

//--------------------------------------- Main --------------------------------------------

int main(int argc, char** argv) {
	/*
	initGL();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutSpecialFunc(key);
	glutKeyboardUpFunc(spaceKeyUp);
	glutKeyboardFunc(spaceKey);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	*/

	initGL();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glClearColor(0.412f, 0.412f, 0.412f, 0.0f);  // background is gray  
	glViewport(0, 0, winX, winY);  // x y width height
	glutIdleFunc(Anim);
	glutSpecialFunc(key);
	glutKeyboardUpFunc(spaceKeyUp);
	glutKeyboardFunc(spaceKey);
	//glutSpecialFunc(KeyboardSpecial);
	glutMainLoop();
}



void Anim()
{
	rotAng += 0.01;

	camera += add;
	if ((int)camera == 0) add = 0.01;
	if ((int)camera == 10) add = -0.01;

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
