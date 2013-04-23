#include<GL/glut.h>
#include<math.h>
#include<stdio.h>

int mptx[12]={150,150,130,50,50,100,100,200,200,250,250,170};
int mpty[12]={470,400,330,225,100,100,0,0,100,100,225,330};
void setPixel(int x,int y)
{
glBegin(GL_POINTS);
glVertex2f(x,y);
glEnd();
glFlush();
}

void midpoint(int r,float xc,float yc)
{       
	glColor3f(1,0,0);
	float x0=0,y0=r;
	setPixel(round(x0),round(y0));
	float x=x0,y=y0,p0,dx;
	p0=5/4-r;
	int i;
	while(x<y)
	{
	  			
		if(p0<0)
		{
			x+=1;
			p0+=2*x+1;
		}
		else
		{
			x+=1;
			y-=1;
			p0+=2*x+1-2*y;
		}

		setPixel(round(x+xc),round(y+yc));
		setPixel(round(x+xc),round(-y+yc));
		setPixel(round(-x+xc),round(y+yc));
		setPixel(round(-x+xc),round(-y+yc));
		setPixel(round(y+xc),round(x+yc));
		setPixel(round(y+xc),round(-x+yc));
		setPixel(round(-y+xc),round(x+yc));
		setPixel(round(-y+xc),round(-x+yc));
}
}
void linebres(int x1,int y1,int x2,int y2)
{

int dx,dy,steps,i,p;
float x,y;
x=x1;
y=y1;
dx=x2-x1;
dy=y2-y1;
if(abs(dx)>abs(dy))
{
p=(2*abs(dy))-abs(dx);
setPixel(x,y);
for(i=0;i<abs(dx)-1;i++)
{
if(p<0)
{
x+=(dx/abs(dx));
p+=(2*abs(dy));
}
else
{
x+=(dx/abs(dx));
y+=(dy/abs(dy));
p+=((2*abs(dy))-(2*abs(dx)));
}
setPixel(x,y);
}
}
else
{
p=(2*abs(dx))-abs(dy);
setPixel(x,y);
for(i=0;i<abs(dy)-1;i++)
{
if(p<0)
{
y+=(dy/abs(dy));
p+=(2*abs(dx));
}
else
{
x+=(dx/abs(dx));
y+=(dy/abs(dy));
p+=((2*abs(dx))-(2*abs(dy)));
}
setPixel(x,y);
}
}
}

void tran(int dx,int dy,int x,int y,int pt)
{

//glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1,0,0);
	
	
	glClear(GL_COLOR_BUFFER_BIT);

int i,j,k,genmat[3][3]={{1,0,dx},{0,1,dy},{0,0,1}},pointmat[3][1]={x,y,1},endmat[3][1];

for(i=0;i<3;i++)
for(j=0;j<1;j++)
{
endmat[i][j]=0;
for(k=0;k<3;k++)
endmat[i][j]+=genmat[i][k]*pointmat[k][j];
}

mptx[pt]=endmat[0][0];
mpty[pt]=endmat[1][0];

}
int keystates[1];
void keySpecial (int key, int x, int y) {
 //keystates[key]=1;
int i,j,t=100;
 if(key==GLUT_KEY_RIGHT)
{
	
	//glColor3f(1,0,0);
	
	//glClear(GL_COLOR_BUFFER_BIT);
        for(j=0;j<12;j++)
		tran(t,0,mptx[j],mpty[j],j);
	
	man();
	//glutSwapBuffers();
	//usleep(7500);
	
}
else if(key==GLUT_KEY_LEFT)
{
	
	glColor3f(1,0,0);
	
	//glClear(GL_COLOR_BUFFER_BIT);
        for(j=0;j<12;j++)
		tran(-t,0,mptx[j],mpty[j],j);
	man();

	
}
else if(key==GLUT_KEY_UP)
{
	
	
        for(j=0;j<12;j++)
		tran(0,t,mptx[j],mpty[j],j);
	man();
	//glutSwapBuffers();
	
}
else if(key==GLUT_KEY_DOWN)
{
	
	//glColor3f(1,0,0);
	
	//glClear(GL_COLOR_BUFFER_BIT);
        for(j=0;j<12;j++)
		tran(0,-t,mptx[j],mpty[j],j);
	man();
	//glutSwapBuffers();
	
}
}

void man()
{

midpoint(70,mptx[0],mpty[0]);
glBegin(GL_TRIANGLES);
glVertex2f(mptx[1],mpty[1]);
glVertex2f(mptx[4],mpty[4]);
glVertex2f(mptx[9],mpty[9]);
glEnd();

linebres(mptx[2],mpty[2],mptx[3],mpty[3]);
linebres(mptx[11],mpty[11],mptx[10],mpty[10]);
linebres(mptx[5],mpty[5],mptx[6],mpty[6]);
linebres(mptx[7],mpty[7],mptx[8],mpty[8]);
glFlush();
}



void display()
{
glClear (GL_COLOR_BUFFER_BIT);
man();
glFlush();
}

void init()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,4700,0,4700);
}

int main(int argc, char** argv)
{
glutInit(&argc,argv);
glutInitWindowSize(700,700);
glutCreateWindow("line");


glutDisplayFunc(display);
glutSpecialFunc(keySpecial);
//glutSpecialUpFunc(keySpecialUp); 
//glutKeyboardFunc(keyPressed);
init();
glutMainLoop();
}
