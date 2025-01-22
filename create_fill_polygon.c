/*********************************
 * Ergasia 1 – Askhsh 4 – 18/01/2022
 * Adamakis Christos 
 *********************************/


#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

// Macros for selection in the menu for switch options.
#define DRAW_OUTLINE 1
#define POLYGON_FILL 2
#define THREE_CORNERS 3
#define FOUR_CORNERS 4
#define FIVE_CORNERS 5
#define SIX_CORNERS 6
#define SEVEN_CORNERS 7
#define EIGHT_CORNERS 8
#define NINE_CORNERS 9
#define TEN_CORNERS 10
#define LINE_RED_COLOR 11
#define LINE_GREEN_COLOR 12
#define LINE_BLUE_COLOR 13
#define CLEAR_SCREEN 14

GLint screenWidth= 640;
GLint screenHeight =480;

int menu; // for menu creation
int selection; // for menu choices
int menu_choice; 
int numCorners = 0;
int numCornersmax = 3;
float edgetable[10][5];

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

// Draw Line given two points.
void drawLine(int ax, int ay, int bx, int by)
{
	glBegin(GL_LINES);
		glVertex2i(ax,ay);
		glVertex2i(bx, by);
	glEnd();

}

// Part of code needed to create the window taken from the examples of the course.
void myInit (void)
{

	glColor3f(0.0,0.0,0.0);			// Point Color selection.
	glPointSize(1);				// set point size
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLfloat)screenWidth, 0.0,(GLfloat)screenHeight);
	glFlush();
}
										


void createEdgeTable(GLintPoint *corner){

	int i;
	
	// Insert the lines connecting the point with the order they were selected to a table.
	// The table contains Xmin, Xmax, 1/slope, b, active state for every line.
	// Active state is initialized as zero
	for(i=0;i<numCornersmax-1;i++){
		if(corner[i+1].x != corner[i].x){
			edgetable[i][0] = (float)(corner[i+1].y - corner[i].y)/(float)(corner[i+1].x - corner[i].x); 
		}
		else{
			edgetable[i][0] = 0;
		}

		edgetable[i][1] = corner[i].y - edgetable[i][0] * corner[i].x;
		if ( corner[i].x < corner[i+1].x){
			edgetable[i][2] = corner[i].x;
			edgetable[i][3] = corner[i+1].x;
		}
		else{
			edgetable[i][3] = corner[i].x;
			edgetable[i][2] = corner[i+1].x;
		} 
		edgetable[i][4] = 0;
		
	}

	//this is the line that connects the first with the last point.
	i = numCornersmax-1;
	edgetable[i][0] = (float)(corner[i].y - corner[0].y)/(float)(corner[i].x - corner[0].x); 
	edgetable[i][1] = corner[0].y - edgetable[i][0] * corner[0].x;

	if ( corner[0].x < corner[i].x){
		edgetable[i][2] = corner[0].x;
		edgetable[i][3] = corner[i].x;
	}
	else{
		edgetable[i][3] = corner[0].x;
		edgetable[i][2] = corner[i].x;
	} 
	edgetable[i][4] = 0;

}

// Find Xmin of polygon
int findMin(){
	int i;
	int min = screenHeight;
	for(i=0;i<numCornersmax;i++){
		if ( edgetable[i][2] < min){
			min = (int)edgetable[i][2];
		}
	}
	return min;
}

// Find Xmax of polygon
int findMax(){
	int i;
	int max = 0;
	for(i=0;i<numCornersmax;i++){
		if ( edgetable[i][3] > max){
			max = (int)edgetable[i][3];
		}
	}
	return max;
}

//Algorithm that fills lines
void fillTable(){

	int i,j,k,l;
	int ymin, ymax;
	int x = 0;
	int count;
	int xline[10];
	int fill = 0;
	int temp, minj;

	ymin = findMin();
	ymax = findMax();

	for(i=ymin;i<ymax;i++){

		// First find the active edges and change the active state to 1.
		count = 0;
		for(j=0;j<numCornersmax;j++){
			if (i>=edgetable[j][2]&&i<edgetable[j][3]){
				edgetable[j][4] = 1;
				count++;
			}
			else{
				edgetable[j][4] = 0;
			}
		}

		// Compute the y coordinate of every active edge.
		k=0;
		for(j=0;j<numCornersmax;j++){
			if (edgetable[j][4] == 1){
				xline[k] = edgetable[j][1]+edgetable[j][0]*i ;
				k++;
			}
		}
		
		// Selection Sort for ypoints that intersect x line.
		for(j = 0;j < k; j++){
			minj = j;
			for (l = j + 1; l < k; l++)
				if (xline[l] < xline[minj])
					minj = l; 
			temp = xline[minj];
			xline[minj] = xline[j];
			xline[j] = temp;
			
		}
		for(l=0;l<k;l++){
			if(l%2 == 0)
				drawLine(i, xline[l], i, xline[l+1]);
		}

		x = 0;
	}
}

