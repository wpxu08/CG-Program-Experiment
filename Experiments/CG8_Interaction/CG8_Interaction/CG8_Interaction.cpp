#include <GL/glut.h>

#include <math.h>

#include <stdlib.h>

#define DEGREES_TO_RADIANS 3.14159/180.0

static GLfloat spin = 0.0;

GLfloat x = 0.0;

GLfloat y = 0.0;

GLfloat size = 50.0;

GLfloat angle = 2.0;

GLsizei wh = 500, ww = 500;

void square()

{

	glBegin(GL_QUADS);

	glVertex2f(x,y);

	glVertex2f(-y,x);

	glVertex2f(-x,-y);

	glVertex2f(y,-x);

	glEnd();

}

void spinDisplay(void)

{

	spin = spin + 2.0;

	if (spin > 360.0) spin = spin - 360.0;

	x=125.0 * cos(DEGREES_TO_RADIANS*spin);

	y=125.0 * sin(DEGREES_TO_RADIANS*spin);

	glutPostRedisplay();

}

void mydisplay()

{

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	square();

	glutSwapBuffers();

}

void init()

{

	glClearColor (0.0, 0.0, 0.0, 1.0);

}

void myreshape(GLint w, GLint h) {

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(-w/2, w/2, -h/2, h/2, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);

	ww = w;

	wh = h;

}

void mymouse(GLint button, GLint state, GLint wx, GLint wy)

{

	if(button ==GLUT_RIGHT_BUTTON && state == GLUT_DOWN)

		exit(0);

	if(button ==GLUT_LEFT_BUTTON && state == GLUT_DOWN)

		glutIdleFunc(spinDisplay);

	if(button== GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)

		glutIdleFunc(NULL);

}

void main(int argc, char** argv)

{

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(500,500);

	glutInitWindowPosition(0,0);

	glutCreateWindow("double");

	init();

	glutDisplayFunc(mydisplay);

	glutReshapeFunc(myreshape);

	glutMouseFunc(mymouse);

	glutIdleFunc(spinDisplay);

	glutMainLoop();

}