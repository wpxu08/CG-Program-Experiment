#include <GL/glut.h>

#include <stdlib.h>

#include<math.h>

#include <iostream>

using namespace std;

int SCREEN_WIDTH = 400; //屏幕尺寸

int SCREEN_HEIGHT = 400;

int posx1 = 150;

int posy1 = 200;

int posx2 = 250;

int posy2 = 200;

int select_point = 0; //1 是第一个点，2是第二个，以此类推

void init()

{ glClearColor(1,1,1,1);}

void draw(GLenum mode)

{ glPointSize(10.0f);

const static GLfloat color_selected[] = {1.0f,0.0f,0.0f};

const static GLfloat color_unselected[] = {0.0f,0.0f,1.0f};

for(int i=1;i<=5;i++)//5个点

{ if (mode == GL_SELECT) glLoadName(i);

glColor3fv( (select_point == i)? color_selected : color_unselected );

glBegin(GL_POINTS);

glVertex2f(i*70,200);

glEnd();

}

}

void display()

{ glClear(GL_COLOR_BUFFER_BIT);

draw(GL_RENDER); //设置渲染模式

glFlush();

}

void reshape(int width, int height)

{ glViewport(0, 0, width, height);

glMatrixMode(GL_PROJECTION);

glLoadIdentity();

gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT);

glMatrixMode(GL_MODELVIEW);

}

void processHits(GLint hits, GLuint buffer[])

{ unsigned int i;

GLuint name;

for(i=0; i<hits;i++)

{ name = buffer[3+i*4];

select_point = name;//每个选中物体有占用名字栈中4个单位，第4个是物体名字值

cout<<"第"<<name<<"个点"<<endl;

}

}

#define SIZE 500

#define N 10

void mouse(int button, int state, int x, int y)

{ GLuint selectBuffer[SIZE]; //存放物体名称的栈

GLint hits; //存放被选中对象个数

GLint viewport[4]; //存放可视区参数

if( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON) //当鼠标左键按下时

{glGetIntegerv(GL_VIEWPORT,viewport); //获取当前视口坐标参数

glSelectBuffer(SIZE,selectBuffer); //选择名称栈存放被选中的名称

glRenderMode(GL_SELECT); //设置当前为选择模式

glInitNames(); //初始化名称栈

glPushName(0);

glMatrixMode(GL_PROJECTION);

glPushMatrix();

glLoadIdentity();

gluPickMatrix(x,viewport[3]-y,N,N,viewport); //创建用于选择的投影矩阵栈

gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT); //设置投影方式

draw(GL_SELECT); //绘制场景,选择模式

glPopMatrix();

glFlush();

hits = glRenderMode(GL_RENDER);

glMatrixMode(GL_MODELVIEW);

if(hits > 0) processHits(hits,selectBuffer);

glutPostRedisplay();

}

if( state == GLUT_UP && button == GLUT_LEFT_BUTTON) //当鼠标左键抬起时

{ select_point = 0;

glRenderMode(GL_RENDER);

draw(GL_RENDER);

glutPostRedisplay();

}

}

int main(int argc, char *argv[])

{ glutInit(&argc, argv);

glutInitDisplayMode(GLUT_RGBA| GLUT_DEPTH );

glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);

glutInitWindowPosition(0, 0);

glutCreateWindow("图形拾取编程实例");

init();

glutDisplayFunc(display);

glutReshapeFunc(reshape);

glutMouseFunc(mouse);

glutMainLoop();

return 1;

}