#include <iostream>
#include <glut.h>
#define GLUT_KEY_ESCAPE 27
#pragma comment(lib, "glut32.lib")

using namespace std;

//------------------------------- identifiers -------------------------------
void Display(void);
void Anim(void);
void reshape(GLsizei width, GLsizei height);
void axis(double length);

//------------------------------- variabes -------------------------------
int dimensions[] = { 7,7,7 };
double eyeX		= dimensions[0] / 2; ;
double eyeY = 10; ;
double eyeZ = dimensions[2] * 3; ;
double centerX = eyeX; ;
double centerY = 5; ;
double centerZ = 2; ;


			// ----------   Screen Size ---------
int win_x = 1024;
int win_y = 768;


int window_height = 600;
int window_width = 1000;
GLfloat  rotateCube = 0.0f;
GLfloat  rotateBall = 0.0f;

//--------------------------------------- init --------------------------------------------
void initGL() {
	/*glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth(1.0f);                  
	glEnable(GL_DEPTH_TEST);   
	glDepthFunc(GL_LEQUAL);    
	glShadeModel(GL_SMOOTH);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);*/

	
}
//--------------------------------------- Model --------------------------------------------

//--------------------------------------- View --------------------------------------------

//translate[3], rotate[4], scale[3], color[3]
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

void DrawWall(int rowSize,int colSize) {
	double color1[3] = { 1.000, 0.000, 0.000 };
	double color2[3] = { 1.000, 0.271, 0.000 };
	double color3[3] = { 1.000, 0.843, 0.000 };
	double color4[3] = { 1.000, 0.843, 0.000 };
	int patternIndex = 0;
	
	for (double i = 0; i < colSize; i++) {
		int modX = (int)(i) % 2;
		for (double j = 0; j < rowSize; j++) {
			double color[3] = { 1.000, 0.000, 0.000 };
			double colorR = i == 0 && j == 0 ? 1 : 0;
			int modY = (int)(j) % 2;
			if (modX == patternIndex) {
				if (modY == patternIndex) {
					color[0] = 0.863;
					color[1] = 0.078;
					color[2] = 0.235;
				}
				else {
					color[0] = 0.196;
					color[1] = 0.804;
					color[2] = 0.196;

				}
			}
			else {
				if (modY == 0) {
					color[0] = 0.000;
					color[1] = 0.000;
					color[2] = 0.804;
				}
				else {
					
					color[0] = 1.000;
					color[1] = 1.000;
					color[2] = 0.878;
				}
				
			}
			DrawCube(new double[3]{ j,0,i }, new double[4]{ 0,0,0,0 }, new double[3]{ 1,1,1 }, color);
		}
	}
}
void DrawWallColored(int rowSize, int colSize,double color[]) {
	for (double i = 0; i < colSize; i++) {
		for (double j = 0; j < rowSize; j++) {
			double colorR = i == 0 && j == 0 ? 1 : 0;
			DrawCube(new double[3]{ j,0,i }, new double[4]{ 0,0,0,0 }, new double[3]{ 1,1,1 }, color);
		}
	}
}
void DrawVerticalWall(int rowSize, int colSize) {
	for (double i = 0; i < colSize; i++) {
		for (double j = 0; j < rowSize; j++) {
			DrawCube(new double[3]{ -1,i,j }, new double[4]{ 90,0,0,1 }, new double[3]{ 1,1,1 }, new double[3]{ j * 5 / 100.0,1,0 });
		}
	}
}

void Display(void) {
	glMatrixMode(GL_PROJECTION); // set the view volume shape
	gluPerspective(45,win_x*1.0/win_y,0.1,100);
	glMatrixMode(GL_MODELVIEW); // position and aim the camera
	glLoadIdentity();
	//glTranslatef(0,0,eyeZ);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen  
	//------------- draw floor of the room
	glPushMatrix();
	glTranslated(0.5, 0, 0.5);
	DrawWall(dimensions[0], dimensions[2]);
	glPopMatrix();

	//------------- draw left wall of the room
	glPushMatrix();
	glRotatef(90,0,0,1);
	glTranslated(0.5,0,0.5);
	DrawWall(dimensions[1], dimensions[0]);
	glPopMatrix();
	//------------- draw right wall of the room
	glPushMatrix();
	glTranslated(dimensions[0], 0, 0);
	glRotatef(90, 0, 0, 1);
	glTranslated(0.5, 0, 0.5);
	DrawWall(dimensions[1], dimensions[0]);
	glPopMatrix();

	//------------- draw the ciel of the room
	glPushMatrix();
	glTranslated(0.5, dimensions[1], 0.5);
	//DrawWallColored(dimensions[0], dimensions[2], new double[3]{ 0.5,0.5,0.5 });
	DrawWall(dimensions[0], dimensions[2]);
	glPopMatrix();





	glutSwapBuffers();
	glFlush();

}
void DisplayOld2(void)
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
	if (rotateCube < 361.0f) rotateCube= rotateCube+0.1;
	else rotateCube = 0.0f;
	if (rotateBall < 361.0f) rotateBall = rotateBall +0.1;
	else rotateBall = 0.0f;
	glutPostRedisplay();
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
		eyeZ += 0.5;
		//centerX = 0;
		printf("z:%f \n",eyeZ);
		break;
	case GLUT_KEY_DOWN:
		eyeZ -= 0.5;
		printf("z:%f \n", eyeZ);
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
	glutInitWindowSize(win_x, win_y);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);  // background is white  
	glViewport(0, 0, 1024, 768);  // x y width height
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(KeyboardSpecial);
	glutMainLoop();
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