#define NDEBUG
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

GLfloat x = 50.0;
GLfloat y = 50.0;
GLfloat size = 50.0;
GLsizei wh = 500, ww = 500;
void drawSquare(GLint x, GLint y) {
	y = wh - y;
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(x + size, y + size, 0);
	glVertex3f(x - size, y + size, 0);
	glVertex3f(x - size, y - size, 0);
	glVertex3f(x + size, y - size, 0);
	glEnd();
}
void mydisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawSquare(x, y);
	glutSwapBuffers();
	glutPostRedisplay();
}
void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	printf("Click Mouse Left Button to Move the Square");
}
void myreshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	ww = w;
	wh = h;
}
void mymouse(GLint button, GLint state, GLint wx, GLint wy)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		exit(0);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x = wx;
		y = wy;
	}
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Move the Square");
	init();
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(myreshape);
	glutMouseFunc(mymouse);
	glutMainLoop();
}