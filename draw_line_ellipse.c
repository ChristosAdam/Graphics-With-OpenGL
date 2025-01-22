/*********************************
 * Ergasia 1 – Askhsh 2 – 18/01/2022
 * Adamakis Christos 
 *********************************/

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

GLint screenWidth= 640;
GLint screenHeight =480;

int menu; 
int selection; 
int menu_choice; 
int numCorners = 0;

 typedef struct 
 { 
	 GLint x,y;
 }
 GLintPoint;

// Draw Point
void drawPoint(int ax, int ay)
{
	glBegin(GL_POINTS);
		glVertex2i(ax,ay);
	glEnd();
	glFlush();
}

// Draw Line with Bresenham Algorithm
void drawLine(int xs, int ys, int xe, int ye)
{

  int x, y;
  int s1, s2, e1, e2;
  float m, m1, w, e;

  // Compute the slope of the line.
  if(xe != xs){
    m = ((float)(ye - ys))/((float)(xe - xs));
  }
  
  // Take the absolute value of m
  if(m<0){
    m= -m;
  }
  
  // If the slope is bigger than 1 then take the mirrored points as of X=Y
  if(m > 1){
    s1 = ys;
    s2 = xs;
    e1 = ye;
    e2 = xe;
    m1 = 1/m;
  }// Else take points as given
  else{
    s1 = xs;
    s2 = ys;
    e1 = xe;
    e2 = ye;
    m1 = m;
  }

  // If the point of x_end is less than x_begin swap the points.
  if(s1 > e1){
    x = e1;
    y = e2;
    e1 = s1;
    e2 = s2;
  }
  else{
    x = s1;
    y = s2;
  }

  // Initiate the values Bresenham uses.
  w = 1 - m1;
  e = 1;
  glBegin(GL_POINTS);

    glColor3f(0,e,0); // Color the pixel in green scale.

    if(m > 1){ // If slope bigger than one then draw y,x point
      glVertex2i(y,x);
    }
    else{
      glVertex2i(x,y);
    }

    while(x < e1){ //Make Bresenham computations

      x++;
      if(e < w){
        e = e + m1;
      }
      else{

        if(y > e2){ // If slope negative lower the value of y.
          y--;
        }
        else{
          y++;
        }
        e = e - w;
      }
   
      // In order to have smoother transition of green scale have a range of percentage used.
      if(e<0.35){
        glColor3f(0,0.35,0);
      }
      else if(e>0.65){
        glColor3f(0,0.65,0);
      }
      else{
        glColor3f(0,e,0);
      }
      
      if(m > 1){
        glVertex2i(y,x);
      }
      else{
        glVertex2i(x,y);
      }
    }
 
  glEnd();

  glFlush();
}

// Plot symmetrical the point of ellipse.
void plotpoints(int xc, int yc, int x, int y, float ang){
  
  int x1,x2,x3,x4,y1,y2,y3,y4;
  int x11,x21,x31,x41,y11,y21,y31,y41;
  x1 = xc+x;
  x2 = xc+x;
  x3 = xc-x;
  x4 = xc-x;
  y1 = yc+y;
  y2 = yc+y;
  y3 = yc-y;
  y4 = yc-y;
  if(ang!=0){ // Rotate the points to fit the original corners given.
    x11 = cos(ang)*(x1-xc)+sin(ang)*(yc-y1) + xc;
    x21 = cos(ang)*(x2-xc)+sin(ang)*(yc-y3) + xc;
    x31 = cos(ang)*(x3-xc)+sin(ang)*(yc-y2) + xc;
    x41 = cos(ang)*(x4-xc)+sin(ang)*(yc-y4) + xc;
    y11 = sin(ang)*(x1-xc)+cos(ang)*(y1-yc) + yc;
    y21 = sin(ang)*(x3-xc)+cos(ang)*(y2-yc) + yc;
    y31 = sin(ang)*(x2-xc)+cos(ang)*(y3-yc) + yc;
    y41 = sin(ang)*(x4-xc)+cos(ang)*(y4-yc) + yc;
  }
  
  glBegin(GL_POINTS); //Draw the symmetrical rotated points
    glVertex2i(x11,y11);
    glVertex2i(x21,y31);
    glVertex2i(x31,y21);
    glVertex2i(x41,y41);
  glEnd();
  glFlush();
  

}

