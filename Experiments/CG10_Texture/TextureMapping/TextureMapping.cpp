#include <stdio.h>
#include <iostream>   //��׼�������ͷ�ļ�
#include <olectl.h>  // OLE���ƿ�ͷ�ļ�
#include <math.h>   //��ѧ����ͷ�ļ�
//#include "Texture.h"
#include <gl/glut.h>
#ifndef GL_TEXTURE_LOADER
#define GL_TEXTURE_LOADER

bool BuildTexture(char *filename, GLuint &texid);
#endif
#define GL_CLAMP_TO_EDGE  0x812F

bool BuildTexture(char *filename, GLuint &texid)  //����һ��.TGA�ļ����ڴ�
{   //δѹ����TGA Header
	GLubyte TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	GLubyte  TGAcompare[12];  //�����Ƚ�TGA Header
	GLubyte  header[6];  //Header�ͷ���������ֽ�
	GLuint  bytesPerPixel; //����TGA�ļ���ÿ�������õ����ֽ���
	GLuint  imageSize; //�����������������ͼ��Ĵ�С
	GLuint temp;  //��ʱ����

	GLuint type = GL_RGBA;  //��Ĭ�ϵ�GLģʽ����ΪRBGA (32 BPP) 
	GLubyte *imageData;  //ͼ�����ݣ����32λ��
	GLuint bpp;  //ÿһ���ص�ͼ����ɫ���
	GLuint width;  // ͼ����
	GLuint height;  // ͼ��߶�

	HDC hdcTemp; // DC��������λͼ
	HBITMAP hbmpTemp; // ������ʱλͼ
	IPicture *pPicture; //����IPicture Interface
	OLECHAR wszPath[MAX_PATH + 1];  //ͼƬ����ȫ·��
	char szPath[MAX_PATH + 1];  //ͼƬ����ȫ·��
	long lWidth;  // ͼ����
	long lHeight;  // ͼ��߶�
	long lWidthPixels;  // ͼ��Ŀ����������Ϊ��λ��
	long lHeightPixels;  // ͼ��ĸߴ���������Ϊ��λ��
	GLint glMaxTexDim;  // ������������ߴ�


	if (strstr(filename, "JPG") || strstr(filename, "bmp") || strstr(filename, "jpg") || strstr(filename, "BMP"))
	{
		if (strstr(filename, "http://"))    //���·��������http://��
		{
			strcpy_s(szPath, filename);  //��·��������szPath
		}
		else   //������ļ�����ͼƬ
		{
			GetCurrentDirectoryA(MAX_PATH, szPath); //ȡ�õ�ǰ·��
			strcat_s(szPath, "\\");  //����ַ���\�� 
			strcat_s(szPath, filename); //���ͼƬ�����·��
		}

		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);
		//��ASCII��ת��ΪUnicode��׼��
		HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);
		if (FAILED(hr))  //�������ʧ��
		{
			//ͼƬ����ʧ�ܳ�����Ϣ
			return FALSE;    //����FALSE
		}
		hdcTemp = CreateCompatibleDC(GetDC(0));   //���������豸������
		if (!hdcTemp)    //����ʧ�ܣ�
		{
			pPicture->Release(); //�ͷ�IPicture
			//ͼƬ����ʧ�ܳ�����Ϣ
			return FALSE;                     //���� FALSE
		}
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);    //ȡ��֧�ֵ��������ߴ�
		pPicture->get_Width(&lWidth);    //ȡ��IPicture��ȣ�ת��ΪPixel��ʽ��
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);    //ȡ��IPicture�߶�  ��ת��ΪPixels��ʽ��
		lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);
		//����ͼƬ����õ�Ч��
		if (lWidthPixels <= glMaxTexDim)    //ͼƬ����Ƿ񳬹��Կ����֧�ֳߴ�
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
		else    //���򣬽�ͼƬ�����Ϊ�Կ����֧�ֳߴ�
			lWidthPixels = glMaxTexDim;
		if (lHeightPixels <= glMaxTexDim)   //ͼƬ�߶��Ƿ񳬹��Կ����֧�ֳߴ�
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
		else    //���򣬽�ͼƬ�߶���Ϊ�Կ����֧�ֳߴ�
			lHeightPixels = glMaxTexDim;

		//����һ����ʱλͼ
		BITMAPINFO    bi = { 0 }; //λͼ������
		DWORD    *pBits = 0; //ָ��λͼBits��ָ��


		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  //���ýṹ��С
		bi.bmiHeader.biBitCount = 32;  //32λ
		bi.bmiHeader.biWidth = lWidthPixels;  //�������ֵ
		bi.bmiHeader.biHeight = lHeightPixels;  //�߶�����ֵ
		bi.bmiHeader.biCompression = BI_RGB;  //RGB��ʽ
		bi.bmiHeader.biPlanes = 1;  //һ��λƽ��
		//����һ��λͼ�������ǿ���ָ����ɫ����Ȳ�����ÿλ��ֵ
		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
		if (!hbmpTemp)   //����ʧ�ܣ�
		{
			DeleteDC(hdcTemp);   //ɾ���豸������
			pPicture->Release();  //�ͷ�IPicture
			//ͼƬ����ʧ�ܳ�����Ϣ
			return FALSE;   //����FALSE
		}
		SelectObject(hdcTemp, hbmpTemp); //ѡ����ʱDC�������ʱλͼ����
		//��λͼ�ϻ���IPicture
		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);
		//��BGRת��ΪRGB,��alphaֵ��Ϊ255 
		for (long i = 0; i < lWidthPixels*lHeightPixels; i++)
		{
			BYTE*  pPixel = (BYTE*)(&pBits[i]);  //��ȡ��ǰ����
			BYTE  temp = pPixel[0];    //��ʱ�洢��һ����ɫ���أ���ɫ��
			pPixel[0] = pPixel[2];    //����ɫֵ�浽��һλ
			pPixel[2] = temp;    //����ɫֵ�浽����λ
			pPixel[3] = 255;    //alphaֵ��Ϊ255
		}
		glGenTextures(1, &texid);   //��������

		//ʹ������λͼ�������ɵĵ�������
		glBindTexture(GL_TEXTURE_2D, texid);       //������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//��С�����������˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//�Ŵ���������˲�
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

		DeleteObject(hbmpTemp);  //ɾ������
		DeleteDC(hdcTemp);  //ɾ���豸������
		pPicture->Release();  //�ͷ�IPicture
	}
	//���·������"http��//"
	else if (strstr(filename, "TGA") || strstr(filename, "tga"))
	{
		// GLuint texID;  //����ID
		FILE *file = fopen(filename, "rb");  //��TGA�ļ�
		if (file == NULL)   //�ļ��Ƿ��Ѵ���
		{
			//ͼƬ����ʧ�ܳ�����Ϣ
			return FALSE;    //����FALSE
		}

		//�Ƿ���ʮ���ֽڿɶ���
		if (fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare)
			|| memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0
			|| fread(header, 1, sizeof(header), file) != sizeof(header))
			//header��������Ҫ���Ƿ������
			//����Ƕ����ֽ�
		{
			fclose(file);    //���ʧ�ܣ��ر��ļ�
			//ͼƬ����ʧ�ܳ�����Ϣ
			return FALSE;    //����FALSE
		}
		width = header[1] * 256 + header[0];//ȷ��TGA���(���ֽ�*256+���ֽڣ�
		height = header[3] * 256 + header[2];//ȷ��TGA�߶�(���ֽ�*256+���ֽ�)

		if (width <= 0     //����Ƿ�С�ڻ����O
			|| height <= 0  //�߶��Ƿ�С�ڻ����0
			|| (header[4] != 24 && header[4] != 32))    //TGA��24λ��32λ��
		{
			fclose(file);  //�κ�һ������������ر��ļ�
			//ͼƬ����ʧ�ܳ�����Ϣ
			return  FALSE;//���� FALSE
		}

		bpp = header[4];    //��ȡTGAÿ�����ص�λ(24��32)
		bytesPerPixel = bpp / 8;    //����8��ȡ��ÿ�����ص��ֽ�
		imageSize = width * height*bytesPerPixel;   //����TGA��������Ҫ���ڴ�

		imageData = (GLubyte*)malloc(imageSize);  //�����ڴ�ռ�洢TGA����

		if (imageData == NULL || fread(imageData, 1, imageSize, file) != imageSize)//�����洢���ڴ��Ƿ���ڣ�//ʵ��ͼ���С�Ƿ����ڴ�ռ��С���
		{
			if (imageData != NULL) //ͼ�������Ƿ�����
			{
				free(imageData); //������ͷ�ͼ������
			}
			fclose(file);    //�ر��ļ�
			return FALSE;    //����FALSE
		}
		//ѭ������ͼ������
		for (GLuint i = 0; i < int(imageSize); i += bytesPerPixel) {   //������һ�͵����ֽڣ��������
			temp = imageData[i];   //��ͼ�����ݡ�i����ֵ������ʱ������
			imageData[i] = imageData[i + 2];   //�������ֽڵ�ֵ�浽��һ�ֽ���
			imageData[i + 2] = temp; //����ʱ������ֵ��������ֽڣ���һ�ֽڵ�ֵ��
		}
		fclose(file);    //�ر��ļ�
		//����һ������
		glGenTextures(1, &texid);   //����OpenGL����ID

		glBindTexture(GL_TEXTURE_2D, texid);   //������
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (bpp == 24)    //TGAͼƬ�ǲ���24λ��
		{
			type = GL_RGB;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
		free(imageData);
	}
	DeleteObject(hbmpTemp); //ɾ������
	DeleteDC(hdcTemp);  //ɾ���豸������
	return TRUE;   //�������ɹ���������ȷ
}

