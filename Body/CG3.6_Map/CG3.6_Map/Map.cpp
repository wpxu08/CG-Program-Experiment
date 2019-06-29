#include <iostream>
#include <fstream>
#include<vector>
#include <GL/glut.h>
using namespace std;

struct MapPoint
{
	double longitude; 
	double latitude;
};
struct PolygonX
{
	vector<MapPoint> points; //����εĶ�������
};

vector<PolygonX*> polys; //����μ���
vector<PolygonX*> ReadMapData(char* filename)
{
	int PointCount;
	vector<PolygonX*> PolygonXs;
	ifstream fs(filename);
	while(fs.eof()!=true)
	{
		PolygonX* poly=new PolygonX;
		fs>>PointCount;
		cout<<PointCount<<endl;
		for(int i=0;i<PointCount;i++)
		{
			MapPoint p;
			fs>>p.longitude>>p.latitude;
			poly->points.push_back(p);
		}
		PolygonXs.push_back(poly);

	}
	return PolygonXs;
}
void display(void)
{    
	glClear (GL_COLOR_BUFFER_BIT);    
	//����ɫɫ���Ƹ�ʡ�߽�
	glColor3f (0.0, 0.0, 1.0);
	glPolygonMode(GL_BACK, GL_LINE);     
	for(int i=0;i<polys.size();i++)
	{        
		vector<MapPoint> points=polys[i]->points;
		glBegin(GL_LINE_STRIP);
		for(int j=0;j<points.size();j++)
		{            
			glVertex3f (points[j].longitude, points[j].latitude, 0.0);        
		}
		glEnd();
	}    
	glFlush();
}
void init (void)
{
	//���ñ�����ɫ
	glClearColor (1.0, 1.0, 1.0, 0.0);
	//��ʼ���۲�ֵ
	glMatrixMode(GL_PROJECTION);    //������ģʽ��ΪͶӰ
	glLoadIdentity();                 //�Ծ�����е�λ��
	glOrtho(110.0, 118.0, 30.0, 38.0, -1.0, 1.0);   //����ƽ��ͶӰ����
}
int main(int argc, char** argv)
{
	//�����ļ��뵽http://files.cnblogs.com/opengl/HenanCounty.rar���طŵ�D�̸�Ŀ¼�²���ѹ
	char* filename="./HenanCounty.txt";

	polys=ReadMapData(filename);    
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);  //�������RGB
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("��ͼ����");
	init ();
	glutDisplayFunc(display);     //��ʾ�ص�����
	glutMainLoop();
	return 0; 
}