void drawEllipse(int xc, int yc, int xa, int ya, int xb, int yb)
{
  int x, y, x2, y2, x22, y22, p, px, py;
  int xr,yr,xr1,yr1;

  float m = 0;
  float xe,ye;
  double ang;
  double pi = 3.141592653;

  // Find the slope of CA in order to rotate.
  if(xa != xc){
    m = (float)(ya-yc)/(float)(xa-xc);
  }

  // Finnd the rotation angle
  ang = atan(m);

  // Rotate a
  xr1 = cos(pi/2-ang)*(xa-xc)+sin(pi/2-ang)*(yc-ya) + xc;
  yr1 = sin(pi/2-ang)*(xa-xc)+cos(pi/2-ang)*(ya-yc) + yc;  

  // Find the b of the function
  xe = (float)sqrt((float)(xr1-xc)*(float)(xr1-xc)+(float)(yr1-yc)*(float)(yr1-yc));
  // Rotate b
  xr = cos(-ang)*(xb-xc)+sin(-ang)*(yc-yb) + xc;
  yr = sin(-ang)*(xb-xc)+cos(-ang)*(yb-yc) + yc;


  if(xe*xe-(xr-xc)*(xr-xc) > 0 ){
    // Find b value of ellipse given the third point.
    ye = sqrt(((yr-yc)*(yr-yc)*xe*xe)/(xe*xe-(xr-xc)*(xr-xc)));
    // Values needed for Bresnham algorithm
    x2 = xe * xe;
    x22 = 2 * x2;
    y2 = ye * ye;
    y22 = 2 * y2;
    x = 0;
    y = (int)ye;
    px = 0;
    py = x22 * y;
  
  
    plotpoints(xc, yc, x, y, ang);

    // P value that dictate the choosing of next pixel to draw.
    p = (int)(y2-(x2*ye)+(0.25*x2)+0.5);
    while(px<py){ // Repeat till drawing slope is less than 1
      x++;

      px = px + y22; // Update the helpful values for the algorithm.
      if(p<0){
        p=p+y2+px;
      }
      else{
        y--;
        py = py - x22;
        p = p + y2 + px -py;
      }
      plotpoints(xc, yc, x, y, ang);
    }  
  
    // P value that dictate the choosing of next pixel to draw.
    p = (int)(y2*(x+0.5)*(x+0.5) + x2 * (y-1)*(y-1) - x2*y2 + 0.5);
  
    while(y>0){ // Repeat till algorithm reaches the x axis.
      y--;
      py = py - x22; // Update the helpful values for the algorithm.
      if(p>0){
        p=p+x2-py;
      }
      else{
        x++;
        px = px + y22;
        p = p + x2 - py + px;
      }
      plotpoints(xc, yc, x, y, ang);
    }
  }
  else{ // Error for B point selection.
    printf("Point b should not be further than the paraller to y lines that are symmetrical to center created by Point A \n");
  } 

  glFlush();
}


// Part of code needed to create the window taken from the examples of the course.
void myInit (void)
{
	glColor3f(0.0,0.0,0.0);
	glPointSize(1);					// set point size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLfloat)screenWidth, 0.0,(GLfloat)screenHeight);
	glFlush();
}
										

// Execute the specified action.
void myMouse (int button, int state, int x1, int y1)
{
	static GLintPoint corner[3];
	//static int numCorners =0;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{		
		corner[numCorners].x =x1;
		corner[numCorners].y = screenHeight-y1;
		drawPoint(corner[numCorners].x,corner[numCorners].y);
		numCorners++;
		if (numCorners == 2)  // Draw Line if choice is 1, ellipse needs 3 corners
		{
			glColor3f(0.0,0.0,1.0);
                        if (menu_choice == 1)
			{
				glColor3f(0.0,1.0, 0.0);
				drawLine(corner[0].x, corner[0].y, corner[1].x, corner[1].y);
				numCorners =0;
			}
			
		}
		else if (numCorners == 3){   // Draw ellipse with the three point given.              
			if(menu_choice == 2)
			{
				glColor3f(1.0,0.0,0.0);	
				drawEllipse(corner[0].x, corner[0].y, corner[1].x, corner[1].y,  corner[2].x, corner[2].y);
			}
			numCorners =0;
		}

	} 
		
	glFlush();	
}

void myMenu(int selection)
{
	if(selection==1) // drawing line with Bresenham
	{
		menu_choice = 1;
		numCorners = 0;
		glColor3f(0.0,1.0,0.0); 
		glutPostRedisplay(); 	
	}
	else if(selection==2) // drawing ellipse
	{	
		menu_choice = 2;
		numCorners = 0;
		glColor3f(1.0,0.0,0.0);
		glutPostRedisplay();
	}
	else if(selection==0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);  // clear the window
		glFlush();	
	}
	else 
	{
		printf("Invalid selection.\n");
	}
}


// Initialization of code needed to create the window taken from the examples of the course.
int main(int argc, char **argv)
{
	// initialize toolkit
	glutInit(&argc, argv);
	// set display mode
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// set window size
	glutInitWindowSize (screenWidth, screenHeight);

	// set window initial position
	glutInitWindowPosition(1,1);
	// creates a window
	glutCreateWindow ("DRAW LINE AND ELLIPSE");
	
	// registers a keyboard handler
	glutMouseFunc(myMouse);

	glClearColor(0.5, 0.5, 0.5, 0.0);	// select background color
	glClear(GL_COLOR_BUFFER_BIT);		// Clear
	glFlush();

	menu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Draw Line",1);
	glutAddMenuEntry("Draw Elleipsi",2);
	glutAddMenuEntry("Clear Screen", 0);
	
	// register menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	myInit();

	// Menu running in loops.
	glutMainLoop();

	glutDestroyMenu(menu);
	return 0;
}

