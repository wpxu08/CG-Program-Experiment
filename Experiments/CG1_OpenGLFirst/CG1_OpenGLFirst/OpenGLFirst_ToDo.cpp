//��Ҫ��ȷ��װFreeGlut
//��װ������ο���https://blog.csdn.net/wpxu08/article/details/87785547
//#define NDEBUG
#include <GL/freeglut.h>

void myDisplay(void)
{
// Please insert your code here

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Hello World!");
	glutDisplayFunc(&myDisplay);
	glutMainLoop();
	return 0;
}