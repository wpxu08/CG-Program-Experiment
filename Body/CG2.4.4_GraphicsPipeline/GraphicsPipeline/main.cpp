#include <GL/glut.h>

float angle = 0.0f; //旋转角度
void Init()
{
	GLfloat light_ambient[] = { 1.5,1.5,1.5,1.0 };//环境光分量RGB值
	float lpos[4] = { 1.0,1.0,1,0 };//灯光坐标位置

	glEnable(GL_DEPTH_TEST); //启用深度测试
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //背景为黑色
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void Reshape(int w, int h)
{
	if (0 == h)		h = 1;  //防止窗口高度太小而造成除零问题
	float ratio = 1.0f* w / h;
	glViewport(0, 0, w, h); //设置视区尺寸

	glMatrixMode(GL_PROJECTION); //指定当前操作投影矩阵堆栈
	glLoadIdentity(); //重置投影矩阵
	
	gluPerspective(45.0f, ratio, 1.0f, 10.0f);//指定透视投影的观察空间
	glMatrixMode(GL_MODELVIEW);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除颜色和深度缓冲区
	glColor3f(0.8f, 0.8f, 0.8f);      //指定绘制立方体颜色
	glMatrixMode(GL_MODELVIEW);       //指定当前操作模型视图矩阵堆栈
	glLoadIdentity();                 //重置模型视图矩阵
	glTranslatef(-1.0f, 0.0f, -5.0f); //将线框立方体沿x轴和z轴负向移动
	glRotatef(angle, 0.0f, 1.0f, 0.0f);//线框立方体绕y轴旋转
	glutWireCube(1);

	glEnable(GL_LIGHTING);              //启用光照
	glEnable(GL_LIGHT0);                //开启灯光0
	glLoadIdentity(); 
	glTranslatef(1.0f, 0.0f, -5.0f); 
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glutSolidCube(1);
	glDisable(GL_LIGHTING);              //关闭光照，因为线框立方体启用光照效果不好

	angle += 0.01f;
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	//窗口使用RGB颜色，双缓存和深度缓存
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 400);
	glutCreateWindow("旋转的立方体");
	glutReshapeFunc(Reshape);
	glutDisplayFunc(myDisplay);
	//glutIdleFunc(myDisplay);
	Init();
	glutMainLoop();

	return 0;
}