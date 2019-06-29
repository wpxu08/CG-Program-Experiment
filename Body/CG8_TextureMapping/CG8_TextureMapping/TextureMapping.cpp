#include <GL/glut.h>

#include <stdlib.h>

#include <stdio.h>

#define stripeImageWidth 32

GLubyte stripeImage[4*stripeImageWidth];

void makeStripeImage(void) //生成纹理

{ int j;

for (j=0; j<stripeImageWidth; j++)

{ stripeImage[4*j+0] = (GLubyte) ((j<=4) ? 255 :0);

stripeImage[4*j+1] = (GLubyte) ((j>4) ? 255 : 0);

stripeImage[4*j+2] = (GLubyte) 0;

stripeImage[4*j+3] = (GLubyte) 255;

}

}

// 平面纹理坐标生成

static GLfloat xequalzero[] = {1.0, 1.0, 1.0, 1.0};

static GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};

static GLfloat *currentCoeff;

static GLenum currentPlane;

static GLint currentGenMode;

static float roangles;

void init(void)

{ glClearColor (1.0, 1.0, 1.0, 1.0);

glEnable(GL_DEPTH_TEST);

glShadeModel(GL_SMOOTH);

makeStripeImage();

glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

glTexImage1D(GL_TEXTURE_1D, 0, 4, stripeImageWidth, 0,

			 GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);

glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

currentCoeff = xequalzero;

currentGenMode = GL_OBJECT_LINEAR;

currentPlane = GL_OBJECT_PLANE;

glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);

glTexGenfv(GL_S, currentPlane, currentCoeff);

glEnable(GL_TEXTURE_GEN_S);

glEnable(GL_TEXTURE_1D);

glEnable(GL_LIGHTING);

glEnable(GL_LIGHT0);

glEnable(GL_AUTO_NORMAL);

glEnable(GL_NORMALIZE);

glFrontFace(GL_CW);

glMaterialf (GL_FRONT, GL_SHININESS, 64.0);

roangles=45.0f;

}

void display(void)

{ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glPushMatrix ();

glRotatef(roangles,0.0,0.0,1.0);

glutSolidSphere(2.0,32,32 );

glPopMatrix ();

glFlush();

}

void reshape(int w, int h)

{ glViewport(0, 0, (GLsizei) w, (GLsizei) h);

glMatrixMode(GL_PROJECTION);

glLoadIdentity();

if (w <= h)

glOrtho (-3.5, 3.5, -3.5*(GLfloat)h/(GLfloat)w,

		 3.5*(GLfloat)h/(GLfloat)w, -3.5, 3.5);

else

glOrtho (-3.5*(GLfloat)w/(GLfloat)h,

		 3.5*(GLfloat)w/(GLfloat)h, -3.5, 3.5, -3.5, 3.5);

glMatrixMode(GL_MODELVIEW);

glLoadIdentity();

}

void idle()

{ roangles += 0.05f;

glutPostRedisplay();

}

int main(int argc, char** argv)

{

	glutInit(&argc, argv);

	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(256, 256);

	glutInitWindowPosition(100, 100);

	glutCreateWindow (argv[0]);

	glutIdleFunc(idle);

	init ();

	glutDisplayFunc(display);

	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;

}