#include <iostream>
#include <sstream>
#include <glut.h>
#include <math.h>
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
int dimensions[] = { 10,10,20 };
double ballRadius = 0.3;
int scaleX = 1;
int scaleY = 1;
int scaleZ = 2;

// ------ prospective --------
double fovy = 45;

// ------ camera --------
double eyeX = dimensions[0] / 2;
double eyeY = (int)(dimensions[1] / 2) + 1;
double eyeZ = dimensions[2];
double centerX = eyeX;
double centerY = eyeY;
double centerZ = 0;
int cameraMode = 1;					//1= simple_mode

// ----------   Game model ---------
GLboolean ballHit = false;
double ballPosX = dimensions[0] / 2;
double ballPosY = dimensions[1] / 2;
double ballPosZ = dimensions[2] * scaleZ - 10;

//------------------- control -------------------
float add = 0.01;
float power = 0;
int shoot = 0;
float moveByPower = 0;
int firstShoot = 1;
float start[3];
float dir[3];
float pointerX = ballPosX;
float pointerY = ballPosY;
float pointerZ = ballPosZ - ballRadius - 5;
float prevPointer[3];
int replayEnable = 0;
const int maxRound = 3;
int currentRound = 1;
int roundScore[maxRound];
int allScore = 0;
stringstream printString;
int scoreForReplay = 0;


