#define NDEBUG
#include <GL/glut.h>

GLint winWidth = 600, winHeight = 600; //设置初始化窗口大小

/* 观察坐标系参数设置*/

GLfloat x0 = 0.0, y0= 0.0, z0 =5.0; // 设置观察坐标系原点

GLfloat xref = 0.0, yref =0.0, zref = 0.0; //设置观察坐标系参考点（视点）

GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0; // 设置观察坐标系向上向量（y轴）

/*观察体（视见体）参数设置 */

GLfloat xwMin = -1.0, ywMin = -1.0, xwMax = 1.0, ywMax = 1.0;//设置裁剪窗口坐标范围

GLfloat dnear = 1.5, dfar = 20.0;//设置远、近裁剪面深度范围

void init (void)

{

	glClearColor(1.0,1.0,1.0,0.0);


	/*①观察变换*/

	/*视点变换*/

	gluLookAt (x0, y0, z0, xref, yref, zref, Vx, Vy, Vz); //指定三维观察参数

	/*模型变换*/

	glMatrixMode (GL_MODELVIEW);

	glScalef (2.0, 2.0, 2.0); //比例放缩变换

	glRotatef(45.0, 0.0, 1.0, 1.0);//旋转变换

	/*②投影变换*/

	glMatrixMode (GL_PROJECTION);

	glLoadIdentity ();

	glFrustum (xwMin, xwMax, ywMin, ywMax, dnear, dfar);//透视投影，设置透视视景体

}

void display (void)

{

	glClear (GL_COLOR_BUFFER_BIT);

	glColor3f (0.0, 1.0, 0.0); // 设置前景色为绿色

	glutSolidCube (1.0);//绘制单位立方体实体

	glColor3f (0.0, 0.0, 0.0); // 设置前景色为黑色

	glLineWidth (2.0); //设置线宽

	glutWireCube (1.0);//绘制单位立方体线框

	glFlush ();

}

void reshape (GLint newWidth, GLint newHeight)

{

	/*③视口变换 */

	glViewport (0, 0, newWidth, newHeight);//定义视口大小

	winWidth = newWidth;

	winHeight = newHeight;

}

void main (int argc, char** argv)

{

	glutInit (&argc, argv);

	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition (100, 100);

	glutInitWindowSize (winWidth, winHeight);

	glutCreateWindow ("单位立方体的透视投影");

	init ();

	glutDisplayFunc (display);

	glutReshapeFunc (reshape);

	glutMainLoop ();

}