#include <stdio.h>
#include <iostream>   //标准输入输出头文件
#include <olectl.h>  // OLE控制库头文件
#include <math.h>   //数学函数头文件
//#include "Texture.h"
#include <gl/glut.h>
#ifndef GL_TEXTURE_LOADER
#define GL_TEXTURE_LOADER

bool BuildTexture(char *filename, GLuint &texid);
#endif
#define GL_CLAMP_TO_EDGE  0x812F

bool BuildTexture(char *filename, GLuint &texid)  //载入一个.TGA文件到内存
{   //未压缩的TGA Header
	GLubyte TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
	GLubyte  TGAcompare[12];  //用来比较TGA Header
	GLubyte  header[6];  //Header里，头六个有用字节
	GLuint  bytesPerPixel; //保存TGA文件里每个像素用到的字节数
	GLuint  imageSize; //用来保存随机产生的图像的大小
	GLuint temp;  //临时变量

	GLuint type = GL_RGBA;  //将默认的GL模式设置为RBGA (32 BPP) 
	GLubyte *imageData;  //图像数据（最高32位）
	GLuint bpp;  //每一像素的图像颜色深度
	GLuint width;  // 图像宽度
	GLuint height;  // 图像高度

	HDC hdcTemp; // DC用来保存位图
	HBITMAP hbmpTemp; // 保存临时位图
	IPicture *pPicture; //定义IPicture Interface
	OLECHAR wszPath[MAX_PATH + 1];  //图片的完全路径
	char szPath[MAX_PATH + 1];  //图片的完全路径
	long lWidth;  // 图像宽度
	long lHeight;  // 图像高度
	long lWidthPixels;  // 图像的宽带（以像素为单位）
	long lHeightPixels;  // 图像的高带（以像素为单位）
	GLint glMaxTexDim;  // 保存纹理的最大尺寸


	if (strstr(filename, "JPG") || strstr(filename, "bmp") || strstr(filename, "jpg") || strstr(filename, "BMP"))
	{
		if (strstr(filename, "http://"))    //如果路径包含“http://”
		{
			strcpy_s(szPath, filename);  //把路径拷贝到szPath
		}
		else   //否则从文件导入图片
		{
			GetCurrentDirectoryA(MAX_PATH, szPath); //取得当前路径
			strcat_s(szPath, "\\");  //添加字符”\” 
			strcat_s(szPath, filename); //添加图片的相对路径
		}

		MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH);
		//把ASCII码转化为Unicode标准码
		HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void**)&pPicture);
		if (FAILED(hr))  //如果导入失败
		{
			//图片载入失败出错信息
			return FALSE;    //返回FALSE
		}
		hdcTemp = CreateCompatibleDC(GetDC(0));   //建立窗口设备描述表
		if (!hdcTemp)    //建立失败？
		{
			pPicture->Release(); //释放IPicture
			//图片载入失败出错信息
			return FALSE;                     //返回 FALSE
		}
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);    //取得支持的纹理最大尺寸
		pPicture->get_Width(&lWidth);    //取得IPicture宽度（转换为Pixel格式）
		lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
		pPicture->get_Height(&lHeight);    //取得IPicture高度  （转换为Pixels格式）
		lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);
		//调整图片到最好的效果
		if (lWidthPixels <= glMaxTexDim)    //图片宽度是否超过显卡最大支持尺寸
			lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
		else    //否则，将图片宽度设为显卡最大支持尺寸
			lWidthPixels = glMaxTexDim;
		if (lHeightPixels <= glMaxTexDim)   //图片高度是否超过显卡最大支持尺寸
			lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
		else    //否则，将图片高度设为显卡最大支持尺寸
			lHeightPixels = glMaxTexDim;

		//建立一个临时位图
		BITMAPINFO    bi = { 0 }; //位图的类型
		DWORD    *pBits = 0; //指向位图Bits的指针


		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  //设置结构大小
		bi.bmiHeader.biBitCount = 32;  //32位
		bi.bmiHeader.biWidth = lWidthPixels;  //宽度像素值
		bi.bmiHeader.biHeight = lHeightPixels;  //高度像素值
		bi.bmiHeader.biCompression = BI_RGB;  //RGB格式
		bi.bmiHeader.biPlanes = 1;  //一个位平面
		//建立一个位图这样我们可以指定颜色和深度并访问每位的值
		hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void**)&pBits, 0, 0);
		if (!hbmpTemp)   //建立失败？
		{
			DeleteDC(hdcTemp);   //删除设备描述表
			pPicture->Release();  //释放IPicture
			//图片载入失败出错信息
			return FALSE;   //返回FALSE
		}
		SelectObject(hdcTemp, hbmpTemp); //选择临时DC句柄和临时位图对象
		//在位图上绘制IPicture
		pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);
		//将BGR转换为RGB,将alpha值设为255 
		for (long i = 0; i < lWidthPixels*lHeightPixels; i++)
		{
			BYTE*  pPixel = (BYTE*)(&pBits[i]);  //获取当前像素
			BYTE  temp = pPixel[0];    //临时存储第一个颜色像素（蓝色）
			pPixel[0] = pPixel[2];    //将红色值存到第一位
			pPixel[2] = temp;    //将蓝色值存到第三位
			pPixel[3] = 255;    //alpha值设为255
		}
		glGenTextures(1, &texid);   //创建纹理

		//使用来自位图数据生成的典型纹理
		glBindTexture(GL_TEXTURE_2D, texid);       //绑定纹理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//缩小采用三线性滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//放大采用线性滤波
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);

		DeleteObject(hbmpTemp);  //删除对象
		DeleteDC(hdcTemp);  //删除设备描述表
		pPicture->Release();  //释放IPicture
	}
	//如果路径包含"http：//"
	else if (strstr(filename, "TGA") || strstr(filename, "tga"))
	{
		// GLuint texID;  //纹理ID
		FILE *file = fopen(filename, "rb");  //打开TGA文件
		if (file == NULL)   //文件是否已存在
		{
			//图片载入失败出错信息
			return FALSE;    //返回FALSE
		}

		//是否有十二字节可读？
		if (fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare)
			|| memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0
			|| fread(header, 1, sizeof(header), file) != sizeof(header))
			//header和我们想要的是否相符？
			//如果是读六字节
		{
			fclose(file);    //如果失败，关闭文件
			//图片载入失败出错信息
			return FALSE;    //返回FALSE
		}
		width = header[1] * 256 + header[0];//确定TGA宽度(高字节*256+低字节）
		height = header[3] * 256 + header[2];//确定TGA高度(高字节*256+低字节)

		if (width <= 0     //宽度是否小于或等于O
			|| height <= 0  //高度是否小于或等于0
			|| (header[4] != 24 && header[4] != 32))    //TGA是24位或32位？
		{
			fclose(file);  //任何一个不成立，则关闭文件
			//图片载入失败出错信息
			return  FALSE;//返回 FALSE
		}

		bpp = header[4];    //获取TGA每个像素的位(24或32)
		bytesPerPixel = bpp / 8;    //除以8以取得每个像素的字节
		imageSize = width * height*bytesPerPixel;   //计算TGA数据所需要的内存

		imageData = (GLubyte*)malloc(imageSize);  //开辟内存空间存储TGA数据

		if (imageData == NULL || fread(imageData, 1, imageSize, file) != imageSize)//用来存储的内存是否存在？//实验图像大小是否与内存空间大小相符
		{
			if (imageData != NULL) //图像数据是否载入
			{
				free(imageData); //如果是释放图像数据
			}
			fclose(file);    //关闭文件
			return FALSE;    //返回FALSE
		}
		//循环遍历图像数据
		for (GLuint i = 0; i < int(imageSize); i += bytesPerPixel) {   //交换第一和第三字节（红和蓝）
			temp = imageData[i];   //将图像数据‘i’的值存在临时变量中
			imageData[i] = imageData[i + 2];   //将第三字节的值存到第一字节里
			imageData[i + 2] = temp; //将临时变量的值存入第三字节（第一字节的值）
		}
		fclose(file);    //关闭文件
		//创建一种纹理
		glGenTextures(1, &texid);   //产生OpenGL纹理ID

		glBindTexture(GL_TEXTURE_2D, texid);   //绑定纹理
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (bpp == 24)    //TGA图片是不是24位的
		{
			type = GL_RGB;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);
		free(imageData);
	}
	DeleteObject(hbmpTemp); //删除对象．
	DeleteDC(hdcTemp);  //删除设备描述表
	return TRUE;   //纹理创建成功，返回正确
}

int width = 1024, height = 768;
int ww = width, hh = height;  //得到当前窗口大小
float rotatex = 0, rotatey = 0;
int edge = 50;            //立方体边界

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
	glutInit(&argc, argv);  //初始化GLUT库
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//设置显示模式（缓存，颜色类型）
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(1024 / 2 - 250, 768 / 2 - 250);
	glutCreateWindow("立方体纹理映射");   //创建窗口
	glutReshapeFunc(myReshape);
	init();
	glutDisplayFunc(myDisplay);  //用于绘制当前窗口
	glutIdleFunc(myIdle);
	glutMainLoop();         //表示开始运行程序，用于程序的结尾
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
	gluPerspective(50, ww / hh, 20, 1000);//逶视投影
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
