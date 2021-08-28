#include <GL/glut.h>

typedef GLfloat point2d[2]; // a point data type

void triangle( point2d a, point2d b, point2d c) // display a triangle

{

	glBegin(GL_TRIANGLES);

	glVertex2fv(a);

	glVertex2fv(b);

	glVertex2fv(c);

	glEnd();

}

void display(void)

{

	point2d v[3] = {{-1.0, -0.58}, {1.0, -0.58}, {0.0, 1.15}}; //initial triangle vertices

	glClear(GL_COLOR_BUFFER_BIT); // Clear display window

	glColor3f(0.0,0.0,1.0); // Set fill color to blue

	glLoadIdentity();//应该放在此处，保证每次显示正确

	glViewport(0, 0, 300, 400); // Set left viewport

	triangle(v[0], v[1], v[2]);

	glColor3f(1.0,0.0,0.0); // Set fill color to red

	glViewport(300, 0, 300, 400); // Set right viewport

	glRotatef(90, 0.0, 0.0, 1.0); // Rotate about z axis

	triangle(v[0], v[1], v[2]); // Display blue triangle

	glFlush();

}

void init()

{

	glMatrixMode(GL_PROJECTION);

	//glLoadIdentity();//如果放在此处，会导致第一次显示正确，再次刷新时图形旋转

	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);

	glClearColor(1.0, 1.0, 1.0,1.0);

}

void main(int argc, char **argv)

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);

	glutInitWindowSize(600, 400);

	glutCreateWindow("Triangle");

	glutDisplayFunc(display);

	init();

	glutMainLoop();

}