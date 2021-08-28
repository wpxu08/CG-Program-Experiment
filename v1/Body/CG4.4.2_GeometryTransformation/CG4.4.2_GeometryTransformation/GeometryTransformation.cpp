#include <GL/glut.h>

#include <stdlib.h>

#include <math.h>

/* ��ʼ����ʾ���ڴ�С */

GLsizei winWidth=600,winHeight=600;

/* ������������ϵ����ʾ��Χ */

GLfloat xwcMin=-300.0,xwcMax=300.0;

GLfloat ywcMin=-300.0,ywcMax=300.0;

void init (void)

{

	/* ������ʾ���ڵı�����ɫΪ��ɫ */

	glClearColor(1.0,1.0,1.0,0.0);

}

class wcPt3D

{

public:

	GLfloat x, y, z;

};

/* ��ά��ת�任����������ת�ᣨ�ɵ�p1��p2���壩����ת�Ƕȣ�thetaDegrees��*/

void rotate3D (wcPt3D p1, wcPt3D p2, GLfloat thetaDegrees)

{

	/* ������ת���ʸ�� */

	float vx = (p2.x - p1.x);

	float vy = (p2.y - p1.y);

	float vz = (p2.z - p1.z);

	/*ͨ��ƽ��-��ת-ƽ�Ƹ��ϱ任����������������ת��ע��OpenGL�еķ����ʾ��*/

	glTranslatef (p1.x, p1.y, p1.z); //���ƶ�p1��ԭʼλ��

	/*�ڹ���ͨ������ԭ�����������ת*/

	glRotatef (thetaDegrees, vx, vy, vz);

	glTranslatef (-p1.x, -p1.y, -p1.z); //���ƶ�p1��ԭ��λ��

}

/* ��ά���������任������������ϵ��sx��sy��sz�͹̶���fixedPt */

void scale3D (GLfloat sx, GLfloat sy, GLfloat sz, wcPt3D fixedPt)

{

	/*ͨ��ƽ��-����-ƽ�Ƹ��ϱ任������������Ϊ���ĵ�ı�������*/

	/* �۷�ƽ�Ƶ�ԭʼλ��*/

	glTranslatef (fixedPt.x, fixedPt.y, fixedPt.z);

	glScalef (sx, sy, sz); // �ڻ���ԭ��ı��������任

	/* �� �ƶ��̶��㵽����ԭ��*/

	glTranslatef (-fixedPt.x, -fixedPt.y, -fixedPt.z);

}

void displayFcn (void)

{

	/* ���ñ任���ĵ�λ�� */

	wcPt3D centroidPt,R_p1, R_p2;

	centroidPt.x=50;

	centroidPt.y=100;

	centroidPt.z=0;

	R_p1=centroidPt;

	R_p2.x=50;

	R_p2.y=100;

	R_p2.z=1;

	/* ���ü��α任����*/

	wcPt3D p1,p2,fixedPt;

	p1= R_p1;

	p2= R_p2;

	fixedPt=centroidPt;

	GLfloat tx=0.0,ty=100.0,tz=0;

	GLfloat sx=0.5,sy=0.5,sz=1;

	GLdouble thetaDegrees = 90;

	glClear(GL_COLOR_BUFFER_BIT); // �����ʾ����

	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity(); //��ձ任����Ϊ��λ���󣬻ָ�ԭʼ����ϵ����

	/* ��ʾ�任ǰ���ζ��� */

	glColor3f(0.0,0.0,1.0); // ����ǰ��ɫΪ��ɫ

	glRecti(50,100,200,150); //��ʾ��ɫ���Σ��任ǰ��

	/* ִ�м��α任��ע���Է�����ʽд����*/

	glTranslatef (tx, ty, tz); // ��ƽ�Ʊ任

	scale3D (sx, sy, sz, fixedPt); // �ڱ��������任

	rotate3D (p1, p2, thetaDegrees); // ����ת�任

	/* ��ʾ�任�󼸺ζ��� */

	glColor3f(1.0,0.0,0.0); //��������ǰ��ɫΪ��ɫ

	glRecti(50,100,200,150); //��ʾ��ɫ���Σ��任��

	glFlush();

}

void winReshapeFcn(GLint newWidth,GLint newHeight)

{

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluOrtho2D(xwcMin,xwcMax,ywcMin,ywcMax);

	glClear(GL_COLOR_BUFFER_BIT);

}

void main(int argc, char ** argv)

{

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);

	glutInitWindowPosition(50,50);

	glutInitWindowSize(winWidth,winHeight);

	glutCreateWindow("��ά���α任ʵ��-OpenGL�渴�ϱ任");

	init();

	glutDisplayFunc(displayFcn);

	glutReshapeFunc(winReshapeFcn);

	glutMainLoop();

}