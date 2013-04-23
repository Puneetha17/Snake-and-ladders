#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define PI 3.14

int a,b,c,d;	
int dx,dy,sx,sy;

void setPixel(int x,int y)
{
glBegin(GL_POINTS);
glVertex2f(x,y);
glEnd();
glFlush();
}

void bres(int x1,int y1,int x2,int y2)
{
int dx,dy,steps,i,p;
float x,y;
x=(float)x1;
y=(float)y1;
dx=x2-x1;
dy=y2-y1;
if(abs(dx)>abs(dy))
{
p=(2*abs(dy))-abs(dx);
setPixel(round(x),round(y));
for(i=0;i<abs(dx)-1;i++)
{
if(p<0)
{
x=x+(dx/(abs(dx)));
p=p+(2*abs(dy));
}
else
{
x=x+(dx/(abs(dx)));
y=y+(dy/(abs(dy)));
p=p+(2*abs(dy))-(2*abs(dx));
}
setPixel(round(x),round(y));
}
}
else
{
p=(2*abs(dx))-abs(dy);
setPixel(round(x),round(y));
for(i=0;i<abs(dy)-1;i++)
{
if(p<0)
{
y=y+(dy/(abs(dy)));
p=p+(2*abs(dx));
}
else
{
x=x+(dx/(abs(dx)));
y=y+(dy/(abs(dy)));
p=p+(2*abs(dx))-(2*abs(dy));
}
setPixel(round(x),round(y));
}
}
}

void matmul(int a[3][3],int b[3][1],int c[3][1])
{
int i,j,k;
for(i=0;i<3;i++)
{
for(j=0;j<1;j++)
{
c[i][j]=0;
for(k=0;k<3;k++)
{
c[i][j]+=(a[i][k]*b[k][j]);
}
}
}
}
void matmul1(float a[3][3],float b[3][1],float c[3][1])
{
int i,j,k;
for(i=0;i<3;i++)
{
for(j=0;j<1;j++)
{
c[i][j]=0;
for(k=0;k<3;k++)
{
c[i][j]+=(a[i][k]*b[k][j]);
}
}
}
}

void translate(int tx,int ty,int x1,int y1,int x2,int y2)
{
int res1[3][1],res2[3][1],trans[3][3]={{1,0,tx},{0,1,ty},{0,0,1}},orig1[3][1]={x1,y1,1},orig2[3][1]={x2,y2,1};
matmul(trans,orig1,res1);
matmul(trans,orig2,res2);
bres(res1[0][0],res1[1][0],res2[0][0],res2[1][0]);
a=(int)res1[0][0];
b=(int)res1[1][0];
c=(int)res2[0][0];
d=(int)res2[1][0];
}

void rotate(float theta,int x1,int y1,int x2,int y2)
{
float res1[3][1],res2[3][1],rot[3][3]={{cos(theta*PI/180.0),-sin(theta*PI/180.0),0},{sin(theta*PI/180.0),cos(theta*PI/180.0),0},{0,0,1}},orig1[3][1]={(float)x1,(float)y1,1.0},orig2[3][1]={(float)x2,(float)y2,1};
matmul1(rot,orig1,res1);
matmul1(rot,orig2,res2);
printf("%d %d %d %d\n",(int)res1[0][0],(int)res1[1][0],(int)res2[0][0],(int)res2[1][0]);
bres((int)res1[0][0],(int)res1[1][0],(int)res2[0][0],(int)res2[1][0]);
a=(int)res1[0][0];
b=(int)res1[1][0];
c=(int)res2[0][0];
d=(int)res2[1][0];
}
void scale(int sx,int sy,int x1,int y1,int x2,int y2)
{
int res1[3][1],res2[3][1],trans[3][3]={{sx,0,0},{0,sy,0},{0,0,1}},orig1[3][1]={x1,y1,1},orig2[3][1]={x2,y2,1};
matmul(trans,orig1,res1);
matmul(trans,orig2,res2);
bres(res1[0][0],res1[1][0],res2[0][0],res2[1][0]);
a=(int)res1[0][0];
b=(int)res1[1][0];
c=(int)res2[0][0];
d=(int)res2[1][0];
}
void shear(int sx,int x1,int y1,int x2,int y2)
{
int res1[3][1],res2[3][1],trans[3][3]={{1,sx,0},{0,1,0},{0,0,1}},orig1[3][1]={x1,y1,1},orig2[3][1]={x2,y2,1};
matmul(trans,orig1,res1);
matmul(trans,orig2,res2);
bres(res1[0][0],res1[1][0],res2[0][0],res2[1][0]);
a=(int)res1[0][0];
b=(int)res1[1][0];
c=(int)res2[0][0];
d=(int)res2[1][0];
}

void keyPressed (unsigned char key, int x, int y) {  
int i;
 if(key=='r')
{
	dx=3;dy=0;
	glColor3f(1,0,0);
	for(i=0;i<dx;i++)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	translate(i,dy,a,b,c,d);

	glutSwapBuffers();
	usleep(7500);
	}
}
else if(key=='l')
{
	dx=-3;dy=0;
	glColor3f(1,0,0);
	for(i=0;i>dx;i--)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	translate(i,dy,a,b,c,d);
	glutSwapBuffers();
	usleep(7500);
	}
}
else if(key=='t')
{
	dx=0;dy=3;
	glColor3f(1,0,0);
	for(i=0;i<dy;i++)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	translate(dx,i,a,b,c,d);
	glutSwapBuffers();
	usleep(7500);
	}
}
else if(key=='b')
{
	dx=0;dy=-3;
	glColor3f(1,0,0);
	for(i=0;i>dy;i--)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	translate(dx,i,a,b,c,d);
	glutSwapBuffers();
	usleep(7500);
	}
}
else if(key=='c')
{
	int angle;
	angle=2;
	for(i=0;i<angle;i++)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	rotate(i,a,b,c,d);
	glutSwapBuffers();
	usleep(7500);
	}	
}
else if(key=='n')
{
	int angle;
	angle=-2;
	for(i=0;i>angle;i--)
	{
	glClear(GL_COLOR_BUFFER_BIT);
	rotate(i,a,b,c,d);
	glutSwapBuffers();
	usleep(7500);
	}	
}
else if(key=='s')
{
	sx=2;sy=2;
	glClear(GL_COLOR_BUFFER_BIT);
	scale(sx,sy,a,b,c,d);
}
else if(key=='h')
{
	sx=2;
	glClear(GL_COLOR_BUFFER_BIT);
	shear(sx,a,b,c,d);
}


 }
void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	a=150; b=350; c=350; d=200; 
	bres(a,b,c,d);
	glColor3f(1,0,0);
	glColor3f(1,0,0);
	glFlush();
}



void init()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,1000,0,1000);
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutCreateWindow("simple");
	init();
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
}
