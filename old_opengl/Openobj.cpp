#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include<string>
#include<fstream>
#include<sstream>
using namespace std;

//需要熟悉的类：ifstream, string
//需要熟悉的函数：std::getline(); std::istringstream(); sscanf()


int v_num = 0; //记录点的数量
int f_num = 0; //记录面的数量
int vn_num = 0;	//记录法向的数量
int vt_num = 0;	//记录纹理坐标的数量

GLfloat **vArr; //存放点的二维数组
GLfloat **vnArr; //存放法向的二维数组
GLfloat **vtArr; //存放纹理坐标的二维数组

int **fvArr;	//存放面顶点的二维数组
int **fnArr;	//存放面法向的二维数组
int **ftArr;	//存放面纹理坐标的二维数组





void getLineNum(string addrstr) //获取点和面的数量
{
	ifstream infile(addrstr.c_str()); //打开指定文件
	string sline;//每一行
		
	while(getline(infile,sline)) //从指定文件逐行读取
	{
		if(sline.empty())
			continue;

		if(sline[0]=='v')
		{
			if(sline[1]=='n')
				vn_num++;
			if(sline[1]=='t')
				vt_num++;
			else
				v_num++;
		}
		if(sline[0]=='f')
		{
			f_num++;
		}
	}

	infile.close();
}

int readfile(string addrstr) //将文件内容读到数组中去
{
	//new二维数组
	int i;
	vArr=new GLfloat*[v_num];	//顶点坐标
	for (i=0;i<v_num;i++)
	{
		vArr[i]=new GLfloat[3];
	}
	vnArr=new GLfloat*[vn_num];	//法向
	for (i=0;i<vn_num;i++)
	{
		vnArr[i]=new GLfloat[3];
	}
	vtArr=new GLfloat*[vt_num];	//纹理坐标
	for (i=0;i<vt_num;i++)
	{
		vtArr[i]=new GLfloat[2];
	}

	fvArr=new int*[f_num];
	fnArr=new int*[f_num];
	ftArr=new int*[f_num];
	for (i=0;i<f_num;i++)
	{
		fvArr[i]=new int[3];
		fnArr[i]=new int[3];
		ftArr[i]=new int[3];
	}

	///////////////////////////////////////////////////////////////
	//开始正式读取数据
	ifstream infile(addrstr.c_str());
	string sline;//每一行
	int vIndex = 0, vnIndex = 0, vtIndex = 0;
	int fIndex = 0;
	
	int v, t, n;
	string str;

	while(getline(infile,sline))
	{
		if(sline.empty())
			continue;

		//顶点数据
		if(sline[0]=='v')
		{
			if(sline[1]=='n')	//vn
			{
				istringstream sin(sline);  //istringstream对象可以绑定一行字符串，然后以空格为分隔符把该行分隔开来。
				sin >> str >> vnArr[vnIndex][0] >> vnArr[vnIndex][1] >> vnArr[vnIndex][2];
				vnIndex++;
			}
			else if(sline[1] == 't')	//vt
			{
				istringstream sin(sline); 
				sin >> str >> vtArr[vtIndex][0] >> vtArr[vtIndex][1] >> vtArr[vtIndex][2];
				vtIndex++;
			}
			else	//v
			{
				istringstream sin(sline);
				sin >> str >> vArr[vIndex][0] >> vArr[vIndex][1] >> vArr[vIndex][2];
				vIndex++;
			}
		}	//end if(sline[0]=='v')

		//面数据
		if (sline[0]=='f') //存储面
		{
			istringstream in(sline);	//sline can be one of v, v//n, v/t, v/t/n

			if(sline.find("//") != string::npos)	//说明是v//n
			{
				in >> str;//去掉f
//				in >> v0 >>  "//" >> vn0;		//看来流操作还是无法实现sscanf的这种灵活的功能，只能还是用sscanf了。

				in >> str;
				sscanf(str.c_str(), "%d//%d", &fvArr[fIndex][0], &fnArr[fIndex][0]);
				in >> str;
				sscanf(str.c_str(), "%d//%d", &fvArr[fIndex][1], &fnArr[fIndex][1]);
				in >> str;
				sscanf(str.c_str(), "%d//%d", &fvArr[fIndex][2], &fnArr[fIndex][2]);
			}
			else if(sscanf(sline.c_str(), "f %d/%d/%d", &v, &t, &n) == 3)
			{
				fvArr[fIndex][0] = v;
				ftArr[fIndex][0] = t;
				fnArr[fIndex][0] = n;
				in >> str >> str;		//把f和第一个顶点的字串都读过去
				in >> str;
				sscanf(str.c_str(), "%d/%d/%d", &fvArr[fIndex][1], &ftArr[fIndex][1], &fnArr[fIndex][1]);
				in >> str;
				sscanf(str.c_str(), "%d/%d/%d", &fvArr[fIndex][2], &ftArr[fIndex][2], &fnArr[fIndex][2]);
			}
			else if(sscanf(sline.c_str(), "f %d/%d", &v, &t) == 2)
			{
				fvArr[fIndex][0] = v;
				ftArr[fIndex][0] = t;
				in >> str >> str;		//把f和第一个顶点的字串都读过去
				in >> str;
				sscanf(str.c_str(), "%d/%d", &fvArr[fIndex][1], &ftArr[fIndex][1]);
				in >> str;
				sscanf(str.c_str(), "%d/%d", &fvArr[fIndex][2], &ftArr[fIndex][2]);			
			}
			else	//v
			{
				in >> str;	//f
				in >> str;
				sscanf(str.c_str(), "%d", &fvArr[fIndex][0]);
				in >> str;
				sscanf(str.c_str(), "%d", &fvArr[fIndex][1]);
				in >> str;
				sscanf(str.c_str(), "%d", &fvArr[fIndex][2]);
			}

			fIndex++;
		}	//end if(sline[0]=='f')
	}	//end while

	infile.close();
	return 0;
}