//--------------------------------------- init --------------------------------------------
void initGL() {
}
//--------------------------------------- Model --------------------------------------------
int updateScore(double x, double y, double z, int isVertical) {
	//------------- horizontal x,z
	//------------- vertical   y,z
	x += 0.5;
	z += 0.5;
	int modX;
	int modY;
	if (isVertical == 1) {
		modX = (int)(ceil(y)) % 2;
		modY = (int)(ceil(z)) % 2;
	}
	else {
		modX = (int)(ceil(x)) % 2;
		modY = (int)(ceil(z)) % 2;
	}
	printf("x: %f , y: %f , z:%f\n", ceil(x), ceil(y), ceil(z));
	if (modX == isVertical) {
		if (modY == 0) {
			//offWhite
			printf("white\n");
			return 5;
		}
		else {
			//blue
			printf("blue\n");
			return -5;
		}
	}
	else {
		if (modY == 0) {
			//red
			printf("red\n");
			return 10;
		}
		else {
			//green
			printf("green\n");
			return -10;
		}

	}
}
void moveInDir(float speed, float Dir[], float initialPos[]) {
	ballPosX = initialPos[0] + speed * Dir[0];
	ballPosY = initialPos[1] + speed * Dir[1];
	ballPosZ = initialPos[2] + speed * Dir[2];
}
float* Reflect(float vector[], float normal[])
{
	float Dot = vector[0] * normal[0] + vector[1] * normal[1] + vector[2] * normal[2];
	float o[3];
	o[0] = vector[0] - 2.f * Dot * normal[0];
	o[1] = vector[1] - 2.f * Dot * normal[1];
	o[2] = vector[2] - 2.f * Dot * normal[2];
	if(shoot == 1)
		roundScore[currentRound-1] += updateScore(ballPosX, ballPosY, ballPosZ, 0);
	else 
		scoreForReplay += updateScore(ballPosX, ballPosY, ballPosZ, 0);
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
			if (isVertical) {

			}
			else {

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
//-----------------------------print on screen--------------------
void print(int x, int y, string s)
{
	glRasterPos2i(x, y);
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glColor3d(1.0, 0.0, 0.0);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}

void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, winX * 1.0 / winY, 0.001f, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//----------------------------- camera --------------------------------
	if (cameraMode == 1) {		// simple
		gluLookAt(eyeX, eyeY, ballPosZ + 10, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);
		//gluLookAt(0, ballPosY, ballPosZ, ballPosX, ballPosY, ballPosZ, 0.0f, 1.0f, 0.0f);
		//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	}
	else {
		gluLookAt(eyeX, eyeY, ballPosZ + 10, ballPosX, ballPosY, ballPosZ, 0.0f, 1.0f, 0.0f);
		//gluLookAt(eyeX, eyeY, ballPosZ + 5, ballPosX, ballPosY, 0, 0.0f, 1.0f, 0.0f);
		//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	}

	//------------- draw back wall of the room

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslated(0.5, 0, 0.5);
	DrawColoredWall(dimensions[0], dimensions[1], new double[3]{ 0,0,0 });
	glPopMatrix();

	//------------- draw floor of the room

	glPushMatrix();
	glTranslated(0.5, 0, 0.5);
	glScalef(scaleX, scaleY, scaleZ);
	DrawWall(dimensions[0], dimensions[2], 0);
	glPopMatrix();



	//------------- draw left wall of the room
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	glScalef(scaleX, scaleY, scaleZ);
	DrawWall(dimensions[1], dimensions[2], 1);
	glPopMatrix();
	//------------- draw right wall of the room
	glPushMatrix();
	glTranslated(dimensions[0], 0, 0);
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	glScalef(scaleX, scaleY, scaleZ);
	DrawWall(dimensions[1], dimensions[2], 1);
	glPopMatrix();

	//------------- draw the ciel of the room
	glPushMatrix();
	glTranslated(0.5, dimensions[1], 0.5);
	glScalef(scaleX, scaleY, scaleZ);
	DrawWall(dimensions[0], dimensions[2], 0);
	glPopMatrix();

	//------------- draw the ball

	glPushMatrix();
	if (shoot == 1 || replayEnable == 1 ) {
		if (firstShoot == 1 && shoot==1) {
			start[0] = ballPosX;
			start[1] = ballPosY;
			start[2] = ballPosZ;
			dir[0] = pointerX - start[0];
			dir[1] = pointerY - start[1];
			dir[2] = -eyeX / 2;
			prevPointer[0] = dir[0];
			prevPointer[1] = dir[1];
			prevPointer[2] = dir[2];
			firstShoot = 0;
			printf("%f %f %f %f %f %f %f %f %f\n", pointerX, pointerY, pointerZ,start[0],start[1],start[2],dir[0], dir[1], dir[2]);
		}else if (firstShoot == 1) {
			dir[0] = prevPointer[0];
			dir[1] = prevPointer[1];
			dir[2] = prevPointer[2];
			firstShoot = 0;
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
		moveInDir(power, dir, new float[3] {(float)ballPosX, (float)ballPosY, (float)ballPosZ});
	}
	//--------- draw arrow target ----------------
	else {
		glColor3f(1.000, 0.871, 0.678);
		Arrow(ballPosX, ballPosY, ballPosZ, pointerX, pointerY, pointerZ, 0.05);
	}

	glTranslatef(ballPosX, ballPosY, ballPosZ);
	DrawBall(ballRadius, new double[3]{ 0.0f, 1.0f, 1.0f });
	if (ballPosZ <= 0) {
		ballPosX = start[0];
		ballPosY = start[1];
		ballPosZ = start[2];
		pointerX = ballPosX;
		pointerY = ballPosY;
		pointerZ = ballPosZ - ballRadius - 5;
		firstShoot = 1;
		if (shoot == 1) {
			allScore += roundScore[currentRound - 1];
			++currentRound;
		}
		shoot = 0;
		replayEnable = 0;
	}
	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, winX, 0.0, winY);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0f, 0.0f, 0.0f);
	if(cameraMode == 1)
		print(winX*0.8, winY * .95, "Simple Mode");
	else
		print(winX * .8, winY * .95, "Rotating Mode");
	printString.str("");
	printString << "Total Score: " << allScore;
	print(winX * 0.01, winY * .95, printString.str());
	float c = 0.91;
	for (int i = 1; i < currentRound; ++i) {
		printString.str("");
		printString <<  "Round "<<i<<" Score: " << roundScore[i-1];
		print(winX * 0.01, winY* c, printString.str());
		c -= 0.04;
	}
	printString.str("");
	printString << currentRound;
	print(winX * 0.49, winY* 0.95, printString.str());
	if(shoot == 1) {
		printString.str("");
		printString << "Score: " << roundScore[currentRound - 1];
		print(winX * 0.8, winY * 0.91, printString.str());
	}
	if (replayEnable == 1) {
		printString.str("");
		printString << "Score: " << scoreForReplay;
		print(winX * 0.8, winY * 0.91, printString.str());
	}
	if (currentRound > maxRound) {
		glColor3f(0.13333, .545, 0.1333);
		glBegin(GL_QUADS);
			glVertex2f(winX * .35, winY * .4);
			glVertex2f(winX * .35, winY * .6);
			glVertex2f(winX * .65, winY * .6);
			glVertex2f(winX * .65, winY * .4);
		glEnd();
		glColor3f(1, 0, 0);
		printString.str("");
		print(winX * .44, winY * .52, "THE END");
		printString << "Your Score is " << allScore;
		print(winX * .39, winY * .48, printString.str());
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);

	glutSwapBuffers();
	glutPostRedisplay();
	
	glFlush();
}

//--------------------------------------- Control --------------------------------------------
void spaceKey(unsigned char key, int x, int y) {
	if (shoot == 0 && currentRound<=maxRound) {
		switch (key) {
		case ' ':
			shoot = 1;
			power = 0.02;
			prevPointer[0] = pointerX;
			prevPointer[1] = pointerY;
			prevPointer[2] = pointerZ;
			glutPostRedisplay();
			break;
		case 'r':
			if (currentRound > 1) {
				power = 0.002;
				replayEnable = 1;
			}
			break;
		case 'c':
			cameraMode = (cameraMode + 1) % 2;
			break;
		case GLUT_KEY_ESCAPE:
			exit(EXIT_SUCCESS);
			break;
		}
		glutPostRedisplay();
	}
}

void key(int key, int x, int y)
{
	if (shoot == 0 && currentRound <= maxRound) {
		switch (key)
		{
		case GLUT_KEY_UP:
			if (pointerY < dimensions[0])
				pointerY += 0.1;
			break;
		case GLUT_KEY_DOWN:
			if (pointerY > 0)
				pointerY -= 0.1;
			break;
		case GLUT_KEY_LEFT:
			if (pointerX > 0)
				pointerX -= 0.1;
			break;
		case GLUT_KEY_RIGHT:
			if (pointerX < dimensions[1])
				pointerX += 0.1;
			break;
		}
		glutPostRedisplay();
	}

}

//--------------------------------------- Main --------------------------------------------

int main(int argc, char** argv) {
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
	glutKeyboardFunc(spaceKey);
	glutMainLoop();
}



void Anim()
{
	glutPostRedisplay();
}