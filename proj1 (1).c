
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif
#include<math.h>
#include <GL/glut.h>
#define PI 3.14
int count=1;
void scanfill(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
void rotate(float,int,int);
void elip(int,int,int,int,int);
void movepawn(int xin[2],int yin[2],int rnd,int xcen[2],int ycen[2]);
void init();
void display();
void dice(int );
int randm();
int dicenum=3;
int xcen[2]={300,300},ycen[2]={100,300},xin[2]={0,0},yin[2]={0,0},flag1=0;
/*----------------------------------------------------------------------------------------
 *	Global Variables
 */

/*
 *	A structure to represent the mouse information
 */
struct Mouse
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/

	/*
	 *	These two variables are a bit odd. Basically I have added these to help replicate
	 *	the way that most user interface systems work. When a button press occurs, if no
	 *	other button is held down then the co-ordinates of where that click occured are stored.
	 *	If other buttons are pressed when another button is pressed it will not update these
	 *	values.
	 *
	 *	This allows us to "Set the Focus" to a specific portion of the screen. For example,
	 *	in maya, clicking the Alt+LMB in a view allows you to move the mouse about and alter
	 *	just that view. Essentually that viewport takes control of the mouse, therefore it is
	 *	useful to know where the first click occured....
	 */
	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

void drawstring(float x,float y,float z,char *string) /* to display text */
{
	char *c;
	glRasterPos3f(x,y,z);
 
	for(c=string;*c!='\0';c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
	}
}

int randm()
{
	return rand()%6+1;
}

/*
 *	rename the structure from "struct Mouse" to just "Mouse"
 */
typedef struct Mouse Mouse;

/*
 *	Create a global mouse structure to hold the mouse information.
 */
Mouse TheMouse = {0,0,0,0,0};

/*
 *	Variables to hold the current size of the window.
 */
int winw = 700;
int winh = 700;


/*----------------------------------------------------------------------------------------
 *	Button Stuff
 */

/*
 *	We will define a function pointer type. ButtonCallback is a pointer to a function that
 *	looks a bit like this :
 *
 *	void func() {
 *	}
 */
typedef void (*ButtonCallback)();

/*
 *	This is a simple structure that holds a button.
 */
struct Button
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
};

typedef struct Button Button;



/*----------------------------------------------------------------------------------------
 *	This is an example callback function. Notice that it's type is the same
 *	as the ButtonCallback type. We can assign a pointer to this function which
 *	we can store and later call.
 */
void TheButtonCallback()
{
	//printf("AAA\n");
	//dicenum=randm();
	dice(6);
	
    //movepawn(xin,yin,dicenum,xcen,ycen);
    movepawn(xin,yin,100,xcen,ycen);
    //usleep(10000);

}

/*----------------------------------------------------------------------------------------
 *	This is the button visible in the viewport. This is a shorthand way of
 *	initialising the structure's data members. Notice that the last data
 *	member is a pointer to the above function.
 */Button MyButton = {625,420, 50,25, 0,0, "Roll Dice", TheButtonCallback };

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws a text string to the screen using glut bitmap s.
 *	\param	font	-	the font to use. it can be one of the following :
 *
 *					GLUT_BITMAP_9_BY_15
 *					GLUT_BITMAP_8_BY_13
 *					GLUT_BITMAP_TIMES_ROMAN_10
 *					GLUT_BITMAP_TIMES_ROMAN_24
 *					GLUT_BITMAP_HELVETICA_10
 *					GLUT_BITMAP_HELVETICA_12
 *					GLUT_BITMAP_HELVETICA_18
 *
 *	\param	text	-	the text string to output
 *	\param	x		-	the x co-ordinate
 *	\param	y		-	the y co-ordinate
 */
