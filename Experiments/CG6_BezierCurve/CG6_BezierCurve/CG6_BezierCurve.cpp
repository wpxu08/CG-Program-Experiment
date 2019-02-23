#define NDEBUG
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;
struct XPoint{ // 为避免与Point名称冲突，改为XPoint
	int x, y;
	XPoint()
	{
		x = 0;
		y = 0;
	}
	XPoint(int ptx, int pty)
	{
		x = ptx;
		y = pty;
	}
};

vector<XPoint> cpt, bzpt; // for Control Point, Bezier Curve Point
bool bDraw;  // 是否显示Bezier曲线
int nInput;  // 输入控制顶点的数量

// 计算Bezier曲线上点的坐标
void CalcBZPoints()
{
	float a0,a1,a2,a3,b0,b1,b2,b3;

	a0=cpt[0].x;
	a1=-3*cpt[0].x+3*cpt[1].x;
	a2=3*cpt[0].x-6*cpt[1].x+3*cpt[2].x;
	a3=-cpt[0].x+3*cpt[1].x-3*cpt[2].x+cpt[3].x;

	b0=cpt[0].y;
	b1=-3*cpt[0].y+3*cpt[1].y;
	b2=3*cpt[0].y-6*cpt[1].y+3*cpt[2].y;
	b3=-cpt[0].y+3*cpt[1].y-3*cpt[2].y+cpt[3].y;

	int num = 10; // 曲线分段数量
	float dt = 1/(float)num;
	bzpt.resize(num + 1); //曲线上点的数量
	float t = 0;
	for (unsigned int i = 0; i < bzpt.size(); i++)
	{
		bzpt[i].x = a0 + a1 * t + a2 * t*t + a3 * t*t*t;
		bzpt[i].y = b0 + b1 * t + b2 * t*t + b3 * t*t*t;
		t = t + dt;
	}
}

void ControlPoint(vector<XPoint> cpt)
{
	glPointSize(3);
	for(unsigned int i=0; i<cpt.size(); i++)
	{
		glBegin (GL_POINTS);
		glColor3f (1.0f, 0.0f, 0.0f); glVertex2i (cpt[i].x,cpt[i].y);
		glEnd ();
	}
}

void PolylineGL(vector<XPoint> cpt)
{
	glBegin (GL_LINE_STRIP);
	for (unsigned int i = 0; i < cpt.size(); i++)
	{
		glColor3f (1.0f, 1.0f, 1.0f);
		glVertex2i (cpt[i].x,cpt[i].y);
	}
	glEnd ();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 0.0f);

	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0.0f); glVertex2i(0, 0);// 显示坐标原点位置
	glEnd();

	if (cpt.size() > 0) {
		ControlPoint(cpt);
		PolylineGL(cpt);
	}

	if(bDraw)
	{
		CalcBZPoints();
		PolylineGL(bzpt);
	}
	glFlush();
}

void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	printf("Please Click left button of mouse to input control Point of Bezier Curve...\n");
}

void Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

void mouse(int button, int state, int x, int y)
{
	//XPoint pt;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		switch (nInput)
		{
		case 0:
			cpt.clear();
			cpt.push_back(XPoint(x, 480-y));
			nInput++;
			bDraw = false;
			glutPostRedisplay();
			break;

		case 1:
			cpt.push_back(XPoint(x, 480 - y));
			nInput++;
			glutPostRedisplay();
			break;

		case 2:
			cpt.push_back(XPoint(x, 480 - y));
			nInput++;
			glutPostRedisplay();
			break;

		case 3:
			cpt.push_back(XPoint(x, 480 - y));
			nInput = 0;
			bDraw = true;
			glutPostRedisplay();
			break;

		default:
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Hello Bezier Curve");

	Init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}