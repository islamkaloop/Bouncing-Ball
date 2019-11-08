#include <iostream>
#include <glut.h>

#pragma comment(lib, "glut32.lib")

using namespace std;

void Display(void);

int window_height = 600;
int window_width = 1000;

int main(int argc, char** argr)
{
	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, window_height);
	glutInitWindowPosition(250, 50);
	glutCreateWindow("Bouncing Ball");
	glutDisplayFunc(Display);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, window_width, 0.0, window_height);

	glutMainLoop();
	return 0;
}

void Display(void)
{
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, 50);
		glVertex2f(50,50);
		glVertex2f(50, 0);
	glEnd();
	glFlush();
}
