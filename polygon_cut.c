/*********************************
 * Ergasia 2 – Askhsh 6 – 22/02/2022
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
int numCorners1 = 0;
int numCornersmax = 3;
float edgetable[20][5];
int numCorner = 3;
int menu_choice1 = 0;

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
void drawClipWindow(int ax, int ay, int bx, int by)
{
	glColor3f(0.0,0.0,0.0);	
	glBegin(GL_POLYGON);
		glVertex2i(ax,ay);
		glVertex2i(ax,by);
		glVertex2i(bx, by);
		glVertex2i(bx, ay);
	glEnd();
	glFlush();

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

// Check if edged out of the box
int in(GLintPoint tp, int ed, GLintPoint downl, GLintPoint upl){
	

	if(ed == 0){ // Out to the left
		if(tp.x < downl.x)
			return 0;
	}
	else if(ed == 1){ // Out to the right
		if(tp.x > upl.x)
			return 0;
	}
	else if(ed == 2){ // Out to the bottom
		if(tp.y < downl.y)
			return 0;
	}
	else if(ed == 3){ // Out to the top
		if(tp.y > upl.y)
			return 0;
	}

	return 1;

}

//Check if a line is completely outside or inside
int cs(GLintPoint tp, GLintPoint tp1, int ed, GLintPoint downl, GLintPoint upl){

	if(in(tp,ed,downl,upl) == in(tp1,ed,downl,upl)){
		return 0;
	}
	return 1;
}


// Find intersection with each side of the box and the polygon
GLintPoint ints(GLintPoint tp, GLintPoint tp1, int ed, GLintPoint downl, GLintPoint upl){
	GLintPoint rpoint;
	float m;

	if(tp.x != tp1.x){ //Find the slope of the line we are checking
		m = (float)(tp.y-tp1.y)/(float)(tp.x-tp1.x);
	}
	if(ed == 0){ //Left intersect
		rpoint.x = downl.x;
		rpoint.y = tp1.y + (downl.x - tp1.x)*m;
	}
	else if(ed == 1){ //Right intersect
		rpoint.x = upl.x;
		rpoint.y = tp1.y + (upl.x - tp1.x)*m;
	}
	else if(ed == 2){ //Bottom intersect
		rpoint.y = downl.y;
		if(tp.x != tp1.x){
			rpoint.x = tp1.x  + (downl.y - tp1.y)/m;
		}
		else{
			rpoint.x = tp1.x;
		}
	}
	else if(ed == 3){ //Top intersect
		rpoint.y = upl.y;
		if(tp.x != tp1.x){
			rpoint.x = tp1.x  + (upl.y - tp1.y)/m;
		}
		else{
			rpoint.x = tp1.x;
		}
	}
	return rpoint;

}

// Check if a new edge is needed to be added recursively and add to table
void cp(GLintPoint tp, int ed, GLintPoint downl, GLintPoint upl, GLintPoint *OutputPoint, int *Iout, GLintPoint *isF[], GLintPoint *Pnt){

	GLintPoint rpoint;

	if(isF[ed] == 0){
		isF[ed] = &tp;
	}
	else{
		if(cs(tp, Pnt[ed], ed, downl, upl)==1){
			rpoint = ints(tp, Pnt[ed], ed, downl, upl);
			if(ed < 3){
				cp(rpoint, ed+1, downl, upl, OutputPoint, Iout, isF, Pnt);
			}
			else{
				OutputPoint[*Iout] = rpoint;
				(*Iout)++;
			}
		}
	}
	Pnt[ed] = tp;

	if(in(tp,ed,downl,upl) == 1){
		if(ed < 3){
			cp(tp, ed+1, downl, upl, OutputPoint, Iout, isF, Pnt);
		}
		else{
			OutputPoint[*Iout] = tp;
			(*Iout)++;
		}
	}
}

// Create new edge table
void cclip(GLintPoint downl, GLintPoint upl, GLintPoint *OutputPoint, int *Iout, GLintPoint *isF[], GLintPoint *Pnt){
	GLintPoint rpoint;
	int ed;
	
	for(ed = 0; ed<=3; ed++){ //For 0 to 3 0-left, 1-right, 2-bottom, 3-top
		if(cs(Pnt[ed], *isF[ed], ed, downl, upl)){  //find intersections and create new table
			rpoint = ints(Pnt[ed],*isF[ed],ed,downl,upl);
			if(ed < 3){
				cp(rpoint, ed+1, downl, upl, OutputPoint, Iout, isF, Pnt);
			}
			else{
				OutputPoint[*Iout] = rpoint;
				(*Iout)++;
			}
		}
	}
}

//Sutherland Hogman algorithm
int SHpc(GLintPoint downl, GLintPoint upl,int n, GLintPoint *PointIn, GLintPoint *OutputPoint){

	GLintPoint *isF[4] ={0};
	GLintPoint Pnt[4];
	int k;
	int Iout=0;

	for(k=0; k<n; k++){	//Add new edges using intersection of vertices with box the table of edges to
		cp(PointIn[k], 0, downl, upl, OutputPoint, &Iout, isF, Pnt);
	}
	//Clip the polygon to fit the box
	cclip(downl, upl, OutputPoint, &Iout, isF, Pnt);

	return Iout;
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
	glColor3f(0.0,0.0,1.0);
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
	static GLintPoint corner1[2];
	static GLintPoint cp[20] = {0};
	static GLintPoint bound[2];
	int i = 0 ;
	int NnumCorners;

	if (button == GLUT_LEFT_BUTTON && state ==GLUT_DOWN)
	{
		
		
		if (menu_choice == 1) //
		{
			corner[numCorners].x =x1;
			corner[numCorners].y = screenHeight-y1;
			drawPoint(corner[numCorners].x,corner[numCorners].y);
			numCorners++;
		}
		if (numCorners >= numCornersmax) 
		{
			if (menu_choice == 1)	//Draw the outline
			{
				for(i=0;i<numCornersmax-1;i++){
					drawLine(corner[i].x, corner[i].y, corner[i+1].x, corner[i+1].y);
				}
				drawLine(corner[0].x, corner[0].y, corner[numCornersmax-1].x, corner[numCornersmax-1].y);
			}
			if(menu_choice == 2)	//Fill the polygon
			{
				printf("%d Points to make polygon\n",numCorners);
				for(i=0;i<numCornersmax;i++){
					printf("%d Point (%d,%d)\n",i+1,corner[i].x, corner[i].y);
				}
				createEdgeTable(corner);
				fillTable();
			}
			
			
		}
		if (menu_choice == 5) //Create clip box
			{
			
			corner1[numCorners1].x =x1;
			corner1[numCorners1].y = screenHeight-y1;
			drawPoint(corner1[numCorners1].x,corner1[numCorners1].y);
			numCorners1++;
			if (numCorners1 >= 2 & menu_choice1 == 0)
			{
				drawClipWindow(corner1[0].x, corner1[0].y, corner1[1].x, corner1[1].y);
			
			}
			
			createEdgeTable(corner);
			fillTable();
			
		}
		if (menu_choice == 6){ //Sutherland choice for a rectangle that already excists 
			for(i=0; i<numCornersmax; i++){
				cp[i] = corner[i];
			}
			corner[numCornersmax] = corner[0];
			if(corner1[0].x < corner1[1].x){  //Create the min max value of the clip box
				bound[0].x = corner1[0].x;
				bound[1].x = corner1[1].x;
			}
			else{
				bound[0].x = corner1[1].x;
				bound[1].x = corner1[0].x;
			}
			if(corner1[0].y < corner1[1].y){
				bound[0].y = corner1[0].y;
				bound[1].y = corner1[1].y;
			}
			else{
				bound[0].y = corner1[1].y;
				bound[1].y = corner1[0].y;
			}

			// Run the algorithm for the polygon with the clip box 
			NnumCorners = SHpc(bound[0], bound[1],numCornersmax+1, corner, cp);
			
			glColor3f(0.0,0.0,1.0);
			for(i=0;i<NnumCorners;i++){
				if(NnumCorners - 1 > i)
					drawLine(cp[i].x, cp[i].y, cp[i+1].x, cp[i+1].y);
			}
			drawLine(cp[NnumCorners-1].x, cp[NnumCorners-1].y, cp[0].x, cp[0].y);
			glClear(GL_COLOR_BUFFER_BIT);  
		
			drawPoint(corner1[numCorners1].x,corner1[numCorners1].y);

			if (numCorners1 >= 2 & menu_choice1 == 0)
			{
				drawClipWindow(corner1[0].x, corner1[0].y, corner1[1].x, corner1[1].y);
			
			}
			numCorner = numCornersmax;
			numCornersmax = NnumCorners;
			createEdgeTable(cp);
			fillTable();
			numCornersmax = numCorner;
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
			//numCorners = 0;
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
			menu_choice = 0;
			numCornersmax = 0;
			numCorners1 = 0;
			numCorner = 0;
			glClear(GL_COLOR_BUFFER_BIT);  
			glFlush();
			break;

		default:
			printf("Invalid menu selection\n");
			break;
	}

}

void keyEvent(unsigned char key,int x,int y){	// x,y mouse location when key pressed
  
    switch (key) {
      // Quit
	case 'C':
	case 'c':
		menu_choice = 6;
		break;	
	case 'F':
	case 'f'://GLUT_KEY_F1 didn't work
		menu_choice = 5;
		numCorners1 = 0;
		break; 
	case 32: // Space
		glClear(GL_COLOR_BUFFER_BIT); 
		if (menu_choice1 == 0) // Hide clip window
			menu_choice1 = 1;
		else			// Show clip window
			menu_choice1 = 0;
		if(numCornersmax != 0)
			fillTable();
		glFlush();
		break;     
	default:	
		break;
   	glutPostRedisplay(); 
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
	glutAddMenuEntry("Create outline", DRAW_OUTLINE);
	glutAddMenuEntry("Polygon Fill", POLYGON_FILL);
	glutAddSubMenu("Choose color", lineColorSubmenu);
	glutAddMenuEntry("Clear Screen", CLEAR_SCREEN);

	// register menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//Initializations
	myInit();
	glutKeyboardFunc(keyEvent);
	// Create loop for menu.
	glutMainLoop();

	glutDestroyMenu(lineColorSubmenu);
	glutDestroyMenu(menu);

	return 0;
}