// Execute the specified action.
void myMouse (int button, int state, int x1, int y1)
{
	static GLintPoint corner[10];
	int i = 0 ;

	if (button == GLUT_LEFT_BUTTON && state ==GLUT_DOWN)
	{
		
		corner[numCorners].x =x1;
		corner[numCorners].y = screenHeight-y1;
		drawPoint(corner[numCorners].x,corner[numCorners].y);
		numCorners++;
		if (numCorners == numCornersmax)
		{
			if (menu_choice == 1)
			{
				for(i=0;i<numCornersmax-1;i++){
					drawLine(corner[i].x, corner[i].y, corner[i+1].x, corner[i+1].y);
				}
				drawLine(corner[0].x, corner[0].y, corner[numCornersmax-1].x, corner[numCornersmax-1].y);
			}
			if(menu_choice == 2)
			{
				createEdgeTable(corner);
				fillTable();
			}
			
			numCorners =0;
		}
		
	}
	
	glFlush();
	
}

void myMenu(int selection)
{
        
	switch(selection) {

		case DRAW_OUTLINE: // Choose number of corners and create one polygon.
			menu_choice=1;
			numCorners = 0;
			glutPostRedisplay(); 
			break;

		case POLYGON_FILL: // Fill POlygon
			menu_choice=2;
			numCorners = 0;
			glutPostRedisplay();
			break;

		case THREE_CORNERS:
			numCornersmax = 3;
			break;

		case FOUR_CORNERS:
			numCornersmax = 4;
			break;
		case FIVE_CORNERS:
			numCornersmax = 5;
			break;

		case SIX_CORNERS:
			numCornersmax = 6;
			break;
		case SEVEN_CORNERS:
			numCornersmax = 7;
			break;

		case EIGHT_CORNERS:
			numCornersmax = 8;
			break;
		case NINE_CORNERS:
			numCornersmax = 9;
			break;

		case TEN_CORNERS:
			numCornersmax = 10;
			break;

		case LINE_RED_COLOR:
			glColor3f(1.0,0.0,0.0);
			break;

		case LINE_GREEN_COLOR:
			glColor3f(0.0,1.0,0.0);
			break;

		case LINE_BLUE_COLOR:
			glColor3f(0.0,0.0,1.0);
			break;

		case CLEAR_SCREEN: // Window background back to grey
			glClear(GL_COLOR_BUFFER_BIT);  
			glFlush();
			break;

		default:
			printf("Invalid menu selection\n");
			break;
	}

}



// Initialization of code needed to create the window taken from the examples of the course.
int main(int argc, char **argv)
{

	int menu, numcornerSubmenu, lineColorSubmenu; // Two sub-menus
	
	// initialize toolkit
	glutInit(&argc, argv);
	// set display mode
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// set window size
	glutInitWindowSize (screenWidth, screenHeight);

	// set window initial position
	glutInitWindowPosition(1,1);

	// creates a window
	glutCreateWindow ("Polygon Fill with YX algorithm");

	// registers a keyboard handler
	glutMouseFunc(myMouse);

	// Create Initial Background color
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);	

	// Create a pop-up menu for number of points selection.
	numcornerSubmenu = glutCreateMenu(myMenu);
	glutAddMenuEntry("3", THREE_CORNERS);
        glutAddMenuEntry("4", FOUR_CORNERS);
	glutAddMenuEntry("5", FIVE_CORNERS);
	glutAddMenuEntry("6", SIX_CORNERS);
        glutAddMenuEntry("7", SEVEN_CORNERS);
	glutAddMenuEntry("8", EIGHT_CORNERS);
	glutAddMenuEntry("9", NINE_CORNERS);
	glutAddMenuEntry("10", TEN_CORNERS);

	//Create a sub-menu for color selection.
	lineColorSubmenu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Red", LINE_RED_COLOR);
	glutAddMenuEntry("Green", LINE_GREEN_COLOR);
	glutAddMenuEntry("Blue", LINE_BLUE_COLOR);

	// menu creation
	menu = glutCreateMenu(myMenu);
	glutAddSubMenu("Number of edges", numcornerSubmenu);
	glutAddMenuEntry("Polygon Fill", POLYGON_FILL);
	glutAddSubMenu("Choose color", lineColorSubmenu);
	glutAddMenuEntry("Clear Screen", CLEAR_SCREEN);

	// register menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Initializations
	myInit();

	// Create loop for menu.
	glutMainLoop();

	glutDestroyMenu(lineColorSubmenu);
	glutDestroyMenu(menu);

	return 0;
}

