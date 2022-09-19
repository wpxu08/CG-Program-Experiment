#define NDEBUG
#include <GL/glut.h>
#include <stdlib.h>

static int year = 0, day = 0, moon = 0;

//定义太阳的材质系数  
GLfloat sun_ambient[] = { 0.2f, 0.2f, 0.0f, 1.0f };
GLfloat sun_diffuse[] = { 0.8f, 0.8f, 0.0f, 1.0f };
GLfloat sun_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat sun_emission[] = { 0.1f, 0.0f, 0.0f, 1.0f };

//定义地球的材质系数  
GLfloat earth_ambient[] = { 0.0f, 0.2f, 0.0f, 1.0f };
GLfloat earth_diffuse[] = { 0.0f, 0.8f, 0.0f, 1.0f };
GLfloat earth_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat earth_shininess[] = { 80.0f };
GLfloat earth_emission[] = { 0.0f, 0.1f, 0.0f, 1.0f };

//定义月球的材质系数  
GLfloat moon_ambient[] = { 0.0f, 0.0f, 0.1f, 1.0f };
GLfloat moon_diffuse[] = { 0.0f, 0.0f, 0.8f, 1.0f };
GLfloat moon_specular[] = { 0.0f, 0.0f, 0.9f, 1.0f };
GLfloat moon_shininess[] = { 50.0f };
GLfloat moon_emission[] = { 0.0f, 0.1f, 0.0f, 1.0f };

void init(void)
{
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat Light_Model_Ambient[] = { 0.2f , 0.2f , 0.2f , 1.0f };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
	glutSolidSphere(1.0, 20, 16);   /* draw sun */
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);

	glTranslatef(2.0, 0.0, 0.0);
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, earth_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, earth_emission);
	glutSolidSphere(0.2, 10, 8);    /* draw earth */

	glTranslatef(1.0, 0.0, 0.0);
	glRotatef((GLfloat)moon, 0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, moon_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, moon_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, moon_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, moon_emission);
	glutSolidSphere(0.15, 10, 8);    /* draw moon */
	glPopMatrix();

	glFlush();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':
		day = (day + 10) % 360;
		moon = (moon + 5) % 360;
		glutPostRedisplay();
		break;
	case 'D':
		day = (day - 10) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		year = (year + 5) % 360;
		day = (day + 10) % 360;
		moon = (moon + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Y':
		year = (year - 5) % 360;
		glutPostRedisplay();
		break;
	case 'm':
		moon = (moon + 5) % 360;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//窗口使用RGB颜色，单缓存和深度缓存
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}