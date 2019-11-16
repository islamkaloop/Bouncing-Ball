#include <iostream>
#include <glut.h>
#define GLUT_KEY_ESCAPE 27
#pragma comment(lib, "glut32.lib")

using namespace std;

//------------------------------- identifiers -------------------------------
void Display(void);
void axis(double length);

//------------------------------- variabes -------------------------------
// ----------   Screen Size ---------
int winX = 800;
int winY = 600;

// ------ dimensions --------
int dimensions[] = { 7,7,7 };
double sphereRadius = 0.5;

// ------ camera --------
double eyeX = dimensions[0] / 2 ;
double eyeY = (int)(dimensions[1] / 2) + 1;
double eyeZ = dimensions[2] *5;
double centerX = eyeX ;
double centerY = eyeY;
double centerZ = eyeZ/2;
// ------ prospective --------
double fovy = 25;

// ----------   Game model ---------
int playerScore = 0;
GLboolean ballHit = false;
int hitVertical = 1;
double sphereX= dimensions[0] / 2;
double sphereY= dimensions[1] / 2;
double sphereZ= dimensions[2];

//--------------------------------------- init --------------------------------------------
void initGL() {	
}
//--------------------------------------- Model --------------------------------------------
void updateScore(double x,double y,double z) {
	//------------- horizontal x,z
	//------------- vertical   y,z
	int modX;
	int modY;
	if (hitVertical==1) {
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
	if (sphereY==0|| sphereY == dimensions[1]) {
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
//--------------------------------------- View --------------------------------------------
void DrawCube(double translate[], double rotate[], double scale[], double color[]) {
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);
	//glScalef(scale[0], scale[1], scale[2]);
	glScalef(1,0.01,1);
	glColor3f(color[0], color[1], color[2]);
	glutSolidCube(1.0);
	glColor3f(0,0,0);
	//glutWireCube(1.0);
	glPopMatrix();

}
void DrawBall(double radius, double color[]) {
	glPushMatrix();
	glColor3f(color[0], color[1], color[2]);
	glutSolidSphere(radius, 50, 50);
	glPopMatrix();

}
void DrawWall(int rowSize,int colSize, int isVertical) {
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
void DrawColoredWall(int rowSize, int colSize,double color[]) {
	for (double i = 0; i < colSize; i++) {
		for (double j = 0; j < rowSize; j++) {
			double colorR = i == 0 && j == 0 ? 1 : 0;
			DrawCube(new double[3]{ j,0,i }, new double[4]{ 0,0,0,0 }, new double[3]{ 1,1,1 }, color);
		}
	}
}

void Display(void) {
	glMatrixMode(GL_PROJECTION); // set the view volume shape
	//gluPerspective(45,winX*1.0/winY,0.1,100);
	gluPerspective(fovy, winX * 1.0 / winY, 0.001, 100);
	glMatrixMode(GL_MODELVIEW); // position and aim the camera
	glLoadIdentity();
	//glTranslatef(0,0,eyeZ);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen  
	
								  
	//------------- draw back wall of the room
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslated(0.5, 0, 0.5);
	DrawColoredWall(dimensions[0], dimensions[2], new double[3]{0,0,0});
	glPopMatrix();

	//------------- draw floor of the room
	glPushMatrix();
	glTranslated(0.5, 0, 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[0], dimensions[2],0);
	glPopMatrix();

	//------------- draw left wall of the room
	glPushMatrix();
	glRotatef(90,0,0,1);
	glTranslated(0.5,0,0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[1], dimensions[2],1);
	glPopMatrix();
	//------------- draw right wall of the room
	glPushMatrix();
	glTranslated(dimensions[0], 0, 0);
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[1], dimensions[2],1);
	glPopMatrix();

	//------------- draw the ciel of the room
	glPushMatrix();
	glTranslated(0.5, dimensions[1], 0.5);
	glScalef(1, 1, 2);
	DrawWall(dimensions[0], dimensions[2],0);
	glPopMatrix();


	//------------- draw the ball
	glPushMatrix();
	glTranslated(sphereX,sphereY,sphereZ);
	DrawBall(sphereRadius, new double[3]{ 1,1,1 });
	glPopMatrix();


	glutSwapBuffers();
	glFlush();
}


//--------------------------------------- Control --------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	/*if (key == GLUT_KEY_ESCAPE)
		exit(EXIT_SUCCESS);*/
	switch (key)
	{
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		break;
	}
}
void KeyboardSpecial(int key, int x, int y) {
	/*if (key == GLUT_KEY_ESCAPE)
		exit(EXIT_SUCCESS);*/
	switch (key)
	{
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		break;
	case GLUT_KEY_UP:
		fovy += 0.5;
		//centerX = 0;
		printf("value:%f \n",fovy);
		break;
	case GLUT_KEY_DOWN:
		fovy -= 0.5;
		printf("value:%f \n", fovy);
		break;
	case GLUT_KEY_LEFT:
		eyeX += 2;
		printf("x:%f \n", eyeX);
		break;
	case GLUT_KEY_RIGHT:
		eyeX -= 2;
		printf("x:%f \n", eyeX);
		break;
	case '8':
		eyeY += 2;
		printf("y:%f \n", eyeY);
		break;
	case '2':
		eyeY -= 2;
		printf("y:%f \n", eyeY);
		break;
	}
	glutPostRedisplay();
}

//--------------------------------------- Main --------------------------------------------
void main(int argc, char** argv) {
	initGL();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winX, winY);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glClearColor(0.412f, 0.412f, 0.412f, 0.0f);  // background is gray  
	glViewport(0, 0, winX, winY);  // x y width height
	glutKeyboardFunc(Keyboard);
	//glutSpecialFunc(KeyboardSpecial);
	glutMainLoop();
}

//-------------------------------------------------------------------------------------------
//--------------------------------------- Others --------------------------------------------
void axis(double length) { // draw a z-axis, with cone at end  
	//glPushMatrix();

	//glBegin(GL_LINES);
	//glVertex3d(0, 0, 0);
	//glVertex3d(0, 0, length); // along the z-axis 
	//glEnd();

	glPushMatrix();
	glTranslated(0, 0, 1.0); // cylinder at (0,0,1)  
	glScalef(2,2,4);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.2, 0.2, 0.4, 8, 8);
	glPopMatrix();

	glTranslated(0, 0, length - 0.2);
	glutWireCone(0.04, 0.2, 12, 9);

	glPopMatrix();
}