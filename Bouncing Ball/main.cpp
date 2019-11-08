#include <iostream>
#include <glut.h>

#pragma comment(lib, "glut32.lib")

using namespace std;

void Display(void);

int window_height = 600;
int window_width = 1000;

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth(1.0f);                  
	glEnable(GL_DEPTH_TEST);   
	glDepthFunc(GL_LEQUAL);    
	glShadeModel(GL_SMOOTH);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  
}

void reshape(GLsizei width, GLsizei height) {  
	if (height == 0) height = 1;                
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();             
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argr)
{
	glutInit(&argc, argr);          
	glutInitDisplayMode(GLUT_DOUBLE); 
	glutInitWindowSize( window_width, window_height);
	glutInitWindowPosition(50, 50); 
	glutCreateWindow("Bouncing Ball");         
	glutDisplayFunc(Display);       
	glutReshapeFunc(reshape);       
	initGL();                      
	glutMainLoop();                 
	return 0;
}

void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);    

	glLoadIdentity();                
	glTranslatef(1.5f, 0.0f, -7.0f);
	glRotatef(45.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);                
	glColor3f(0.0f, 1.0f, 0.0f);     
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(1.0f, 0.5f, 0.0f);    
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor3f(1.0f, 0.0f, 0.0f);   
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 0.0f);     
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);    
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 0.0f, 1.0f);     
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  

	glColor3f(1.0, 0.0, 0.0);
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -7.0f);
	glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glutWireSphere(1.0, 20.0, 20.0);

	glutSwapBuffers();
}
