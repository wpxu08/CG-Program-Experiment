#include <GL/glut.h>

#include <stdlib.h>

#include<math.h>

#include <iostream>

using namespace std;

int SCREEN_WIDTH = 400; //��Ļ�ߴ�

int SCREEN_HEIGHT = 400;

int posx1 = 150;

int posy1 = 200;

int posx2 = 250;

int posy2 = 200;

int select_point = 0; //1 �ǵ�һ���㣬2�ǵڶ������Դ�����

void init()

{ glClearColor(1,1,1,1);}

void draw(GLenum mode)

{ glPointSize(10.0f);

const static GLfloat color_selected[] = {1.0f,0.0f,0.0f};

const static GLfloat color_unselected[] = {0.0f,0.0f,1.0f};

for(int i=1;i<=5;i++)//5����

{ if (mode == GL_SELECT) glLoadName(i);

glColor3fv( (select_point == i)? color_selected : color_unselected );

glBegin(GL_POINTS);

glVertex2f(i*70,200);

glEnd();

}

}

void display()

{ glClear(GL_COLOR_BUFFER_BIT);

draw(GL_RENDER); //������Ⱦģʽ

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

select_point = name;//ÿ��ѡ��������ռ������ջ��4����λ����4������������ֵ

cout<<"��"<<name<<"����"<<endl;

}

}

#define SIZE 500

#define N 10

void mouse(int button, int state, int x, int y)

{ GLuint selectBuffer[SIZE]; //����������Ƶ�ջ

GLint hits; //��ű�ѡ�ж������

GLint viewport[4]; //��ſ���������

if( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON) //������������ʱ

{glGetIntegerv(GL_VIEWPORT,viewport); //��ȡ��ǰ�ӿ��������

glSelectBuffer(SIZE,selectBuffer); //ѡ������ջ��ű�ѡ�е�����

glRenderMode(GL_SELECT); //���õ�ǰΪѡ��ģʽ

glInitNames(); //��ʼ������ջ

glPushName(0);

glMatrixMode(GL_PROJECTION);

glPushMatrix();

glLoadIdentity();

gluPickMatrix(x,viewport[3]-y,N,N,viewport); //��������ѡ���ͶӰ����ջ

gluOrtho2D(0,SCREEN_WIDTH,0,SCREEN_HEIGHT); //����ͶӰ��ʽ

draw(GL_SELECT); //���Ƴ���,ѡ��ģʽ

glPopMatrix();

glFlush();

hits = glRenderMode(GL_RENDER);

glMatrixMode(GL_MODELVIEW);

if(hits > 0) processHits(hits,selectBuffer);

glutPostRedisplay();

}

if( state == GLUT_UP && button == GLUT_LEFT_BUTTON) //��������̧��ʱ

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

glutCreateWindow("ͼ��ʰȡ���ʵ��");

init();

glutDisplayFunc(display);

glutReshapeFunc(reshape);

glutMouseFunc(mouse);

glutMainLoop();

return 1;

}