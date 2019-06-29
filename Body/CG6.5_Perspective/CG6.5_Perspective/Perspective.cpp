#define NDEBUG
#include <GL/glut.h>

GLint winWidth = 600, winHeight = 600; //���ó�ʼ�����ڴ�С

/* �۲�����ϵ��������*/

GLfloat x0 = 0.0, y0= 0.0, z0 =5.0; // ���ù۲�����ϵԭ��

GLfloat xref = 0.0, yref =0.0, zref = 0.0; //���ù۲�����ϵ�ο��㣨�ӵ㣩

GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; // ���ù۲�����ϵ����������y�ᣩ

/*�۲��壨�Ӽ��壩�������� */

GLfloat xwMin = -1.0, ywMin = -1.0, xwMax = 1.0, ywMax = 1.0;//���òü��������귶Χ

GLfloat dnear = 1.5, dfar = 20.0;//����Զ�����ü�����ȷ�Χ

void init (void)

{

	glClearColor(1.0,1.0,1.0,0.0);


	/*�ٹ۲�任*/

	/*�ӵ�任*/

	gluLookAt (x0, y0, z0, xref, yref, zref, Vx, Vy, Vz); //ָ����ά�۲����

	/*ģ�ͱ任*/

	glMatrixMode (GL_MODELVIEW);

	glScalef (2.0, 2.0, 2.0); //���������任

	glRotatef(45.0, 0.0, 1.0, 1.0);//��ת�任

	/*��ͶӰ�任*/

	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	glFrustum (xwMin, xwMax, ywMin, ywMax, dnear, dfar);//͸��ͶӰ������͸���Ӿ���

}

void display (void)

{

	glClear (GL_COLOR_BUFFER_BIT);

	glColor3f (0.0, 1.0, 0.0); // ����ǰ��ɫΪ��ɫ

	glutSolidCube (1.0);//���Ƶ�λ������ʵ��

	glColor3f (0.0, 0.0, 0.0); // ����ǰ��ɫΪ��ɫ

	glLineWidth (2.0); //�����߿�

	glutWireCube (1.0);//���Ƶ�λ�������߿�

	glFlush ();

}

void reshape (GLint newWidth, GLint newHeight)

{

	/*���ӿڱ任 */

	glViewport (0, 0, newWidth, newHeight);//�����ӿڴ�С

	winWidth = newWidth;

	winHeight = newHeight;

}

void main (int argc, char** argv)

{

	glutInit (&argc, argv);

	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition (100, 100);

	glutInitWindowSize (winWidth, winHeight);

	glutCreateWindow ("��λ�������͸��ͶӰ");

	init ();

	glutDisplayFunc (display);

	glutReshapeFunc (reshape);

	glutMainLoop ();

}