int width = 1024, height = 768;
int ww = width, hh = height;  //�õ���ǰ���ڴ�С
float rotatex = 0, rotatey = 0;
int edge = 50;            //������߽�

GLuint textureid[6];

void init();
void Display(void);
void Reshape(int w, int h);
void myidle(void);
void Cube(void);

int main(int argc, char** argv)
{

	//char *argv[] = { "hello", "  " };
	//int argc = 2;
	glutInit(&argc, argv);  //��ʼ��GLUT��
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//������ʾģʽ�����棬��ɫ���ͣ�
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(1024 / 2 - 250, 768 / 2 - 250);
	glutCreateWindow("����������ӳ��");   //��������
	glutReshapeFunc(myReshape);
	init();
	glutDisplayFunc(myDisplay);  //���ڻ��Ƶ�ǰ����
	glutIdleFunc(myIdle);
	glutMainLoop();         //��ʾ��ʼ���г������ڳ���Ľ�β
	return 0;
}
void init(void)
{
	//glClearColor (1.0, 1.0, 1.0, 1.0) ;
	glLineWidth(1.0);
	glPointSize(1.0);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	BuildTexture("smile.jpg", textureid[0]);
	BuildTexture("smilel.jpg", textureid[1]);
	BuildTexture("smile2.jpg", textureid[2]);
	BuildTexture("smile3.jpg", textureid[3]);
	BuildTexture("smile4.jpg", textureid[4]);
	BuildTexture("smile5.jpg", textureid[5]);

	glEnable(GL_TEXTURE_2D);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(2 * edge, 2 * edge, 2 * edge, 0, 0, 0, 0, 1, 0);
	glRotatef(rotatey, 0, 1, 0);
	glRotatef(rotatex, 1, 0, 0);
	Cube();
	glutSwapBuffers();
}

void myReshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ww = w;
	hh = h;
	gluPerspective(50, ww / hh, 20, 1000);//����ͶӰ
	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}
void  myCube()
{
	glBindTexture(GL_TEXTURE_2D, textureid[0]);
	glBegin(GL_QUADS);

	//frontface
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f);

	glVertex3f(edge / 2, edge / 2, edge / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-edge / 2, edge / 2, edge / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-edge / 2, -edge / 2, edge / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(edge / 2, -edge / 2, edge / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureid[1]);

	glBegin(GL_QUADS);
	//back face
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-edge / 2, edge / 2, -edge / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(edge / 2, edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(edge / 2, -edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-edge / 2, -edge / 2, -edge / 2);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, textureid[2]);
	glBegin(GL_QUADS);
	//left face
	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-edge / 2, edge / 2, edge / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-edge / 2, edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-edge / 2, -edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-edge / 2, -edge / 2, edge / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureid[3]);
	glBegin(GL_QUADS);
	//right face
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(edge / 2, edge / 2, -edge / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(edge / 2, edge / 2, edge / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(edge / 2, -edge / 2, edge / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(edge / 2, -edge / 2, -edge / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureid[4]);
	glBegin(GL_QUADS);
	//upper face
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(edge / 2, edge / 2, edge / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(edge / 2, edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-edge / 2, edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-edge / 2, edge / 2, edge / 2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textureid[5]);
	glBegin(GL_QUADS);
	//lower face
	glNormal3f(0, -1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(edge / 2, -edge / 2, edge / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-edge / 2, -edge / 2, edge / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-edge / 2, -edge / 2, -edge / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(edge / 2, -edge / 2, -edge / 2);
	glEnd();
}


void myIdle()
{
	::Sleep(1);
	rotatex += 1;
	rotatey += 1;
	glutPostRedisplay();
}
