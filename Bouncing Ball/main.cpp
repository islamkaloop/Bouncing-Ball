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
	if (key == GLUT_KEY_ESCAPE)
		exit(EXIT_SUCCESS);
}
//--------------------------------------- Main --------------------------------------------
void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);  // background is white  
	glViewport(0, 0, 1024, 768);  // x y width height
	glutKeyboardFunc(Keyboard);
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