void Font(void *font,char *text,int x,int y)
{
	glRasterPos2i(x, y);

	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is used to see if a mouse click or event is within a button
 *			client area.
 *	\param	b	-	a pointer to the button to test
 *	\param	x	-	the x coord to test
 *	\param	y	-	the y-coord to test
 */
int ButtonClickTest(Button* b,int x,int y)
{
	if( b)
	{
		/*
		 *	If clicked within button area, then return true
		 */
	    if( x > b->x      &&
			x < b->x+b->w &&
			y > b->y      &&
			y < b->y+b->h ) {
				return 1;
		}
	}

	/*
	 *	otherwise false.
	 */
	return 0;
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonRelease(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	If the mouse button was pressed within the button area
		 *	as well as being released on the button.....
		 */
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) &&
			ButtonClickTest(b,x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
			if (b->callbackFunction) {
				b->callbackFunction();
			}
		}

		/*
		 *	Set state back to zero.
		 */
		b->state = 0;
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPress(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse click was within the buttons client area,
		 *	set the state to true.
		 */
		if( ButtonClickTest(b,x,y) )
		{
			b->state = 1;
		}
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPassive(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse moved over the control
		 */
		if( ButtonClickTest(b,x,y) )
		{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw.
		 */
		if( b->highlighted == 1 )
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to draw.
 */
void ButtonDraw(Button *b)
{
	int fontx;
	int fonty;

	if(b)
	{
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted)
			glColor3f(0.7f,0.7f,0.8f);
		else
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button.
		 */
		if (b->state)
			glColor3f(0.4f,0.4f,0.4f);
		else
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state)
			glColor3f(0.8f,0.8f,0.8f);
		else
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);


		/*
		 *	Calculate the x and y coords for the text string in order to center it.
		 */
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,b->label)) / 2 ;
		fonty = b->y + (b->h+10)/2;

		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) {
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called to initialise opengl.
 */

void Init()
{
	glEnable(GL_LIGHT0);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw the 3D scene
 */
void Draw3D()
{
	gluLookAt(0,1,5,0,0,0,0,1,0);
	glutSolidTeapot(1);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw an overlay over the 3D scene.
 *	This will be used to draw our fonts, buttons etc......
 */
void Draw2D()
{
	ButtonDraw(&MyButton);
}

/*----------------------------------------------------------------------------------------
 *	This is the main display callback function. It sets up the drawing for
 *	The 3D scene first then calls the Draw3D() function. After that it switches to
 *	an orthographic projection and calls Draw2D().
 */

void Draw()
{
	/*
	 *	Clear the background
	 */
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

	/*
	 *	Enable lighting and the z-buffer
	 */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	/*
	 *	Set perspective viewing transformation
	 */

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(winh==0)?(1):((float)winw/winh),1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	/*
	 *	Draw the 3D elements in the scene

	Draw3D();*/

	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,winw,winh,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 2D overlay
	 */
 Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	init();
	 display();
	glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
 *	This function is called when the window is resized. All this does is simply
 *	store the new width and height of the window which are then referenced by
 *	the draw function to set the correct viewing transforms
 */
void Resize(int w, int h)
{
	winw = w;
	winh = h;

	/*
	 *	Allow drawing in full region of the screen
	 */
	glViewport(0,0,w,h);
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever a mouse button is pressed or released
 *	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
 *	\param	state	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
 *						or pressed respectivly.
 *	\param	x		-	the x-coord of the mouse cursor.
 *	\param	y		-	the y-coord of the mouse cursor.
 */
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN)
	{
		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		}

		/*
		 *	Which button was pressed?
		 */
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 1;
			ButtonPress(&MyButton,x,y);
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 1;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 1;
			break;
		}
	}
	else
	{
		/*
		 *	Which button was released?
		 */
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 0;
			ButtonRelease(&MyButton,x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 0;
			break;
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MouseMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;


	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(&MyButton,x,y);

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(&MyButton,x,y);

	/*
	 *	Note that I'm not using a glutPostRedisplay() call here. The passive motion function
	 *	is called at a very high frequency. We really don't want much processing to occur here.
	 *	Redrawing the screen every time the mouse moves is a bit excessive. Later on we
	 *	will look at a way to solve this problem and force a redraw only when needed.
	 */
}

/*----------------------------------------------------------------------------------------
 *
 */
 void draw_pixel(int x,int y)
{
    glPointSize(0.1);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
    glFlush();
}

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

void one(int x,int y,int offset)
{
int x1,y1,x2,y2;
x1=x+offset;
y1=y+25;
x2=x+offset;
y2=y1+50;
linebres(x1,y1,x2,y2);
}

void two(int x,int y,int offset)
{
int x1,y1,x2,y2;
x1=x+offset;
x2=x+offset+50;
y1=y+25;
y2=y+25;
linebres(x1,y1,x2,y2);
x1=x2=x+offset;
y1=y+25;
y2=y1+25;
linebres(x1,y1,x2,y2);
x1=x+offset;
x2=x+offset+50;
y1=y+50;
y2=y+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
x2=x+offset+50;
y1=y+75;
y2=y+75;
linebres(x1,y1,x2,y2);
x1=x2=x+offset+50;
y1=y+50;
y2=y1+25;
linebres(x1,y1,x2,y2);
}

void three(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset;
x2=x1+50;
y1=y2=y+25;
linebres(x1,y1,x2,y2);
x1=x+offset;
x2=x1+50;
y1=y2=y+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
x2=x1+50;
y1=y2=y+75;
linebres(x1,y1,x2,y2);
x1=x2=x+offset+50;
y1=y+25;
y2=y1+50;
linebres(x1,y1,x2,y2);
}

void four(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset+50;
x2=x+offset+50;
y1=y+25;
y2=y1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y+50;
x2=x1;
y2=y1+25;
linebres(x1,y1,x2,y2);
x1=offset+x;
y1=y+50;
x2=x1+50;
y2=y1;
linebres(x1,y1,x2,y2);
}

void five(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset;
y1=y2=y+25;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+50;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+75;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x2=x+offset+50;
y1=y+25;
y2=y+50;
linebres(x1,y1,x2,y2);
x1=x2=x+offset;
y1=y+50;
y2=y+75;
linebres(x1,y1,x2,y2);
}

void six(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset;
y1=y2=y+25;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+50;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+75;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x2=x+offset+50;
y1=y+25;
y2=y+50;
linebres(x1,y1,x2,y2);
x1=x2=x+offset;
y1=y+25;
y2=y+75;
linebres(x1,y1,x2,y2);
}

void nine(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset+50;
x2=x+offset+50;
y1=y+25;
y2=y1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y+50;
x2=x1;
y2=y1+25;
linebres(x1,y1,x2,y2);
x1=offset+x;
y1=y+50;
x2=x1+50;
y2=y1;
linebres(x1,y1,x2,y2);
x1=offset+x;
y1=y+25;
x2=x1+50;
y2=y1;
linebres(x1,y1,x2,y2);
x1=offset+x;
y1=y+75;
x2=x1+50;
y2=y1;
linebres(x1,y1,x2,y2);
}

void seven(int x,int y,int offset)
{
int x1,y1,x2,y2;
x1=x+offset+50;
y1=y+25;
x2=x+offset+50;
y2=y1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y+75;
x2=x+offset;
y2=y+60;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y+75;
x2=x1+50;
y2=y1;
linebres(x1,y1,x2,y2);
}

void eight(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset;
y1=y2=y+25;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+50;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+75;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x2=x+offset+50;
y1=y+25;
y2=y+75;
linebres(x1,y1,x2,y2);
x1=x2=x+offset;
y1=y+25;
y2=y+75;
linebres(x1,y1,x2,y2);
}

void zero(int x,int y,int offset)
{
int x1,x2,y1,y2;
x1=x+offset;
y1=y2=y+25;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x+offset;
y1=y2=y+75;
x2=x1+50;
linebres(x1,y1,x2,y2);
x1=x2=x+offset+50;
y1=y+25;
y2=y+75;
linebres(x1,y1,x2,y2);
x1=x2=x+offset;
y1=y+25;
y2=y+75;
linebres(x1,y1,x2,y2);
}

void num(int x,int y,int flag)
{
if(flag==0)
{

one(x,y,125);
two(x+2*200,y,100);
three(x+2*400,y,100);
four(x+2*600,y,100);
five(x+2*800,y,100);
six(x+2*1000,y,100);
seven(x+2*1200,y,100);
eight(x+2*1400,y,100);
nine(x+2*1600,y,100);
zero(x+2*1800,y,100);
}
else
{
x=x+2*1800;
one(x,y,125);
two(x-2*200,y,100);
three(x-2*400,y,100);
four(x-2*600,y,100);
five(x-2*800,y,100);
six(x-2*1000,y,100);
seven(x-2*1200,y,100);
eight(x-2*1400,y,100);
nine(x-2*1600,y,100);
zero(x-2*1800,y,100);
}
}

void hundred(int x,int y)
{

  one(x,y,125);
two(x-2*200,y,100);
three(x-2*400,y,100);
four(x-2*600,y,100);
five(x-2*800,y,100);
six(x-2*1000,y,100);
seven(x-2*1200,y,100);
eight(x-2*1400,y,100);
nine(x-2*1600,y,100);
x=0;
one(x,y,25);
zero(x,y,50);
zero(x,y,100);
}




void init()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,4700,0,4700);
}

void midpoint(int r,float xc,float yc,int flag)//flag=1=>right,0=>left
{
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
		if(flag==0)
		{
			setPixel(round(x+xc),round(y+yc));
			setPixel(round(x+xc),round(-y+yc));
			setPixel(round(-x+xc),round(y+yc));
			setPixel(round(-x+xc),round(-y+yc));
			setPixel(round(y+xc),round(x+yc));
			setPixel(round(y+xc),round(-x+yc));
			setPixel(round(-y+xc),round(x+yc));
			setPixel(round(-y+xc),round(-x+yc));
		}
  		else if(flag==1)
		{
			rotate(15.0,(int)(y+xc),(int)(x+yc));
	 		rotate(15.0,(int)(y+xc),(int)(-x+yc));
  	 	}
 		else
 		{
			rotate(15.0,(int)(-y+xc),(int)(x+yc));
			rotate(15.0,(int)(-y+xc),(int)(-x+yc));
		}
	}
}
void snake1()
{
int i;
    glColor3f(0.64,0.3,0.16);
for(i=0;i<=100;i+=1)
{
midpoint(1000,800+i,2640,1);
midpoint(1000,2225+i,1240,-1);
}
//midpoint(1000,900,2640,1);
//midpoint(1000,2325,1240,-1);
linebres(1330,905,1530,860);
linebres(1530,860,1430,930);
elip(2400,2900,75,150,30);
}
void snake2()
{
    glColor3f(1,0,0);
midpoint(1000,3000,1020,1);
midpoint(1000,2925,1000,1);
linebres(3445,1220,3505,1260);

}
void dice(int n)
{


    /*glColor3f(0.8,0.4,0.0);
       glBegin(GL_POLYGON);
    glVertex2f(4200,2000);
   glVertex2f(4200,2300);
   glVertex2f(4500,2300);
   glVertex2f(4500,2000);
   glEnd();*/

			glColor3f(0.7f,0.5f,0.7f);

		/*
		 *	draw background for the dice.
		 */
		glBegin(GL_QUADS);
			glVertex2i( 4200    ,2000     );
			glVertex2i( 4200    ,2300 );
			glVertex2i( 4500 ,2300 );
			glVertex2i( 4500, 2000      );
		glEnd();

		/*
		 *	Draw an outline around the dice with width 3
		 */
		glLineWidth(5);

		/*
		 *	The colours for the outline are reversed when the dice.
		 */
			glColor3f(0.6f,0.4f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i(4500, 2000    );
			glVertex2i(4200    , 2000     );
			glVertex2i( 4200    , 2300 );
		glEnd();

			glColor3f(0.8f,0.3f,0.5f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( 4200 ,  2300 );
			glVertex2i( 4500, 2300 );
			glVertex2i( 4500, 2000  );
		glEnd();



   glColor3f(1.0,1.0,1.0);
	switch(n)
	{
	case 1:midpoint(10,4200+150,2000+150,0);
	break;
	case 2:midpoint(10,4275,2150,0);
		midpoint(10,4425,2150,0);
		break;
	case 3:midpoint(10,4200+150,2000+150,0);
		midpoint(10,4250,2215,0);
		midpoint(10,4450,2085,0);
		break;
	case 4:midpoint(10,4250,2215,0);
		midpoint(10,4450,2085,0);
		midpoint(10,4250,2085,0);
		midpoint(10,4450,2215,0);
		break;
	case 5:midpoint(10,4250,2215,0);
		midpoint(10,4450,2085,0);
		midpoint(10,4250,2085,0);
		midpoint(10,4450,2215,0);
		midpoint(10,4200+150,2000+150,0);
		break;
	case 6:midpoint(10,4250,2215,0);
		midpoint(10,4450,2085,0);
		midpoint(10,4250,2085,0);
		midpoint(10,4450,2215,0);
		midpoint(10,4350,2085,0);
		midpoint(10,4350,2215,0);
		break;
	}
}


void ladder(int xb1,int yb1,int xb2,int yb2,int xu1,int yu1,int xu2,int yu2)
{
     glColor3f(0,0,0);
   linebres(xb1,yb1,xu1,yu1);
   linebres(xb2,yb2,xu2,yu2);
   float dy=(yu1-yb1);
 float dx=(xu1-xb1);
  float slope=dy/dx;

 dx=100/slope;

   int i,j;

   for(i=xb1,j=yb1;i<xu1-200,j<yu1-200;)
   {
     i+=dx;
     j+=100;
     xb2+=dx;
     yb2+=100;

	linebres(i,j,xb2,yb2);
   }

}
void matmul(float a[3][3],float b[3][1],float c[3][1])
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
void rotate(float theta,int x1,int y1)
{
float res1[3][1],res2[3][1],rot[3][3]={{cos(theta*PI/180.0),-sin(theta*PI/180.0),0},{sin(theta*PI/180.0),cos(theta*PI/180.0),0},{0,0,1}},orig1[3][1]={(float)x1,(float)y1,1.0};
matmul(rot,orig1,res1);
//matmul(rot,orig2,res2);
setPixel((int)res1[0][0],(int)res1[1][0]);
//printf("%d %d %d %d\n",(int)res1[0][0],(int)res1[1][0],(int)res2[0][0],(int)res2[1][0]);
//bres((int)res1[0][0],(int)res1[1][0],(int)res2[0][0],(int)res2[1][0]);
if(count==0)
printf("\n%f %f",res1[0][0],res1[1][0]);
count++;
}

void elip(int xcen,int ycen,int rx,int ry,int theta)
{
 float x,y,rx2,ry2;
 float p1,p2;
 p1=(ry*ry)-(rx*rx*ry)+(rx*rx*0.25);
 x=0;
 y=ry;
 rotate(theta,x+xcen,y+ycen);
 rotate(theta,-x+xcen,y+ycen);
 rotate(theta,x+xcen,-y+ycen);
 rotate(theta,-x+xcen,-y+ycen);
 ry2=ry*ry;
 rx2=rx*rx;

 while((2*ry2*x)<(2*rx2*y))
 {
  if(p1<0)
  {
  x++;
  p1=p1+(2*ry2*x)+(ry2);

  }
  else
  {
  y--;
  x++;
  p1=p1+(2*ry2*x)-(2*rx2*y)+(ry2);

  }

 rotate(theta,x+xcen,y+ycen);
 rotate(theta,-x+xcen,y+ycen);
 rotate(theta,x+xcen,-y+ycen);
 rotate(theta,-x+xcen,-y+ycen);
 }

 p2=(ry2*(x+0.5)*(x+0.5))+(rx2*(y-1)*(y-1))-(rx2*ry2);

 while(y>0)
 {
  if(p2>0)
  {
  y--;
  p2+=(-(2*rx2*y)+(rx2));
  }
  else
  {
  y--;
  x++;
  p2+=((2*ry2*x)-(2*rx2*y)+(rx2));
  }
 rotate(theta,x+xcen,y+ycen);
 rotate(theta,-x+xcen,y+ycen);
 rotate(theta,x+xcen,-y+ycen);
 rotate(theta,-x+xcen,-y+ycen);
 }

}



void numbers(int xcen[2],int ycen[2])
{
int i;

    int j,count=0;
for(i=0;i<10;i++)
{
  count++;
  for(j=0;j<10;j++)
  {
    if((count++)%2)
   {
    glColor3f(0.0,1.0,0.0);


   }
  else
  {
    glColor3f(0.0,0.0,1.0);

  }
    glBegin(GL_POLYGON);
    glVertex2f(i*400,j*400);
   glVertex2f((i+1)*400,j*400);
   glVertex2f((i+1)*400,(j+1)*400);
   glVertex2f(i*400,(j+1)*400);
   glEnd();
glFlush();

 }
}

glColor3f(0,0,0);

for(i=1;i<=9;i++)
one(i*2*200,2*200,75);

for(i=0;i<9;i++)
two(i*2*200,2*400,25);
for(i=1;i<=9;i++)
three(i*2*200,2*600,25);
for(i=0;i<9;i++)
four(i*2*200,2*800,25);
for(i=1;i<=9;i++)
five(i*2*200,2*1000,25);
for(i=0;i<9;i++)
six(i*2*200,2*1200,25);
for(i=1;i<=9;i++)
seven(i*2*200,2*1400,25);
for(i=0;i<9;i++)
eight(i*2*200,2*1600,25);
for(i=1;i<=9;i++)
nine(i*2*200,2*1800,25);

for(i=0;i<9;i++)
num(0,i*2*200,i%2);


hundred(2*1800,2*1800);


one(2*1800,0,75);
two(0,2*200,25);
three(2*1800,2*400,25);
four(0,2*600,25);
five(2*1800,2*800,25);
six(0,2*1000,25);
seven(2*1800,2*1200,25);
eight(0,2*1400,25);
nine(2*1800,2*1600,25);

ladder(2800+175,300,2800+275,225,3600+275,1200+200,3600+375,1200+125);
ladder(2000+75,400+275,2000+175,400+350,1200+275,3600+75,1200+375,3600+150);
ladder(2000+275,2400+300,2000+375,2400+225,2400+275,3200+200,2400+375,3200+125);
snake1();
snake2();

midpoint(60,xcen[0],ycen[0],0);
midpoint(60,xcen[1],ycen[1],0);
glColor3f(1,1,1);
dice(dicenum);
if(flag1==0)
	{
		glColor3f(0,0,0);
		drawstring(4050.0,2500.0,0.0,"PLAYER 2");
		glColor3f(0.5,0.5,0.5);
		drawstring(4050.0,2500.0,0.0,"PLAYER 1");
		
	}
	else
	{
		glColor3f(0,0,0);
		drawstring(4050.0,2500.0,0.0,"PLAYER 1");
		glColor3f(0.5,0.5,0.5);
		drawstring(4050.0,2500.0,0.0,"PLAYER 2");
		
	}
glColor3f(1,0,0);

}

void translate(int dx,int dy,int x1[2],int y1[2],int flag,int rnd)
{

//glClear(GL_COLOR_BUFFER_BIT);

int i,j,k,genmat[3][3]={{1,0,dx},{0,1,dy},{0,0,1}},pointmat[3][1]={x1[flag],y1[flag],1},endmat[3][1];

for(i=0;i<3;i++)
for(j=0;j<1;j++)
{
endmat[i][j]=0;
for(k=0;k<3;k++)
endmat[i][j]+=genmat[i][k]*pointmat[k][j];
}

int finx,finy;
x1[flag]=endmat[0][0];
y1[flag]=endmat[1][0];
 midpoint(60,x1[flag],y1[flag],0);

usleep(1000000);
glutSwapBuffers();
//numbers(x1,y1,rnd);
}

void snake(int xec,int yec,int xfc,int yfc,int xtc,int ytc)
{
elip(xec,yec,400,650,10);
elip(xec,yec,330,550,10);
midpoint(65,xfc,yfc,0);

}



void display()
{
int i;
//glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1,1,1);
numbers(xcen,ycen);

//movepawn(xin,yin,dicenum,xcen,ycen,1);
glFlush();
}

void movepawn(int xin[2],int yin[2],int rnd,int xcen[2],int ycen[2])
{
int direc,i,random;
int start[5][2]={{7*400,0},{5*400,400},{5*400,6*400},{400,400*9},{7*400,6*400}};
int end[5][2]={{9*400,3*400},{3*400,9*400},{6*400,8*400},{3*400,400*2},{8*400,3*400}};
random=rnd;
if(yin[flag1]%800==0)
	direc=1;
else
	direc=0;
while(rnd--)
{
	//printf("%d ",rnd);
	if(direc==1)
	{
		if(xin[flag1]%800==0)
		glColor3f(0,1,0);
		else
		glColor3f(0,0,1);
		if(xin[flag1]==3600)
		{
		direc=0;
		midpoint(60,xcen[flag1],ycen[flag1],0);
		glColor3f(1,0,0);
		translate(0,400,xcen,ycen,flag1,random);
		yin[flag1]+=400;
		}
		else
		{
		midpoint(60,xcen[flag1],ycen[flag1],0);
		glColor3f(1,0,0);
		translate(400,0,xcen,ycen,flag1,random);
		xin[flag1]+=400;
		}
	}
	else
	{
		if(xin[flag1]%800==0)
		glColor3f(0,0,1);
		else
		glColor3f(0,1,0);
		if(xin[flag1]==0)
		{
		direc=1;
		midpoint(60,xcen[flag1],ycen[flag1],0);
		glColor3f(1,0,0);
		translate(0,400,xcen,ycen,flag1,random);
		yin[flag1]+=400;
		}
		else
		{
		if(xin[flag1]-(random*400)>0)
		{
		midpoint(60,xcen[flag1],ycen[flag1],0);
		glColor3f(1,0,0);
		translate(-400,0,xcen,ycen,flag1,random);
		xin[flag1]-=400;
		}
		else if(xin[flag1]-(random*400)==0)
		{
	 	glClear(GL_COLOR_BUFFER_BIT);
		drawstring(2000.0,2000.0,0.0,"CONGRATULATIONS!");
		break;
		}
		}
	}
}
for(i=0;i<5;i++)
	if(xin[flag1]==start[i][0]&&yin[flag1]==start[i][1])
		{
		if(direc==0)
		{
			if(xin[flag1]%800==0)
			glColor3f(0,0,1);
			else
			glColor3f(0,1,0);
		}
		else
		{
			if(xin[flag1]%800==0)
			glColor3f(0,1,0);
			else
			glColor3f(0,0,1);
		}
		midpoint(60,xcen[flag1],ycen[flag1],0);
		glColor3f(1,0,0);
	 	xin[flag1]=end[i][0];
		yin[flag1]=end[i][1];
  		translate(end[i][0]-start[i][0],end[i][1]-start[i][1],xcen,ycen,flag1,random);
		}

	if(flag1==0)
	{
		glColor3f(0,0,0);
		drawstring(4050.0,2500.0,0.0,"PLAYER 1");
		glColor3f(0.5,0.5,0.5);
		drawstring(4050.0,2500.0,0.0,"PLAYER 2");
		flag1=1;
	}
	else
	{
		glColor3f(0,0,0);
		drawstring(4050.0,2500.0,0.0,"PLAYER 2");
		glColor3f(0.5,0.5,0.5);
		drawstring(4050.0,2500.0,0.0,"PLAYER 1");
		flag1=0;
	}
}

void edgedetect(float x1,float y1,float x2,float y2,int *le,int *re)
{
    float temp,x,mx;
    int i;

    if(y1>y2)
    {
        temp=x1,x1=x2,x2=temp;
        temp=y1,y1=y2,y2=temp;
    }

    if(y1==y2)
        mx=x2-x1;
    else
        mx=(x2-x1)/(y2-y1);

    x=x1;
   for(i=((int)y1);i<=((int)y2);i++)
    {
        if(x<(float)le[i] )
           le[i]=((int)x);
        if( x>(float)re[i])
           re[i]=((int)x);
        x+=mx;
    }
}

void scanfill(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
    int le[4700],re[4700],i,j;

    for(i=0;i<4700;i++)
        le[i]=4700,re[i]=0;

    edgedetect(x1,y1,x2,y2,le,re);
    edgedetect(x2,y2,x3,y3,le,re);
    edgedetect(x3,y3,x4,y4,le,re);
    edgedetect(x4,y4,x1,y1,le,re);

    for(j=0;j<4700;j++)
    {
          if(le[j]<=re[j])
            for(i=le[j];i<re[j];i++)
                draw_pixel(i,j);
    }
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	//glutInitWindowPosition(200,100);
	glutCreateWindow("03 - Mouse Motion");

	glutDisplayFunc(Draw);
	//glutReshapeFunc(Resize);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	Init();

	glutMainLoop();
}















