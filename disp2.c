#include <GL/glut.h>
#include <stdio.h>
int numLines;
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
int x1,y1,ymax,ymin,xmax,xmin;
int flag1=0,flag2=0;
typedef enum state
{
waitingforclick,
clickedonce,
};
typedef struct point
{
int x;
int y;
}point;

point lines[256][4] ;

int gState=waitingforclick;
int lineisvalid=0;
int gHeight;
float gColor[3]={0,1,0};

void setPixel(int x,int y)
{
glBegin(GL_POINTS);
glVertex2f(x,y);
glEnd();
glFlush();
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


int setcode(int x, int y)
{
if(x<xmin && y>ymax)
return 9;
else if(x>xmin && x<xmax && y>ymax)
return 8;
else if(x>xmax && y>ymax)
return 10;
else if(x<xmin && y>ymin && y<ymax)
return 1;
else if(x>xmin && x<xmax && y>ymin && y<ymax)
return 0;
else if(x>xmax && y>ymin && y<ymax)
return 2;
else if(y<ymin && x<xmin)
return 5;
else if(y<ymin && x>xmin && x<xmax)
return 4;
else if(x>xmax && y<ymin)
return 6;
}

void clipping(int x1,int y1,int x2,int y2)
{
int code1[5]={0},code2[5]={0};
int cd1,cd2,var; 
int new;
if(y1>ymax)
 code1[0]=1;
if(y1<ymin)
 code1[1]=1;
if(x1>xmax)
 code1[2]=1;
if(x1<xmin)
 code1[3]=1;

cd1=1*code1[3]+2*code1[2]+4*code1[1]+8*code1[0];

if(y2>ymax)
 code2[0]=1;
if(y2<ymin)
 code2[1]=1;
if(x2>xmax)
 code2[2]=1;
if(x2<xmin)
 code2[3]=1;

cd2=1*code2[3]+2*code2[2]+4*code2[1]+8*code2[0];

if(!(cd1|cd2))
{ 
 linebres(x1,y1,x2,y2);
  
}
//else if(cd1&cd2!=0)

else if(!(cd1&cd2))
{
 if(cd1!=0)
 {
  if(code1[0]==1)
  { new=((ymax-y1)*(x2-x1))/(y2-y1)+x1;
    clipping(new,ymax,x2,y2);
  }
  else if(code1[1]==1)
  {
    new=((ymin-y1)*(x2-x1))/(y2-y1)+x1;
    clipping(new,ymin,x2,y2);
  }
  else if(code1[2]==1)
  {
   var=(y2-y1)/(x2-x1);
   new=y2-((x2-xmax)*var);
   clipping(xmax,new,x2,y2);
  }
  else if(code1[3]==1)
  {
   new=y1-(((y2-y1)*(x1-xmin))/(x2-x1));
   clipping(xmin,new,x2,y2);
  }
 }
 else
 {
  if(code2[0]==1)
  { new=((ymax-y1)*(x2-x1))/(y2-y1)+x1;
    clipping(x1,y1,new,ymax);
  }
  else if(code2[1]==1)
  {
    new=((ymin-y1)*(x2-x1))/(y2-y1)+x1;
    clipping(x1,y1,new,ymin);
  }
  else if(code2[2]==1)
  {
   new=y1-(((y2-y1)*(x1-xmax))/(x2-x1));
   clipping(x1,y1,xmax,new);
  }
  else if(code2[3]==1)
  {
   new=y1-(((y2-y1)*(x1-xmin))/(x2-x1));
   clipping(x1,y1,xmin,new);
  }
 
 }
}
}
 



void midptcircle(int r,int xc, int yc)
{
int p;
int x=0,y=r;
setPixel(0,r);
p=1-r;
	while(x<y)
	{
		if(p<0)
		{
			x+=1;
			p=p+(2*x)+1;
		}
		else
		{
			x+=1;
			y-=1;
			p=p+(2*x)+1-(2*y);
		}
		setPixel(xc+x,yc+y);
		setPixel(xc+y,yc+x);
		setPixel(xc-x,yc+y);
		setPixel(xc-y,yc+x);
		setPixel(xc-x,yc-y);
		setPixel(xc-y,yc-x);
		setPixel(xc+x,yc-y);
		setPixel(xc+y,yc-x);

	}
}



void drawlines()
{int i;
glColor3fv(gColor);
glBegin(GL_LINES);
for( i=0; i<=numLines; i++)
{
glVertex2i(lines[i][0].x, gHeight-lines[i][0].y);
glVertex2i(lines[i][2].x, gHeight-lines[i][2].y);

glVertex2i(lines[i][2].x, gHeight-lines[i][2].y);

glVertex2i(lines[i][3].x, gHeight-lines[i][3].y);
glVertex2i(lines[i][3].x, gHeight-lines[i][3].y);
glVertex2i(lines[i][1].x, gHeight-lines[i][1].y);
glVertex2i(lines[i][1].x, gHeight-lines[i][1].y);
glVertex2i(lines[i][0].x, gHeight-lines[i][0].y);


}
glEnd();
}

void back()
{
int i;
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1,1,1);
glBegin(GL_LINES);
for(i=0;i<=700;i+=50)
{
glVertex2i(0+i,0);
glVertex2i(700,700-i);
}
for(i=0;i<=700;i+=50)
{
glVertex2i(0,0+i);
glVertex2i(700-i,700);
}
for(i=0;i<=700;i+=50)
{
glVertex2i(0+i,0);
glVertex2i(0,0+i);
}
for(i=0;i<=700;i+=50)
{
glVertex2i(0+i,700);
glVertex2i(700,0+i);
}
glEnd();
glColor3f(1,0,0);
midptcircle(8,500,500);
glColor3f(0,1,0);
midptcircle(8,100,300);
glColor3f(0,0,1);
midptcircle(8,600,500);
glColor3f(1,0.5,0.5);
midptcircle(8,200,600);
glColor3f(0.5,0.5,0.5);
midptcircle(8,600,200);
glColor3f(0.3,0.7,0.5);
midptcircle(8,350,350);
glColor3f(1,0.5,1);
midptcircle(8,200,200);
glFlush();
}
void display()
{
//glClear(GL_COLOR_BUFFER_BIT);
if(flag1==0)
{back();
flag1=1;
}
if(flag2==0)
{
drawlines();
}
glutSwapBuffers();
}