void init(void)
{
	getLineNum("E:/VisualStudio/vscode/opengl/obj/wan.obj");
	readfile("E:/VisualStudio/vscode/opengl/obj/wan.obj");    
	
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0f, 1.0, 0.0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);	
    glShadeModel(GL_SMOOTH);

//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	

	 /*
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation
	*/

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-30.0);
//	glScalef(0.1,0.1,0.1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glBegin(GL_TRIANGLES);
    for (int i=0;i<f_num;i++)	//目前不用法向，不加光照，只是为了把读入的模型显示一下
	{		
//		glNormal3f(vnArr[fnArr[i][0]-1][0], vnArr[fnArr[i][0]-1][1], vnArr[fnArr[i][0]-1][2]);
		glVertex3f(vArr[fvArr[i][0]-1][0], vArr[fvArr[i][0]-1][1], vArr[fvArr[i][0]-1][2]);
		
//		glNormal3f(vnArr[fnArr[i][1]-1][0], vnArr[fnArr[i][1]-1][1], vnArr[fnArr[i][1]-1][2]);
		glVertex3f(vArr[fvArr[i][1]-1][0], vArr[fvArr[i][1]-1][1], vArr[fvArr[i][1]-1][2]);
		
//		glNormal3f(vnArr[fnArr[i][2]-1][0], vnArr[fnArr[i][2]-1][1], vnArr[fnArr[i][2]-1][2]);
		glVertex3f(vArr[fvArr[i][2]-1][0], vArr[fvArr[i][2]-1][1], vArr[fvArr[i][2]-1][2]);
	}
	glEnd();
//	glutWireTeapot(1.0f);

	glutSwapBuffers();
//	glFlush();
	
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 60.0);

/*	if (w <= h)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h/(GLfloat)w, 1.5 * (GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-1.5*(GLfloat)w/(GLfloat)h, 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
*/	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Load a model");

	init();
    glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
    glutMainLoop();
    return 0;
}



/*	for(i = 0; i<3; i++)
	{
		in >> s1;
		a=0;
		strSize = s1.size();
		for(k=0; s1[k]!='/'; k++)	//这是采用逐字符计算的方法来得到整数
		{
			a = a*10 + (s1[k]-48);	//s1[k]-48就从字符转化成了整数数字
		}
		fvArr[kk][i] = a;
		for(k=k+2; k < strSize; k++)
		{
			a = a*10 + (s1[k]-48);	//s1[k]-48就从字符转化成了整数数字
		}
		fn[kk][i] = a;
	}
*/