void init()
{
glClearColor(0,0,0,1);
glMatrixMode(GL_PROJECTION);
//glOrtho(-1, 1.0, -1, 1.0, -1.0, 1.0);
gluOrtho2D(0,700,0,700);
numLines=-1;
//glMatrixMode(GL_MODELVIEW);

}

void clip()
{
glClear(GL_COLOR_BUFFER_BIT);

int i;
//clipping(400,300,650,600);
for(i=0;i<=700;i+=50)
{
clipping(i,0,700,700-i);
}
for(i=0;i<=700;i+=50)
{
clipping(0,i,700-i,700);
}
for(i=0;i<=700;i+=50)
{
clipping(i,0,0,i);
}
for(i=0;i<=700;i+=50)
{
clipping(i,700,700,i);
}

}

void mouseclick(int button, int state,int x, int y)
{
int i;
if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
{
switch(gState)
{

case waitingforclick:
printf("one clidk");
++numLines;
lines[numLines][0].x=x;
lines[numLines][0].y=y;
lines[numLines][1].x=x;
lines[numLines][1].y=y;
lines[numLines][2].x=x;
lines[numLines][2].y=y;
lines[numLines][3].x=x;
lines[numLines][3].y=y;
x1=x;
y1=y;
gState++;
flag2=0;
break;
case clickedonce:
printf("2 clidk");
lines[numLines][0].x=x1;
lines[numLines][0].y=y1;
lines[numLines][1].x=x;
lines[numLines][1].y=y1;
lines[numLines][2].x=x1;
lines[numLines][2].y=y;
lines[numLines][3].x=x;
lines[numLines][3].y=y;
back();
drawlines();

printf("%d %d %d %d \n",lines[numLines][0].x,lines[numLines][0].y,lines[numLines][3].x,lines[numLines][3].y);
if((lines[numLines][0].x<600) && (lines[numLines][3].x>600)&& (lines[numLines][0].y<200) && (lines[numLines][3].y>200))
{         
        flag2=1;
        printf("yaay!");
       xmax=lines[numLines][3].x;
        xmin=lines[numLines][0].x;
        ymax=700-lines[numLines][0].y;
        ymin=700-lines[numLines][3].y;
        
        
//printf("window: %d %d %d %d",xmax,xmin,ymax,ymin);      
//printf("Hello");
  clip();
  //flag2=0;
 }
gState=waitingforclick;

break;
}
}
glutPostRedisplay();
}
void mousedrag(int x, int y)
{
if(gState==clickedonce)
{
lines[numLines][0].x=x1;
lines[numLines][0].y=y1;
lines[numLines][1].x=x;
lines[numLines][1].y=y1;
lines[numLines][2].x=x1;
lines[numLines][2].y=y;
lines[numLines][3].x=x;
lines[numLines][3].y=y;
}

glutPostRedisplay();
}
void reshape(int width, int height)
{
gHeight=height;
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,700 , 0,700);
glMatrixMode(GL_MODELVIEW);
} 
int main(int argc, char ** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
glutInitWindowPosition(100,100);
glutInitWindowSize(700,700);
glutCreateWindow("My Drawing App");
glutReshapeFunc(reshape);
glutDisplayFunc(display);
glutMouseFunc(mouseclick);
glutPassiveMotionFunc(mousedrag);
glutPostRedisplay(); //added function for speed and clarity
init();
glutMainLoop();
return 0;
}
